#ifndef __HARLAN_PAGE_H__
#define __HARLAN_PAGE_H__

#include "vertex.h"

#define RAD (180.0f / M_PI)

namespace harlan {

  class Page
  {
  public:
    Page () {
      width = 0.8f;
      height = 1.0f;
      columns = 20; // 8; // 20
      rows = 25; //10;   // 25

      theta = 90.0f;
      rotation = 0.0f;
      translation = 0.0f;

      inMesh = NULL;
      outMesh = NULL;
      texCoords = NULL;
      frontStrip = NULL;
      backStrip = NULL;

      createMesh ();
    }

    virtual ~Page () {
      if (inMesh != NULL)
        delete [] inMesh;
      if (outMesh != NULL)
        delete [] outMesh;
      if (texCoords != NULL)
        delete [] texCoords;
      if (frontStrip != NULL)
        delete [] frontStrip;
      if (backStrip != NULL)
        delete [] backStrip;
    }

    float funcLinear (float ft, float f0, float f1) {
      return f0 + (f1 - f0) * ft;
    }

    Vector3f* getVertices () {
      return outMesh;
    }

    Vector2f* getTexCoords () {
      return texCoords;
    }

    unsigned short *getFrontStrip () {
      return frontStrip;
    }

    unsigned short *getBackStrip () {
      return backStrip;
    }

    unsigned int stripLength () {
      return stripLen;
    }

  protected:
    void createMesh () {
      int cx = columns + 1;
      int cy = rows + 1;

      n_faces = columns * rows * 2;
      n_vertices = cx * cy;

      if (inMesh != NULL)
        delete [] inMesh;
      if (outMesh != NULL)
        delete [] outMesh;
      if (texCoords != NULL)
        delete [] texCoords;

      inMesh = new Vector2f[n_vertices];
      outMesh = new Vector3f[n_vertices];
      texCoords = new Vector2f[n_vertices];

      int i = 0;

      for (int iy = 0; iy < cy; iy++) {
        for (int ix = 0; ix < cx; ix++) {
          float px = (float)ix * width / columns;
          float py = (float)iy * height / rows;

          inMesh[i].x = px;
          inMesh[i].y = py;

          texCoords[i].x = (float)(cx - ix) / columns;
          texCoords[i].y = (float)(cy - iy) / rows;

          i++;
        }
      }

      stripify ();
      updateTime (0.0);
    }

    void stripify () {
      int cx = columns + 1;
      int cy = rows + 1;

      printf ("%d columns, %d rows\n", columns, rows);

      stripLen = (cx * 2) * (cy - 1) + (cy - 2);

      printf ("len == %d\n", stripLen);

      if (frontStrip != NULL)
        delete [] frontStrip;
      if (backStrip != NULL)
        delete [] backStrip;

      frontStrip = new unsigned short[stripLen];
      backStrip = new unsigned short[stripLen];

      int i = 0;
      int offset = 0;

      for (int iy = 0; iy < rows; iy++) {
        bool last = iy == (rows - 1);
        bool odd = iy & 1;
        offset = iy * cx;

        for (int ix = 0; ix <= columns; ix++) {
          if (odd) {
            frontStrip[i]  = offset + columns - ix + cx;
            backStrip[i++] = offset + ix + cx;
            frontStrip[i]  = offset + columns - ix;
            backStrip[i++] = offset + ix;
          } else {
            frontStrip[i]  = offset + ix + cx;
            backStrip[i++] = offset + columns - ix + cx;
            frontStrip[i]  = offset + ix;
            backStrip[i++] = offset + columns - ix;
          }
        }

        if (!last) {
          if (odd) {
            frontStrip[i]  = offset + cx;
            backStrip[i++] = offset + cx + columns;
          } else {
            frontStrip[i]  = offset + cx + columns;
            backStrip[i++] = offset + cx;
          }
        }
      }
    }

  public:
    void updateTime (float time) {
      float angle1 = 90.0f / RAD;
      float angle2 = 8.0f / RAD;
      float angle3 = 6.0f / RAD;
      float A1 = -15.0f;
      float A2 = -2.5f;
      float A3 = -3.5f;
      float theta1 = 0.05f;
      float theta2 = 0.5f;
      float theta3 = 10.0f;
      float theta4 = 2.0f;

      float f1, f2, dt;

      rotation = time * M_PI;

      if (time <= 0.15f) {
        dt = time / 0.15;
        f1 = sin (M_PI * pow (dt, theta1) / 2.0);
        f2 = sin (M_PI * pow (dt, theta2) / 2.0);
        theta = funcLinear (f1, angle1, angle2);
        translation = funcLinear (f2, A1, A2);
      } else if (time <= 0.4) {
        dt = (time - 0.15) / 0.25;
        theta = funcLinear (dt, angle2, angle3);
        translation = funcLinear (dt, A2, A3);
      } else if (time <= 1.0) {
        dt = (time - 0.4) / 0.6;
        f1 = sin (M_PI * pow (dt, theta3) / 2.0);
        f2 = sin (M_PI * pow (dt, theta4) / 2.0);
        theta = funcLinear (f1, angle3, angle1);
        translation = funcLinear (f2, A3, A1);
      }

      deform ();
    }

  protected:

    void deform () {
      Vector2f in;
      Vector3f tmp;
      Vector3f *out;

      float R, r, beta;

      for (int i = 0; i < n_vertices; i++) {
        in = inMesh[i];
        R = sqrt (in.x * in.x + pow (in.y - translation, 2.0f));
        r = R * sin (theta);
        beta = asin (in.x / R) / sin (theta);

        tmp.x = r * sin (beta);
        tmp.y = R + translation - r * (1.0f - cos (beta)) * sin (theta);
        tmp.z = r * (1.0f - cos (beta)) * cos (theta);

        out = &outMesh[i];
        out->x = (tmp.x * cos (rotation) - tmp.z * sin (rotation));
        out->y = tmp.y;
        out->z = (tmp.x * sin (rotation) + tmp.z * cos (rotation));
      }
    }

  private:

    int rows;
    int columns;
    float width;
    float height;
    float rotation;
    float theta;
    float translation;

    unsigned short *frontStrip;
    unsigned short *backStrip;
    unsigned int stripLen;

    int n_faces;
    int n_vertices;

    Vector2f *inMesh;
    Vector3f *outMesh;
    Vector2f *texCoords;
  };

}

#endif /* __HARLAN_PAGE_H__ */
