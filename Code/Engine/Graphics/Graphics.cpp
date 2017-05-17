//
#include "Graphics.h"
#include <cassert>
//#include "Mesh.h"
#include <vector>
#include "material.h"
#include "GameSprite.h"

void eae6320::Graphics::Render()
{
	
	Clear();
	Begin_render();
	Bind();
	renderable();
	End_render();
	Back_Buffer();
	eae6320::Graphics::GetDebugLineList()->clear();
}

void eae6320::Graphics::Bind()
{
    	
	//getGlobalEffect()->BindEffect();
}

void eae6320::Graphics::renderable()
{
	eae6320::Math::cQuaternion o_quaternion;


	//if(x!=0)
  // r_x =  x;
	//r_x = r_x + 1;
	//r_x = r_x + 1;
	/*if (getGlobalSprite() && getGlobalSprite()->IsInitialized()) getGlobalSprite()->Draw(512.0f,0.0f,512.0f,0.0f);
	if(r_x==0)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f,0.0f,51.2f,0.0f);
	if (r_x == 1)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 102.4f, 51.2f);
	if (r_x == 2)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 153.6f, 102.4f);
	if (r_x == 3)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 204.8f, 153.6f);
	if (r_x == 4)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 256.0f, 204.8f);
	if (r_x == 5)
	if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 307.2f, 256.0f);
	if (r_x == 6)
		if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 358.4f, 307.2f);
	if (r_x == 7)
		if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 409.6f, 358.4f);
	if (r_x == 8)
		if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 460.8f, 409.6f);
	if (r_x == 9)
		if (getGlobalSprite2() && getGlobalSprite2()->IsInitialized()) getGlobalSprite2()->Draw(64.0f, 0.0f, 512.0f, 460.8f);*/

	getGlobalDefaultMaterial()->i_effect->BindEffect();
	getGlobalDefaultMaterial()->UpdateMaterial();
	
	getGlobalDefaultMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	bool res_draw = getGlobalDraw2().draw();
	assert(res_draw);

	
	getGlobalDefaultMaterial()->i_effect->BindEffect();
	getGlobalDefaultMaterial()->UpdateMaterial();

	getGlobalDefaultMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	res_draw = getGlobalDraw().draw();
	

	getGlobalRedMaterial()->i_effect->BindEffect();
	getGlobalRedMaterial()->UpdateMaterial();
	
	getGlobalRedMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	res_draw = getGlobalDraw3().draw();


	getGlobalYellowMaterial()->i_effect->BindEffect();
	getGlobalYellowMaterial()->UpdateMaterial();

	getGlobalYellowMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	 res_draw = getGlobalDraw4().draw();

	getGlobalGreenMaterial()->i_effect->BindEffect();
	getGlobalGreenMaterial()->UpdateMaterial();

	getGlobalGreenMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	res_draw = getGlobalDraw6().draw();
	Player1->DrawObject();

	getGlobalBlueMaterial()->i_effect->BindEffect();
	getGlobalBlueMaterial()->UpdateMaterial();

	getGlobalBlueMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	res_draw = getGlobalDraw7().draw();

	getGlobalYellowMaterial()->i_effect->BindEffect();
	getGlobalYellowMaterial()->UpdateMaterial();

	getGlobalYellowMaterial()->i_effect->SetPositionOffset(0, 0, 0, o_quaternion);
	res_draw = getGlobalDraw5().draw();

	getGlobalYellowMaterial()->i_effect->BindEffect();
	getGlobalYellowMaterial()->UpdateMaterial();
	
	//getGlobalYellowMaterial()->i_effect->SetPositionOffset(0, 0,0, o_quaternion);
	//res_draw = getGlobalDraw8().draw();

	

	eae6320::Graphics::LoadDebugLine(eae6320::Math::cVector(0, 0, 0), eae6320::Math::cVector(100, 50, 30), eae6320::Math::cVector(1, 0, 0));
	eae6320::Graphics::LoadDebugLine(eae6320::Math::cVector(0, 0, 0), eae6320::Math::cVector(-100, 50, 30), eae6320::Math::cVector(1, 1, 0));

	{
		eae6320::Graphics::SetDebugVertexDeclaration();
		{
			//Drawing DebugLines
			{
				size_t debugLineSize = eae6320::Graphics::GetDebugLineList()->size();
				for (int i = 0; i < debugLineSize; i++)
				{
					
					DrawDebugLine(eae6320::Graphics::GetDebugLineList()->at(i));
				}
			}

			
		}
		eae6320::Graphics::SetsVertexDeclaration();
	}

	/*
	getGlobalYellowMaterial()->i_effect->BindEffect();
	getGlobalYellowMaterial()->UpdateMaterial();

	getGlobalYellowMaterial()->i_effect->SetPositionOffset(-5,-3, 0);
	 res_draw = getGlobalDraw().draw();
	assert(res_draw);
	
	getGlobalRedMaterial()->i_effect->BindEffect();
	getGlobalRedMaterial()->UpdateMaterial();

	getGlobalRedMaterial()->i_effect->SetPositionOffset(2, 0, 0);
	res_draw = getGlobalDraw2().draw();
	assert(res_draw);

	getGlobalBlueMaterial()->i_effect->BindEffect();
	getGlobalBlueMaterial()->UpdateMaterial();

	getGlobalBlueMaterial()->i_effect->SetPositionOffset(-4, 0, 0);
	res_draw = getGlobalDraw2().draw();
	assert(res_draw);

	getGlobalGreenMaterial()->i_effect->BindEffect();
	getGlobalGreenMaterial()->UpdateMaterial();

	getGlobalGreenMaterial()->i_effect->SetPositionOffset(4, 0, 0);
	res_draw = getGlobalDraw2().draw();
	assert(res_draw);
*/
	
}

