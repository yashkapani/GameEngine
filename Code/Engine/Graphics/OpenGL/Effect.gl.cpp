#include "../Effect.h"
#include "../Mesh.h"
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <gl/GL.h>
#include <string>
#include <iostream>
#include <fstream> 
#include <gl/GLU.h>
#include "../../UserOutput/UserOutput.h"
#include "../../Windows/WindowsFunctions.h"
#include <sstream>
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../Vertex_Data.h"
#include "../Graphics.h"
#include "../../Math/Functions.h"
#include "../Camera.h"

namespace
{
	struct sLogInfo
	{
		GLchar* memory;
		sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
		~sLogInfo() { if (memory) free(memory); }
	};
}
bool eae6320::Effect::CreateEffect(const char* const path)
{
	
	
	
	char *vertex_path;
	char *fragment_path;

	std::ifstream is(path, std::ifstream::binary);
	is.seekg(0, is.end);
	size_t length = static_cast<int>(is.tellg());
	is.seekg(0, is.beg);
	char * buffer = new char[length];
	is.read(buffer, length);
	is.close();
	char * shaderpaths = buffer;
	//vertex_path = new char[strlen(shaderpaths)];
	vertex_path = reinterpret_cast<char*>(shaderpaths);
	//shaderpaths = buffer + strlen(vertex_path) + 1;
	//fragment_path = new char[strlen(shaderpaths)];
	fragment_path = reinterpret_cast<char*>(shaderpaths + strlen(vertex_path) + 1);
	renderstatevalue = reinterpret_cast<uint8_t>(shaderpaths + strlen(vertex_path) + 1 + strlen(fragment_path) + 1);	
	CreateProgram(vertex_path,fragment_path);
	//LoadVertexShader(vertex_path);
	//LoadFragmentShader(fragment_path);
	
	return true;
}


