#pragma once

#include "GuidGenerator.h"
#include "Globals.h"

namespace fengine
{
    class Object
    {
    public:

	virtual ~Object() {};
	inline obj_id GetId() const;
	virtual void Destroy() = 0;

protected:

	enum class ObjectType
	{
		SceneObject = 0,
		Resource = 1
	};

	inline Object(ObjectType type);

private:

	obj_id m_id = 0;

};

inline fengine::obj_id Object::GetId() const
{
	return m_id;
}

inline Object::Object(ObjectType type)
{
	if (type == ObjectType::SceneObject)
	{
		m_id = GuidGenerator::GenerateNextSceneObjectId();
	}
	else
	{
		m_id = GuidGenerator::GenerateNextResourceId();
	}
}

}
