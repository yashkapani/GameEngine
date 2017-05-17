#ifndef EAE6320_Effect_H
#define EAE6320_Effect_H
#include <sstream>
#include <d3d9.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "../Math/cMatrix_transformation.h"
#ifdef EAE6320_PLATFORM_D3D
#include <d3dx9shader.h>
#endif

namespace eae6320
{
	class Effect
	{

	public:
		typedef
#if defined( EAE6320_PLATFORM_D3D )
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			const char*
#elif defined( EAE6320_PLATFORM_GL )
			GLint
#endif
			tUniformHandle;

		enum  renderstates :uint8_t
		{
			alpha = 1 << 0,
			depthTest = 1 << 1,
			depthwrite = 1 << 2,
		};
		
		uint8_t renderstatevalue;
		bool CreateEffect(const char* const path);
		//bool CreateEffect(const char * vertex_path);
		bool LoadVertexShader(const char *vertex_path);
		bool LoadFragmentShader(const char *fragment_path);
		void BindEffect();
	

		void SetTransforLocaltoWorld(float x, float y, float z);
		void SetTransformWorldtoView(float x, float y, float z);
		void SetTransformViewtoScene(float x, float y, float z);
		void SetPositionOffset(float x, float y, float z,eae6320::Math::cQuaternion o_quaternion);
#ifdef EAE6320_PLATFORM_D3D
		void UpdateMaterialUniform(int ShaderType, float value[4], float floatcount,D3DXHANDLE unifortoset);
		IDirect3DVertexShader9* s_vertexShader;
		IDirect3DPixelShader9* s_fragmentShader;
		IDirect3DDevice9* m_direct3Ddevice;
		ID3DXConstantTable* vertexShaderConstantTable;
		D3DXHANDLE MaterialUniform(const char * i_uniform);
		ID3DXConstantTable* fragmentShaderConstantTable;
		D3DXHANDLE uniformhandle_localtoworld;
		D3DXHANDLE uniformhandle_worldtoview;
		D3DXHANDLE uniformhandle_viewtoscreen;

		//public:
		//	Effect(IDirect3DVertexShader9* i_vertexShader, IDirect3DPixelShader9* i_fragmentShader):
		//		s_vertexShader(i_vertexShader),
		//		s_fragmentShader(i_fragmentShader)
		//	{}
#elif defined EAE6320_PLATFORM_GL
		bool LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage);
	public:
		GLuint i_programId = 0;
		bool CreateProgram(char * vertex_path, char * fragment_path);
		GLint location;
		GLint MaterialUniform(const char * i_uniform);
		GLint uniformhandle_localtoworld;
		GLint uniformhandle_worldtoview;
		GLint uniformhandle_viewtoscreen;
		void eae6320::Effect::UpdateMaterialUniform(int ShaderType, float values[4], float floatcount, GLint uniformtoset);

#endif

	};
}
#endif