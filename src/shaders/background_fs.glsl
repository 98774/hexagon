#version 330 core
out vec4 FragColor;

in vec3 FragPos;
//in vec3 ourColor;
//in vec2 TexCoord;

uniform vec3 playerColor;
//uniform sampler2D texture2;

void main()
{
    FragColor = vec4(0.5+ FragPos.x,0.5+FragPos.y, 0.1, 1.0f);
  //  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue);
}
