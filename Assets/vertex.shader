#include "shaders.inc"

// Declaration of offset position
uniform float4x4 g_transform_localToWorld;
uniform float4x4 g_transform_worldToView;
uniform float4x4 g_transform_viewToScreen;
//uniform float3 g_position_offset;

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateVertexDeclaration()

	// These values come from one of the sVertex that we filled the vertex buffer with in C code
	in const float3 i_position_local : POSITION,
	in const float4 i_color : COLOR,
	in const float2 i_texture : TEXCOORD0,

	// Output
	//=======

	// A POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 o_position_screen : POSITION,

	// Any other data is optional; the GPU doesn't know or care what it is,
	// and will merely interpolate it across the triangle
	// and give us the resulting interpolated value in a fragment shader.
	// It is then up to us to use it however we want to.
	// The "semantics" are used to match the vertex shader outputs
	// with the fragment shader inputs
	// (note that OpenGL uses arbitrarily assignable number IDs to do the same thing).
	out float4 o_color : COLOR,
	out float2 o_texture : TEXCOORD0

	)

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// These values come from one of the sVertex that we filled the vertex buffer with in C code
layout(location = 0) in vec3 i_position_local;
layout(location = 1) in vec4 i_color;
layout(location = 2) in vec2 i_texture;
// Output
//=======

// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an implicit required variable called "gl_Position"

// Any other data is optional; the GPU doesn't know or care what it is,
// and will merely interpolate it across the triangle
// and give us the resulting interpolated value in a fragment shader.
// It is then up to us to use it however we want to.
// The locations are used to match the vertex shader outputs
// with the fragment shader inputs
// (note that Direct3D uses arbitrarily assignable "semantics").
layout(location = 0) out vec4 o_color;
layout(location = 1) out vec2 o_texture;
// Entry Point
//============

void main()

#endif


{
	// Calculate position
	// Calculate the position of this vertex on screen
	{
		// The position stored in the vertex is in "local space",
		// meaning that it is relative to the center (or "origin", or "pivot")
		// of the mesh.
		// The graphics hardware needs the position of the vertex
		// in normalized device coordinates,
		// meaning where the position of the vertex should be drawn
		// on the screen.
		// This position that we need to output, then,
		// is the result of taking the original vertex in local space
		// and transforming it into "screen space".

		// Any matrix transformations that include translation
		// will operate on a float4 position,
		// which _must_ have 1 for the w value
		//g_transform_localToWorld = mat4(1.0);
		//g_transform_worldToView = mat4(1.0);
		//g_transform_viewToScreen = mat4(1.0);
		
		float4 position_world = Transform(float4(i_position_local, 1.0), g_transform_localToWorld);		
		float4 position_view = Transform(position_world, g_transform_worldToView);
		O_POSITION_SCREEN = Transform(position_view, g_transform_viewToScreen);

		//gl_Position = float4(i_position_local.xyz,1) ;
	}
	// Pass the input color to the fragment shader unchanged:
	{
		o_color = i_color;
		o_texture = i_texture;
	}
}