
#ifndef EAE6320_GAMEOBJECT_H
#define EAE6320_GAMEOBJECT_H

// Header Files
//=============
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		
		class GameObject
		{
		private:
			struct BoxCollider
			{
				Math::cVector sExtends;
			};

		public:
			Graphics::Mesh m_mesh;
			material *m_material;
			
			Math::cVector m_position;
			Math::cQuaternion m_orientation;
			BoxCollider m_collider;
			Math::cVector m_velocity;
			char*  m_path_mesh;
			char* m_path_material;
			float m_alpha;
			float m_boundingSphereRadus;

		public:
			GameObject(char* const i_path_mesh, char* const i_path_material);

			bool LoadObject();
			void DrawObject();
			void UpdatePosition(Math::cVector i_position_offset);
			void ReleaseGameObject();
		};

	}
}
#endif	// EAE6320_GAMEOBJECT_H