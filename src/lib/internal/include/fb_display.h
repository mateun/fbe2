#ifndef FB_RENDERER
#define FB_RENDERER

#include "fb_export_spec.h"
#include "fb_result.h"

// This forward declaration is needed 
// to compile. For some reason I can not
// include SDL.h here... ?!
struct SDL_Window;

class FB_ENGINE_API Display {
private:
	int _width;
	int _height;
	bool _fullScreen;
	SDL_Window *_window;
	void *_glContext;
	Uint8* _keyState;


public:
	// Initializes the SDL subsystem
	Display();

	// Creates a window with the given dimensions.
	FBResult ShowWindow(int width, int height, bool fullScreen);

	// Polls for OS events for the given window.
	void PollForEvents();

	// Clear the backbuffer to the color.
	void Clear(float r, float g, float b);

	// Flip the backbuffer and present
	// its contents on the screen.
	void Present();

	bool IsKeyDown(char key);
	bool IsKeyDown(SDL_Scancode scancode);
	
};
#endif