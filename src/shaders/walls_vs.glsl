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
    gl_Position = projection * view * model * vec4(aPos, 1.0);
  pos = aPos;
  //  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue);
}
