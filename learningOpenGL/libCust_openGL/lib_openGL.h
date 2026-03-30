//
// Created by brii on 27/03/26.
//

#ifndef LEARNING_LIB_OPENGL_H
#define LEARNING_LIB_OPENGL_H

#include "learningOpenGL.h"

namespace lib_SHADER_openGL
{
  inline void set_glUniform1i(const int& location, const int& value)
  {
    glUniform1i(location, value);
  }
  inline void set_glUniform1f(const int& location, const float& value)
  {
    glUniform1f(location, value);
  }
  inline void set_glUniform1f_array(const int& location, const std::vector<float>& values)
  {
    glUniform1fv(location, values.size(), values.data());
  }
  inline void set_glUniform1f_array_ptr(const int& location, const std::vector<float*>& values)
  {
    glUniform1fv(location, values.size(), *values.data());
  }
  inline void set_glUniform2f(const int& location, const glm::vec2& value)
  {
    glUniform2f(location, value.x, value.y);
  }
  inline void set_glUniform3fv(const int& location, const glm::vec3& value)
  {
    glUniform3fv(location, GL_TRUE, glm::value_ptr(value));
  }
  inline void set_glUniformMatrix3fv(const int& location, const glm::mat3& value)
  {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }
  inline void set_glUniformMatrix4fv(const int& location, const glm::mat4& value)
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }

  inline void set_glUniformMatrix4fv_array(const int& location, const std::vector<glm::mat4>& values)
  {
   glUniformMatrix4fv(location, values.size(), GL_FALSE, glm::value_ptr(values[0]));
  }

  inline void set_glUniformMatrix4fv_array_ptr(const int& location, const std::vector<glm::mat4*>& values)
  {
    glUniformMatrix4fv(location, values.size(), GL_FALSE, glm::value_ptr(*values[0]));
  }

}


#endif //LEARNING_LIB_OPENGL_H