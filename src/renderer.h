#ifndef __HARLAN_RENDERER_H__
#define __HARLAN_RENDERER_H__

#include <math.h>

#include <SDL_image.h>

#include "vertex.h"
#include "page.h"

#define DEGREES_TO_RADIANS(degrees) (degrees / 180.0f * M_PI)

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

      setPointers = false;
      rotX = 45.0f;
      rotY = 90.0f;
      rotZ = 0.0f;
      transX = 0.0f;
      transY = 0.0f;
      transZ = 0.0f;

      time = 0.0;

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
      const GLfloat znear = 0.01f;
      const GLfloat zfar = 100.0f;
      const GLfloat view = 40.0f;
      GLfloat size = znear * tanf(DEGREES_TO_RADIANS (view) / 2.0f);
      GLfloat aspect = (GLfloat)width / height;

      glClearColor (0, 0, 0, 0);

      glViewport (0, 0, width, height);

      glMatrixMode (GL_PROJECTION);
      glFrustum (-size, size, -size / aspect, size / aspect, znear, zfar);

      glTranslatef(0.0f, -0.025f, 0.85f);

      glMatrixMode (GL_MODELVIEW);

      glEnable (GL_DEPTH_TEST);
    }

    void initializeTextures () {
      GLenum textureFormat;

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

    void drawPage (Page *page, GLfloat scale) {
      glPushMatrix ();

      if (!setPointers) {
        glVertexPointer (3, GL_FLOAT, 0, page->getVertices());
        glTexCoordPointer (2, GL_FLOAT, 0, page->getTexCoords());

        setPointers = true;
      }

      glRotatef (270.0f, 1.0f, 0.0f, 0.0f);
      glScalef (0.05f * scale, 0.05f * scale, 0.05f * scale);
      glTranslatef (0.0f, 0.0f, -20.0f);

      glDrawElements (GL_TRIANGLE_STRIP, page->stripLength(),
                      GL_UNSIGNED_SHORT, page->getFrontStrip ());

      glPopMatrix ();
    }

#define ROT_FACTOR 0.05

    void update (Page *page) {
      page->updateTime (time);

      time += 0.01;
      if (time >= 1.0)
        time -= 1.0;

      /*
      rotY += 20.0f;
      if (rotY > 360.0f)
        rotY -= 360.0f;
      */

      /*
      rotX += 360.0f * ROT_FACTOR;
      rotY += 360.0f * ROT_FACTOR;

      while (rotX >= 360.0f)
        rotX -= 360.0f;
      while (rotY >= 360.0f)
        rotY -= 360.0f;

      printf ("rot: %f, %f\n", rotX, rotY);
      */
    }

    void render (Page *page) {
      Vector3f *vertices = page->getVertices ();
      Vector2f *texCoords = page->getTexCoords ();

      glLoadIdentity ();
      glClearColor (0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      rotX = 90.0f;
      rotY = 0.0f;

      update (page);

      glBindTexture (GL_TEXTURE_2D, texture[0]);

      glEnable (GL_TEXTURE_2D);

      glEnableClientState (GL_VERTEX_ARRAY);
      glEnableClientState (GL_TEXTURE_COORD_ARRAY);

      glPushMatrix ();
      glTranslatef (transX, transY, transZ);
      glRotatef (rotX, 1.0f, 0.0f, 0.0f);
      glRotatef (rotY, 0.0f, 1.0f, 0.0f);
      glRotatef (rotZ, 0.0f, 0.0f, 1.0f);

      drawPage (page, 1.0f);

      glPopMatrix ();
      glDisable (GL_TEXTURE_2D);
      glDisableClientState (GL_VERTEX_ARRAY);
      glDisableClientState (GL_TEXTURE_COORD_ARRAY);
    }

  protected:
    GLint width;
    GLint height;
    bool  setPointers;

    GLuint defaultFramebuffer;
    GLuint colorRenderbuffer;
    GLuint texture[2];

    GLfloat transX;
    GLfloat transY;
    GLfloat transZ;
    GLfloat rotX;
    GLfloat rotY;
    GLfloat rotZ;

    float time;
  };

}

#endif /* __HARLAN_RENDERER_H__ */
