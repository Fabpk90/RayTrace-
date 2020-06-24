#version 330

uniform sampler2D tex;

in vec2 uv;
out vec4 color;
void main()
{
    //color = vec4(1.0f);
   // color = vec4(uv.x, uv.y, 0, 1.0f);
    color = texture2D(tex, uv);
}
