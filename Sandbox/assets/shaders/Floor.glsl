#type vertex
#version 460 core

out vec2 v_NDC;

void main()
{
    vec2 p = vec2(float((gl_VertexID << 1) & 2), float(gl_VertexID & 2));
    v_NDC = p * 2.0 - 1.0;
    gl_Position = vec4(v_NDC, -1.0, 1.0);
}


#type fragment
#version 460 core

#include "Lighting.glsl"

layout(location = 0) out vec4 color;

in vec2 v_NDC;

uniform mat4 u_ViewProjection;
uniform mat4 u_InvViewProjection;
uniform vec4 u_Color;
uniform bool u_GridEnabled;
uniform vec3 u_GridColor;
uniform float u_GridScale;
uniform float u_FadeStart;
uniform float u_FadeEnd;
uniform vec3 u_FadeColor;
uniform samplerCube u_Skybox;
uniform bool u_HasSkybox;

void main()
{
    vec4 nearP = u_InvViewProjection * vec4(v_NDC, -1.0, 1.0);
    vec4 farP  = u_InvViewProjection * vec4(v_NDC,  1.0, 1.0);
    vec3 origin = nearP.xyz / nearP.w;
    vec3 dir    = normalize(farP.xyz / farP.w - origin);

    if (abs(dir.y) < 1e-6) discard;
    float t = -origin.y / dir.y;
    if (t <= 0.0) discard;

    vec3 worldPos = origin + dir * t;

    vec4 clip = u_ViewProjection * vec4(worldPos, 1.0);
    gl_FragDepth = (clip.z / clip.w) * 0.5 + 0.5;

    vec3 result = LitSurface(vec3(0.0, 1.0, 0.0), worldPos);

    float fade = 1.0 - smoothstep(u_FadeStart, u_FadeEnd, t);

    vec3 lit = u_Color.rgb * result;
    if (u_GridEnabled)
    {
        vec2 coord = worldPos.xz / max(u_GridScale, 1e-4);
        vec2 f = fract(coord);
        vec2 d = min(f, 1.0 - f);
        vec2 w = fwidth(coord);

        float lx = (1.0 - min(d.x / w.x, 1.0)) * (1.0 - smoothstep(0.5, 1.0, w.x));
        float ly = (1.0 - min(d.y / w.y, 1.0)) * (1.0 - smoothstep(0.5, 1.0, w.y));
        float line = max(lx, ly);

        lit = mix(lit, u_GridColor * result, line);
    }

    vec3 fadeColor = u_FadeColor;
    if (u_HasSkybox)
        fadeColor = texture(u_Skybox, dir).rgb;

    color = vec4(mix(fadeColor, lit, fade), u_Color.a);
}