// Header Files
//=============
#include "Graphics.h"
#include "DebugShapes.h"
#include "../Math/Functions.h"

#include <math.h>
#include <vector>

// Interface
//==========

// DebugLine Interface
void eae6320::Graphics::DebugLine::LoadDebugLine()
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ m_startPoint.x, m_startPoint.y, m_startPoint.z, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_endPoint.x, m_endPoint.y, m_endPoint.z, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		// Using same color for the both vertices to draw line of same color
	};

	// create a vertex buffer interface called i_buffer
	const unsigned int vertexCount = 2;
	const unsigned int bufferSize = vertexCount * sizeof(CUSTOMVERTEX);
	DWORD usage = 0;
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = 0;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* const notUsed = NULL;
	//Graphics::Mesh::s_direct3dDevice
		Graphics::Mesh::s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
		&m_vBuffer, notUsed);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	m_vBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	m_vBuffer->Unlock();

	return;
}

void eae6320::Graphics::DebugLine::DrawLine()
{
	Graphics::Mesh::s_direct3dDevice->SetStreamSource(0, m_vBuffer, 0, sizeof(CUSTOMVERTEX));

	const unsigned int primitiveCount = 1;
	Graphics::Mesh::s_direct3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, primitiveCount);
}

// DebugBox Interface
void eae6320::Graphics::DebugBox::LoadDebugBox()
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ m_position.x - m_size, m_position.y + m_size, m_position.z - m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x + m_size, m_position.y + m_size, m_position.z - m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x - m_size, m_position.y - m_size, m_position.z - m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x + m_size, m_position.y - m_size, m_position.z - m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x - m_size, m_position.y + m_size, m_position.z + m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x + m_size, m_position.y + m_size, m_position.z + m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x - m_size, m_position.y - m_size, m_position.z + m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
		{ m_position.x + m_size, m_position.y - m_size, m_position.z + m_size, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255), },
	};

	// create a vertex buffer interface called v_buffer
	const unsigned int vertexCount = 8;
	const unsigned int bufferSize = vertexCount * sizeof(CUSTOMVERTEX);
	DWORD usage = 0;
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = 0;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* const notUsed = NULL;
	Graphics::Mesh::s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
		&m_vBuffer, notUsed);

	VOID* pVoid;    // a void pointer
	int temp = sizeof(vertices);
	// lock v_buffer and load the vertices into it
	m_vBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	m_vBuffer->Unlock();

	// create the indices using an int array
	short indices[] =
	{
		0, 2, 1,    // side 1
		2, 3, 1,
		4, 6, 0,    // side 2
		6, 2, 0,
		7, 6, 5,    // side 3
		6, 4, 5,
		3, 7, 1,    // side 4
		7, 5, 1,
		4, 0, 5,    // side 5
		0, 1, 5,
		3, 2, 7,    // side 6
		2, 6, 7,
	};

	// create an index buffer interface called i_buffer
	Graphics::Mesh::s_direct3dDevice->CreateIndexBuffer(36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_iBuffer,
		NULL);

	// lock i_buffer and load the indices into it
	m_iBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	m_iBuffer->Unlock();
}

