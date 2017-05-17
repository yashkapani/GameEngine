/*
This file contains the function declarations for graphics
*/

#ifndef DEBUG_SHAPES_H
#define DEBUG_SHAPES_H

// Header Files
//=============
#include <cstdint>
#include "../Math/cVector.h"
#include <d3d9.h>

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		struct CUSTOMVERTEX
		{
			float x, y, z;
			uint8_t b, g, r, a;

			CUSTOMVERTEX()
			{

			}


			CUSTOMVERTEX(float i_x, float i_y, float i_z, uint8_t i_r, uint8_t i_b, uint8_t i_g)
			{
				x = i_x;
				y = i_y;
				z = i_z;
				r = i_r;
				b = i_b;
				g = i_g;
				a = 255;
			}
		};

		class DebugLine {

			Math::cVector m_startPoint;
			Math::cVector m_endPoint;
			Math::cVector m_color;
			LPDIRECT3DVERTEXBUFFER9 m_vBuffer;

		public:
			DebugLine() {};
			DebugLine(Math::cVector i_start, Math::cVector i_end, Math::cVector i_color) :
				m_startPoint(i_start),
				m_endPoint(i_end),
				m_color(i_color)
			{
			}
			void LoadDebugLine();
			void DrawLine();
		};

		class DebugBox {
			Math::cVector m_position;
			float m_size;
			Math::cVector m_color;
			LPDIRECT3DVERTEXBUFFER9 m_vBuffer;
			LPDIRECT3DINDEXBUFFER9 m_iBuffer;

		public:
			DebugBox() {};
			DebugBox(Math::cVector i_position, float i_size, Math::cVector i_color) :
				m_position(i_position),
				m_size(i_size),
				m_color(i_color)
			{
			}
			void LoadDebugBox();
			void DrawBox();
		};

		class DebugSphere {
			Math::cVector m_center;
			float m_radius;
			size_t m_slices;
			size_t m_stacks;
			Math::cVector m_color;
			LPDIRECT3DVERTEXBUFFER9 m_vBuffer;
			LPDIRECT3DINDEXBUFFER9 m_iBuffer;
			uint32_t m_vertexCount;
			uint32_t m_indexCount;

		public:
			DebugSphere() {};
			DebugSphere(Math::cVector i_center, float i_radius, size_t i_slices, size_t i_stacks, Math::cVector i_color = Math::cVector(1.0f, 0.0f, 0.0f)) :
				m_center(i_center),
				m_radius(i_radius),
				m_slices(i_slices),
				m_stacks(i_stacks),
				m_color(i_color)
			{
			}
			void LoadDebugSphere();
			void DrawSphere();
		};
	}
}

#endif	// DEBUG_SHAPES_H