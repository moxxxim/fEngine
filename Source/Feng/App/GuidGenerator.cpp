#include "GuidGenerator.h"

feng::obj_id feng::GuidGenerator::s_nextSceneObjectId = 1;
feng::obj_id feng::GuidGenerator::s_nextResourceId = 1000000000;
feng::obj_id feng::GuidGenerator::s_currentMaxSceneObjectId = 1;
feng::obj_id feng::GuidGenerator::s_currentMaxResourceId = 1000000000;

void feng::GuidGenerator::ResetNextSceneObjectId()
{
	s_currentMaxSceneObjectId = s_nextSceneObjectId = 0;
}

void feng::GuidGenerator::SetNextResourceId(feng::obj_id id)
{
	s_nextResourceId = id;
}

void feng::GuidGenerator::SetNextSceneObjectId(feng::obj_id id)
{
	s_nextSceneObjectId = id;
}

unsigned long feng::GuidGenerator::GenerateNextSceneObjectId()
{
	obj_id next = s_nextSceneObjectId;
	UpdateNextSceneObjectId(next);

	return next;
}

unsigned long feng::GuidGenerator::GenerateNextResourceId()
{
	obj_id next = s_nextResourceId;
	UpdateNextResourceId(next);

	return next;
}

void feng::GuidGenerator::UpdateNextSceneObjectId(obj_id current)
{
	if (current > s_currentMaxSceneObjectId)
	{
		s_currentMaxSceneObjectId = current;
	}

	s_nextSceneObjectId = s_currentMaxSceneObjectId + 1;
}

void feng::GuidGenerator::UpdateNextResourceId(obj_id current)
{
	if (current > s_currentMaxResourceId)
	{
		s_currentMaxResourceId = current;
	}

	s_nextResourceId = s_currentMaxResourceId + 1;
}
