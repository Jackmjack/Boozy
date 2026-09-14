#define MAX_LIGHTS 8

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

vec3 LitSurface(vec3 N, vec3 worldPos)
{
    N = normalize(N);
    vec3 result = u_AmbientColor;

    for (int i = 0; i < u_LightCount; i++)
    {
        if (u_LightType[i] == 0) // Directional
        {
            vec3 L = normalize(-u_LightDirection[i]); // 从物体指向光源
            float NdotL = max(dot(N, L), 0.0);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL;
        }
        else if (u_LightType[i] == 1) // Point
        {
            vec3 L = normalize(u_LightPosition[i] - worldPos);
            float NdotL = max(dot(N, L), 0.0);
            float d = length(u_LightPosition[i] - worldPos);
            float attenuation = 1.0 / (u_LightConstant[i] + u_LightLinear[i] * d + u_LightQuadratic[i] * d * d);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation;
        }
        else if (u_LightType[i] == 2)
        {
            vec3 L = normalize(u_LightPosition[i] - worldPos);
            float NdotL = max(dot(N, L), 0.0);
            float d = length(u_LightPosition[i] - worldPos);
            float attenuation = 1.0 / (u_LightConstant[i] + u_LightLinear[i] * d + u_LightQuadratic[i] * d * d);
            float theta = dot(L, normalize(-u_LightDirection[i])); // 从物体指向光源
            float eps = max(u_LightCosInner[i] - u_LightCosOuter[i], 1e-4);
            float spot = clamp((theta - u_LightCosOuter[i]) / eps, 0.0, 1.0);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation * spot;
        }
    }

    return result;
}