/*
void eae6320::Graphics::Render()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	
	eae6320::Graphics::Clear();
	// The actual function calls that draw geometry must be made between paired calls to
	// BeginScene() and EndScene()
	{
		eae6320::Graphics::Begin_render();
		{
			// Set the shaders
			{
				eae6320::Graphics::getGlobalEffect()->BindEffect();

				//s_Effect->eae6320::Effect::BindEffect();
				//HRESULT result = eae6320::Graphics::Mesh::s_direct3dDevice->SetVertexShader(s_vertexShadera);
				//assert(SUCCEEDED(result));
				//result = eae6320::Graphics::Mesh::s_direct3dDevice->SetPixelShader(s_fragmentShader);
				//assert(SUCCEEDED(result));

			}
			// Bind a specific vertex buffer to the device as a data source
			/*{
			// There can be multiple streams of data feeding the display adaptor at the same time
			const unsigned int streamIndex = 0;
			// It's possible to start streaming data in the middle of a vertex buffer
			const unsigned int bufferOffset = 0;
			// The "stride" defines how large a single vertex is in the stream of data
			const unsigned int bufferStride = sizeof(sVertex);
			eae6320::Graphics::Mesh::s_direct3dDevice->SetStreamSource(streamIndex, s_draw.s_vertexBuffer, bufferOffset, bufferStride);
			assert(SUCCEEDED(result));
			}


			//s_Effect->SetPositionOffset(x, y);
			//s_Effect->SetPositionOffset(0, 0);
			bool res_draw = s_draw.draw();
			assert(res_draw);
			eae6320::Graphics::getGlobalEffect()->SetPositionOffset(0, 0);
			bool res_draw2 = s_draw2.draw();
			eae6320::Graphics::getGlobalEffect()->SetPositionOffset(1, 0);
			res_draw2 = s_draw2.draw();
			assert(res_draw2);

			/* // Bind a specific index buffer to the device as a data source
			{
			result = s_direct3dDevice->SetIndices(s_indexBuffer);
			assert(SUCCEEDED(result));
			}
			// Render objects from the current streams
			{
			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			const D3DPRIMITIVETYPE primitiveType = D3DPT_TRIANGLELIST;
			// It's possible to start rendering primitives in the middle of the stream
			const unsigned int indexOfFirstVertexToRender = 0;
			const unsigned int indexOfFirstIndexToUse = 0;
			// We are drawing a square
			const unsigned int vertexCountToRender =6;	// How vertices from the vertex buffer will be used?
			const unsigned int primitiveCountToRender = 2;	// How many triangles will be drawn?
			result = s_direct3dDevice->DrawIndexedPrimitive(primitiveType,
			indexOfFirstVertexToRender, indexOfFirstVertexToRender, vertexCountToRender,
			indexOfFirstIndexToUse, primitiveCountToRender);
			assert(SUCCEEDED(result));
			}
		}
		eae6320::Graphics::End_render();
		eae6320::Graphics::Back_Buffer();
		
	}

	// Everything been drawn to the "back buffer", which is just an image in memory.
	// In order to display it, the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		eae6320::Graphics::Back_Buffer();
		
	}
} */
//}
