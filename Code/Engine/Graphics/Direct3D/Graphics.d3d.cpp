// Header Files
//=============

#include "../Graphics.h"
#include "../material.h"
#include <cassert>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <sstream>
#include "../../UserOutput/UserOutput.h"
#include "../GameSprite.h"
#include "../GameObject.h"
eae6320::Graphics::GameObject* eae6320::Graphics::Player1 = NULL;
eae6320::Graphics::CollisionDetection* eae6320::Graphics::s_collisionDet = NULL;
//eae6320::Graphics::GameObject* eae6320::Graphics::Player1 = NULL;
namespace
{
	
	eae6320::Graphics::Mesh s_draw;
	eae6320::Graphics::Mesh s_draw2;
	eae6320::Graphics::Mesh s_draw3;
	eae6320::Graphics::Mesh s_draw4;
	eae6320::Graphics::Mesh s_draw5;
	eae6320::Graphics::Mesh s_draw6;
	eae6320::Graphics::Mesh s_draw7;
	eae6320::Graphics::Mesh s_draw8;
	GameSprite *player;
	GameSprite *player2;
	
	eae6320::Math::cVector s_playerPrevPos;
	eae6320::Math::cVector s_camPrevPos;
	std::vector<eae6320::Graphics::sDebugLine> s_debugLines;
	//eae6320::Effect *s_Effect;
	//eae6320::Effect *s_Effect2;
	eae6320::material *red_material;
	eae6320::material *default_material;
	eae6320::material *green_material;
	eae6320::material *yellow_material;
	eae6320::material *blue_material;
	//eae6320::Effect* getGlobalEffect();
}




std::vector<eae6320::Graphics::sDebugLine>* eae6320::Graphics::GetDebugLineList()
{
	return &s_debugLines;
}

eae6320::material* eae6320::Graphics::getGlobalGreenMaterial()
{
	return green_material;
}
eae6320::material* eae6320::Graphics::getGlobalYellowMaterial()
{
	return yellow_material;
}
eae6320::material* eae6320::Graphics::getGlobalRedMaterial()
{
	return red_material;
}
eae6320::material* eae6320::Graphics::getGlobalBlueMaterial()
{
	return blue_material;
}
eae6320::material* eae6320::Graphics::getGlobalDefaultMaterial()
{
	return default_material;
}
//eae6320::Effect* eae6320::Graphics::getGlobalEffect()
//{
//	return s_Effect;
//}
//eae6320::Effect* eae6320::Graphics::getGlobalEffect2()
//{
//	return s_Effect2;
//}

eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw()
{
	return s_draw;
}

eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw2()
{
	return s_draw2;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw3()
{
	return s_draw3;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw4()
{
	return s_draw4;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw5()
{
	return s_draw5;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw6()
{
	return s_draw6;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw7()
{
	return s_draw7;
}
eae6320::Graphics::Mesh eae6320::Graphics::getGlobalDraw8()
{
	return s_draw8;
}
GameSprite* eae6320::Graphics::getGlobalSprite()
{
	return player;
}
GameSprite* eae6320::Graphics::getGlobalSprite2()
{
	return player2;
}
// Static Data Initialization
//===========================
//eae6320::Effect *eae6320::Graphics::s_Effect = NULL;
namespace
{
	HWND s_renderingWindow = NULL;
	IDirect3D9 * s_direct3dInterface = NULL;

	
	// This struct determines the layout of the data that the CPU will send to the GPU
	//struct sVertex
	//{
	//	// POSITION
	//	// 2 floats == 8 bytes
	//	// Offset = 0
	//	float x, y;
	//	// COLOR0
	//	// 4 uint8_ts == 4 bytes
	//	// Offset = 8
	//	uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
	//};
	/*eae6320::Graphics::Mesh s_draw;
	eae6320::Graphics::Mesh s_draw2;*/
	//eae6320::Effect *s_Effect;
	//IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;

	// The vertex buffer holds the data for each vertex
	//IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
	// An index buffer describes how to make triangles with the vertices
	// (i.e. it defines the vertex connectivity)
	//IDirect3DIndexBuffer9* s_indexBuffer = NULL;

	// The vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* 
	//		(So that the grapPositionhics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want
	IDirect3DVertexShader9* s_vertexShader = NULL;
	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	IDirect3DPixelShader9* s_fragmentShader = NULL;
}

IDirect3DDevice9* eae6320::Graphics::Mesh::s_direct3dDevice;
// Helper Function Declarations
//=============================

namespace
{
	eae6320::Graphics::DebugLine s_debugLine1;
	eae6320::Graphics::DebugLine s_debugLine2;
	eae6320::Graphics::DebugBox s_debugBox1;
	eae6320::Graphics::DebugBox s_debugBox2;
	eae6320::Graphics::DebugSphere s_debugSphere1;
	eae6320::Graphics::DebugSphere s_debugSphere2;


	bool CreateDevice();
	//bool CreateIndexBuffer();
	bool CreateInterface();
	//bool CreateVertexBuffer();
	HRESULT  GetVertexProcessingUsage(DWORD& o_usage);
	bool LoadFragmentShader();
	bool LoadVertexShader();
}

// Interface
//==========

bool eae6320::Graphics::Initialize(const HWND i_renderingWindow)
{
	
	//s_draw.ReadSquareValues();
	s_renderingWindow = i_renderingWindow;
	player = new GameSprite(100, 500);
	player2 = new GameSprite(700, 200);
	//s_Effect = new eae6320::Effect();
	//s_Effect2 = new eae6320::Effect();
	red_material = new eae6320::material();
	blue_material = new eae6320::material();
	default_material = new eae6320::material();
	green_material = new eae6320::material();
	yellow_material = new eae6320::material();
	red_material->init_effect();
	blue_material->init_effect();
	default_material->init_effect();
	yellow_material->init_effect();
	green_material->init_effect();
	
	s_collisionDet = new eae6320::Graphics::CollisionDetection();
	// Initialize the interface to the Direct3D9 library
	if (!CreateInterface())
	{
		return false;
	}
	// Create an interface to a Direct3D device
	if (!CreateDevice())
	{
		goto OnError;
	}

	//s_Effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	//s_Effect2->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	red_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	blue_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	default_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	green_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	yellow_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;


	player->Initialize(eae6320::Graphics::Mesh::s_direct3dDevice,"data/numbers.png",200,200);
	player2->Initialize(eae6320::Graphics::Mesh::s_direct3dDevice, "data/numbers.png", 512, 64);
	// Initialize the graphics objects
	Player1 = new GameObject("data/Player1.mesh", "data/yeloow.material");
	s_collisionDet->LoadCollisionData("data/collisiondata.mesh");
	if (!s_draw.ReadSquareValues("data/cement.mesh"))
	{
		goto OnError;
	}
	if (!s_draw.CreateIndexBuffer())
	{
		goto OnError;
	}

	if (!s_draw2.ReadSquareValues("data/ceiling.mesh"))
	{
		goto OnError;
	}
	if (!s_draw2.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw3.ReadSquareValues("data/railing.mesh"))
	{
		goto OnError;
	}
	if (!s_draw3.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw4.ReadSquareValues("data/walls.mesh"))
	{
		goto OnError;
	}
	if (!s_draw4.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw5.ReadSquareValues("data/lambert3.mesh"))
	{
		goto OnError;
	}
	if (!s_draw5.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw6.ReadSquareValues("data/metal.mesh"))
	{
		goto OnError;
	}
	if (!s_draw6.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw7.ReadSquareValues("data/floor.mesh"))
	{
		goto OnError;
	}
	if (!s_draw7.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!s_draw8.ReadSquareValues("data/Player.mesh"))
	{
		goto OnError;
	}
	if (!s_draw8.CreateIndexBuffer())
	{
		goto OnError;
	}
	if (!default_material->CreateMaterial("data/Default.material"))
	{
		goto OnError;
	}
	if (!red_material->CreateMaterial("data/Red.material"))
	{
		goto OnError;
	}
	if (!blue_material->CreateMaterial("data/Blue.material"))
	{
		goto OnError;
	}
	if (!green_material->CreateMaterial("data/Green.material"))
	{
		goto OnError;
	}
	if (!yellow_material->CreateMaterial("data/Yellow.material"))
	{
		goto OnError;
	}
	if (!Player1->LoadObject())
	{
		goto OnError;
	}
	s_debugLine1 = eae6320::Graphics::DebugLine(Math::cVector(100.0f, 0.0f, 0.0f), Math::cVector(75.0f, 50.0f, -50.0f), Math::cVector(0.0f, 0.4f, 0.2f));
	s_debugLine2 = eae6320::Graphics::DebugLine(Math::cVector(-100.0f, 0.0f, -70.0f), Math::cVector(-50.0f, 50.0f, -50.0f), Math::cVector(1.0f, 0.0f, 0.0f));
	s_debugBox1 = eae6320::Graphics::DebugBox(Math::cVector(30.0f, 20.0f, -40.0f), 20.0f, Math::cVector(1.0f, 0.0f, 1.0f));
	s_debugBox2 = eae6320::Graphics::DebugBox(Math::cVector(-30.0f, 20.0f, -40.0f), 15.0f, Math::cVector(1.0f, 0.0f, 0.0f));
	s_debugSphere1 = eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, 0.0f, -150.0f), 20.0f, 20, 20, Math::cVector(0.2f, 0.4f, 0.0f));
	s_debugSphere2 = eae6320::Graphics::DebugSphere(Math::cVector(-50.0f, 0.0f, -250.0f), 30.0f, 20, 20, Math::cVector(0.0f, 1.0f, 1.0f));
	
	// Loading Debug Shapes
	s_debugLine1.LoadDebugLine();
	s_debugLine2.LoadDebugLine();
	s_debugBox1.LoadDebugBox();
	s_debugBox2.LoadDebugBox();
	s_debugSphere1.LoadDebugSphere();
	s_debugSphere2.LoadDebugSphere();
	
	

	

	//red_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	//blue_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	/*if (!s_Effect->CreateEffect("data/Shader.effect"))
	{
		goto OnError;
	}
	if (!s_Effect2->CreateEffect("data/ShaderTransp.effect"))
	{
		goto OnError;
	}*/
	//if (!LoadVertexShader())
	//{
	//	goto OnError;
	//}
	//if (!LoadFragmentShader())
	//{
	//	goto OnError;
	//}
	//draw debug Line
	eae6320::Graphics::LoadDebugLine(eae6320::Math::cVector(0, 0, 0), eae6320::Math::cVector(100, 50, 30), eae6320::Math::cVector(1, 0, 0));

	HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	result = eae6320::Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	 result = eae6320::Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	return true;

OnError:

	ShutDown();
	return false;
}
void eae6320::Graphics::Clear()
{
	{
		const D3DRECT* subRectanglesToClear = NULL;
		const DWORD subRectangleCount = 0;
		const DWORD clearTheRenderTarget = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
		D3DCOLOR clearColor;
		{
			// Black is usually used:
			clearColor = D3DCOLOR_XRGB(0, 0, 0);
		}
		const float ZBuffer = 1.0f;
		const DWORD noStencilBuffer = 0;
		HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->Clear(subRectangleCount, subRectanglesToClear,
			clearTheRenderTarget, clearColor, ZBuffer, noStencilBuffer);
		assert(SUCCEEDED(result));

	}
}
void eae6320::Graphics::Begin_render()
{
	HRESULT result = s_draw.Mesh::s_direct3dDevice->BeginScene();
	assert(SUCCEEDED(result));
	//s_debugBox1.DrawBox();

	//s_debugBox2.DrawBox();
	
}
void eae6320::Graphics::End_render()
{
	{
		Player1->m_velocity = Player1->m_position - s_playerPrevPos;
		Player1->m_position = s_collisionDet->CollideWithWorld(Player1->m_position, Player1->m_velocity, 50, false);
		Player1->m_velocity = Math::cVector(0, -9.8f, 0) * 5;
		Player1->m_position = s_collisionDet->CollideWithWorld(Player1->m_position, Player1->m_velocity, 20, false);
		s_playerPrevPos = Player1->m_position;
		
			Math::cVector playerDir = Player1->m_position -Camera::getInstance().camera_axis;
		Math::cMatrix_transformation mat = Math::cMatrix_transformation::cMatrix_transformation(Player1->m_orientation, Player1->m_position);
		Math::cVector offset = Math::cMatrix_transformation::matrixMulVector(mat, Math::cVector(0, 120, 200));
		Math::cVector rayOrigin = offset;
		s_collisionDet->CollideWithWorld(rayOrigin, playerDir, playerDir.GetLength(), true);
		}
	
	/*
	s_debugLine1.DrawLine();
	s_debugLine2.DrawLine();*/
	//s_debugSphere1.DrawSphere();
	//s_debugSphere2.DrawSphere();
	
	HRESULT result = result = eae6320::Graphics::Mesh::s_direct3dDevice->EndScene();
	assert(SUCCEEDED(result));
}
void eae6320::Graphics::Back_Buffer()
{
	const RECT* noSourceRectangle = NULL;
	const RECT* noDestinationRectangle = NULL;
	const HWND useDefaultWindow = NULL;
	const RGNDATA* noDirtyRegion = NULL;
	HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->Present(noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion);
	assert(SUCCEEDED(result));
}

//void eae6320::Graphics::Render()
//{
//	// Every frame an entirely new image will be created.
//	// Before drawing anything, then, the previous image will be erased
//	// by "clearing" the image buffer (filling it with a solid color)
//	{
//	const D3DRECT* subRectanglesToClear = NULL;
//		const DWORD subRectangleCount = 0;
//		const DWORD clearTheRenderTarget = D3DCLEAR_TARGET;
//		D3DCOLOR clearColor;
//		{
//			// Black is usually used:
//			clearColor = D3DCOLOR_XRGB(0, 0, 0);
//		}
//		const float noZBuffer = 0.0f;
//		const DWORD noStencilBuffer = 0;
//		HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->Clear(subRectangleCount, subRectanglesToClear,
//		clearTheRenderTarget, clearColor, noZBuffer, noStencilBuffer);
//	assert(SUCCEEDED(result));
//
//	}
////
////	// The actual function calls that draw geometry must be made between paired calls to
////	// BeginScene() and EndScene()
//	{
//	HRESULT result = s_draw.Mesh::s_direct3dDevice->BeginScene();
//		assert(SUCCEEDED(result));
//	{
//			// Set the shaders
//			{
//				//s_Effect->BindEffect();
//
//				eae6320::Graphics::Bind();
//				//HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexShader(s_vertexShader);
//				//assert(SUCCEEDED(result));
//				//result = eae6320::Graphics::Mesh::s_direct3dDevice->SetPixelShader(s_fragmentShader);
//				//assert(SUCCEEDED(result));
//
//			}
//			// Bind a specific vertex buffer to the device as a data source
//			/*{
//			// There can be multiple streams of data feeding the display adaptor at the same time
//			const unsigned int streamIndex = 0;
//			// It's possible to start streaming data in the middle of a vertex buffer
//			const unsigned int bufferOffset = 0;
//			// The "stride" defines how large a single vertex is in the stream of data
//			const unsigned int bufferStride = sizeof(sVertex);
//			eae6320::Graphics::Mesh::s_direct3dDevice->SetStreamSource(streamIndex, s_draw.s_vertexBuffer, bufferOffset, bufferStride);
//			assert(SUCCEEDED(result));
//			}*/
//
//			eae6320::Graphics::renderable();
//		//s_Effect->SetPositionOffset(x, y);
//		//s_Effect->SetPositionOffset(0, 0);
//		/*bool res_draw = s_draw.draw();
//		assert(res_draw);
//		s_Effect->SetPositionOffset(0, 0);
//		bool res_draw2 = s_draw2.draw();
//		s_Effect->SetPositionOffset(1, 0);
//		 res_draw2 = s_draw2.draw();
//		assert(res_draw2);*/
//			
//			/* // Bind a specific index buffer to the device as a data source
//			{
//			result = s_direct3dDevice->SetIndices(s_indexBuffer);
//			assert(SUCCEEDED(result));
//			}
//			// Render objects from the current streams
//			{
//			// We are using triangles as the "primitive" type,
//			// and we have defined the vertex buffer as a triangle list
//			// (meaning that every triangle is defined by three vertices)
//			const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
//			// It's possible to start rendering primitives in the middle of the stream
//			const unsigned int indexOfFirstVertexToRender = 0;
//			const unsigned int indexOfFirstIndexToUse = 0;
//			// We are drawing a square
//			const unsigned int vertexCountToRender =6;	// How vertices from the vertex buffer will be used?
//			const unsigned int primitiveCountToRender = 2;	// How many triangles will be drawn?
//			result = s_direct3dDevice->DrawIndexedPrimitive(primitiveType,
//			indexOfFirstVertexToRender, indexOfFirstVertexToRender, vertexCountToRender,
//			indexOfFirstIndexToUse, primitiveCountToRender);
//			assert(SUCCEEDED(result));
//			}*/
//		}
//		result = eae6320::Graphics::Mesh::s_direct3dDevice->EndScene();
//		assert(SUCCEEDED(result));
//	}
//
////	// Everything been drawn to the "back buffer", which is just an image in memory.
////	// In order to display it, the contents of the back buffer must be "presented"
////	// (to the front buffer)
//	{
//		const RECT* noSourceRectangle = NULL;
//		const RECT* noDestinationRectangle = NULL;
//		const HWND useDefaultWindow = NULL;
//		const RGNDATA* noDirtyRegion = NULL;
//		HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->Present(noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion);
//		assert(SUCCEEDED(result));
//	}
//}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if (s_direct3dInterface)
	{
		if (eae6320::Graphics::Mesh::s_direct3dDevice)
		{
			if (s_vertexShader)
			{
				s_vertexShader->Release();
				s_vertexShader = NULL;
			}
			if (s_fragmentShader)
			{
				s_fragmentShader->Release();
				s_fragmentShader = NULL;
			}

			if (s_draw.s_vertexBuffer)
			{
				s_draw.s_vertexBuffer->Release();
				s_draw.s_vertexBuffer = NULL;
			}
			if (s_draw.s_indexBuffer)
			{
				s_draw.s_indexBuffer->Release();
				s_draw.s_indexBuffer = NULL;
			}
			if (s_draw.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw.s_vertexDeclaration->Release();
				s_draw.s_vertexDeclaration = NULL;
			}
			if (s_draw2.s_vertexBuffer)
			{
			s_draw2.s_vertexBuffer->Release();
			s_draw2.s_vertexBuffer = NULL;
			}
			if (s_draw2.s_indexBuffer)
			{
			s_draw2.s_indexBuffer->Release();
			s_draw2.s_indexBuffer = NULL;
			}
			if (s_draw2.s_vertexDeclaration)
			{
			eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
			s_draw2.s_vertexDeclaration->Release();
			s_draw2.s_vertexDeclaration = NULL;
			}
			if (s_draw3.s_vertexBuffer)
			{
				s_draw3.s_vertexBuffer->Release();
				s_draw3.s_vertexBuffer = NULL;
			}
			if (s_draw3.s_indexBuffer)
			{
				s_draw3.s_indexBuffer->Release();
				s_draw3.s_indexBuffer = NULL;
			}
			if (s_draw3.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw3.s_vertexDeclaration->Release();
				s_draw3.s_vertexDeclaration = NULL;
			}

			if (s_draw4.s_vertexBuffer)
			{
				s_draw4.s_vertexBuffer->Release();
				s_draw4.s_vertexBuffer = NULL;
			}
			if (s_draw4.s_indexBuffer)
			{
				s_draw4.s_indexBuffer->Release();
				s_draw4.s_indexBuffer = NULL;
			}
			if (s_draw4.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw4.s_vertexDeclaration->Release();
				s_draw4.s_vertexDeclaration = NULL;
			}

			if (s_draw5.s_vertexBuffer)
			{
				s_draw5.s_vertexBuffer->Release();
				s_draw5.s_vertexBuffer = NULL;
			}
			if (s_draw5.s_indexBuffer)
			{
				s_draw5.s_indexBuffer->Release();
				s_draw5.s_indexBuffer = NULL;
			}
			if (s_draw5.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw5.s_vertexDeclaration->Release();
				s_draw5.s_vertexDeclaration = NULL;
			}

			if (s_draw6.s_vertexBuffer)
			{
				s_draw6.s_vertexBuffer->Release();
				s_draw6.s_vertexBuffer = NULL;
			}
			if (s_draw6.s_indexBuffer)
			{
				s_draw6.s_indexBuffer->Release();
				s_draw6.s_indexBuffer = NULL;
			}
			if (s_draw6.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw6.s_vertexDeclaration->Release();
				s_draw6.s_vertexDeclaration = NULL;
			}

			if (s_draw7.s_vertexBuffer)
			{
				s_draw7.s_vertexBuffer->Release();
				s_draw7.s_vertexBuffer = NULL;
			}
			if (s_draw7.s_indexBuffer)
			{
				s_draw7.s_indexBuffer->Release();
				s_draw7.s_indexBuffer = NULL;
			}
			if (s_draw7.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw7.s_vertexDeclaration->Release();
				s_draw7.s_vertexDeclaration = NULL;
			}

			if (s_draw8.s_vertexBuffer)
			{
				s_draw8.s_vertexBuffer->Release();
				s_draw8.s_vertexBuffer = NULL;
			}
			if (s_draw8.s_indexBuffer)
			{
				s_draw8.s_indexBuffer->Release();
				s_draw8.s_indexBuffer = NULL;
			}
			if (s_draw8.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				s_draw8.s_vertexDeclaration->Release();
				s_draw8.s_vertexDeclaration = NULL;
			}


			if (Player1->m_mesh.s_vertexDeclaration)
			{
				Player1->m_mesh.s_vertexBuffer->Release();
				Player1->m_mesh.s_vertexBuffer = NULL;
			}
			if (Player1->m_mesh.s_indexBuffer)
			{
				Player1->m_mesh.s_indexBuffer->Release();
				Player1->m_mesh.s_indexBuffer = NULL;
			}
			if (Player1->m_mesh.s_vertexDeclaration)
			{
				eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(NULL);
				Player1->m_mesh.s_vertexDeclaration->Release();
				Player1->m_mesh.s_vertexDeclaration = NULL;
			}

			eae6320::Graphics::Mesh::s_direct3dDevice->Release();
			eae6320::Graphics::Mesh::s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_renderingWindow = NULL;

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateDevice()
	{
		const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			{
				const unsigned int useRenderingWindowDimensions = 0;
				presentationParameters.BackBufferWidth = useRenderingWindowDimensions;
				presentationParameters.BackBufferHeight = useRenderingWindowDimensions;
			}
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = s_renderingWindow;
			presentationParameters.Windowed = TRUE;
			presentationParameters.EnableAutoDepthStencil = TRUE;
			presentationParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = s_direct3dInterface->CreateDevice(useDefaultDevice, useHardwareRendering,
			s_renderingWindow, useHardwareVertexProcessing, &presentationParameters, &eae6320::Graphics::Mesh::s_direct3dDevice);
		if (SUCCEEDED(result))
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 device");
			return false;
		}
	}

	/*	bool CreateIndexBuffer()
	{
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
	// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
	const HRESULT result = GetVertexProcessingUsage(usage);
	if (FAILED(result))
	{
	return false;
	}
	// Our code will only ever write to the buffer
	usage |= D3DUSAGE_WRITEONLY;
	}

	// Create an index buffer
	unsigned int bufferSize;
	{
	// We are drawing a square
	const unsigned int triangleCount =2;	// How many triangles does a square have?
	const unsigned int vertexCountPerTriangle = 3;
	bufferSize = triangleCount * vertexCountPerTriangle * sizeof(uint32_t);
	// We'll use 32-bit indices in this class to keep things simple
	// (i.e. every index will be a 32 bit unsigned integer)
	const D3DFORMAT format = D3DFMT_INDEX32;
	// Place the index buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* notUsed = NULL;
	const HRESULT result = s_draw.s_direct3dDevice->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
	&s_draw.s_indexBuffer, notUsed);
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to create an index buffer");
	return false;
	}
	}
	// Fill the index buffer with the triangles' connectivity data
	{
	// Before the index buffer can be changed it must be "locked"
	uint32_t* indexData;

	{
	const unsigned int lockEntireBuffer = 0;
	const DWORD useDefaultLockingBehavior = 0;
	const HRESULT result = s_draw.s_indexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
	reinterpret_cast<void**>(&indexData), useDefaultLockingBehavior);
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to lock the index buffer");
	return false;
	}
	}
	// Fill the buffer
	{
	// EAE6320_TODO:
	// You will need to fill in 3 indices for each triangle that needs to be drawn.
	// Each index will be a 32-bit unsigned integer,
	// and will index into the vertex buffer array that you have created.
	// The order of indices is important, but the correct order will depend on
	// which vertex you have assigned to which spot in your vertex buffer
	// (also remember to maintain the correct handedness for the triangle winding order).

	// Triangle 0
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 3;

	// Triangle 1
	indexData[3] = 0;
	indexData[4] =3;
	indexData[5] = 2;
	// etc...
	}
	// The buffer must be "unlocked" before it can be used
	{
	const HRESULT result = s_draw.s_indexBuffer->Unlock();
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to unlock the index buffer");
	return false;
	}
	}
	}

	return true;
	}*/

	bool CreateInterface()
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		s_direct3dInterface = Direct3DCreate9(D3D_SDK_VERSION);
		if (s_direct3dInterface)
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print("DirectX failed to create a Direct3D9 interface");
			return false;
		}
	}

	/*bool CreateVertexBuffer()
	{
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
	// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
	const HRESULT result = GetVertexProcessingUsage(usage);
	if (FAILED(result))
	{
	return false;
	}
	// Our code will only ever write to the buffer
	usage |= D3DUSAGE_WRITEONLY;
	}

	// Initialize the vertex format
	{
	// These elements must match the VertexFormat::sVertex layout struct exactly.
	// They instruct Direct3D how to match the binary data in the vertex buffer
	// to the input elements in a vertex shader
	// (by using D3DDECLUSAGE enums here and semantics in the shader,
	// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
	// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
	D3DVERTEXELEMENT9 vertexElements[] =
	{
	// Stream 0

	// POSITION
	// 2 floats == 8 bytes
	// Offset = 0
	{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

	// COLOR0
	// D3DCOLOR == 4 bytes
	// Offset = 8
	{ 0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

	// The following marker signals the end of the vertex declaration
	D3DDECL_END()
	};
	HRESULT result = s_draw.s_direct3dDevice->CreateVertexDeclaration(vertexElements, &s_draw.s_vertexDeclaration);
	if (SUCCEEDED(result))
	{
	result = s_draw.s_direct3dDevice->SetVertexDeclaration(s_draw.s_vertexDeclaration);
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to set the vertex declaration");
	return false;
	}
	}
	else
	{
	eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 vertex declaration");
	return false;
	}
	}

	// Create a vertex buffer
	{
	// We are drawing one square
	const unsigned int vertexCount = 4;	// What is the minimum number of vertices a square needs (so that no data is duplicated)?
	const unsigned int bufferSize = vertexCount * sizeof(sVertex);
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = 0;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* const notUsed = NULL;
	const HRESULT result = s_draw.s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
	&s_draw.s_vertexBuffer, notUsed);
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to create a vertex buffer");
	return false;
	}
	}
	// Fill the vertex buffer with the triangle's vertices
	{
	// Before the vertex buffer can be changed it must be "locked"
	sVertex* vertexData;
	{
	const unsigned int lockEntireBuffer = 0;
	const DWORD useDefaultLockingBehavior = 0;
	const HRESULT result = s_draw.s_vertexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
	reinterpret_cast<void**>(&vertexData), useDefaultLockingBehavior);
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to lock the vertex buffer");
	return false;
	}
	}

	// Fill the buffer
	{
	// You will need to fill in two pieces of information for each vertex:
	//	* 2 floats for the POSITION
	//	* 4 uint8_ts for the COLOR

	// The floats for POSITION are for the X and Y coordinates, like in Assignment 02.
	// The difference this time is that there should be fewer (because we are sharing data).

	// The uint8_ts for COLOR are "RGBA", where "RGB" stands for "Red Green Blue" and "A" for "Alpha".
	// Conceptually each of these values is a [0,1] value, but we store them as an 8-bit value to save space
	// (color doesn't need as much precision as position),
	// which means that the data we send to the GPU will be [0,255].
	// For now the alpha value should _always_ be 255, and so you will choose color by changing the first three RGB values.
	// To make white you should use (255, 255, 255), to make black (0, 0, 0).
	// To make pure red you would use the max for R and nothing for G and B, so (1, 0, 0).
	// Experiment with other values to see what happens!

	vertexData[0].x = 0.0f;
	vertexData[0].y = 0.0f;
	// Red
	vertexData[0].r = 255;
	vertexData[0].g = 0;
	vertexData[0].b = 0;
	vertexData[0].a = 255;

	vertexData[1].x = 0.0f;
	vertexData[1].y = 1.0f;
	// Red
	vertexData[1].r = 0;
	vertexData[1].g = 0;
	vertexData[1].b = 255;
	vertexData[1].a = 255;
	// etc.
	vertexData[2].x = 1.0f;
	vertexData[2].y = 0.0f;
	// Red
	vertexData[2].r = 0;
	vertexData[2].g = 255;
	vertexData[2].b = 0;
	vertexData[2].a = 255;

	vertexData[3].x = 1.0f;
	vertexData[3].y = 1.0f;
	// Red
	vertexData[3].r = 255;
	vertexData[3].g = 255;
	vertexData[3].b = 0;
	vertexData[3].a = 255;
	}
	// The buffer must be "unlocked" before it can be used
	{
	const HRESULT result = s_draw.s_vertexBuffer->Unlock();
	if (FAILED(result))
	{
	eae6320::UserOutput::Print("Direct3D failed to unlock the vertex buffer");
	return false;
	}
	}
	}

	return true;
	}*/

	HRESULT GetVertexProcessingUsage(DWORD& o_usage)
	{
	D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
	const HRESULT result = s_draw.s_direct3dDevice->GetCreationParameters(&deviceCreationParameters);
	if (SUCCEEDED(result))
	{
	DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
	(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
	o_usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
	}
	else
	{
	eae6320::UserOutput::Print("Direct3D failed to get the device's creation parameters");
	}
	return result;
	}

	bool LoadFragmentShader()
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = "data/fragment.shader";
			const D3DXMACRO defines[] = {
				{ "EAE6320_PLATFORM_D3D", "1" },{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "ps_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the fragment shader object
		bool wereThereErrors = false;
		{
			HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&s_fragmentShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}

	bool LoadVertexShader()
	{
		// Load the source code from file and compile it
		ID3DXBuffer* compiledShader;
		{
			const char* sourceCodeFileName = "data/vertex.shader";
			const D3DXMACRO defines[] = {
				{ "EAE6320_PLATFORM_D3D", "1" },{ NULL, NULL }
			};
			ID3DXInclude* noIncludes = NULL;
			const char* entryPoint = "main";
			const char* profile = "vs_3_0";
			const DWORD noFlags = 0;
			ID3DXBuffer* errorMessages = NULL;
			ID3DXConstantTable** noConstants = NULL;
			HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
				&compiledShader, &errorMessages, noConstants);
			if (SUCCEEDED(result))
			{
				if (errorMessages)
				{
					errorMessages->Release();
				}
			}
			else
			{
				if (errorMessages)
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
						<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
					eae6320::UserOutput::Print(errorMessage.str());
					errorMessages->Release();
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
					eae6320::UserOutput::Print(errorMessage.str());
				}
				return false;
			}
		}
		// Create the vertex shader object
		bool wereThereErrors = false;
		{
			HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
				&s_vertexShader);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
				wereThereErrors = true;
			}
			compiledShader->Release();
		}
		return !wereThereErrors;
	}


}


