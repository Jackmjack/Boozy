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

#define MAX_LIGHTS 8

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_UV;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

// Lights
uniform vec3 u_AmbientColor;

uniform int   u_LightCount;
uniform int   u_LightType[MAX_LIGHTS];
uniform vec3  u_LightColor[MAX_LIGHTS];
uniform float u_LightIntensity[MAX_LIGHTS];

// Point and Spot
uniform vec3  u_LightPosition[MAX_LIGHTS];

// Directional and Spot
uniform vec3  u_LightDirection[MAX_LIGHTS];

// Point
uniform float u_LightConstant[MAX_LIGHTS];
uniform float u_LightLinear[MAX_LIGHTS];
uniform float u_LightQuadratic[MAX_LIGHTS];

// Spot
uniform float u_LightCosInner[MAX_LIGHTS];
uniform float u_LightCosOuter[MAX_LIGHTS];

void main()
{
    vec3 result = u_AmbientColor;
    for (int i = 0; i < u_LightCount; i++)
    {
        if (u_LightType[i] == 0)
        {
            vec3 N = normalize(v_Normal);
            vec3 L = normalize(-u_LightDirection[i]); // 从物体指向光源
            float NdotL = max(dot(N, L), 0.0);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL;
        }
        else if (u_LightType[i] == 1)
        {
            vec3 N = normalize(v_Normal);
            vec3 L = normalize(u_LightPosition[i] - v_WorldPos);
            float NdotL = max(dot(N, L), 0.0);
            float d = length(u_LightPosition[i] - v_WorldPos);
            float attenuation = 1.0 / (u_LightConstant[i] + u_LightLinear[i] * d + u_LightQuadratic[i] * d * d);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation;
        }
        else if (u_LightType[i] == 2)
        {
            vec3 N = normalize(v_Normal);
            vec3 L = normalize(u_LightPosition[i] - v_WorldPos);
            float NdotL = max(dot(N, L), 0.0);
            float d = length(u_LightPosition[i] - v_WorldPos);
            float attenuation = 1.0 / (u_LightConstant[i] + u_LightLinear[i] * d + u_LightQuadratic[i] * d * d);
            float theta = dot(L, normalize(-u_LightDirection[i])); // 从物体指向光源
            float eps = max(u_LightCosInner[i] - u_LightCosOuter[i], 1e-4);
            float spot = clamp((theta - u_LightCosOuter[i]) / eps, 0.0, 1.0);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation * spot;
        }
    }

    color = vec4(texture(u_Texture, v_UV).rgb * u_Color.rgb * result, u_Color.a);
}