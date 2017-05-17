// Header Files
//=============

#include "../cFragmentShaderBuilder.h"

#include <sstream>
#include "../../../Engine/Windows/WindowsIncludes.h"
#include "../../../Engine/Windows/WindowsFunctions.h"

// Interface
//==========

// Build
//------

bool eae6320::cFragmentShaderBuilder::Build( const std::vector<std::string>& i_arguments )
{
	// Decide which kind of shader program to compile

	// Get the path to the shader compiler
	std::string path_fxc;
	{
		// Get the path to the DirectX SDK
		std::string path_sdk;
		{
			std::string errorMessage;
			if ( !GetEnvironmentVariable( "DXSDK_DIR", path_sdk, &errorMessage ) )
			{
				std::stringstream decoratedErrorMessage;
				decoratedErrorMessage << "Windows failed to get the path to the DirectX SDK: " << errorMessage;
				OutputErrorMessage( decoratedErrorMessage.str().c_str(), __FILE__ );
				return false;
			}
		}
		path_fxc = path_sdk + "Utilities/bin/" +
#ifndef _WIN64
			"x86"
#else
			"x64"
#endif
			+ "/fxc.exe";
	}
	// Create the command to run
	std::string command;
	{
		std::stringstream commandToBuild;
		commandToBuild << "\"" << path_fxc << "\"";
		// Target profile
		
			commandToBuild << " /Tps_3_0";
		
		// Entry point
		commandToBuild << " /Emain"
			// #define the platform
			<< " /DEAE6320_PLATFORM_D3D"
#ifdef EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
			// Disable optimizations so that debugging is easier
			<< " /Od"
			// Enable debugging
			<< " /Zi"
#endif
			// Target file
			<< " /Fo\"" << m_path_target << "\""
			// Don't output the logo
			<< " /nologo"
			// Source file
			<< " \"" << m_path_source << "\""
		;
		command = commandToBuild.str();
	}
	// Execute the command
	{
		DWORD exitCode;
		std::string errorMessage;
		if ( ExecuteCommand( command.c_str(), &exitCode, &errorMessage ) )
		{
			return exitCode == EXIT_SUCCESS;
		}
		else
		{
			OutputErrorMessage( errorMessage.c_str(), m_path_source );
			return false;
		}
	}
}
