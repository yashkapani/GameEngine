#include "CollisionDetection.h"
#include "../../Engine/Time/Time.h"
#include <assert.h>
#include <fstream>
#include "Graphics.h"
#include "Camera.h"
#include "Mesh.h"

namespace eae6320
{
	namespace Graphics
	{

		void CollisionDetection::LoadCollisionData(const char* meshPath)
		{
			assert(meshPath);


			//std::ifstream inputBinaryMeshFile(meshPath, std::ifstream::binary);
			std::ifstream is(meshPath, std::ifstream::binary);

			if (is) {

				// Get length of the file
				is.seekg(0, is.end);
				size_t fileLength = (size_t)is.tellg();
				is.seekg(0, is.beg);

				// Read and store into buffer
				char * binaryReadBuffer = new char[fileLength];
				is.read(binaryReadBuffer, fileLength);
				is.close();
				// Extracting corresponding info after reading binary
				m_vertexCount = *reinterpret_cast<uint32_t*>(binaryReadBuffer);
				m_vertexData = reinterpret_cast<Mesh::sVertex*>(binaryReadBuffer + sizeof(uint32_t));

				m_indexCount = *reinterpret_cast<uint32_t*>(binaryReadBuffer + sizeof(uint32_t) + (sizeof(eae6320::Graphics::Mesh::sVertex)*m_vertexCount));
				m_indexData = reinterpret_cast<uint32_t*>(binaryReadBuffer + sizeof(uint32_t) + (sizeof(eae6320::Graphics::Mesh::sVertex)*m_vertexCount) + sizeof(uint32_t));
			}

			for (int i = 0; i < m_vertexCount; i++)
			{
				mVertexPosition.push_back(Math::cVector(m_vertexData[i].x, m_vertexData[i].y, m_vertexData[i].z));
				mTriangleNormal.push_back(Math::cVector(m_vertexData[i].nx, m_vertexData[i].ny, m_vertexData[i].nz));
			}

			for (int i = 0; i < m_indexCount; i += 3)
			{
				mVertexIndices.push_back(m_indexData[i]);
				mVertexIndices.push_back(m_indexData[i + 1]);
				mVertexIndices.push_back(m_indexData[i + 2]);
			}

			playerHeight = Math::cVector(0, 10, 0);
		}

		Math::cVector  CollisionDetection::CollideWithWorld(Math::cVector& position, Math::cVector& velocity, float length, bool isCamCollision)
		{

			if (velocity.GetLength() == 0)
				return position;

			CollisionData coll;
			coll.foundCollision = false;
			coll.nearestDistance = 0;
			coll.sourcepoint = position;
			coll.velocity = velocity.CreateNormalized();
			coll.length = length;

			for (int j = 0; j < mVertexPosition.size(); j += 3)
			{
				Math::cVector planeOrigin = mVertexPosition[mVertexIndices[j]];
				Math::cVector B = mVertexPosition[mVertexIndices[j + 1]];
				Math::cVector C = mVertexPosition[mVertexIndices[j + 2]];

				RayTriangleIntersection(planeOrigin, B, C, length, &coll);
			}
			if (!isCamCollision)
				return	ResolvePlayerCollision(position, velocity, coll);
			else
				return ResolveCameraCollision(position, velocity, coll);
		}

		Math::cVector CollisionDetection::ResolveCameraCollision(Math::cVector position, Math::cVector velocity, CollisionData coll)
		{
			if (coll.foundCollision == true)
			{
				Camera::getInstance().offset = Math::cVector(0, 50, 80);
			}
			else
			{
				Camera::getInstance().offset = Math::cVector(0, 80, 300);
			}
			return position;
		}

		Math::cVector CollisionDetection::ResolvePlayerCollision(Math::cVector position, Math::cVector velocity, CollisionData coll)
		{
			Math::cVector pos = position;
			if (!coll.foundCollision)
			{
				pos += velocity *Time::GetSecondsElapsedThisFrame();

			}
			else
			{
				Math::cVector newSourcePoint;
				{

					Math::cVector slidingPlaneNormal = coll.collNormal;
					pos = coll.sourcepoint - ((coll.velocity + coll.collNormal) * (coll.length - coll.nearestDistance) *0.5f);// -slidingPlaneNormal* amount;

				}
			}
			return pos;
		}


		int CollisionDetection::RayTriangleIntersection(Math::cVector& v1, Math::cVector& v2, Math::cVector& v3, float length, CollisionData *collD)
		{
			Math::cVector e1, e2;
			e1 = v2 - v1;
			e2 = v3 - v1;
			/*
			Math::cVector n = Math::Cross(e1, e2);
			n = n.CreateNormalized();
			float det = Math::Dot(n, collD->velocity );
			if (det <= 0.0f) return 0;

			float inv_det = 1 / det;

			Math::cVector sourceToVertex = v1 - collD->sourcepoint;
			float e = Math::Dot(sourceToVertex,n);
			if (e < 0.0f)
			return 0;
			//if (e > collD->length)
			//return 0;


			Math::cVector o = Math::Cross(collD->velocity  ,sourceToVertex);
			float v = Math::Dot(e2,o);
			if (v<0.0f || v>collD->length) return 0;
			float w = -Math::Dot(e1,o);
			if (w<0.0f || v + w>collD->length) return 0;

			e *= inv_det;
			v *= inv_det;
			w *= inv_det;
			float u = 1.0f - v - w;
			return 1;

			*/


			Math::cVector n = Math::Cross(e1, e2);
			n = n.CreateNormalized();

			Math::cVector ePoint = collD->sourcepoint;// -n;
			float det = Math::Dot(n, collD->velocity);
			if (det == 0.0f) return 0;

			float inv_det = 1 / det;
			float a = Math::Dot(v1, n);
			float b = Math::Dot(n, ePoint);

			float t = (a - b) * inv_det;

			if (t < 0)
				return 0;

			if (t > 0 && t <= collD->length)
			{
				Math::cVector collPnt = ePoint + (collD->velocity * t);

				if (CheckIfPointInTriangle(collPnt, v1, v2, v3))
				{
					{
						collD->nearestPloygonIntersectionPoint = collPnt;
						collD->collNormal = n;
						collD->foundCollision = true;
						collD->nearestDistance = t;
						return 1;
					}

				}
			}
			return 0;
		}


		bool CollisionDetection::CheckIfPointInTriangle(Math::cVector &polygonInterectionPoint, Math::cVector &vertex1, Math::cVector &vertex2, Math::cVector &vertex3)
		{
			float totalAngles = 0;
			Math::cVector A = polygonInterectionPoint - vertex1;
			Math::cVector B = polygonInterectionPoint - vertex2;
			Math::cVector C = polygonInterectionPoint - vertex3;

			A = A.CreateNormalized();
			B = B.CreateNormalized();
			C = C.CreateNormalized();
			float Dot1 = Math::Dot(A, B);
			if (Dot1 < -1)
				Dot1 = -1;
			if (Dot1 > 1)
				Dot1 = 1;
			totalAngles += acos(Dot1);

			float Dot2 = Math::Dot(B, C);
			if (Dot2 < -1)
				Dot2 = -1;
			if (Dot2 > 1)
				Dot2 = 1;
			totalAngles += acos(Dot2);

			float Dot3 = Math::Dot(A, C);
			if (Dot3 < -1)
				Dot3 = -1;
			if (Dot3 > 1)
				Dot3 = 1;
			totalAngles += acos(Dot3);

			if (fabs(totalAngles - 2 * 3.14159265f) <= 0.005f)
			{
				return true;
			}

			return false;
		}

	}
}
