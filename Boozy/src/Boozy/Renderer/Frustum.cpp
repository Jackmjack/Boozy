#include "bzpch.h"
#include "Frustum.h"

namespace Boozy {

    static glm::vec4 Normalize(const glm::vec4& p)
    {
        const float len = glm::length(glm::vec3(p));
        return len > 0.0f ? p / len : p;
    }

    Frustum::Frustum(const glm::mat4& vp)
    {
        const glm::vec4 row0(vp[0][0], vp[1][0], vp[2][0], vp[3][0]);
        const glm::vec4 row1(vp[0][1], vp[1][1], vp[2][1], vp[3][1]);
        const glm::vec4 row2(vp[0][2], vp[1][2], vp[2][2], vp[3][2]);
        const glm::vec4 row3(vp[0][3], vp[1][3], vp[2][3], vp[3][3]);

        m_Planes[0] = Normalize(row3 + row0);   // 左
        m_Planes[1] = Normalize(row3 - row0);   // 右
        m_Planes[2] = Normalize(row3 + row1);   // 下
        m_Planes[3] = Normalize(row3 - row1);   // 上
        m_Planes[4] = Normalize(row3 + row2);   // 近
        m_Planes[5] = Normalize(row3 - row2);   // 远
    }

    bool Frustum::IsInside(const glm::vec3& center, float radius) const
    {
        for (const glm::vec4& p : m_Planes)
            if (glm::dot(glm::vec3(p), center) + p.w < -radius)
                return false;
        return true;
    }

    bool Frustum::IsInside(const glm::vec3& mn, const glm::vec3& mx) const
    {
        for (const glm::vec4& p : m_Planes)
        {
            const glm::vec3 positive{
                p.x >= 0.0f ? mx.x : mn.x,
                p.y >= 0.0f ? mx.y : mn.y,
                p.z >= 0.0f ? mx.z : mn.z,
            };
            if (glm::dot(glm::vec3(p), positive) + p.w < 0.0f)
                return false;
        }
        return true;
    }
}