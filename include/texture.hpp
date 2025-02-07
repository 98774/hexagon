#ifndef TEXTURE_H
#define TEXTURE_H

/*
 * Code from Learn OpenGL
 * https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
 */
#include "stb_image.h"
#include <glad/glad.h>

#include <iostream>
#include <ostream>

#define MAX_TEXTURES 16
class TextureLoader {
public:
  struct TextureData {
    unsigned int id;
    char *texturePath;
    unsigned int format;
    unsigned int wrap_s;
    unsigned int wrap_t;
  };
  TextureData textures[MAX_TEXTURES];
  // constructor generates an object with available textures on the fly
  // ------------------------------------------------------------------------
  TextureLoader() {
    for (int i = 0; i < MAX_TEXTURES; i++) {
      textures[i].id = 0;
      textures[i].texturePath = nullptr;
      textures[i].format = GL_RGB;
      textures[i].wrap_s = GL_REPEAT;
      textures[i].wrap_t = GL_REPEAT;
    }
  }

  void load(unsigned int texture_index, const char *texturePath,
            unsigned int format) {
    // Bind to the correct texture
    glActiveTexture(indexToGLIndex(texture_index));
    glBindTexture(GL_TEXTURE_2D, textures[texture_index].id);
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // object)
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
  }
  // ------------------------------------------------------------------------
  //  void setMat4(const std::string &name, glm::mat4 value) const {
  //    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
  //                       glm::value_ptr(value));
  //  }
  void bind(unsigned int texture_index) {
    glActiveTexture(indexToGLIndex(texture_index));
    glBindTexture(GL_TEXTURE_2D, textures[texture_index].id);
  }

private:
  // Adds constant from GLAD file to give correct index
  unsigned int indexToGLIndex(unsigned int index) {
    if (index < 0 || index > MAX_TEXTURES)
      return -1;

    return GL_TEXTURE0 + index;
  }
};
#endif
