#define MAX_LIGHTS 8

uniform vec3 u_AmbientColor;

uniform int   u_LightCount;
uniform int   u_LightType[MAX_LIGHTS];
uniform vec3  u_LightColor[MAX_LIGHTS];
uniform float u_LightIntensity[MAX_LIGHTS];

// Point and Spot
uniform vec3 u_LightPosition[MAX_LIGHTS];

// Directional and Spot
uniform vec3 u_LightDirection[MAX_LIGHTS];

// Point
uniform float u_LightConstant[MAX_LIGHTS];
uniform float u_LightLinear[MAX_LIGHTS];
uniform float u_LightQuadratic[MAX_LIGHTS];

// Spot
uniform float u_LightCosInner[MAX_LIGHTS];
uniform float u_LightCosOuter[MAX_LIGHTS];

// Shadow
uniform sampler2DShadow u_ShadowMap[MAX_LIGHTS];
uniform int       u_HasShadow[MAX_LIGHTS];
uniform mat4      u_LightSpaceMatrix[MAX_LIGHTS];

// Point Shadow
uniform samplerCubeShadow u_ShadowCube[MAX_LIGHTS];
uniform int         u_HasShadowCube[MAX_LIGHTS];
uniform mat4        u_MatrixCube[MAX_LIGHTS * 6];

uniform float u_ShadowSoftness;

float ShadowVisibility2D(vec3 worldPos, int lightIndex)
{
    if (u_HasShadow[lightIndex] == 0) return 1.0;

    vec4 lp = u_LightSpaceMatrix[lightIndex] * vec4(worldPos, 1.0);
    vec3 c  = lp.xyz / lp.w * 0.5 + 0.5;
    if (c.x < 0.0 || c.x > 1.0 || c.y < 0.0 || c.y > 1.0 || c.z > 1.0) return 1.0;

    vec2 step = u_ShadowSoftness / vec2(textureSize(u_ShadowMap[lightIndex], 0));

    float vis = 0.0;
    for (int y = -1; y <= 1; ++y)
        for (int x = -1; x <= 1; ++x)
            vis += texture(u_ShadowMap[lightIndex], vec3(c.xy + vec2(x, y) * step, c.z));

    return vis * (1.0 / 9.0);
}

vec3 CubeDirFromFaceUV(int face, vec2 uv)
{
    vec2 t = uv * 2.0 - 1.0;                        // [0,1] → [-1,1]
    if (face == 0) return vec3( 1.0, -t.y, -t.x);   // +X
    if (face == 1) return vec3(-1.0, -t.y,  t.x);   // -X
    if (face == 2) return vec3( t.x,  1.0,  t.y);   // +Y
    if (face == 3) return vec3( t.x, -1.0, -t.y);   // -Y
    if (face == 4) return vec3( t.x, -t.y,  1.0);   // +Z
    return                vec3(-t.x, -t.y, -1.0);   // -Z
}

float ShadowVisibilityCube(vec3 worldPos, int lightIndex)
{
    if (u_HasShadowCube[lightIndex] == 0) return 1.0;

    vec3 v = worldPos - u_LightPosition[lightIndex];
    vec3 a = abs(v);
    int face = (a.x >= a.y && a.x >= a.z) ? (v.x > 0.0 ? 0 : 1)
             : (a.y >= a.z)               ? (v.y > 0.0 ? 2 : 3)
                                          : (v.z > 0.0 ? 4 : 5);

    vec4 lp = u_MatrixCube[lightIndex * 6 + face] * vec4(worldPos, 1.0);
    vec3 c  = lp.xyz / lp.w * 0.5 + 0.5;
    if (c.z > 1.0 || c.z < 0.0) return 1.0;

    float step = u_ShadowSoftness / float(textureSize(u_ShadowCube[lightIndex], 0).x);

    float vis = 0.0;
    for (int y = -1; y <= 1; ++y)
        for (int x = -1; x <= 1; ++x)
        {
            vec2 uv = c.xy + vec2(x, y) * step;
            vec3 d  = CubeDirFromFaceUV(face, uv);
            vis += texture(u_ShadowCube[lightIndex], vec4(d, c.z));
        }
    return vis * (1.0 / 9.0);
}

vec3 LitSurface(vec3 N, vec3 worldPos)
{
    N = normalize(N);
    vec3 result = u_AmbientColor;

    for (int i = 0; i < u_LightCount; i++)
    {
        if (u_LightType[i] == 0)
        {
            vec3 L = normalize(-u_LightDirection[i]); // 从物体指向光源
            float NdotL = max(dot(N, L), 0.0);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * ShadowVisibility2D(worldPos, i);
        }
        else if (u_LightType[i] == 1)
        {
            vec3 L = normalize(u_LightPosition[i] - worldPos);
            float NdotL = max(dot(N, L), 0.0);
            float d = length(u_LightPosition[i] - worldPos);
            float attenuation = 1.0 / (u_LightConstant[i] + u_LightLinear[i] * d + u_LightQuadratic[i] * d * d);
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation * ShadowVisibilityCube(worldPos, i);
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
            result += u_LightColor[i] * u_LightIntensity[i] * NdotL * attenuation * spot * ShadowVisibility2D(worldPos, i);
        }
    }

    return result;
}
