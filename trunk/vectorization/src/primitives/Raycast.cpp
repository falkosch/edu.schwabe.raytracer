#include "../stdafx.h"

#include "primitives/Raycast.h"

using namespace vectorization;

namespace primitives
{

	Raycast::Raycast()
		:
		ray(),
		cullingMask(),
		originIds(),
		maxDistance(Float_Limits::max())
	{}

	Raycast::Raycast(
		const Ray & rayIn,
		const Float4::VectorBoolType & cullingMaskIn,
		const Size2 & originIdsIn,
		const Float maxDistanceIn)
		:
		ray(rayIn),
		cullingMask(cullingMaskIn),
		originIds(originIdsIn),
		maxDistance(maxDistanceIn)
	{ }

	const Raycast toObjectSpace(const Raycast & r, const Float44 & transInvModelMatrix, const Float4 & osOrigin)
	{
		return Raycast(
			Ray(osOrigin, normalize3(r.ray.direction * transInvModelMatrix)),
			r.cullingMask,
			r.originIds,
			distance3(osOrigin, farPoint(r) * transInvModelMatrix));
	}

	const Raycast toObjectSpace(const Raycast & r, const Float44 & transInvModelMatrix)
	{
		return toObjectSpace(r, transInvModelMatrix, r.ray.origin * transInvModelMatrix);
	}

	const Float4::VectorBoolType backfaceCulledv(const Raycast & r)
	{
		return xxxx(r.cullingMask);
	}

	const bool backfaceCulled(const Raycast & r)
	{
		return !!x(backfaceCulledv(r));
	}

	const Float4::VectorBoolType frontfaceCulledv(const Raycast & r)
	{
		return yyyy(r.cullingMask);
	}

	const bool frontfaceCulled(const Raycast & r)
	{
		return !!x(frontfaceCulledv(r));
	}

	const Float4::VectorBoolType notCulledv(const Raycast & r)
	{
		return ~(backfaceCulledv(r) ^ frontfaceCulledv(r));
	}

	const bool notCulled(const Raycast & r)
	{
		return !!x(notCulledv(r));
	}

	const Int cullingOrientation(const Raycast & r)
	{
		if (notCulled(r)) return Zero<Int>();
		return backfaceCulled(r) ? NegOne<Int>() : One<Int>();
	}

	const Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation)
	{
		return convert<Float4::VectorBoolType>(
			cullingOrientation < Zero<Float4::BoolType>(),
			cullingOrientation > Zero<Float4::BoolType>());
	}

	const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId)
	{
		return y(r.originIds) == toCheckId;
	}

	const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId, const Float t, const Float epsilon)
	{
		return selfOcclusion(r, toCheckId) & (t <= epsilon);
	}

	const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId, const Float t)
	{
		return selfOcclusion(r, toCheckId, t, Epsilon<Float>());
	}

	const bool outOfReach(const Raycast & r, const Float t)
	{
		return allTrue(outOfReach(r, Float4(t)));
	}

	const Float4::VectorBoolType outOfReach(const Raycast & r, const Float4 & t)
	{
		const Float4 rMaxDistanceV = Float4(r.maxDistance);
		// t == NaN or (r.maxDistance != NaN and t >= r.maxDistance)
		return isNaN(t) | (andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV));
	}

	const Float4 farPoint(const Raycast & r)
	{
		return point(r.ray, r.maxDistance);
	}

}
