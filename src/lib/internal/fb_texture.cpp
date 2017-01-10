#include "include\fb_texture.h"

Texture::Texture(const char* fileName) {
	SDL_Surface* _imageSurface;
	_imageSurface = SDL_LoadBMP(fileName);

	if (_imageSurface == NULL)
		SDL_Log("Error loading the bitmap: %s", SDL_GetError());

	GLenum err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after creating image surface: %d", err);
	}


	SDL_Log("Image w/h: %d/%d", _imageSurface->w, _imageSurface->h);

	_textureHandle = 0;
	glGenTextures(1, &_textureHandle);
	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after creating texture handle: %d", err);
	}


	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after binding the texture: %d", err);
	}

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		_imageSurface->w,
		_imageSurface->h,
		0,
		GL_BGR,
		GL_UNSIGNED_BYTE,
		_imageSurface->pixels);

	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after loading up texture: %d", err);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after setting up texture: %d", err);
	}

	if (_imageSurface)
		SDL_FreeSurface(_imageSurface);


	glBindTexture(GL_TEXTURE_2D, 0);

}


Texture::~Texture() {

}

GLuint Texture::GetTextureHandle() {
	return _textureHandle;
}