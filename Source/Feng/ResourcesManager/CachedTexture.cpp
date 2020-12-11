#include <Feng/ResourcesManager/CachedTexture.h>

#include <cstring>
#include <utility>

namespace feng
{
    CachedTexture::CachedTexture(const CachedTexture& other):
        CachedResource(other),
        WrapMode(other.WrapMode),
        Type(other.Type),
        MinFilter(other.MinFilter),
        MagFilter(other.MagFilter),
        GenerateMipMaps(other.GenerateMipMaps)
    {
        if (other.Location)
        {
            Location = new char[strlen(other.Location) + 1];
            strcpy(Location, other.Location);
        }
    }

    CachedTexture::CachedTexture(CachedTexture&& other):
        CachedResource(std::move(other)),
        WrapMode(other.WrapMode),
        Type(other.Type),
        MinFilter(other.MinFilter),
        MagFilter(other.MagFilter),
        GenerateMipMaps(other.GenerateMipMaps)
    {
        delete[] Location;

        Location = other.Location;
        other.Location = nullptr;
    }

    CachedTexture& CachedTexture::operator = (const CachedTexture& other)
    {
        if (this != &other)
        {
            CachedResource::operator=(other);
            if (other.Location)
            {
                char *temp = new char[strlen(other.Location) + 1];
                strcpy(temp, other.Location);

                delete[] Location;
                Location = temp;
                WrapMode = other.WrapMode;
                Type = other.Type;
                MinFilter = other.MinFilter;
                MagFilter = other.MagFilter;
                GenerateMipMaps = other.GenerateMipMaps;
            }
        }

        return *this;
    }

    CachedTexture& CachedTexture::operator = (CachedTexture&& other)
    {
        if (this != &other)
        {
            CachedResource::operator=(std::move(other));

            delete[] Location;
            Location = other.Location;
            WrapMode = other.WrapMode;
            Type = other.Type;
            MinFilter = other.MinFilter;
            MagFilter = other.MagFilter;
            GenerateMipMaps = other.GenerateMipMaps;

            other.Location = nullptr;
        }

        return *this;
    }
}
