#ifndef EAE6320_MATERIAL_H
#define EAE6320_MATERIAL_H

#include "Effect.h"
#include <sstream>
#include <iostream>
#include <fstream> 
#include <iostream>
#include "../../External/OpenGlExtensions/OpenGlExtensions.h"
namespace eae6320
{
	class material
	{
		public:
		enum ShaderTypes
		{
			vertexshader,
			fragmentshader,
		};
		struct sParameterToSet
		{
			Effect::tUniformHandle uniformHandle;
			ShaderTypes shaderType;
			float values[4];
			uint8_t valueCountToSet;
		};
		char *texturename;
		char *texturepath;
		Effect *i_effect;
#if defined( EAE6320_PLATFORM_D3D )
		IDirect3DTexture9* loadedtexture;
		DWORD samplerid;
		D3DXHANDLE textureHandle;
		void loadtexture();
#elif defined( EAE6320_PLATFORM_GL )
		GLuint loadedtexture;
		GLint samplerid;
		GLenum textureUnit = 0;
		bool LoadTexture(const char* const i_path, GLuint& o_texture);
#endif
		
		std::string * uniformnames;
		int i;
		char *effectpath;
		uint32_t uniformCount;
		sParameterToSet *uniformdata;
		bool CreateMaterial(const char* const path);
		void SetMaterialUniforms();
		void UpdateMaterial();
		void init_effect();
		
		void loadtexturehandle();
		void 	loadsamplerid();
		void updateTexture();
	};
}

#endif