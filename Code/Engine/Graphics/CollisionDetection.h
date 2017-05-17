#ifndef EAE6320_COLLISIONDETECTION_H
#define EAE6320_COLLISIONDETECTION_H

#include "Mesh.h"
#include "../Math/cVector.h"
#include "Camera.h"
namespace eae6320
{
	namespace Graphics
	{
		class CollisionDetection
		{
		private:
			Math::cVector playerHeight;
			std::vector<Math::cVector> mVertexPosition;
			std::vector<uint32_t> mVertexIndices;
			std::vector<Math::cVector> mTriangleNormal;
			Math::cVector raySource;
			int m_indexCount;
			int m_vertexCount;

			uint32_t* m_indexData;
			Mesh::sVertex* m_vertexData;

			struct CollisionData
			{
				Math::cVector velocity;
				Math::cVector sourcepoint;
				Math::cVector collNormal;
				bool foundCollision;
				float nearestDistance;
				Math::cVector  nearestPloygonIntersectionPoint;
				float length;
			};

		public:
			void LoadCollisionData(const char* meshPath);
			int RayTriangleIntersection(Math::cVector& v1, Math::cVector &v2, Math::cVector &v3, float length, CollisionData *collD);
			Math::cVector  CollideWithWorld(Math::cVector& sourcePnt, Math::cVector& velocity, float length, bool isCamCollision);
			Math::cVector ResolvePlayerCollision(Math::cVector position, Math::cVector velocity, CollisionData collD);
			Math::cVector ResolveCameraCollision(Math::cVector position, Math::cVector velocity, CollisionData coll);
			Math::cVector  CameraCollision(Math::cVector& sourcePnt, Math::cVector& velocity, float length);
			bool CheckIfPointInTriangle(Math::cVector &polygonInterectionPoint, Math::cVector &vertex1, Math::cVector &vertex2, Math::cVector &vertex3);
		};
	}
}
#endif