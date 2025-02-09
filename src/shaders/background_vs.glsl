#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;

uniform mat4 view;

out vec3 FragPos;  
//out vec3 Normal;
  
void main()
{
    gl_Position = vec4(aPos.xy, 0, 1.0);
    FragPos = aPos; //vec3(view * vec4(aPos, 1.0));
 //   Normal = mat3(transpose(inverse(model))) * aNormal;  
}  
