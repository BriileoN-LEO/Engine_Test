//
// Created by brii on 08/03/26.
//

#ifndef SHADOWS_D
#define SHADOWS_D

#include "learningOpenGL.h"
//#include "Render/frameBuffers.h"

namespace frameBuff
{
 struct data_frameBuffer;

}

namespace shadows_manager {
 class shadow_map
 {
 private:

  std::unique_ptr<frameBuff::data_frameBuffer> dataBuffer{nullptr};

  void load_ShadowMap();

 public:

  shadow_map();

 };
}

#endif //SHADOWS_D