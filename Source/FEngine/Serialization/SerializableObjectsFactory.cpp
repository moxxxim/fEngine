//#include <cstring>
//#include "SerializableObjectsFactory.h"
//
//namespace fengine
//{
//    Vector<const char *> SerializableObjectsFactory::s_names;
//    Vector<SerializableObjectsFactory::SerialiableObjectFactory *> SerializableObjectsFactory::s_factories;
//
//    SerializableObject * SerializableObjectsFactory::GetSerializableObject(const char *name)
//    {
//        int factoryIndex = GetFactoryIndex(name);
//        if (factoryIndex < 0)
//        {
//            return nullptr;
//        }
//
//        SerialiableObjectFactory *factory = s_factories[factoryIndex];
//        return factory->CreateSerializableObject();
//    }
//
//    int SerializableObjectsFactory::GetFactoryIndex(const char *name)
//    {
//        unsigned int keysCount = s_names.GetSize();
//        for (unsigned i = 0; i < keysCount; ++i)
//        {
//            auto cachedName = s_names[i];
//            if (!std::strcmp(name, cachedName))
//            {
//                return i;
//            }
//        }
//
//        return -1;
//    }
//}
