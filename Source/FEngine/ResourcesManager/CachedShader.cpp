//#include <FEngine/ResourcesManager/CachedShader.h>
//
//#include <cstring>
//#include <utility>
//
//namespace feng
//{
//    CachedShader::CachedShader(const CachedShader& other)
//        : CachedResource{other}
//        , States{other.States}
//        , Fs{}
//    {
//    }
//
//    CachedShader::CachedShader(CachedShader&& other) :
//        CachedResource(std::move(other)),
//        States(other.States),
//        Vs(other.Vs),
//        Fs(other.Fs)
//    {
//        other.Vs = other.Fs = nullptr;
//    }
//
//    CachedShader& CachedShader::operator = (const CachedShader& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(other);
//
//            States = other.States;
//            if (other.Vs)
//            {
//                char *temp = new char[strlen(other.Vs) + 1];
//                strcpy(temp, other.Vs);
//
//                delete[] Vs;
//                Vs = temp;
//            }
//            else
//            {
//                delete[] Vs;
//                Vs = nullptr;
//            }
//
//            if (other.Fs)
//            {
//                char *temp = new char[strlen(other.Fs) + 1];
//                strcpy(temp, other.Fs);
//
//                delete[] Fs;
//                Fs = temp;
//            }
//            else
//            {
//                delete[] Fs;
//                Fs = nullptr;
//            }
//        }
//
//        return *this;
//    }
//
//    CachedShader& CachedShader::operator = (CachedShader&& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(std::move(other));
//
//            delete[] Vs;
//            delete[] Fs;
//
//            Vs = other.Vs;
//            Fs = other.Fs;
//            States = other.States;
//            other.Vs = other.Fs = nullptr;
//        }
//
//        return *this;
//    }
//}
