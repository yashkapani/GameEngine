#include "../Effect.h"
#include "../Mesh.h"
#include <cassert>
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include <sstream>
#include <iostream>
#include "../Camera.h"
#include <fstream> 
#include "../../UserOutput/UserOutput.h"
#include "../../Math/Functions.h"

void eae6320::Effect::BindEffect()
{
	HRESULT result = m_direct3Ddevice->SetVertexShader(s_vertexShader);
	assert(SUCCEEDED(result));
	result = m_direct3Ddevice->SetPixelShader(s_fragmentShader);
	assert(SUCCEEDED(result));
	if (renderstatevalue   & alpha)
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_direct3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_direct3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	}
	else
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_direct3Ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_direct3Ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (renderstatevalue & depthTest)
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_direct3Ddevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	else
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	if (renderstatevalue & depthwrite )
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	else
	{
		m_direct3Ddevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

bool eae6320::Effect::LoadVertexShader(const char * vertex_path)
{
	// Load the source code from file and compile it
	//ID3DXBuffer* compiledShader;
	//{
	//	//const char* sourceCodeFileName = "data/vertex.shader";
	//	const D3DXMACRO defines[] = {
	//		{ "EAE6320_PLATFORM_D3D", "1" },{ NULL, NULL }
	//	};
	//	ID3DXInclude* noIncludes = NULL;
	//	const char* entryPoint = "main";
	//	const char* profile = "vs_3_0";
	//	const DWORD noFlags = 0;
	//	ID3DXBuffer* errorMessages = NULL;
	//	ID3DXConstantTable** noConstants = NULL;
	//	

	//	HRESULT result = D3DXCompileShaderFromFile(vertex_path, defines, noIncludes, entryPoint, profile, noFlags,
	//		&compiledShader, &errorMessages, &vertexShaderConstantTable);
		/*if (SUCCEEDED(result))
		{
			uniformhandle = vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
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
				errorMessage << "Direct3D failed to compile the vertex shader from the file " << vertex_path
					<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
				eae6320::UserOutput::Print(errorMessage.str());
				errorMessages->Release();
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "Direct3D failed to compile the vertex shader from the file " << vertex_path;
				eae6320::UserOutput::Print(errorMessage.str());
			}
			return false;
		}
	}*/
	// Create the vertex shader object
	std::ifstream is(vertex_path, std::ifstream::binary);
	is.seekg(0, is.end);
	int length = static_cast<int>(is.tellg());
	is.seekg(0, is.beg);
	char * buffer = new char[length];
	is.read(buffer, length);
	is.close();
	bool wereThereErrors = false;
	{
		HRESULT result = m_direct3Ddevice->CreateVertexShader(reinterpret_cast<DWORD*>(buffer),
			&s_vertexShader);

		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
			wereThereErrors = true;
		}
		D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &vertexShaderConstantTable);
		uniformhandle_localtoworld = vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_localToWorld");
		uniformhandle_worldtoview = vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_worldToView");
		uniformhandle_viewtoscreen = vertexShaderConstantTable->GetConstantByName(NULL, "g_transform_viewToScreen");
		//compiledShader->Release();
	}
	return !wereThereErrors;

}


bool eae6320::Effect::LoadFragmentShader(const char * fragment_path)
{
	//// Load the source code from file and compile it
	//ID3DXBuffer* compiledShader;
	//{
	//	//const char* sourceCodeFileName = "data/fragment.shader";
	//	const D3DXMACRO defines[] = {
	//		{ "EAE6320_PLATFORM_D3D", "1" },{ NULL, NULL }
	//	};
	//	ID3DXInclude* noIncludes = NULL;
	//	const char* entryPoint = "main";
	//	const char* profile = "ps_3_0";
	//	const DWORD noFlags = 0;
	//	ID3DXBuffer* errorMessages = NULL;
	//	ID3DXConstantTable** noConstants = NULL;
	//	//ID3DXConstantTable* noConstants = NULL;

	//	HRESULT result = D3DXCompileShaderFromFile(fragment_path, defines, noIncludes, entryPoint, profile, noFlags,
	//		&compiledShader, &errorMessages, noConstants);


	//	if (SUCCEEDED(result))
	//	{
	//		if (errorMessages)
	//		{
	//			errorMessages->Release();
	//		}
	//	}
	//	else
	//	{
	//		if (errorMessages)
	//		{
	//			std::stringstream errorMessage;
	//			errorMessage << "Direct3D failed to compile the fragment shader from the file " << fragment_path
	//				<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
	//			eae6320::UserOutput::Print(errorMessage.str());
	//			errorMessages->Release();
	//		}
	//		else
	//		{
	//			std::stringstream errorMessage;
	//			errorMessage << "Direct3D failed to compile the fragment shader from the file " << fragment_path;
	//			eae6320::UserOutput::Print(errorMessage.str());
	//		}
	//		return false;
	//	}
	//}
	// Create the fragment shader object
	std::ifstream is(fragment_path, std::ifstream::binary);
	is.seekg(0, is.end);
	int length = static_cast<int>(is.tellg());
	is.seekg(0, is.beg);
	char * buffer = new char[length];
	is.read(buffer, length);
	is.close();
	bool wereThereErrors = false;
	{
		HRESULT result = m_direct3Ddevice->CreatePixelShader(reinterpret_cast<DWORD*>(buffer),
			&s_fragmentShader);
		if (FAILED(result))
		{
			eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
			wereThereErrors = true;
		}

		//compiledShader->Release();
	}
	D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &fragmentShaderConstantTable);

	return !wereThereErrors;
}

