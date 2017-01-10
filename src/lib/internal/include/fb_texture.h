#ifndef TEXTURE_H
#define TEXTURE_H

#include "fb_export_spec.h"
#include <SDL.h>
#include <glew.h>
#include <glm\glm.hpp>

#ifdef _WIN32
class FB_ENGINE_API Texture
#else
class Texture
#endif
{

public:
	Texture(const char* imageFileName);
	~Texture();
	GLuint GetTextureHandle();

private:
	GLuint _textureHandle;
	GLint _samplerLocation;

};


#endif