//#include <FEngine/Serialization/SerializationNode.h>
//
//#include <cstring>
//#include <vector>
//
//namespace feng
//{
//    SerializationNode::~SerializationNode()
//    {
//        delete[] m_name;
//    }
//
//    SerializationNode::SerializationNode(const SerializationNode& other):
//        m_floats(other.m_floats),
//        m_longs(other.m_longs)
//    {
//        SetName(other.m_name);
//    }
//
//    SerializationNode& SerializationNode::operator = (const SerializationNode& other)
//    {
//        if (this != &other)
//        {
//            m_floats = other.m_floats;
//            m_longs = other.m_longs;
//            SetName(other.m_name);
//        }
//
//        return *this;
//    }
//
//    void SerializationNode::SetName(const char *name)
//    {
//        char *nameCopy = new char[strlen(name) + 1];
//        strcpy(nameCopy, name);
//        delete[] m_name;
//        m_name = nameCopy;
//    }
//
//    const char * SerializationNode::GetName() const
//    {
//        return m_name;
//    }
//
//    bool SerializationNode::SetFloat(const char *key, float value)
//    {
//        if (m_floats.HasKey(key))
//        {
//            return false;
//        }
//
//        m_floats.Add(key, value);
//        return true;
//    }
//
//    bool SerializationNode::SetFloatsArray(const char *key, float *values, int count)
//    {
//        if (m_floatArrays.HasKey(key))
//        {
//            return false;
//        }
//
//        m_floatArrays.Add(key, std::vector<float>(values, count));
//        return true;
//    }
//
//    bool SerializationNode::SetLong(const char *key, long value)
//    {
//        if (m_longs.HasKey(key))
//        {
//            return false;
//        }
//
//        m_longs.Add(key, value);
//        return true;
//    }
//
//    bool SerializationNode::SetBool(const char *key, bool value)
//    {
//        if (m_longs.HasKey(key))
//        {
//            return false;
//        }
//
//        m_longs.Add(key, value);
//        return true;
//    }
//
//    bool SerializationNode::TryGetFloat(const char *key, float& value) const
//    {
//        return m_floats.TryGetValue(key, value);
//        return false;
//    }
//
//    bool SerializationNode::TryGetLong(const char *key, long& value) const
//    {
//        return m_longs.TryGetValue(key, value);
//        return false;
//    }
//
//    bool SerializationNode::TryGetBool(const char *key, bool& value) const
//    {
//        long longValue;
//        if (m_longs.TryGetValue(key, longValue))
//        {
//            value = longValue;
//            return true;
//        }
//
//        return false;
//    }
//
//    bool SerializationNode::TryGetFloatsArray(const char *key, float *&values) const
//    {
//        if (m_floatArrays.HasKey(key))
//        {
//            std::vector<float> vector = m_floatArrays[key];
//            values = vector.GetArray();
//            return true;
//        }
//
//        return false;
//    }
//}
