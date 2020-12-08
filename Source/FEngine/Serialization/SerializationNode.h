//#pragma once
//
//#include <map>
//
//namespace feng
//{
//    class SerializationNode final
//    {
//    public:
//        SerializationNode() = default;
//        ~SerializationNode();
//
//        SerializationNode(const SerializationNode& other);
//        SerializationNode& operator = (const SerializationNode& other);
//
//        void SetName(const char *name);
//        const char * GetName() const;
//
//        bool SetFloat(const char *key, float value);
//        bool SetFloatsArray(const char *key, float *values, int count);
//        bool SetLong(const char *key, long value);
//        bool SetBool(const char *key, bool value);
//
//        bool TryGetLong(const char *key, long& value) const;
//        bool TryGetFloat(const char *key, float& value) const;
//        bool TryGetBool(const char *key, bool& value) const;
//        bool TryGetFloatsArray(const char *key, float *&values) const;
//
//    private:
//        char *m_name = nullptr;
//        std::map<const char *, float> m_floats;
//        std::map<const char *, std::vector<float>> m_floatArrays;
//        std::map<const char *, long> m_longs;
//    };
//}
