// Header Files
//=============
#include "GameObject.h"
#include "Graphics.h"
#include "../Math/cMatrix_transformation.h"

// Interface
//==========

eae6320::Graphics::GameObject::GameObject(char* const i_path_mesh, char* const i_path_material)
{
	m_material = new eae6320::material();
	m_material->init_effect();
	m_material->i_effect->m_direct3Ddevice = eae6320::Graphics::Mesh::s_direct3dDevice;
	//m_mesh = Mesh(i_path_mesh);
	//m_material = Material(i_path_material);
	m_path_material = i_path_material;
	m_path_mesh = i_path_mesh;
	m_position = Math::cVector();
	m_orientation = Math::cQuaternion();
	m_velocity = Math::cVector(0, 0, 0);
	m_collider.sExtends = Math::cVector(0, 0, 0);
	

	m_alpha = 1.0;
}

bool eae6320::Graphics::GameObject::LoadObject()
{

	if (!m_mesh.ReadSquareValues(m_path_mesh))
	{
		return false;
	}
	if (!m_mesh.CreateIndexBuffer())
	{
		return false;
	}
	if (!m_material->CreateMaterial("data/Red.material"))
	{
		return false;
	}
	return true;
}

void eae6320::Graphics::GameObject::DrawObject()
{
	m_material->i_effect->BindEffect();
	m_material->UpdateMaterial();
	m_material->i_effect->SetPositionOffset(m_position.x, m_position.y, m_position.z, m_orientation);
	m_mesh.draw();
		//m_material.SetMaterial();
	//m_material.SetMaterialUniforms();
	//m_material.SetEngineUniforms(Math::cMatrix_transformation(m_orientation, m_position), *eae6320::Graphics::s_camera);

	//m_mesh.DrawMesh();
}

void eae6320::Graphics::GameObject::UpdatePosition(Math::cVector i_position_offset)
{
	m_position += i_position_offset;
}

void eae6320::Graphics::GameObject::ReleaseGameObject()
{
	//m_mesh.ReleaseMesh();
	//
	//m_material.ReleaseMaterial();
}