D3DXHANDLE eae6320::Effect::MaterialUniform(const char * i_uniform)
{
	return fragmentShaderConstantTable->GetConstantByName(NULL,i_uniform);
}
bool eae6320::Effect::CreateEffect(const char* const path)
{
	char *vertex_path;
	char *fragment_path;
	
	std::ifstream is(path, std::ifstream::binary);
	is.seekg(0, is.end);
	int length = static_cast<int>(is.tellg());
	is.seekg(0, is.beg);
	char * buffer = new char[length];
	is.read(buffer, length);
	is.close();
	char * shaderpaths = buffer;
	vertex_path = reinterpret_cast<char*>(shaderpaths);
	fragment_path = reinterpret_cast<char*>(shaderpaths + strlen(vertex_path)+1);
	renderstatevalue = reinterpret_cast<uint8_t>(shaderpaths + strlen(vertex_path) + 1 + strlen(fragment_path) + 1);
	LoadVertexShader(vertex_path);
	LoadFragmentShader(fragment_path);
	return true;
}
void eae6320::Effect::UpdateMaterialUniform(int ShaderType, float values[4], float floatcount, D3DXHANDLE uniformtoset)
{
	if (ShaderType == 1)
	{
		
		HRESULT result = fragmentShaderConstantTable->SetFloatArray(m_direct3Ddevice, uniformtoset, values, static_cast<int>(floatcount));
		//assert(result);
	}
	else
	{
		HRESULT result = vertexShaderConstantTable->SetFloatArray(m_direct3Ddevice, uniformtoset, values, static_cast<int>(floatcount));
		assert(result);
	}
}

void eae6320::Effect::SetPositionOffset(float x, float y, float z, eae6320::Math::cQuaternion o_quaternion)
{

	float floatArray[] = { x,y,z };
	int floatCount = 3;
	eae6320::Math::cVector offset(x, y,z);
	
	eae6320::Math::cMatrix_transformation i_locltoworld = eae6320::Math::cMatrix_transformation(o_quaternion, offset);
	eae6320::Math::cMatrix_transformation i_worldtoview = eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(Camera::getInstance().camear_rotation , Camera::getInstance().camera_axis);
	eae6320::Math::cMatrix_transformation i_viewtoscreen = eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(Math::ConvertDegreesToRadians(60), 800.0f/600.0f, 0.1f, 10000.0f);
	
	HRESULT result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_localtoworld,
		reinterpret_cast<const D3DXMATRIX*>(&i_locltoworld));
	 result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_worldtoview,
		reinterpret_cast<const D3DXMATRIX*>(&i_worldtoview));
	 result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_viewtoscreen,
		reinterpret_cast<const D3DXMATRIX*>(&i_viewtoscreen));
}

void eae6320::Effect::SetTransforLocaltoWorld(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	int floatCount = 3;
	HRESULT result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_localtoworld,
		reinterpret_cast<const D3DXMATRIX*>(&floatArray));
	
	//HRESULT result = vertexShaderConstantTable->SetFloatArray(m_direct3Ddevice, uniformhandle, floatArray, floatCount);
}
void eae6320::Effect::SetTransformWorldtoView(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	int floatCount = 3;
	HRESULT result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_worldtoview,
		reinterpret_cast<const D3DXMATRIX*>(&floatArray));
}
void eae6320::Effect::SetTransformViewtoScene(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	int floatCount = 3;
	HRESULT result = vertexShaderConstantTable->SetMatrixTranspose(m_direct3Ddevice, uniformhandle_viewtoscreen,
		reinterpret_cast<const D3DXMATRIX*>(&floatArray));
}
