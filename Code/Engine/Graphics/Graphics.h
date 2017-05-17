/*
This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H
#include <d3d9.h>
#include "../Graphics/Effect.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/material.h"
#include "../Graphics/GameSprite.h"
#include "../Math/cVector.h"
#include <vector>
#include "../Graphics/GameObject.h"
#include "CollisionDetection.h"
// Header Files
//=============

#include "../Windows/WindowsIncludes.h"
#include "DebugShapes.h"
// Interface
//==========

namespace
{
	//eae6320::Graphics::Mesh s_draw;
	//eae6320::Graphics::Mesh s_draw2;
	//eae6320::Effect *s_Effect;
}

namespace eae6320
{
	
	namespace Graphics
	{
		

		static float r_x = 0.0f;
		static float r_y = 51.2f;
		 //eae6320::Effect *s_Effect;
		static float m_x =0;
		static float m_y=0;
	

		
		bool Initialize(const HWND i_renderingWindow);
		void Render();
		void Clear();
		void renderable();
		void Begin_render();
		void End_render();
		void Back_Buffer();
		bool ShutDown();
		IDirect3DDevice9* Get3ddevice();
		void UpdatePosition(float i_x, float i_y);
		eae6320::Effect* getGlobalEffect();
		eae6320::Effect* getGlobalEffect2();
		eae6320::Graphics::Mesh getGlobalDraw();
		eae6320::Graphics::Mesh getGlobalDraw2();
		eae6320::Graphics::Mesh getGlobalDraw3();
		eae6320::Graphics::Mesh getGlobalDraw4();
		eae6320::Graphics::Mesh getGlobalDraw5();
		eae6320::Graphics::Mesh getGlobalDraw6();
		eae6320::Graphics::Mesh getGlobalDraw7();
		eae6320::Graphics::Mesh getGlobalDraw8();
		eae6320::Graphics::Mesh getGlobalcylinder();
		eae6320::material* getGlobalRedMaterial();
		eae6320::material* getGlobalBlueMaterial();
		eae6320::material* getGlobalDefaultMaterial();
		eae6320::material* getGlobalGreenMaterial();
		eae6320::material* getGlobalYellowMaterial();
		eae6320::material* getGlobalCylinderMaterial();
		extern GameObject* Player1;
		extern CollisionDetection* s_collisionDet;
		GameSprite* getGlobalSprite();
		GameSprite* getGlobalSprite2();
		void Bind();
		struct sDebugLine
		{
			IDirect3DVertexBuffer9* m_vertexBuffer = NULL;
		};
		std::vector<sDebugLine>* GetDebugLineList();
		struct sVertexDebug
		{
			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			float x, y, z;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 12
			uint8_t b, g, r, a;	// 8 bits [0,255] per RGBA channel (the alpha channel is unused but is present so that color uses a full 4 bytes)
		};
		void LoadDebugLine(eae6320::Math::cVector i_start, eae6320::Math::cVector i_end, Math::cVector i_color);
		void DrawDebugLine(const sDebugLine& i_line);
		bool SetsVertexDeclaration();
		bool SetDebugVertexDeclaration();
		
	
		
	}
}


#endif	// EAE6320_GRAPHICS_H
