#version 330 core
out vec3 pos;

in vec3 aPos;
//in vec3 ourColor;
//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
  gl_Position = vec4(aPos, 1.0f);
  pos = aPos;
  //  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue);
}
