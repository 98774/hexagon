#version 330 core
out vec4 FragColor;

in vec3 pos;
//in vec3 ourColor;
//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
//    FragColor = vec4(pos.x, pos.y, pos.z, 1.0f);
    FragColor = vec4(1.0f);
  //  FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), mixValue);
}
