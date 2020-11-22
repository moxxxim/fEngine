//#include <FEngine/ResourcesManager/CachedMesh.h>
//
//#include <cstring>
//#include <utility>
//
//namespace fengine
//{
//    CachedMesh::CachedMesh(const CachedMesh& other):
//        CachedResource(other)
//    {
//        if (other.Location)
//        {
//            Location = new char[strlen(other.Location) + 1];
//            strcpy(Location, other.Location);
//        }
//    }
//
//    CachedMesh::CachedMesh(CachedMesh&& other) :
//        CachedResource(std::move(other)),
//        Location(other.Location)
//    {
//        other.Location = nullptr;
//    }
//
//    CachedMesh& CachedMesh::operator = (const CachedMesh& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(other);
//            if (other.Location)
//            {
//                char *temp = new char[strlen(other.Location) + 1];
//                strcpy(temp, other.Location);
//
//                delete[] Location;
//                Location = temp;
//            }
//        }
//
//        return *this;
//    }
//
//    CachedMesh& CachedMesh::operator = (CachedMesh&& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(std::move(other));
//
//            delete[] Location;
//            Location = other.Location;
//            other.Location = nullptr;
//        }
//
//        return *this;
//    }
//}
