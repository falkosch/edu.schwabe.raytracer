#pragma once

#include "../KDTreeTraverser.h"

#include <cassert>

namespace raytracer
{
	using namespace vectorization;
	using namespace primitives;

	template <typename IntersectionInfoType>
	class NaiveKDTreeTraverser
		: public KDTreeTraverser < IntersectionInfoType >
	{

		static void findNearestIntersection(
			const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
			const KDTreeNode & node,
			const IntersectionInfoType * const originIntersection,
			Raycast & r,
			IntersectionInfoType & intersectionOut
		)
		{
			// Traverse childs
			const KDTreeNode * traversel = &node;

			while (traversel->childs) {
				const KDTreeNodeChilds & childs = *traversel->childs;
				const Float4::VectorBoolType leftRightHit = overlaps(r, childs.boundingA, childs.boundingB);

				if (allTrue(leftRightHit)) {
					// Set right as next iteration step
					traversel = &childs.childB;
					// We need a recursive call to test the left node. (depth-first traversal)
					findNearestIntersection(geometryNodesTraverser, childs.childA, originIntersection, r, intersectionOut);
					continue;
				}

				// select one of the nodes as next iteration step
				traversel = x(leftRightHit) ? (&childs.childA) : (&childs.childB);
			}

			// At a non-empty leaf node, its geometry nodes are tested for intersection.
			assert(traversel->isNonEmptyLeaf());
			r.maxDistance = geometryNodesTraverser.findNearestIntersection(
				*traversel->geometryNodes,
				r,
				originIntersection,
				intersectionOut
			);
		}

		static void findAnyIntersection(
			const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
			const KDTreeNode & node,
			const Raycast & r,
			const IntersectionInfoType * const originIntersection,
			Raycast & tr,
			IntersectionInfoType & intersectionOut
		)
		{
			// Traverse childs
			const KDTreeNode * traversel = &node;

			while (traversel->childs) {
				const KDTreeNodeChilds & childs = *traversel->childs;
				const Float4::VectorBoolType leftRightHit = overlaps(tr, childs.boundingA, childs.boundingB);

				if (allTrue(leftRightHit)) {
					// Set right as next iteration step
					traversel = &childs.childB;
					// We need a recursive call to test the left node. (depth-first traversal)
					findAnyIntersection(geometryNodesTraverser, childs.childA, r, originIntersection, tr, intersectionOut);

					// Short circuit the recursion on hit
					if (!outOfReach(r, tr.maxDistance)) {
						return;
					}

					// Or continue with iteration on childB
					continue;
				}

				// select one of the nodes as next iteration step
				traversel = x(leftRightHit) ? (&childs.childA) : (&childs.childB);
			}

			// At a non-empty leaf node, its geometry nodes are tested for intersection.
			assert(traversel->isNonEmptyLeaf());
			tr.maxDistance = geometryNodesTraverser.findAnyIntersection(
				*traversel->geometryNodes,
				tr,
				originIntersection,
				intersectionOut
			);
		}

	public:

		virtual ~NaiveKDTreeTraverser() { }

		const Float findNearestIntersection(
			const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
			const KDTreeRoot & root,
			const Raycast & r,
			const IntersectionInfoType * const originIntersection,
			IntersectionInfoType & intersectionOut
		) const
		{
			// Do we have a hit at root?
			if (!overlaps(r, root.rootBounding)) {
				return r.maxDistance;
			}

			Raycast tr = r;
			findNearestIntersection(geometryNodesTraverser, root.rootNode, originIntersection, tr, intersectionOut);
			return tr.maxDistance;
		}

		const Float findAnyIntersection(
			const GeometryNodesTraverser<IntersectionInfoType> & geometryNodesTraverser,
			const KDTreeRoot & root,
			const Raycast & r,
			const IntersectionInfoType * const originIntersection,
			IntersectionInfoType & intersectionOut
		) const
		{
			// Do we have a hit at root?
			if (!overlaps(r, root.rootBounding)) {
				return r.maxDistance;
			}

			Raycast tr = r;
			findAnyIntersection(geometryNodesTraverser, root.rootNode, r, originIntersection, tr, intersectionOut);
			return tr.maxDistance;
		}

	};

}
