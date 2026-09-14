#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec3 v_WorldPos;
out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    vec4 world = u_Model * vec4(a_Position, 1.0);
    v_WorldPos = world.xyz;
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_UV = a_UV;
    gl_Position = u_ViewProjection * world;
}


#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_UV;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

#include "Lighting.glsl"

void main()
{
    color = vec4(texture(u_Texture, v_UV).rgb * u_Color.rgb * LitSurface(v_Normal, v_WorldPos), u_Color.a);
}