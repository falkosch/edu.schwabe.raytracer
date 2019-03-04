#pragma once

#include "../ObjectShader.h"

namespace raytracer
{
	using namespace vectorization;

	class IntersectionNormalShader : public Shader < ObjectShader, FacetIntersection, Float4 >
	{
	public:

		ALIGNED_ALLOCATORS(__alignof(IntersectionNormalShader));

		IntersectionNormalShader();

		virtual ~IntersectionNormalShader();

		const Float4 sample(const ObjectShader & objectShader, const FacetIntersection & intersection) const;

		const Float4 operator()(const ObjectShader & objectShader, const FacetIntersection & intersection) const;

	};

}