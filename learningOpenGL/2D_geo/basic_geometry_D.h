//
// Created by brii on 08/03/26.
//

#ifndef BASIC_GEOMETRY_D
#define BASIC_GEOMETRY_D

#include "learningOpenGL.h"
#include "GLM_test.h"

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

    bool buffer_build{ false };

    transformation_basics::basics_Model3D pointCoord{};
    glm::vec3 color{};
    float size{1.0f};
    std::string shaderID{};

    void setup_point(glm::vec3& posicion);

  public:

      point_geo();
      point_geo(glm::vec3 posicion, glm::vec3 color, float size, std::string shaderID);
      ~point_geo();

      void build_pointGeo(std::string shaderID);
      void draw();
      void setPosicion(glm::vec3 posicion);
      void setColor(glm::vec3 color);
      void setSize(float size);
      const bool& bufferBuild();

      void clean_data();

  };


}


#endif //LEARNING_BASIC_GEOMETRY_D_H