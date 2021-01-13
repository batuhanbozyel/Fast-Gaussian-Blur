#type vertex
#version 450 core
		
layout(location = 0) out vec2 v_TexCoord;

void main() 
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);

    gl_Position = vec4(-1.0f + x*2.0f, -1.0f+y*2.0f, 0.0f, 1.0f);
    v_TexCoord = vec2(x, y);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_TexCoord;

layout(std140, binding = 0) uniform TextureSize
{
	float u_Width;
    float u_Height;
};

uniform sampler2D u_Texture;						

uniform float u_Weight[7] = float[](0.09207, 0.090803, 0.087105, 0.081275, 0.073762, 0.065113, 0.055908);

void main()
{
	color = texture2D(u_Texture, v_TexCoord) * u_Weight[0];

    color += texture2D(u_Texture, v_TexCoord + vec2(1.0 / u_Width, 0.0)) * u_Weight[1];
    color += texture2D(u_Texture, v_TexCoord - vec2(1.0 / u_Width, 0.0)) * u_Weight[1];

    color += texture2D(u_Texture, v_TexCoord + vec2(2.0 / u_Width, 0.0)) * u_Weight[2];
    color += texture2D(u_Texture, v_TexCoord - vec2(2.0 / u_Width, 0.0)) * u_Weight[2];

    color += texture2D(u_Texture, v_TexCoord + vec2(3.0 / u_Width, 0.0)) * u_Weight[3];
    color += texture2D(u_Texture, v_TexCoord - vec2(3.0 / u_Width, 0.0)) * u_Weight[3];

    color += texture2D(u_Texture, v_TexCoord + vec2(4.0 / u_Width, 0.0)) * u_Weight[4];
    color += texture2D(u_Texture, v_TexCoord - vec2(4.0 / u_Width, 0.0)) * u_Weight[4];

    color += texture2D(u_Texture, v_TexCoord + vec2(5.0 / u_Width, 0.0)) * u_Weight[5];
    color += texture2D(u_Texture, v_TexCoord - vec2(5.0 / u_Width, 0.0)) * u_Weight[5];

    color += texture2D(u_Texture, v_TexCoord + vec2(6.0 / u_Width, 0.0)) * u_Weight[6];
    color += texture2D(u_Texture, v_TexCoord - vec2(6.0 / u_Width, 0.0)) * u_Weight[6];
}