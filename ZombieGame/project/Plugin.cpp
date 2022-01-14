#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"

#include <set>



//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	srand(time(NULL));



	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	auto world = m_pInterface->World_GetInfo();
	m_ExplorationGrid = ExplorationGrid(world.Center, world.Dimensions.x / 2, world.Dimensions.y / 2, 20, 20);
	//m_Target = m_ExplorationGrid.GetRandomUnexploredCell().GetCellCenter();
	



	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "myBot";
	info.Student_FirstName = "Jitse";
	info.Student_LastName = "Verstraete";
	info.Student_Class = "2DAE07";
}

//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
	m_pZombieEvadeBehavior = new EvadeZombies();
	m_pSeekBehavior = new Seek();
	m_pRadarBehavior = new Radar();
	m_pAimBehavior = new AimZombie();


	std::vector <BlendedSteering::WeightedBehavior> behaviors{ {BlendedSteering::WeightedBehavior(m_pSeekBehavior, 1), BlendedSteering::WeightedBehavior(m_pZombieEvadeBehavior, 1) } };
	m_pEvasiveSeek = new BlendedSteering(behaviors);



}

//Called only once
void Plugin::DllShutdown()
{
	//Called when the plugin gets unloaded
	SAFE_DELETE(m_pZombieEvadeBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pEvasiveSeek);
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 30; //How many enemies? (Default = 20)
	params.GodMode = true; //GodMode > You can't die, can be usefull to inspect certain behaviours (Default = false)
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
		
	}

	//DEBUG RENDERING


	//Debug Print
	m_DebugPrintTimer += dt;
	if (m_DebugPrintTimer > m_DebugPrintInterval)
	{
		m_DebugPrintTimer -= m_DebugPrintInterval;

		std::cout << "---Debug print record---\n";
		std::cout << "Number of houses recorded: " << m_KnownHouses.size() << std::endl;
		std::cout << "Number of items recorded: " << m_KnownItems.size() << std::endl;
		std::cout << std::endl;

	}

	//draw recorded enemy positions
	for (const auto& e : m_ZombieHordeInfo.GetRecordedEnemies())
	{
		m_pInterface->Draw_Point(e.Position, 3.f, { 0.f, 1.f, 0.f }, 0.f);
	}

	//draw exploration grid
	for (const auto& cell : m_ExplorationGrid.GetCells())
	{

		if (cell.isExplored)
		{
			m_pInterface->Draw_SolidPolygon(cell.GetRectPoints().data(), 4, { 1.f, 0.5f, 0.5f }, 0.1f);
		}
		else
		{
			m_pInterface->Draw_Polygon(cell.GetRectPoints().data(), 4, { 1.f, 1.f, 1.f }, 0.2f);
		}
	}


	//world border
	Elite::Vector2 center{ m_pInterface->World_GetInfo().Center };
	Elite::Vector2 bounds{ m_pInterface->World_GetInfo().Dimensions };
	Elite::Vector2 points[4]{
	Elite::Vector2(center - bounds / 2),
	Elite::Vector2(center.x - bounds.x / 2, center.y + bounds.y / 2),
	Elite::Vector2(center + bounds / 2),
	Elite::Vector2(center.x + bounds.x / 2, center.y - bounds.y / 2),
	};

	m_pInterface->Draw_Polygon(points, 4, { 1.f, 0.f, 0.f });
}

//Update
//This function calculates the new SteeringOutput, called once per frame
SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{

	auto steering = SteeringPlugin_Output();

	//GETTING INFO
	AgentInfo aInfo = m_pInterface->Agent_GetInfo();
	vector<EntityInfo> entities = GetEntitiesInFOV();
	vector<HouseInfo> houses = GetHousesInFOV();


	m_ZombieHordeInfo.Update(dt);
	m_ExplorationGrid.Update(dt, aInfo);

	EnemyInfo tempEnemy{};
	ItemInfo tempItem{};


	//record entities
	for (const auto& ent : entities)
	{
		switch (ent.Type)
		{
		case eEntityType::ENEMY:
			m_pInterface->Enemy_GetInfo(ent, tempEnemy);
			m_ZombieHordeInfo.AddEnemy(tempEnemy);
			break;
		case eEntityType::ITEM:
			m_pInterface->Item_GetInfo(ent, tempItem);
			m_KnownItems.insert(std::pair<int, ItemInfo>(tempItem.ItemHash, tempItem));
			break;
		case eEntityType::PURGEZONE:
			break;
		default:
			break;
		}
	}

	//record houses
	for (const HouseInfo& hi : houses)
	{
		m_KnownHouses.insert(hi);
	}



	float distanceToAgent{};
	//choose if the agent should run  and how much it should prioritize fleeing
	if (m_ZombieHordeInfo.GetRecordedEnemies().size() > 0)
	{
		float closestEnemyDistance{ ClosestEnemyDistance(m_ZombieHordeInfo.GetRecordedEnemies()) };
		if (closestEnemyDistance < m_RunRange && aInfo.Stamina > 5.f)
		{
			steering.RunMode = true;
		}

		float newFleeWeight{ (m_VisionRange * m_VisionRange) / (closestEnemyDistance * closestEnemyDistance) };
		m_pEvasiveSeek->SetBehaviorWeight(1, newFleeWeight);

	}
	else
	{
		steering.RunMode = false;
		m_pEvasiveSeek->SetBehaviorWeight(1, 0.f);
	}

	//update behaviors
	m_pZombieEvadeBehavior->SetZombieInfo(m_ZombieHordeInfo);
	m_pAimBehavior->SetZombieInfo(m_ZombieHordeInfo);
	m_pSeekBehavior->SetTargetInfo(TargetInfo(m_pInterface->NavMesh_GetClosestPathPoint(m_Target), Elite::Vector2()));

	SteeringOutput movementOutput = m_pEvasiveSeek->CalculateSteering(dt, aInfo);

	steering.LinearVelocity = movementOutput.LinearVelocity;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= aInfo.MaxLinearSpeed;

	steering.AngularVelocity = m_pAimBehavior->CalculateSteering(dt, aInfo).AngularVelocity;

	return steering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}

vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}

float Plugin::ClosestEnemyDistance(const std::vector<EnemyRecord>& enemies) const
{
	float toReturn{ FLT_MAX };

	Elite::Vector2 agentPos = m_pInterface->Agent_GetInfo().Position;
	auto it = std::min_element(enemies.begin(), enemies.end(), [agentPos](const EnemyRecord& e1, const EnemyRecord& e2) {return Elite::DistanceSquared(agentPos, e1.Position) < Elite::DistanceSquared(agentPos, e2.Position); });

	if (it != enemies.end())
	{
		return Elite::Distance(agentPos, it->Position);
	}


	return toReturn;
}
