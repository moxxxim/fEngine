#include "GuidGenerator.h"

fengine::obj_id fengine::GuidGenerator::s_nextSceneObjectId = 1;
fengine::obj_id fengine::GuidGenerator::s_nextResourceId = 1000000000;
fengine::obj_id fengine::GuidGenerator::s_currentMaxSceneObjectId = 1;
fengine::obj_id fengine::GuidGenerator::s_currentMaxResourceId = 1000000000;

void fengine::GuidGenerator::ResetNextSceneObjectId()
{
	s_currentMaxSceneObjectId = s_nextSceneObjectId = 0;
}

void fengine::GuidGenerator::SetNextResourceId(fengine::obj_id id)
{
	s_nextResourceId = id;
}

void fengine::GuidGenerator::SetNextSceneObjectId(fengine::obj_id id)
{
	s_nextSceneObjectId = id;
}

unsigned long fengine::GuidGenerator::GenerateNextSceneObjectId()
{
	obj_id next = s_nextSceneObjectId;
	UpdateNextSceneObjectId(next);

	return next;
}

unsigned long fengine::GuidGenerator::GenerateNextResourceId()
{
	obj_id next = s_nextResourceId;
	UpdateNextResourceId(next);

	return next;
}

void fengine::GuidGenerator::UpdateNextSceneObjectId(obj_id current)
{
	if (current > s_currentMaxSceneObjectId)
	{
		s_currentMaxSceneObjectId = current;
	}

	s_nextSceneObjectId = s_currentMaxSceneObjectId + 1;
}

void fengine::GuidGenerator::UpdateNextResourceId(obj_id current)
{
	if (current > s_currentMaxResourceId)
	{
		s_currentMaxResourceId = current;
	}

	s_nextResourceId = s_currentMaxResourceId + 1;
}