bool eae6320::Graphics::SetsVertexDeclaration()
{
	//Creating Vertex Buffer
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage);
		if (FAILED(result))
		{
			return false;
		}
		// Our code will only ever write to the buffer
		usage |= D3DUSAGE_WRITEONLY;
	}

	// Initialize the vertex format
	{
		// These elements must match the VertexFormat::sVertex layout struct exactly.
		// They instruct Direct3D how to match the binary data in the vertex buffer
		// to the input elements in a vertex shader
		// (by using D3DDECLUSAGE enums here and semantics in the shader,
		// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
		// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
		D3DVERTEXELEMENT9 vertexElements[] =
		{
			// Stream 0

			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

			// TEXTURE COORDINATES
			// 2 floats == 8 bytes
			// Offset = 12
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },

			// COLOR0
			// D3DCOLOR == 4 bytes
			// Offset = 12+8
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			{ 0, 32, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

			// The following marker signals the end of the vertex declaration
			D3DDECL_END()
		};
		IDirect3DVertexDeclaration9* m_vertexDeclaration;
		HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->CreateVertexDeclaration(vertexElements, &(m_vertexDeclaration));
		if (SUCCEEDED(result))
		{
			result = eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(m_vertexDeclaration);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to set the vertex declaration");
				return false;
			}
		}
		else
		{
			eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 vertex declaration");
			return false;
		}
	}

	return true;
}
void eae6320::Graphics::LoadDebugLine(eae6320::Math::cVector i_start, eae6320::Math::cVector i_end, Math::cVector i_color = Math::cVector(1.0f, 1.0f, 1.0f))
{
	sDebugLine o_line;

	// create the vertices using the sVertexDebug struct
	sVertexDebug vertices[2] =
	{
		{ i_start.x, i_start.y, i_start.z,(uint8_t)(i_color.z * 255), (uint8_t)(i_color.y * 255), (uint8_t)(i_color.x * 255),  (uint8_t)255 },
		{ i_end.x, i_end.y, i_end.z,(uint8_t)(i_color.z * 255), (uint8_t)(i_color.y * 255), (uint8_t)(i_color.x * 255),  (uint8_t)255 }
		// Using same color for the both vertices to draw line of same color
	};

	// create a vertex buffer interface called i_buffer
	const unsigned int vertexCount = 2;
	const unsigned int bufferSize = vertexCount * sizeof(sVertexDebug);
	DWORD usage = 0;
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = 0;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* const notUsed = NULL;
	//s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,&(o_line.m_vertexBuffer), notUsed);
	eae6320::Graphics::Mesh::s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool, &(o_line.m_vertexBuffer), notUsed);


	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	o_line.m_vertexBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	o_line.m_vertexBuffer->Unlock();

	std::vector<eae6320::Graphics::sDebugLine>* m_DebugLineList = &s_debugLines;
	m_DebugLineList->push_back(o_line);

	return;
}

