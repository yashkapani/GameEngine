#include "Graphics.h"
#include "Renderable.h"


		

		void eae6320::Graphics::Renderable::Rebderable_draw()
		{
			s_Effect->BindEffect();
			//s_Effect->SetPositionOffset(x,y,0);
			s_draw.draw();
		}
		
		/*
		void eae6320::Graphics::Renderable::UpdatePosition(Math::cVector i_position_offset)
		{
			m_position_offset += i_position_offset;
		*/

