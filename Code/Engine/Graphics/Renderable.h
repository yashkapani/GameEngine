#include "../Graphics/Mesh.h"
#include "../Graphics/Effect.h"
#ifndef EAE6320_GRAPHICS_Renderable_H
#define EAE6320_GRAPHICS_H

namespace eae6320
{
	namespace Graphics
	{
		
		class Renderable
		{
		public:
			 float x, y;
			 eae6320::Graphics::Mesh s_draw;
			 eae6320::Effect *s_Effect;
			void Rebderable_draw();
			

		};
	}
}

#endif