bool eae6320::Graphics::SetDebugVertexDeclaration()
{
	//Creating Vertex Buffer
	// The usage tells Direct3D how this vertex buffer will be used
	DWORD usage = 0;
	{
		// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
		const HRESULT result = GetVertexProcessingUsage(usage);
		if (FAILED(result))
		{
			return false;
		}
		// Our code will only ever write to the buffer
		usage |= D3DUSAGE_WRITEONLY;

	}

	// Initialize the vertex format
	{
		// These elements must match the VertexFormat::sVertex layout struct exactly.
		// They instruct Direct3D how to match the binary data in the vertex buffer
		// to the input elements in a vertex shader
		// (by using D3DDECLUSAGE enums here and semantics in the shader,
		// so that, for example, D3DDECLUSAGE_POSITION here matches with POSITION in shader code).
		// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
		D3DVERTEXELEMENT9 vertexElements[] =
		{
			// Stream 0

			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

			// COLOR0
			// D3DCOLOR == 4 bytes
			// Offset = 20
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

			// The following marker signals the end of the vertex declaration
			D3DDECL_END()
		};
		IDirect3DVertexDeclaration9* m_vertexDeclaration;
		HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->CreateVertexDeclaration(vertexElements, &(m_vertexDeclaration));
		if (SUCCEEDED(result))
		{
			result = eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexDeclaration(m_vertexDeclaration);
			if (FAILED(result))
			{
				eae6320::UserOutput::Print("Direct3D failed to set the vertex declaration");
				return false;
			}
		}
		else
		{
			eae6320::UserOutput::Print("Direct3D failed to create a Direct3D9 vertex declaration");
			return false;
		}
	}
	return true;
}

void eae6320::Graphics::DrawDebugLine(const sDebugLine& i_line)
{

	eae6320::Graphics::Mesh::s_direct3dDevice->SetStreamSource(0, i_line.m_vertexBuffer, 0, sizeof(sVertexDebug));
	const unsigned int primitiveCount = 1;
	eae6320::Graphics::Mesh::s_direct3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, primitiveCount);
}
/*
void eae6320::Graphics::UpdatePosition(float i_x, float i_y)
{
	m_x = i_x;
	m_y = i_y;
}
*/