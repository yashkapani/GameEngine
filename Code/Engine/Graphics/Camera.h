#ifndef EAE6320_CAMERA_H
#define EAE6320_CAMERA_H

#include "../Math/cQuaternion.h"
#include "../Math/cVector.h"
#include "../Math/cMatrix_transformation.h"

class Camera
{

	public:
		eae6320::Math::cVector camera_axis = eae6320::Math::cVector(0,0,0); // POSITION
		eae6320::Math::cQuaternion camear_rotation; // ORIENTATION
		eae6320::Math::cVector axisofrotation = eae6320::Math::cVector(0,0,0);
		eae6320::Math::cVector offset = eae6320::Math::cVector(0, 0, 100);
		eae6320::Math::cVector velocity = eae6320::Math::cVector(0, 0, 0);
		static Camera& getInstance()
		{
			static Camera    instance; // Guaranteed to be destroyed.
								  // Instantiated on first use.
			return instance;
		}

		void UpdatePosition(eae6320::Math::cVector i_position_offset)
		{
			// To move the camera in the direction it is pointing at
			eae6320::Math::cMatrix_transformation i_localToWorldTransform = eae6320::Math::cMatrix_transformation(camear_rotation, camera_axis);
			//camera_axis = eae6320::Math::cMatrix_transformation::matrixMulVector(i_localToWorldTransform, i_position_offset);
			camera_axis = eae6320::Math::cMatrix_transformation::matrixMulVector(i_localToWorldTransform, i_position_offset);
	
		}

		void UpdateOrientation(eae6320::Math::cVector i_rotation_offset)
		{
			camear_rotation = camear_rotation * eae6320::Math::cQuaternion(i_rotation_offset.x, eae6320::Math::cVector(1.0f, 0.0f, 0.0f));
			camear_rotation = camear_rotation * eae6320::Math::cQuaternion(i_rotation_offset.y, eae6320::Math::cVector(0.0f, 1.0f, 0.0f));
			camear_rotation = camear_rotation * eae6320::Math::cQuaternion(i_rotation_offset.z, eae6320::Math::cVector(0.0f, 0.0f, 1.0f));
		}

	private:
		Camera() {
			
		};                   // Constructor? (the {} brackets) are needed here.

								  // C++ 11
								  // =======
								  // We can use the better technique of deleting the methods
								  // we don't want.
		Camera(Camera const&) = delete;
		void operator=(Camera const&) = delete;

};
#endif