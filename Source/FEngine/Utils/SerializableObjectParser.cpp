//#include <FEngine/Utils/SerializableObjectParser.h>
//
//#include <FEngine/App/GuidGenerator.h>
//#include <FEngine/Serialization/SerializableObjectsFactory.h>
//#include <FEngine/Utils/Debug.h>
//
//#include <cstring>
//
//namespace fengine
//{
//    const char *SerializableObjectParser::k_objectIdTag = "Id ";
//    const char *SerializableObjectParser::k_serializedObjectInfoStart = "[";
//    const char *SerializableObjectParser::k_serializedObjectInfoEnd = "]";
//    const char *SerializableObjectParser::k_longValuePrefix = "l_";
//    const char *SerializableObjectParser::k_floatValuePrefix = "f_";
//    const char *SerializableObjectParser::k_floatPointerPrefix = "F_";
//
//    void SerializableObjectParser::ReadSceneObject(FILE *file, SerializableObject *&object, SerializationNode *& node)
//    {
//        //ReadObject(file, object, node, true);
//    }
//
//    void SerializableObjectParser::ReadResource(FILE *file, SerializableObject *&object, SerializationNode *& node)
//    {
//        //ReadObject(file, object, node, false);
//    }
//
//    SerializableObject * SerializableObjectParser::ReadResource(FILE *file)
//    {
//        SerializableObject *object = nullptr;
//        SerializationNode *node = nullptr;
//
//        ReadObject(file, object, node, false);
//
//        if (object)
//        {
//            object->Deserialize(*node);
//            delete node;
//        }
//
//        return object;
//    }
//
//    void SerializableObjectParser::ReadObject(
//                                              FILE *file,
//                                              SerializableObject *&object,
//                                              SerializationNode *& node,
//                                              bool isSceneObject)
//    {
////        char objectName[256];
////        fengine::obj_id id;
////
////        fscanf(file, "%s ", objectName);
////        fscanf(file, k_objectIdTag);
////        fscanf(file, "%lu ", &id);
////
////        if (isSceneObject)
////        {
////            GuidGenerator::SetNextSceneObjectId(id);
////        }
////        else
////        {
////            GuidGenerator::SetNextResourceId(id);
////        }
////
////        object = SerializableObjectsFactory::GetSerializableObject(objectName);
////        if (object)
////        {
////            node = new SerializationNode();
////            node->SetName(objectName);
////            ReadComponentInfo(file, node);
////        }
////        else
////        {
////            const char *message[] = {
////                "Couldn't instantiate game object component: ",
////                objectName
////            };
////
////            fengine::Debug::LogError(message, 2);
////        }
//    }
//
//    void SerializableObjectParser::ReadComponentInfo(FILE *file, SerializationNode *node)
//    {
////        char key[128];
////        fscanf(file, k_serializedObjectInfoStart);
////        fscanf(file, " %s ", key);
////
////        while (std::strcmp(key, k_serializedObjectInfoEnd))
////        {
////            if (IsLongValue(key))
////            {
////                unsigned long value;
////                fscanf(file, "%li ", &value);
////                node->SetLong(key + k_prefixLength, value);
////            }
////            else if (IsFloatArray(key))
////            {
////                unsigned long count;
////                fscanf(file, "%i ", &count);
////                float *values = new float[count];
////                ReadFloatsArray(file, values, count);
////
////                if (count)
////                {
////                    node->SetFloatsArray(key + k_prefixLength, values, count);
////                }
////
////                delete[] values;
////            }
////            else
////            {
////                float value;
////                fscanf(file, "%f ", &value);
////                node->SetFloat(key + k_prefixLength, value);
////            }
////
////            fscanf(file, " %s ", key);
////        }
//    }
//
//    void SerializableObjectParser::ReadFloatsArray(FILE *file, float *values, int count)
//    {
////        fscanf_s(file, "[ ");
////
////        for (int i = 0; i < count; ++i)
////        {
////            float value;
////            fscanf(file, "%f ", &value);
////            values[i] = value;
////        }
////
////        fscanf_s(file, "] ");
//    }
//
//    bool SerializableObjectParser::IsLongValue(const char *key)
//    {
//        return !strncmp(k_longValuePrefix, key, k_prefixLength);
//    }
//
//    bool SerializableObjectParser::IsFloatArray(const char *key)
//    {
//        return !strncmp(k_floatPointerPrefix, key, k_prefixLength);
//    }
//}
