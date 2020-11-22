//#pragma once
//
//namespace fengine
//{
//    class SerializableObject;
//    class SerializationNode;
//
//    // TODO: m.alekseev Get rid of this class.
//    class FILE;
//
//    class SerializableObjectParser final
//    {
//    public:
//        static void ReadSceneObject(FILE *file, SerializableObject *& object, SerializationNode *& node);
//        static void ReadResource(FILE *file, SerializableObject *& object, SerializationNode *& node);
//        static SerializableObject * ReadResource(FILE *file);
//
//    private:
//        static void ReadObject(FILE *file, SerializableObject *& object, SerializationNode *& node, bool isSceneObject);
//        static void ReadComponentInfo(FILE *file, SerializationNode *node);
//        static void ReadFloatsArray(FILE *file, float *values, int count);
//        static bool IsLongValue(const char *key);
//        static bool IsFloatArray(const char *key);
//
//        static const char *k_objectIdTag;
//        static const char *k_serializedObjectInfoStart;
//        static const char *k_serializedObjectInfoEnd;
//        static const char *k_longValuePrefix;
//        static const char *k_floatValuePrefix;
//        static const char *k_floatPointerPrefix;
//        static const int k_prefixLength = 2;
//    };
//}
