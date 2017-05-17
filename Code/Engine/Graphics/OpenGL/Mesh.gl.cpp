#include "../Graphics.h"
#include "../Mesh.h"
#include <cassert>
#include <gl/GLU.h>
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../../UserOutput/UserOutput.h"
#include <sstream>

namespace eae6320
{
	namespace Graphics
	{
		

		bool Mesh::draw()
		{
			{
				glBindVertexArray(s_vertexArrayId);
				assert(glGetError() == GL_NO_ERROR);
			}
			// We are using triangles as the "primitive" type,
			// and we have defined the vertex buffer as a triangle list
			// (meaning that every triangle is defined by three vertices)
			const GLenum mode = GL_TRIANGLES;
			
			// We'll use 32-bit indices in this class to keep things simple
			// (i.e. every index will be a 32 bit unsigned integer)
			const GLenum indexType = GL_UNSIGNED_INT;
			// It is possible to start rendering in the middle of an index buffer
			const GLvoid* const offset = 0;
			// We are d//rawing a square
			const GLsizei primitiveCountToRender = m_indicesCount / 3;	// How many triangles will be drawn?
			const GLsizei vertexCountPerTriangle = 3;
			const GLsizei vertexCountToRender = primitiveCountToRender * vertexCountPerTriangle;
			glDrawElements(mode, vertexCountToRender, indexType, offset);
			assert(glGetError() == GL_NO_ERROR);
			return true;
		}

		bool Mesh::CreateVertexArray()
		{
			bool wereThereErrors = false;
			GLuint vertexBufferId = 0;
			GLuint indexBufferId = 0;

			// Create a vertex array object and make it active
			{
				const GLsizei arrayCount = 1;
				glGenVertexArrays(arrayCount, &s_vertexArrayId);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindVertexArray(s_vertexArrayId);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to bind the vertex array: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get an unused vertex array ID: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}

			// Create a vertex buffer object and make it active
			{
				const GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &vertexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to bind the vertex buffer: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get an unused vertex buffer ID: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Assign the data to the buffer
			{
				// We are drawing a square
				//const unsigned int vertexCount = 4;	// What is the minimum number of vertices a square needs (so that no data is duplicated)?
				//sVertex vertexData[vertexCount];
				// Fill in the data for the triangle
				/*{
				// You will need to fill in two pieces of information for each vertex:
				//	* 2 floats for the POSITION
				//	* 4 uint8_ts for the COLOR

				// The floats for POSITION are for the X and Y coordinates, like in Assignment 02.
				// The difference this time is that there should be fewer (because we are sharing data).

				// The uint8_ts for COLOR are "RGBA", where "RGB" stands for "Red Green Blue" and "A" for "Alpha".
				// Conceptually each of these values is a [0,1] value, but we store them as an 8-bit value to save space
				// (color doesn't need as much precision as position),
				// which means that the data we send to the GPU will be [0,255].
				// For now the alpha value should _always_ be 255, and so you will choose color by changing the first three RGB values.
				// To make white you should use (255, 255, 255), to make black (0, 0, 0).
				// To make pure red you would use the max for R and nothing for G and B, so (1, 0, 0).
				// Experiment with other values to see what happens!

				vertexData[0].x = 0.0f;
				vertexData[0].y = 0.0f;
				// Red
				vertexData[0].r = 255;
				vertexData[0].g = 0;
				vertexData[0].b = 0;
				vertexData[0].a = 255;

				vertexData[1].x = 0.0f;
				vertexData[1].y = 1.0f;
				// Red
				vertexData[1].r = 255;
				vertexData[1].g = 255;
				vertexData[1].b = 0;
				vertexData[1].a = 255;
				// etc.
				vertexData[2].x = 1.0f;
				vertexData[2].y = 0.0f;
				// Red
				vertexData[2].r = 0;
				vertexData[2].g = 255;
				vertexData[2].b = 0;
				vertexData[2].a = 255;

				vertexData[3].x = 1.0f;
				vertexData[3].y = 1.0f;
				// Red
				vertexData[3].r = 0;
				vertexData[3].g = 0;
				vertexData[3].b = 255;
				vertexData[3].a = 255;
				// etc.
				}*/
				sVertex* ss_vertexData = reinterpret_cast<sVertex*>(malloc(sizeof(sVertex) * m_verticesCount));
				memcpy(ss_vertexData, m_vertexData, sizeof(m_vertexData[0]) * m_verticesCount);

				glBufferData(GL_ARRAY_BUFFER, m_verticesCount * sizeof(sVertex), reinterpret_cast<GLvoid*>(ss_vertexData),
					// Our code will only ever write to the buffer
					GL_STATIC_DRAW);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to allocate the vertex buffer: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Initialize the vertex format
			{
				const GLsizei stride = sizeof(sVertex);
				GLvoid* offset = 0;

				// Position (0)
				// 3 floats == 12 bytes
				// Offset = 0
				{
					const GLuint vertexElementLocation = 0;
					const GLint elementCount = 3;
					const GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride, offset);
					const GLenum errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							offset = reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + (elementCount * sizeof(float)));
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to enable the POSITION vertex attribute: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to set the POSITION vertex attribute: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				// TEXCOORD0 (2)
				// 2 floats == 8 bytes
				// Offset = 12
				{
					const GLuint vertexElementLocation = 2;
					const GLint elementCount = 2;
					const GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride, offset);
					const GLenum errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							offset =  reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + (elementCount * sizeof(float)));
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to enable the TEXCOORD0 vertex attribute: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to set the POSITION vertex attribute: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				// Color (1)
				// 4 uint8_ts == 4 bytes
				// Offset = 20
				{
					const GLuint vertexElementLocation = 1;
					const GLint elementCount = 4;
					// Each element will be sent to the GPU as an unsigned byte in the range [0,255]
					// but these values should be understood as representing [0,1] values
					// and that is what the shader code will interpret them as
					// (in other words, we could change the values provided here in C code
					// to be floats and sent GL_FALSE instead and the shader code wouldn't need to change)
					const GLboolean normalized = GL_TRUE;
					glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, normalized, stride, offset);
					const GLenum errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						glEnableVertexAttribArray(vertexElementLocation);
						const GLenum errorCode = glGetError();
						if (errorCode == GL_NO_ERROR)
						{
							offset = reinterpret_cast<GLvoid*>(reinterpret_cast<uint8_t*>(offset) + (elementCount * sizeof(uint8_t)));
						}
						else
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to enable the COLOR0 vertex attribute: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
							goto OnExit;
						}
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to set the COLOR0 vertex attribute: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
			}

