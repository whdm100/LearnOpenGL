#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "GLImp.h"

class Image
{
public:
    Image() : _buff(nullptr), _length(0){};
    ~Image() { UnloadImage(); };

    GLRESULT LoadImage(const char *name)
    {
        int width, height;
        _buff = SOIL_load_image
            (
            "D:\\Work\\Codes\\vs2013\\OpenGLTest\\OpenGLTest\\tex.jpg",
            &width, &height, nullptr, SOIL_LOAD_RGB
            );
    }
    void UnloadImage();

    const unsigned char* ImageBuff() { return _buff; }
    int ImageLength() { return _length; }

private:
    unsigned char *_buff;
    int _length;
};

#endif // __TEXTURE_H