void eae6320::Graphics::DebugBox::DrawBox()
{
	// Turning on wireframe
	Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// select the vertex and index buffers to use
	Graphics::Mesh::s_direct3dDevice->SetStreamSource(0, m_vBuffer, 0, sizeof(CUSTOMVERTEX));
	Graphics::Mesh::s_direct3dDevice->SetIndices(m_iBuffer);

	// draw the cube
	Graphics::Mesh::s_direct3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	// Turning off wireframe
	Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

// DebugSphere Interface
void eae6320::Graphics::DebugSphere::LoadDebugSphere()
{
	//ID3DXMesh *i_sphereMesh;
	//
	//D3DXCreateSphere(Graphics::Mesh::s_direct3dDevice, m_radius, m_slices, m_stacks, &i_sphereMesh, NULL);

	//i_sphereMesh->GetVertexBuffer(&m_vBuffer);
	//i_sphereMesh->GetIndexBuffer(&m_iBuffer);

	//CUSTOMVERTEX* pVoid;

	//m_vBuffer->Lock(0, 0, (void**)&pVoid, 0);
	//m_vertexCount = i_sphereMesh->GetNumVertices();
	//m_triangleCount = i_sphereMesh->GetNumFaces();
	///*for (size_t i = 0; i < m_vertexCount; ++i)
	//{
	//	pVoid[i].x += m_center.x;
	//	pVoid[i].y += m_center.y;
	//	pVoid[i].z += m_center.z;

	//	pVoid[i].r = (uint8_t)(m_color.x * 255);
	//	pVoid[i].g = (uint8_t)(m_color.y * 255);
	//	pVoid[i].b = (uint8_t)(m_color.z * 255);
	//}*/
	//m_vBuffer->Unlock();


	std::vector<CUSTOMVERTEX> i_vertexData;
	std::vector<unsigned int> i_indexData;

	i_vertexData.push_back(CUSTOMVERTEX(0.0f, m_radius, 0.0f, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255)));
	float phiStep = Math::Pi / m_stacks;
	float thetaStep = 2.0f * Math::Pi / m_slices;

	for (size_t i = 1; i <= m_stacks - 1; ++i) {
		float phi = i * phiStep;
		for (size_t j = 0; j <= m_slices; ++j) {
			float theta = j * thetaStep;
			i_vertexData.push_back(CUSTOMVERTEX((m_radius * sin(phi) * cos(theta)),
				(m_radius * cos(phi)),
				(m_radius * sin(phi) * sin(theta)),
				(uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255)));
		}
	}

	i_vertexData.push_back(CUSTOMVERTEX(0.0f, -m_radius, 0.0f, (uint8_t)(m_color.z * 255), (uint8_t)(m_color.y * 255), (uint8_t)(m_color.x * 255)));

	for (size_t i = 1; i <= m_slices; ++i) {
		i_indexData.push_back(0);
		i_indexData.push_back(i);
		i_indexData.push_back(i + 1);
	}
	size_t baseIndex = 1;
	size_t ringVertexCount = m_slices + 1;
	for (size_t i = 0; i < m_stacks - 2; ++i) {
		for (size_t j = 0; j < m_slices; ++j) {
			i_indexData.push_back(baseIndex + i*ringVertexCount + j + 1);
			i_indexData.push_back(baseIndex + i*ringVertexCount + j);
			i_indexData.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			i_indexData.push_back(baseIndex + i*ringVertexCount + j + 1);
			i_indexData.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			i_indexData.push_back(baseIndex + (i + 1) *ringVertexCount + j + 1);
		}
	}
	size_t southPoleIndex = i_indexData.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (size_t i = 0; i < m_slices; ++i) {
		i_indexData.push_back(southPoleIndex);
		i_indexData.push_back(baseIndex + i);
		i_indexData.push_back(baseIndex + i + 1);
	}

	m_vertexCount = i_vertexData.size();
	m_indexCount = i_indexData.size();

	CUSTOMVERTEX *vertices = new CUSTOMVERTEX[m_vertexCount];
	for (int i = 0; i < m_vertexCount; i++)
		vertices[i] = i_vertexData[i];

	unsigned int *indices = new unsigned int[m_indexCount];
	for (int i = 0; i < m_indexCount; i++)
		indices[i] = i_indexData[i];

	// create a vertex buffer interface called v_buffer
	const unsigned int bufferSize = m_vertexCount * sizeof(CUSTOMVERTEX);
	DWORD usage = 0;
	// We will define our own vertex format
	const DWORD useSeparateVertexDeclaration = 0;
	// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
	const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
	HANDLE* const notUsed = NULL;
	Graphics::Mesh::s_direct3dDevice->CreateVertexBuffer(bufferSize, usage, useSeparateVertexDeclaration, useDefaultPool,
		&m_vBuffer, notUsed);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	m_vBuffer->Lock(0, 0, (void**)&pVoid, 0);
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].x += m_center.x;
		vertices[i].y += m_center.y;
		vertices[i].z += m_center.z;
	}
	memcpy(pVoid, vertices, m_vertexCount * sizeof(CUSTOMVERTEX));
	m_vBuffer->Unlock();

	// create an index buffer interface called i_buffer
	Graphics::Mesh::s_direct3dDevice->CreateIndexBuffer(m_indexCount * sizeof(unsigned int),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&m_iBuffer,
		NULL);

	// lock i_buffer and load the indices into it
	m_iBuffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, m_indexCount * sizeof(unsigned int));
	m_iBuffer->Unlock();
	delete[] indices;
	delete[] vertices;
}

void eae6320::Graphics::DebugSphere::DrawSphere()
{
	// Turning on wireframe
	Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// select the vertex and index buffers to use
	Graphics::Mesh::s_direct3dDevice->SetStreamSource(0, m_vBuffer, 0, sizeof(CUSTOMVERTEX));
	Graphics::Mesh::s_direct3dDevice->SetIndices(m_iBuffer);

	// draw the cube
	Graphics::Mesh::s_direct3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vertexCount, 0, m_indexCount / 3);

	// Turning off wireframe
	Graphics::Mesh::s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
