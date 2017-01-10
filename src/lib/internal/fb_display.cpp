#include "include\fb_engine.h"
#include <SDL.h>
#include <glew.h>

Display::Display() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Error during SDL init: %s\n", SDL_GetError());
		exit(1);
	}
	
	SDL_Log("SDL initialized\n");
}

FBResult Display::ShowWindow(int w, int h, bool fullScreen) {
	
	this->_width = w;
	this->_height = h;
	this->_fullScreen = fullScreen;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	Uint32 windowFlags = SDL_WINDOW_OPENGL;
	if (fullScreen) windowFlags |= SDL_WINDOW_FULLSCREEN;

	_window = SDL_CreateWindow("FB Game Engine Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		windowFlags);

	FBResult result;
		
	if (_window != NULL) {
		result.resultType = FB_RESULT_TYPE::OK;
	}
	else {
		result.resultType = FB_RESULT_TYPE::ERR;
	}

	_glContext = SDL_GL_CreateContext(_window);

	GLenum err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after creating the context for OpenGL: %d", err);
	}


	glewExperimental = GL_TRUE;
	err = glewInit();
	if (err != GLEW_OK) {
		SDL_Log("Error in Glew_Init, ex != GLEW_OK. %s", glewGetErrorString(err));
		exit(1);
	}
	else {
		SDL_Log("GLEW init ok %d", err);
	}

	err = glGetError();
	if (err != 0) {
		SDL_Log("GL error after initializing Glew: %d - ignoring! see http://stackoverflow.com/questions/10857335/opengl-glgeterror-returns-invalid-enum-after-call-to-glewinit", err);
		result.resultType = FB_RESULT_TYPE::ERR;
	}

	GLint majVer, minVer;
	glGetIntegerv(GL_MAJOR_VERSION, &majVer);
	glGetIntegerv(GL_MAJOR_VERSION, &minVer);
	SDL_Log("GL Version %d/%d", majVer, minVer);

	return result;
}

void Display::PollForEvents()
{
	SDL_assert(_window != NULL);

	SDL_Event event;
	while (SDL_PollEvent(&event) != NULL) {
		if (event.type == SDL_QUIT) {
			exit(0);
		}

		if (event.type == SDL_KEYDOWN) {
			// propagate keydown events to listeners
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				exit(0);
			}
		}
	}

	_keyState = (Uint8*) SDL_GetKeyboardState(NULL);
	
}

bool Display::IsKeyDown(SDL_Scancode scancode) {
	return (_keyState[scancode]);	
}

bool Display::IsKeyDown(char key) {
	return (_keyState[key]);
}

void Display::Clear(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::Present() {
	SDL_GL_SwapWindow(_window);
}

