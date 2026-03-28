#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

class TextureLoader
{
public:
    // Returns the OpenGL Texture ID
    static unsigned int LoadTexture(const char *path);
};

#endif