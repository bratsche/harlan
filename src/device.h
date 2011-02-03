#ifndef __HARLAN_RENDER_DEVICE_H__
#define __HARLAN_RENDER_DEVICE_H__

namespace harlan {
  namespace render {

    class Device
    {
      Device ();
      virtual ~Device ();

      virtual void flip () = 0;
    }

  }
}

#endif
