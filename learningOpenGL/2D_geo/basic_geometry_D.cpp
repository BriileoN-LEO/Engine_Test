//
// Created by brii on 08/03/26.
//

#include "basic_geometry_D.h"
#include "SHADER_H.h"
#include "Render/RenderData.h"
#include "GLM_test.h"

namespace geo_2D {
  void point_geo::setup_point(glm::vec3& posicion)
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &posicion, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);

    buffer_build = true;
  }

  point_geo::point_geo()
  {
    glm::vec3 pos{glm::vec3(0.0f)};

    ////transformation_basics::basics_Model3D coords{std::make_unique<transformation_basics::basics_Model3D>()};

    pointCoord.posModel = pos;
    pointCoord.posModel_Base = pos;

    //pointCoord = coords;
    setup_point(pos);

  };

  point_geo::point_geo(glm::vec3 posicion, glm::vec3 color, float size, std::string shaderID) :
  color(color),
  size(size),
  shaderID(shaderID)
  {
    // std::unique_ptr<transformation_basics::basics_Model3D> coords{std::make_unique<transformation_basics::basics_Model3D>()};

    pointCoord.posModel = posicion;
    pointCoord.posModel_Base = posicion;

    //pointCoord = std::move(coords);

    setup_point(posicion);
  }
  point_geo::~point_geo()
  {
    clean_data();
  }

  void point_geo::build_pointGeo(std::string shaderID)
  {
    this->shaderID = shaderID;
    setup_point(pointCoord.posModel);
  }

  void point_geo::draw()
  {
    shading::shader& shaderPt{ RenderData_Set::shader_D[shaderID] };

    shaderPt.use();
    shaderPt.transformMat("model", pointCoord.model);
    shaderPt.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
    shaderPt.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
    shaderPt.setFloat("sizePointer", size);
    shaderPt.setVec3("PointColor", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
  }

  void point_geo::draw_PrePass_editMode(shading::shader& shader)
  {
    shader.transformMat("model", pointCoord.model);
    shader.transformMat("view", cameras::cameras_D[cameras::name_CurrentCamera].cam);
    shader.transformMat("projection", cameras::cameras_D[cameras::name_CurrentCamera].camProjection);
    shader.setFloat("sizePointer", size);
    shader.setVec3("PointColor", color);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
  }

  void point_geo::setPosicion(glm::vec3 posicion)
  {
    pointCoord.posModel = posicion;
    glm::vec3 translate_pos { pointCoord.posModel_Base - posicion};
    translate_pos = pointCoord.posModel_Base - translate_pos;

    pointCoord.translateModel(translate_pos);
    pointCoord.setTransformsAll();  ///change this if not works

  }
  void point_geo::setColor(glm::vec3 color)
  {
    this->color = color;
  }
  void point_geo::setSize(float size)
  {
    this->size = size;
  }
  const bool& point_geo::bufferBuild()
  {
    return buffer_build;
  }
  void point_geo::clean_data()
  {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    color = glm::vec3(0.0f);
    size = 0.0f;
    shaderID = "";
    pointCoord.resetAll();

    buffer_build = false;
  }

}