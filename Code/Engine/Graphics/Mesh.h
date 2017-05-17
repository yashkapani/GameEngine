#ifndef MESH_H_
#define MESH_H_
#include <d3d9.h>
#include <gl/GL.h>
#include <cstdint>
#include <cstdlib>
#include <vector>
namespace eae6320
{
	namespace Graphics
	{

		class Mesh
		{
		private:
			bool CreateVertexArray();

			HRESULT GetVertexProcessingUsage(DWORD& o_usage);

		public:
			//Mesh();
			bool draw();
			bool ReadSquareValues(const char* const path);
			bool CreateIndexBuffer();
			//~Mesh();

			int m_verticesCount;
			struct sVertex
			{
				// POSITION
				// 5 floats == 20 bytes
				// Offset = 0
				float x, y ,z;
				float nx, ny, nz;
				float u, v;
			
				// COLOR0
				// 4 uint8_ts == 4 bytes
				// Offset = 20
#if defined( EAE6320_PLATFORM_D3D )
				uint8_t b, g, r, a;
#elif defined( EAE6320_PLATFORM_GL )
				uint8_t r, g, b, a;	// 8 bits [0,255] per RGBA channel (the alpha channel is unused but is present so that color uses a full 4 bytes)
#endif
			};
			int m_indicesCount;
			uint32_t* m_indexData;
			sVertex *m_vertexData;
#if defined( EAE6320_PLATFORM_D3D )
			IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;
			IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
			IDirect3DIndexBuffer9* s_indexBuffer = NULL;
			static IDirect3DDevice9* s_direct3dDevice;

#elif defined( EAE6320_PLATFORM_GL )
			GLuint s_vertexArrayId = 0;
#endif
		};
	}
}
#endif