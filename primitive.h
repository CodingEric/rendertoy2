#pragma once

#include <glm/glm.hpp>
#include <assimp/vector3.h>

#include "rendertoy_internal.h"
#include "accelerate.h"
#include "intersectinfo.h"

namespace rendertoy
{
    class Primitive
    {
    public:
        virtual const bool Intersect(const glm::vec3 &origin, const glm::vec3 &direction, IntersectInfo RENDERTOY_FUNC_ARGUMENT_OUT intersect_info) const = 0;
        virtual const BBox GetBoundingBox() const = 0;

        virtual ~Primitive() {}
    };

    class Triangle : public Primitive
    {
    private:
        glm::vec3 _vert[3]; // Relative coordinate to the nearest origin.
    public:
        Triangle(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
        Triangle(const aiVector3D &p0, const aiVector3D &p1, const aiVector3D &p2);
        virtual const bool Intersect(const glm::vec3 &origin, const glm::vec3 &direction, IntersectInfo RENDERTOY_FUNC_ARGUMENT_OUT intersect_info) const final;
        virtual const BBox GetBoundingBox() const;
    };

    class UVSphere : public Primitive
    {
    private:
        glm::vec3 _origin;
        glm::float32 _radius;
    public:
        UVSphere(const glm::vec3 &origin, const float &radius);
        virtual const bool Intersect(const glm::vec3 &origin, const glm::vec3 &direction, IntersectInfo RENDERTOY_FUNC_ARGUMENT_OUT intersect_info) const final;
        virtual const BBox GetBoundingBox() const;
    };

    class TriangleMesh : public Primitive
    {
    private:
        glm::vec3 _origin;
        BVH<Triangle> triangles;
    public:
        TriangleMesh() = default;
        TriangleMesh(const TriangleMesh &) = delete;
        friend const std::vector<std::unique_ptr<TriangleMesh>> ImportMeshFromFile(const std::string &path);
        virtual const bool Intersect(const glm::vec3 &origin, const glm::vec3 &direction, IntersectInfo RENDERTOY_FUNC_ARGUMENT_OUT intersect_info) const final;
        virtual const BBox GetBoundingBox() const;
    };
}