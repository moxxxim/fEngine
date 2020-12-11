//#pragma once
//
//#include <Feng/Serialization/SerializableObject.h>
//#include <vector>
//
//namespace feng
//{
//    class SerializableObjectsFactory final
//    {
//    public:
//        template <class T>
//        static void RegisterSerializableObject(const char *name);
//
//        template <class T>
//        static T * GetSerializableObject();
//
//        static SerializableObject * GetSerializableObject(const char *name);
//
//    private:
//        class SerialiableObjectFactory
//        {
//        public:
//            virtual SerializableObject * CreateSerializableObject() = 0;
//        };
//
//        template <class TObject>
//        class GenericSerializableObjectFactory final : public SerialiableObjectFactory
//        {
//        public:
//            virtual SerializableObject * CreateSerializableObject() override
//            {
//                return new TObject{};
//            }
//        };
//
//        static int GetFactoryIndex(const char *name);
//
//        static std::vector<const char *> s_names;
//        static std::vector<SerialiableObjectFactory *> s_factories;
//    };
//
//    template <class T>
//    void SerializableObjectsFactory::RegisterSerializableObject(const char *name)
//    {
//        int factoryIndex = GetFactoryIndex(name);
//        if (factoryIndex < 0)
//        {
//            s_names.Add(name);
//            s_factories.Add(new GenericSerializableObjectFactory<T>());
//        }
//    }
//
//    template<class T>
//    inline T * SerializableObjectsFactory::GetSerializableObject()
//    {
//        return T{};
//    }
//}