			// Create an index buffer object and make it active
			{
				const GLsizei bufferCount = 1;
				glGenBuffers(bufferCount, &indexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
					const GLenum errorCode = glGetError();
					if (errorCode != GL_NO_ERROR)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to bind the index buffer: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get an unused index buffer ID: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Allocate space and copy the triangle data into the index buffer
			{
				// We are drawing a square
				//const unsigned int triangleCount = 2;	// How many triangles does a square have?
				//const unsigned int vertexCountPerTriangle = 3;
				//uint32_t indexData[triangleCount * vertexCountPerTriangle];
				// Fill in the data for the triangle
				/*{
					// EAE6320_TODO:
					// You will need to fill in 3 indices for each triangle that needs to be drawn.
					// Each index will be a 32-bit unsigned integer,
					// and will index into the vertex buffer array that you have created.
					// The order of indices is important, but the correct order will depend on
					// which vertex you have assigned to which spot in your vertex buffer
					// (also remember to maintain the correct handedness for the triangle winding order).

					// Triangle 0
					indexData[0] = 0;
					indexData[1] = 2;
					indexData[2] = 3;

					// Triangle 1
					indexData[3] = 0;
					indexData[4] = 3;
					indexData[5] = 1;
					// etc...
				}*/

				const GLsizeiptr bufferSize = m_indicesCount *  sizeof(uint32_t);
				uint32_t *s_indexData = reinterpret_cast<uint32_t*>(malloc(sizeof(uint32_t) * m_indicesCount));
				memcpy(s_indexData, m_indexData, sizeof(uint32_t) * m_indicesCount);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, reinterpret_cast<const GLvoid*>(s_indexData),
					// Our code will only ever write to the buffer
					GL_STATIC_DRAW);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to allocate the index buffer: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}

		OnExit:

			// Delete the buffer object
			// (the vertex array will hold a reference to it)
			if (s_vertexArrayId != 0)
			{
				// Unbind the vertex array
				// (this must be done before deleting the vertex buffer)
				glBindVertexArray(0);
				const GLenum errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (vertexBufferId != 0)
					{
						// NOTE: If you delete the vertex buffer object here, as I recommend,
						// then gDEBugger won't know about it and you won't be able to examine the data.
						// If you find yourself in a situation where you want to see the buffer object data in gDEBugger
						// comment out this block of code temporarily
						// (doing this will cause a memory leak so make sure to restore the deletion code after you're done debugging).
						const GLsizei bufferCount = 1;
						glDeleteBuffers(bufferCount, &vertexBufferId);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "OpenGL failed to delete the vertex buffer: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
						}
						vertexBufferId = 0;
					}
					if (indexBufferId != 0)
					{
						// NOTE: See the same comment above about deleting the vertex buffer object here and gDEBugger
						// holds true for the index buffer
						const GLsizei bufferCount = 1;
						glDeleteBuffers(bufferCount, &indexBufferId);
						const GLenum errorCode = glGetError();
						if (errorCode != GL_NO_ERROR)
						{
							wereThereErrors = true;
							std::stringstream errorMessage;
							errorMessage << "\nOpenGL failed to delete the index buffer: " <<
								reinterpret_cast<const char*>(gluErrorString(errorCode));
							eae6320::UserOutput::Print(errorMessage.str());
						}
						indexBufferId = 0;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to unbind the vertex array: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
				}
			}

			return !wereThereErrors;
		}
	}
}