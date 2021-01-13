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

uniform sampler2D u_Texture;

uniform float u_Weight[7] = float[](0.1963806152,  // 12870
                                    0.1745605468,  // 11440
                                    0.1221923828,  // 8008
                                    0.0666503906,  // 4368
                                    0.0277709960,  // 1820
                                    0.0085449218,  // 560
                                    0.0018310546); // 120

void main()
{
	color = texture2D(u_Texture, v_TexCoord) * u_Weight[0];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.001)) * u_Weight[1];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.001)) * u_Weight[1];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.002)) * u_Weight[2];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.002)) * u_Weight[2];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.003)) * u_Weight[3];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.003)) * u_Weight[3];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.004)) * u_Weight[4];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.004)) * u_Weight[4];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.005)) * u_Weight[5];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.005)) * u_Weight[5];

    color += texture2D(u_Texture, v_TexCoord + vec2(0.0, 0.006)) * u_Weight[6];
    color += texture2D(u_Texture, v_TexCoord - vec2(0.0, 0.006)) * u_Weight[6];
}