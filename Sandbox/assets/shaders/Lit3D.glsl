#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec3 v_Normal;
out vec2 v_UV;

void main()
{
    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    v_UV = a_UV;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}


#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_UV;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform vec3 u_AmbientColor;

void main()
{
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(-u_LightDirection); // 从物体指向光源
    float NdotL = max(dot(N, L), 0.0);
    vec3 lighting = u_AmbientColor + u_LightColor * NdotL;
    color = vec4(texture(u_Texture, v_UV).rgb * u_Color.rgb * lighting, u_Color.a);
}