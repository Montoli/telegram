//Using SDL and standard IO
#include <SDL.h>

#include <stdio.h>

#include "GL/glew.h"

#include "states/state_manager.h"
#include "states/main_state.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int TARGET_FPS = 60;
const float TARGET_FRAME_TIME = 1000 / TARGET_FPS;

void PrintGLString(GLenum name)
{
  const GLubyte* ret = glGetString(name);
  if (ret == 0)
    SDL_Log("Failed to get GL string: %d\n", name);
  else
    SDL_Log("%s\n", ret);
}


int main(int argc, char* args[])
{
  //The window we'll be rendering to
  SDL_Window* window = NULL;

  //The surface contained by the window
  SDL_Surface* screen_surface = NULL;


  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }
  else
  {
    //Create window
    window = SDL_CreateWindow(
			"Telegram", 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH,
			SCREEN_HEIGHT, 
			SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {



			SDL_GLContext gl_context = SDL_GL_CreateContext(window);
      SDL_Log("GL_VERSION: ");
      PrintGLString(GL_VERSION);
      SDL_Log("GL_RENDERER: ");
      PrintGLString(GL_RENDERER);
      SDL_Log("GL_SHADING_LANGUAGE_VERSION: ");
      PrintGLString(GL_SHADING_LANGUAGE_VERSION);
//SDL_Log("GL_EXTENSIONS: ");
//      PrintGLString(GL_EXTENSIONS);


			// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
			//SDL_GL_DeleteContext(glcontext);

			// Set our OpenGL version.
			// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

			// Turn on double buffering with a 24bit Z buffer.
			// You may need to change this to 16 or 32 for your system
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			// This makes our buffer swap syncronized with the monitor's vertical refresh
			SDL_GL_SetSwapInterval(1);


			// weird - seems like this has to be initted AFTER SDL has created
			// the context?
			glewInit();
      SDL_GL_SetSwapInterval(0);

      StateManager state_manager;

      state_manager.PushState(new MainState(window, screen_surface, gl_context,
				SCREEN_WIDTH, SCREEN_HEIGHT));


      int frame_counter = 0;
      Uint64 end_of_second = SDL_GetPerformanceCounter() + SDL_GetPerformanceFrequency();
      Uint64 now;

      Uint64 next_frame_update = SDL_GetPerformanceCounter();
      const Uint64 time_between_updates = SDL_GetPerformanceFrequency() / TARGET_FPS;

      while (!state_manager.IsAppQuitting()) {
        Uint64 current_time = SDL_GetPerformanceCounter();

        // regulate our framerate - block until it's time to move on.
        while (current_time < next_frame_update) {
          current_time = SDL_GetPerformanceCounter();
        }
        next_frame_update += time_between_updates;
        if (next_frame_update < current_time) next_frame_update = current_time;

        state_manager.Update(TARGET_FRAME_TIME);
        state_manager.Render(TARGET_FRAME_TIME);

        // track framerate:
        frame_counter++;
        now = SDL_GetPerformanceCounter();
        if (now > end_of_second) {
          SDL_Log("FPS: %d", frame_counter);
          frame_counter = 0;
          end_of_second = SDL_GetPerformanceCounter() + SDL_GetPerformanceFrequency();
        }

			}
			SDL_GL_DeleteContext(gl_context);
		}
  }

  //Destroy window
	SDL_DestroyWindow(window);

  //Quit SDL subsystems
  SDL_Quit();

  return 0;
}