#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "Steering\SteeringBehaviors.h"
#include "Steering\CombinedSteeringBehaviors.h"

#include <map>
#include <set>

class IBaseInterface;
class IExamInterface;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin() {};

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;
	vector<HouseInfo> GetHousesInFOV() const;
	vector<EntityInfo> GetEntitiesInFOV() const;

	float ClosestEnemyDistance(const std::vector<EnemyInfo>& enemies) const;


	//behaviors
	BlendedSteering* m_pEvasiveSeek = nullptr;

	EvadeZombies* m_pZombieEvadeBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Face* m_pFaceBehavior = nullptr;



	//information
	std::map<int, ItemInfo> m_KnownItems;
	std::set<HouseInfo, less<HouseInfo>> m_KnownHouses;


	//constant values
	const float m_GrabRange = 3.f;
	const float m_VisionRange = 16.f;
	const float m_RunRange = 8.f;

	//debug print
	const float m_DebugPrintInterval = 2.f;
	float m_DebugPrintTimer = 0.f;

	//mouse target
	Elite::Vector2 m_Target = {};

};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}