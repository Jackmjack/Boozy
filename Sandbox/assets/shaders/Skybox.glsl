#type vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

out vec3 v_LocalPos;

uniform mat4 u_ViewProjection;

void main()
{
    const vec3 kPos[8] = vec3[8](
        vec3(-1, -1, -1), vec3( 1, -1, -1), vec3( 1,  1, -1), vec3(-1,  1, -1),
        vec3(-1, -1,  1), vec3( 1, -1,  1), vec3( 1,  1,  1), vec3(-1,  1,  1)
    );

    const int kIdx[36] = int[36](
        0, 1, 2,  0, 2, 3,   // -Z
        4, 7, 6,  4, 6, 5,   // +Z
        0, 4, 5,  0, 5, 1,   // -Y
        3, 2, 6,  3, 6, 7,   // +Y
        0, 3, 7,  0, 7, 4,   // -X
        1, 5, 6,  1, 6, 2    // +X
    );

    vec3 pos = kPos[kIdx[gl_VertexID]];
    v_LocalPos = pos;

    vec4 clip = u_ViewProjection * vec4(pos, 1.0);
    gl_Position = clip.xyww;
}

#type fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_LocalPos;

uniform samplerCube u_Skybox;

void main()
{
    color = vec4(texture(u_Skybox, v_LocalPos).rgb, 1.0);
}