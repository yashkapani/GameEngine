#ifndef Vertex_Data_H_
#define VBertex_Data_H_
#include <d3d9.h>
#include <gl/GL.h>
class Vertex_Data
{
public:
#if defined( EAE6320_PLATFORM_D3D )
	IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;
	IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
	IDirect3DIndexBuffer9* s_indexBuffer = NULL;
#elif defined( EAE6320_PLATFORM_GL )
	GLuint s_vertexArrayId = 0;
#endif

	bool draw();
};

#endif