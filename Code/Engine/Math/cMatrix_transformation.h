/*
	This class represents a full transformation
	(i.e. a 4x4 matrix composed of a 3x3 rotation matrix and a 3 element translation vector)

	Matrix Facts:
		* The float elements are stored as columns
			* In other words, the second float stored is the first element in the second row
			* This is so that all 16 floats can be copied directly to Direct3D or OpenGL
				(Both HLSL and GLSL interpret a matrix's registers to be columns)
		* The vectors are the rows
			* In other words, the first three rows are the rotation and the last row is the translation
			* This is an arbitrary choice, but it is arguably the most common one in computer graphics
				(every other field outside of computer graphics that I know of uses column vectors, though)
			* This matches standard Direct3D but is backwards from standard OpenGL
		* The vectors are right-handed
			* In other words, the first vector is right, the second vector is up, and the third vector is back
			* This is an arbitrary choice; it matches Maya and standard OpenGL, and is backwards from standard Direct3D
*/

#ifndef EAE6320_MATH_CMATRIX_TRANSFORMATION_H
#define EAE6320_MATH_CMATRIX_TRANSFORMATION_H

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		class cVector;
		class cQuaternion;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Math
	{
		class cMatrix_transformation
		{
			// Interface
			//==========

		public:

			static cMatrix_transformation CreateWorldToViewTransform(
				const cQuaternion& i_cameraOrientation, const cVector& i_cameraPosition );
			static cMatrix_transformation CreateViewToScreenTransform(
				const float i_fieldOfView_y, const float i_aspectRatio,
				const float i_z_nearPlane, const float i_z_farPlane );
			static eae6320::Math::cVector eae6320::Math::cMatrix_transformation::matrixMulVector(const cMatrix_transformation& i_mtx, const cVector& i_vec);
			float Get(size_t i_row, size_t i_col) const { return *((&m_00) + i_row * 4 + i_col); }
			
			
			void Set(int i_row, int i_col, float i_m) { *((&m_00) + i_row * 4 + i_col) = i_m; };
			
		   void Transpose();
			// Initialization / Shut Down
			//---------------------------

			cMatrix_transformation();	// Identity
			cMatrix_transformation( const cQuaternion& i_rotation, const cVector& i_translation );

			// Data
			//=====

		private:

			// Storage is column-major; see notes at the top of the file
			float m_00, m_10, m_20, m_30,
				m_01, m_11, m_21, m_31,
				m_02, m_12, m_22, m_32,
				m_03, m_13, m_23, m_33;

			// Implementation
			//===============

		private:

			// Initialization / Shut Down
			//---------------------------

			cMatrix_transformation(
				const float i_00, const float i_10, const float i_20, const float i_30,
				const float i_01, const float i_11, const float i_21, const float i_31,
				const float i_02, const float i_12, const float i_22, const float i_32,
				const float i_03, const float i_13, const float i_23, const float i_33 );
		};
	}
}

#endif	// EAE6320_MATH_CMATRIX_TRANSFORMATION_H
