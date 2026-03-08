//
// Created by brii on 08/03/26.
//

#ifndef BASIC_GEOMETRY_D
#define BASIC_GEOMETRY_D

#include "learningOpenGL.h"

namespace transformation_basics
{
    class basics_Model3D;

}

namespace geo_2D
{
  class point_geo
  {
   private:

    unsigned int VAO{};
    unsigned int VBO{};

    std::unique_ptr<transformation_basics::basics_Model3D> pointCoord{nullptr};
    glm::vec3 color{};
    float size{1.0f};
    std::string shaderID{};

    void setup_point(glm::vec3& posicion);

  public:

      point_geo();
      point_geo(glm::vec3 posicion, glm::vec3 color, float size, std::string shaderID);

      void draw();
      void setPosicion(glm::vec3 posicion);
      void setColor(glm::vec3 color);

  };


}


#endif //LEARNING_BASIC_GEOMETRY_D_H