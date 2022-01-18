#pragma once

#include <Exam_HelperStructs.h>
#include <IExamInterface.h>

#include <set>

class ItemsManager
{
public:
	ItemsManager() = default;
	~ItemsManager() = default;

	void Update(float dt, IExamInterface* pInterface);
	void AddItem(EntityInfo ent);
	

private:
	std::vector<EntityInfo> m_Pisotls;
	std::vector<EntityInfo> m_Medkits;
	std::vector<EntityInfo> m_Foods;

	

};