bool eae6320::Effect::LoadFragmentShader(const char * fragment_path)
{
	// Verify that compiling shaders at run-time is supported
	{
		GLboolean isShaderCompilingSupported;
		glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
		if (!isShaderCompilingSupported)
		{
			eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
			return false;
		}
	}

	bool wereThereErrors = false;

	// Load the source code from file and set it into a shader
	GLuint fragmentShaderId = 0;
	void* shaderSource = NULL;
	{
		// Load the shader source code
		size_t fileSize;
		{
			//const char* sourceCodeFileName = "data/fragment.shader";
			std::string errorMessage;
			if (!LoadAndAllocateShaderProgram(fragment_path, shaderSource, fileSize, &errorMessage))
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print(errorMessage);
				goto OnExit;
			}
		}
		// Generate a shader
		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to get an unused fragment shader ID: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
			else if (fragmentShaderId == 0)
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print("OpenGL failed to get an unused fragment shader ID");
				goto OnExit;
			}
		}
		// Set the source code into the shader
		{
			const GLsizei shaderSourceCount = 1;
			const GLchar* shaderSources[] =
			{
				reinterpret_cast<GLchar*>(shaderSource)
			};
			const GLint* sourcesAreNullTerminated = NULL;

			glShaderSource(fragmentShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the fragment shader source code: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
	}


	// Compile the shader source code
	{
		glCompileShader(fragmentShaderId);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get compilation info
			// (this won't be used unless compilation fails
			// but it can be useful to look at when debugging)
			std::string compilationInfo;
			{
				GLint infoSize;
				glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetShaderInfoLog(fragmentShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						compilationInfo = info.memory;
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get compilation info of the fragment shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the fragment shader compilation info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Check to see if there were compilation errors
			GLint didCompilationSucceed;
			{
				glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didCompilationSucceed == GL_FALSE)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "The fragment shader failed to compile:\n" << compilationInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if compilation of the fragment shader source code succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to compile the fragment shader source code: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Attach the shader to the program
	{
		glAttachShader(i_programId, fragmentShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to attach the fragment shader to the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

OnExit:

	if (fragmentShaderId != 0)
	{
		// Even if the shader was successfully compiled
		// once it has been attached to the program we can (and should) delete our reference to it
		// (any associated memory that OpenGL has allocated internally will be freed
		// once the program is deleted)
		glDeleteShader(fragmentShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to delete the fragment shader ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
		}
		fragmentShaderId = 0;
	}
	if (shaderSource != NULL)
	{
		free(shaderSource);
		shaderSource = NULL;
	}

	return !wereThereErrors;
}

bool eae6320::Effect::LoadVertexShader(const char * vertex_path)
{
	// Verify that compiling shaders at run-time is supported
	{
		GLboolean isShaderCompilingSupported;
		glGetBooleanv(GL_SHADER_COMPILER, &isShaderCompilingSupported);
		if (!isShaderCompilingSupported)
		{
			eae6320::UserOutput::Print("Compiling shaders at run-time isn't supported on this implementation (this should never happen)");
			return false;
		}
	}

	bool wereThereErrors = false;

	// Load the source code from file and set it into a shader
	GLuint vertexShaderId = 0;
	void* shaderSource = NULL;
	{
		// Load the shader source code
		size_t fileSize;
		{
			//const char* sourceCodeFileName = "data/vertex.shader";
			std::string errorMessage;
			if (!LoadAndAllocateShaderProgram(vertex_path, shaderSource, fileSize, &errorMessage))
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print(errorMessage);
				goto OnExit;
			}
		}
		// Generate a shader
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to get an unused vertex shader ID: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
			else if (vertexShaderId == 0)
			{
				wereThereErrors = true;
				eae6320::UserOutput::Print("OpenGL failed to get an unused vertex shader ID");
				goto OnExit;
			}
		}
		// Set the source code into the shader
		{
			const GLsizei shaderSourceCount = 1;
			const GLchar* shaderSources[] =
			{
				reinterpret_cast<GLchar*>(shaderSource)
			};
			const GLint* sourcesAreNullTerminated = NULL;
			glShaderSource(vertexShaderId, shaderSourceCount, shaderSources, sourcesAreNullTerminated);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "OpenGL failed to set the vertex shader source code: " <<
					reinterpret_cast<const char*>(gluErrorString(errorCode));
				eae6320::UserOutput::Print(errorMessage.str());
				goto OnExit;
			}
		}
	}
	// Compile the shader source code
	{
		glCompileShader(vertexShaderId);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get compilation info
			// (this won't be used unless compilation fails
			// but it can be useful to look at when debugging)
			std::string compilationInfo;
			{
				GLint infoSize;
				glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetShaderInfoLog(vertexShaderId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						compilationInfo = info.memory;
					}
					else
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get compilation info of the vertex shader source code: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the vertex shader compilation info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
			// Check to see if there were compilation errors
			GLint didCompilationSucceed;
			{
				glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &didCompilationSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didCompilationSucceed == GL_FALSE)
					{
						wereThereErrors = true;
						std::stringstream errorMessage;
						errorMessage << "The vertex shader failed to compile:\n" << compilationInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if compilation of the vertex shader source code succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					goto OnExit;
				}
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to compile the vertex shader source code: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}
	// Attach the shader to the program
	{
		glAttachShader(i_programId, vertexShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to attach the vertex shader to the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			goto OnExit;
		}
	}

OnExit:

	if (vertexShaderId != 0)
	{
		// Even if the shader was successfully compiled
		// once it has been attached to the program we can (and should) delete our reference to it
		// (any associated memory that OpenGL has allocated internally will be freed
		// once the program is deleted)
		glDeleteShader(vertexShaderId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to delete the vertex shader ID: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
		}
		vertexShaderId = 0;
	}
	if (shaderSource != NULL)
	{
		free(shaderSource);
		shaderSource = NULL;
	}

	return !wereThereErrors;
}


bool eae6320::Effect::LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage)
{
	bool wereThereErrors = false;

	// Load the shader source from disk
	o_shader = NULL;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	{
		// Open the file
		{
			const DWORD desiredAccess = FILE_GENERIC_READ;
			const DWORD otherProgramsCanStillReadTheFile = FILE_SHARE_READ;
			SECURITY_ATTRIBUTES* useDefaultSecurity = NULL;
			const DWORD onlySucceedIfFileExists = OPEN_EXISTING;
			const DWORD useDefaultAttributes = FILE_ATTRIBUTE_NORMAL;
			const HANDLE dontUseTemplateFile = NULL;
			fileHandle = CreateFile(i_path, desiredAccess, otherProgramsCanStillReadTheFile,
				useDefaultSecurity, onlySucceedIfFileExists, useDefaultAttributes, dontUseTemplateFile);
			if (fileHandle == INVALID_HANDLE_VALUE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to open the shader file: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		// Get the file's size
		{
			LARGE_INTEGER fileSize_integer;
			if (GetFileSizeEx(fileHandle, &fileSize_integer) != FALSE)
			{
				assert(fileSize_integer.QuadPart <= SIZE_MAX);
				o_size = static_cast<size_t>(fileSize_integer.QuadPart);
			}
			else
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to get the size of shader: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
			// Add an extra byte for a NULL terminator
			o_size += 1;
		}
		// Read the file's contents into temporary memory
		o_shader = malloc(o_size);
		if (o_shader)
		{
			DWORD bytesReadCount;
			OVERLAPPED* readSynchronously = NULL;
			if (ReadFile(fileHandle, o_shader, o_size,
				&bytesReadCount, readSynchronously) == FALSE)
			{
				wereThereErrors = true;
				if (o_errorMessage)
				{
					std::string windowsErrorMessage = eae6320::GetLastWindowsError();
					std::stringstream errorMessage;
					errorMessage << "Windows failed to read the contents of shader: " << windowsErrorMessage;
					*o_errorMessage = errorMessage.str();
				}
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			if (o_errorMessage)
			{
				std::stringstream errorMessage;
				errorMessage << "Failed to allocate " << o_size << " bytes to read in the shader program " << i_path;
				*o_errorMessage = errorMessage.str();
			}
			goto OnExit;
		}
		// Add the NULL terminator
		reinterpret_cast<char*>(o_shader)[o_size - 1] = '\0';
	}

OnExit:

	if (wereThereErrors && o_shader)
	{
		free(o_shader);
		o_shader = NULL;
	}
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		if (CloseHandle(fileHandle) == FALSE)
		{
			if (!wereThereErrors && o_errorMessage)
			{
				std::string windowsError = eae6320::GetLastWindowsError();
				std::stringstream errorMessage;
				errorMessage << "Windows failed to close the shader file handle: " << windowsError;
				*o_errorMessage = errorMessage.str();
			}
			wereThereErrors = true;
		}
		fileHandle = INVALID_HANDLE_VALUE;
	}

	return !wereThereErrors;
}

bool  eae6320::Effect::CreateProgram(char * vertex_path, char * fragment_path)
{
	{
		i_programId = glCreateProgram();
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to create a program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
		else if (i_programId == 0)
		{
			eae6320::UserOutput::Print("OpenGL failed to create a program");
			return false;
		}
	}
	//CreateEffect(path);
	// Load and attach the shaders
	
	LoadVertexShader(vertex_path);
	LoadFragmentShader(fragment_path);

	/*
	if (!LoadVertexShader("data/vertex.shader"))
	{
		return false;
	}
	if (!LoadFragmentShader("data/fragment.shader"))
	{
		return false;
	}
	*/
	// Link the program
	{
		glLinkProgram(i_programId);
		GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// Get link info
			// (this won't be used unless linking fails
			// but it can be useful to look at when debugging)
			std::string linkInfo;
			{
				GLint infoSize;
				glGetProgramiv(i_programId, GL_INFO_LOG_LENGTH, &infoSize);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					sLogInfo info(static_cast<size_t>(infoSize));
					GLsizei* dontReturnLength = NULL;
					glGetProgramInfoLog(i_programId, static_cast<GLsizei>(infoSize), dontReturnLength, info.memory);
					errorCode = glGetError();
					if (errorCode == GL_NO_ERROR)
					{
						linkInfo = info.memory;
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "OpenGL failed to get link info of the program: " <<
							reinterpret_cast<const char*>(gluErrorString(errorCode));
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to get the length of the program link info: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
			}
			// Check to see if there were link errors
			GLint didLinkingSucceed;
			{
				
				glGetProgramiv(i_programId, GL_LINK_STATUS, &didLinkingSucceed);
				errorCode = glGetError();
				if (errorCode == GL_NO_ERROR)
				{
					if (didLinkingSucceed == GL_FALSE)
					{
						std::stringstream errorMessage;
						errorMessage << "The program failed to link:\n" << linkInfo;
						eae6320::UserOutput::Print(errorMessage.str());
						return false;
					}
					
				}
				else
				{
					std::stringstream errorMessage;
					errorMessage << "OpenGL failed to find out if linking of the program succeeded: " <<
						reinterpret_cast<const char*>(gluErrorString(errorCode));
					eae6320::UserOutput::Print(errorMessage.str());
					return false;
				}
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "OpenGL failed to link the program: " <<
				reinterpret_cast<const char*>(gluErrorString(errorCode));
			eae6320::UserOutput::Print(errorMessage.str());
			return false;
		}
	
	}
	uniformhandle_localtoworld = glGetUniformLocation(i_programId, "g_transform_localToWorld");
	uniformhandle_worldtoview = glGetUniformLocation(i_programId, "g_transform_worldToView");
	uniformhandle_viewtoscreen = glGetUniformLocation(i_programId, "g_transform_viewToScreen");
	
	return true;

}
GLint eae6320::Effect::MaterialUniform(const char * i_uniform)
{
	return glGetUniformLocation(i_programId, i_uniform);
}
void eae6320::Effect::UpdateMaterialUniform(int ShaderType, float values[4], float floatcount, GLint uniformtoset)
{
	GLsizei uniformcount = 1;
	if(floatcount == 1.0f)
	glUniform1fv(uniformtoset, uniformcount, values);
	if (floatcount == 2.0f)
		glUniform2fv(uniformtoset, uniformcount, values);
	if (floatcount == 3.0f)
		glUniform3fv(uniformtoset, uniformcount, values);
	if (floatcount == 4.0f)
		glUniform4fv(uniformtoset, uniformcount, values);
}
void eae6320::Effect::BindEffect()
{
	glUseProgram(i_programId);
	assert(glGetError() == GL_NO_ERROR);
	glBindVertexArray( eae6320::Graphics::getGlobalDraw().s_vertexArrayId);
	glBindVertexArray(eae6320::Graphics::getGlobalDraw2().s_vertexArrayId);
	glBindVertexArray(eae6320::Graphics::getGlobalDraw3().s_vertexArrayId);
	assert(glGetError() == GL_NO_ERROR);
	if (renderstatevalue   & alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}
	else
	{
		glDisable(GL_BLEND);
	}
	if (renderstatevalue & depthTest)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
	if (renderstatevalue & depthwrite)
	{
		glDepthMask(GL_TRUE);
	}
	else
	{
		glDepthMask(GL_TRUE);
	}

}


void eae6320::Effect::SetPositionOffset(float x1, float y1, float z1)
{
	float a =0.0f, c=0.0f, b=0.0f;
	a += x1;
	b += y1;
	c += z1;
	eae6320::Math::cVector offset(a, b, 0);
	eae6320::Math::cQuaternion o_quaternion;
	eae6320::Math::cMatrix_transformation i_locltoworld = eae6320::Math::cMatrix_transformation(o_quaternion,offset);
	eae6320::Math::cMatrix_transformation i_worldtoview = eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(Camera::getInstance().camear_rotation, Camera::getInstance().camera_axis);
	eae6320::Math::cMatrix_transformation i_viewtoscreen = eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(Math::ConvertDegreesToRadians(60), 800.0f / 600.0f, 0.0001f, 10000);

	eae6320::Math::cMatrix_transformation temp = eae6320::Math::cMatrix_transformation();
	const GLboolean dontTranspose = false; // Matrices are already in the correct format
	const GLsizei uniformCountToSet = 1;
	glUniformMatrix4fv(uniformhandle_localtoworld, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_locltoworld));
	
	glUniformMatrix4fv(uniformhandle_worldtoview, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_worldtoview));
	glUniformMatrix4fv(uniformhandle_viewtoscreen, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&i_viewtoscreen));


}

void eae6320::Effect::SetTransforLocaltoWorld(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	float uniformCount = 1;
	//glUniform3fv(uniformlocation, uniformCount, floatArray);
	const GLboolean dontTranspose = false; // Matrices are already in the correct format
	const GLsizei uniformCountToSet = 1;
	glUniformMatrix4fv(uniformhandle_localtoworld, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&floatArray));
}
void eae6320::Effect::SetTransformWorldtoView(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	float uniformCount = 1;
	//glUniform3fv(uniformlocation, uniformCount, floatArray);
	const GLboolean dontTranspose = false; // Matrices are already in the correct format
	const GLsizei uniformCountToSet = 1;
	glUniformMatrix4fv(uniformhandle_worldtoview, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&floatArray));
}
void eae6320::Effect::SetTransformViewtoScene(float x, float y, float z)
{
	float floatArray[] = { x,y,z };
	float uniformCount = 1;
	//glUniform3fv(uniformlocation, uniformCount, floatArray);
	const GLboolean dontTranspose = false; // Matrices are already in the correct format
	const GLsizei uniformCountToSet = 1;
	glUniformMatrix4fv(uniformhandle_viewtoscreen, uniformCountToSet, dontTranspose, reinterpret_cast<const GLfloat*>(&floatArray));
}