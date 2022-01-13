#pragma once

#include "../inc/Exam_HelperStructs.h"

struct EnemyRecord
{
	EnemyRecord(const Elite::Vector2& pos = { 0.f, 0.f }, const Elite::Vector2& vel = { 0.f, 0.f }, float life = 4.f)
		: Position{ pos }
		, LinearVelocity{ vel }
		, lifetime{ life }
	{
	}

	Elite::Vector2 Position;
	Elite::Vector2 LinearVelocity;

	float lifetime;
};


class ZombieHordeInfo
{
public:
	ZombieHordeInfo() = default;
	~ZombieHordeInfo() = default;


	const vector<EnemyInfo>& GetEnemiesInFov() const { return m_EnemiesInFov; }
	const vector<EnemyRecord> GetRecordedEnemies() const { return m_RecordeEnemyPositions; }


	void Update(float dt)
	{
		//decrease the recorded enemy pos lifetime
		for (EnemyRecord& e : m_RecordeEnemyPositions)
		{
			e.lifetime -= dt;
			e.Position += e.LinearVelocity * dt;
		}

		//remove if lifetime is done
		m_RecordeEnemyPositions.erase( std::remove_if(m_RecordeEnemyPositions.begin(), m_RecordeEnemyPositions.end(), [](const EnemyRecord& e) {return e.lifetime < 0.f; }), m_RecordeEnemyPositions.end());

		//reset the enemies in fov
		m_EnemiesInFov.resize(0);
	}

	void AddEnemy(const EnemyInfo& enInf)
	{
		m_EnemiesInFov.push_back(enInf);
		m_RecordeEnemyPositions.push_back(EnemyRecord(enInf.Location, enInf.LinearVelocity));
	}



private:
	vector<EnemyInfo> m_EnemiesInFov;
	vector<EnemyRecord> m_RecordeEnemyPositions;
};

