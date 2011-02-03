#ifndef __HARLAN_RENDERER_H__
#define __HARLAN_RENDERER_H__

#include <math.h>

#include <SDL_image.h>

#include "vertex.h"
#include "page.h"

#define DEG2RAD(degrees) (degrees / 180.0f * M_PI)

namespace harlan {

  class Renderer
  {
  public:

    Renderer (int w, int h) : width(w), height(h) {
      printf ("OpenGL version: %s\n", glGetString (GL_VERSION));
      printf ("OpenGL vendor: %s\n", glGetString (GL_VENDOR));
      printf ("OpenGL renderer: %s\n", glGetString (GL_RENDERER));

      /*
      glGenFramebuffersEXT (1, &defaultFramebuffer);
      glGenRenderbuffersEXT (1,&colorRenderbuffer);

      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT,defaultFramebuffer);
      glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, colorRenderbuffer);

      glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT,
                                    GL_COLOR_ATTACHMENT0_EXT,
                                    GL_RENDERBUFFER_EXT,
                                    colorRenderbuffer);
      */

      setup ();
      initializeTextures ();
    }

    virtual ~Renderer() {
      if (defaultFramebuffer) {
        glDeleteFramebuffersEXT (1, &defaultFramebuffer);
        defaultFramebuffer = 0;
      }

      if (colorRenderbuffer) {
        glDeleteRenderbuffersEXT (1, &colorRenderbuffer);
        colorRenderbuffer = 0;
      }

      glDeleteTextures (2, &texture[0]);
    }

    void setup () {
      glClearColor (0, 0, 0, 0);

      glViewport (0, 0, width, height);

      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();

      glOrtho (0, width, height, 0, 1, -1);

      glMatrixMode (GL_MODELVIEW);

      glEnable (GL_TEXTURE_2D);

      glLoadIdentity ();
    }

    void initializeTextures () {
      GLenum textureFormat;

      /*
      glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, defaultFramebuffer);

      const GLfloat near = 0.01f;
      const GLfloat far  = 100.0f;
      const GLfloat view = 40.0f;

      GLfloat size = near * tanf (DEG2RAD (view) / 2.0f);
      GLfloat aspect = (GLfloat)width / height;

      GLenum textureFormat;

      glViewport (0, 0, width, height);

      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();

      glOrtho(0, width, height, 0, 1, -1);

      glFrustum (-size, size, -size / aspect, size / aspect, near, far);
      //glTranslatef (0.0f, -0.5f, -2.5f);

      glEnable (GL_DEPTH_TEST);
      glEnable (GL_CULL_FACE);
      glEnable (GL_TEXTURE_2D);
      */

      SDL_Surface *surface;

      if ((surface = IMG_Load ("front512.png"))) {
        GLint n_colors = surface->format->BytesPerPixel;

        printf ("Loaded front512.png\n");

        if (n_colors == 4) {
          if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
          else
            textureFormat = GL_BGRA;
        } else if (n_colors == 3) {
          if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGB;
          else
            textureFormat = GL_BGR;
        }

        glGenTextures (2, &texture[0]);

        glBindTexture (GL_TEXTURE_2D, texture[0]);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D (GL_TEXTURE_2D, 0, n_colors, surface->w, surface->h,
                      0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface (surface);
      }

      if ((surface = IMG_Load ("back512.png"))) {
        GLint n_colors = surface->format->BytesPerPixel;

        printf ("Loaded back512.png\n");

        if (n_colors == 4) {
          if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGBA;
          else
            textureFormat = GL_BGRA;
        } else if (n_colors == 3) {
          if (surface->format->Rmask == 0x000000ff)
            textureFormat = GL_RGB;
          else
            textureFormat = GL_BGR;
        }

        glBindTexture (GL_TEXTURE_2D, texture[1]);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D (GL_TEXTURE_2D, 0, n_colors, surface->w, surface->h,
                      0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

        SDL_FreeSurface (surface);
      }
    }

    void resize () {
    }

    void render (Page *page) {
      Vector3f *vertices = page->getVertices ();
      Vector2f *texCoords = page->getTexCoords ();

      glMatrixMode (GL_MODELVIEW);
      glLoadIdentity ();

      glClearColor (0.2f, 0.2f, 0.2f, 1.0f);
      //glClearColor((128.0f / 255.0f), 1.0f, 1.0f, 1.0f);
      glClear (GL_COLOR_BUFFER_BIT);

      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_FLOAT, 0, vertices);
      glEnableClientState (GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer (2, GL_FLOAT, 0, texCoords);

      GLushort *front = page->getFrontStrip ();
      GLushort *back = page->getBackStrip ();
      GLuint len = page->stripLength ();

      glBindTexture (GL_TEXTURE_2D, texture[0]);
      glDrawElements (GL_TRIANGLE_STRIP, len, GL_UNSIGNED_SHORT, front);

      glBindTexture (GL_TEXTURE_2D, texture[1]);
      glDrawElements (GL_TRIANGLE_STRIP, len, GL_UNSIGNED_SHORT, back);

      glBindRenderbufferEXT (GL_RENDERBUFFER_EXT, colorRenderbuffer);
    }

  protected:
    GLint width;
    GLint height;

    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint texture[2];
  };

}

#endif /* __HARLAN_RENDERER_H__ */
