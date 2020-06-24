#version 330

layout(location=0) in vec3 vs_position_in;
layout(location=1) in vec2 vs_uv_in;


out vec2 uv;

void main()
{
    uv = vs_uv_in;
    //uv = (vs_position_in.xy+vec2(1,1))/2;
    gl_Position = vec4(vs_position_in, 1.0f);
}
