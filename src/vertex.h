#ifndef __HARLAN_RENDER_VERTEX_H__
#define __HARLAN_RENDER_VERTEX_H__

namespace harlan {

  struct Vector2f
  {
    float x;
    float y;
  };

  struct Vector2i
  {
    unsigned int x;
    unsigned int y;
  };

  struct Vector3f
  {
    float x;
    float y;
    float z;
  };

  struct Vector3i
  {
    unsigned int x;
    unsigned int y;
    unsigned int z;
  };

  struct Color
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
  };

}

#endif
