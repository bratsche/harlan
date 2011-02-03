#ifndef __HARLAN_VERTEXBUFFER_H__
#define __HARLAN_VERTEXBUFFER_H__

namespace harlan {

  class VertexBuffer
  {
  public:

    VertexBuffer (unsigned int type,
                  unsigned int vertices,
                  const void* data = 0);

  protected:
    virtual ~VertexBuffer() { }
  };

}

#endif /* __HARLAN_VERTEXBUFFER_H__ */
