#include <stdio.h>

#define GL_GLEXT_PROTOTYPES

#include <SDL.h>
#include <SDL_opengl.h>

#include "renderer.h"
#include "page.h"

using namespace harlan;

int main (int argc, char **argv)
{
  SDL_Surface *window = NULL;
  SDL_Event    event;
  int          loop = 1;
  Renderer    *renderer = NULL;
  Page        *page = NULL;

  if ((SDL_Init (SDL_INIT_VIDEO | SDL_INIT_TIMER) == -1))
    {
      printf ("Failed to initialize SDL: %s.\n", SDL_GetError ());
      return 0;
    }

  SDL_WM_SetCaption ("Page Curl", NULL);

  SDL_GL_SetAttribute (SDL_GL_RED_SIZE,       8);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE,     8);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE,     16);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,     32);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER,   1);

  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);

  window = SDL_SetVideoMode (800, 600, 0,
                             SDL_OPENGL | SDL_RESIZABLE | SDL_HWSURFACE | SDL_DOUBLEBUF);

  if (!window)
    {
      printf ("Failed to open window: %s.\n", SDL_GetError ());
      return 0;
    }

  renderer = new Renderer (800, 600);
  page = new Page ();

  while (loop)
    {
      SDL_PollEvent (&event);

      switch (event.type)
        {
        case SDL_QUIT:
          loop = 0;
          break;

        case SDL_MOUSEBUTTONDOWN:
          break;

        case SDL_KEYDOWN:
          break;

        case SDL_VIDEORESIZE:
          break;

        default:
          break;
        }

      /*
       * Update animations, render, and shit.
       */
      renderer->render (page);

      SDL_UpdateRect (window, 0, 0, 800, 600);

      SDL_GL_SwapBuffers();

      SDL_Delay (20);
    }

  SDL_Quit ();

  return 0;
}
