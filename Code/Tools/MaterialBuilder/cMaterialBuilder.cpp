// Header Files
//=============

#include "cMaterialBuilder.h"
#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>
#include <string>
#include "../../External/Lua/Includes.h"
#include "../../Engine/Windows/WindowsFunctions.h"
#include "../../Engine/Graphics/Effect.h"

// Interface
//==========

// Build
//------
namespace eae6320 {
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
	//std::string *uniforrmnames;
	std::string * uniformnames;
	char *texturename;
	char *texturepath;
	uint32_t i;
	char *effectpath;
	uint32_t uniformCount;
	sParameterToSet *uniformdata;
	
	bool ReadValues(const char* const path);
	 bool LoadAsset(const char* const i_path);
	bool LoadTableValues(lua_State& io_luaState);
	bool LoadTableValues_Path(lua_State& io_luaState);
	bool LoadTableValues_Uniform(lua_State& io_luaState);
	bool LoadTableValues_Uniform_data(lua_State& io_luaState);
	bool LoadTableValues_Uniform_data_Name(lua_State& io_luaState);
	bool LoadTableValues_Uniform_data_Values(lua_State& io_luaState);
	bool LoadTableValues_Uniform_data_Values_data(lua_State& io_luaState);
	bool LoadTableValues_Uniform_data_Type(lua_State& io_luaState);
	bool LoadTableValues_Texture(lua_State& io_luaState);
	bool LoadTableValues_Texture_data(lua_State& io_luaState);
	bool LoadTableValues_Texture_data_Name(lua_State& io_luaState);
	bool LoadTableValues_Texture_data_Path(lua_State& io_luaState);
	bool eae6320::cMaterialBuilder::Build(const std::vector<std::string>&)
	{
		std::ofstream outfile(m_path_target, std::ofstream::binary);
		bool wereThereErrors = false;
		ReadValues(m_path_source);
		//char* buffer = reinterpret_cast<char*>(verticesCount);
		outfile.write(reinterpret_cast<char*>(effectpath), strlen(effectpath));
		outfile.write("\0", 1);
		outfile.write(reinterpret_cast<char*>(&uniformCount), sizeof(uint32_t));
		for (uint32_t k = 0; k < uniformCount; k++)
		{
			//char *name = reinterpret_cast<char*>(uniforrmnames[k]);
		
		outfile.write((uniformnames[k].c_str()), uniformnames[k].length());
			outfile.write("\0", 1);
		}
		
			outfile.write(reinterpret_cast<char*>(uniformdata), (sizeof(sParameterToSet) * uniformCount));
	
			outfile.write(reinterpret_cast<char*>(texturename), strlen(texturename));
			outfile.write("\0", 1);
			outfile.write(reinterpret_cast<char*>(texturepath), strlen(texturepath));
			outfile.write("\0", 1);
		//outfile.write(reinterpret_cast<char*>(&verticesCount), sizeof(uint32_t));
	//	outfile.write(reinterpret_cast<char*>(vertexData), (sizeof(Graphics::Mesh::sVertex) * verticesCount));
	//	outfile.write(reinterpret_cast<char*>(&indicesCount), sizeof(uint32_t));
	//	outfile.write(reinterpret_cast<char*>(indexData), (sizeof(uint32_t) * indicesCount));

		//outfile.close();

		return !wereThereErrors;
	}
	bool ReadValues(const char* const path)
	{
		
		//const char* const path = "data/square.mesh";
		if (!LoadAsset(path))
		{
		return false;
		}
		// Writing values to the table
		// allocate memory for file content
		
		
		return true;
}
	bool LoadAsset(const char* const i_path)
	{
		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		{
			luaState = luaL_newstate();
			if (!luaState)
			{
				wereThereErrors = true;
				std::cerr << "Failed to create a new Lua state"
					"\n";
				goto OnExit;
			}
		}

		// Load the asset file as a "chunk",
		// meaning there will be a callable function at the top of the stack
		{
			const int luaResult = luaL_loadfile(luaState, i_path);
			if (luaResult != LUA_OK)
			{
				wereThereErrors = true;
				std::cerr << lua_tostring(luaState, -1);
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// Execute the "chunk", which should load the asset
		// into a table at the top of the stack
		{
			const int argumentCount = 0;
			const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
			const int noMessageHandler = 0;
			const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
			if (luaResult == LUA_OK)
			{
				// A well-behaved asset file will only return a single value
				const int returnedValueCount = lua_gettop(luaState);
				if (returnedValueCount == 1)
				{
					// A correct asset file _must_ return a table
					if (!lua_istable(luaState, -1))
					{
						wereThereErrors = true;
						std::cerr << "Asset files must return a table (instead of a " <<
							luaL_typename(luaState, -1) << ")\n";
						// Pop the returned non-table value
						lua_pop(luaState, 1);
						goto OnExit;
					}
				}
				else
				{
					wereThereErrors = true;
					std::cerr << "Asset files must return a single table (instead of " <<
						returnedValueCount << " values)"
						"\n";
					// Pop every value that was returned
					lua_pop(luaState, returnedValueCount);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::cerr << lua_tostring(luaState, -1);
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1
		if (!LoadTableValues(*luaState))
		{
			wereThereErrors = true;
		}

		// Pop the table
		lua_pop(luaState, 1);

	OnExit:

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack,
			// regardless of any errors encountered while loading the file:
			assert(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = NULL;
		}

		return !wereThereErrors;
	}

	bool LoadTableValues(lua_State& io_luaState)
	{
		const char* const path = "Path";
		lua_pushstring(&io_luaState, path);
		lua_gettable(&io_luaState, -2);
		effectpath = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);

		if (!LoadTableValues_Uniform(io_luaState))
		{
			return false;
		}
		if (!LoadTableValues_Texture(io_luaState))
		{
			return false;
		}
		return true;
	}

	
	bool LoadTableValues_Uniform(lua_State& io_luaState)
	{

		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Uniform";
		lua_pushstring(&io_luaState, key);  //Pushing the index dictionary
		lua_gettable(&io_luaState, -2);
		// It can be hard to remember where the stack is at
		// and how many values to pop.
		// One strategy I would suggest is to always call a new function
		// When you are at a new level:
		// Right now we know that we have an original table at -2,
		// and a new one at -1,
		// and so we _know_ that we always have to pop at least _one_
		// value before leaving this function
		// (to make the original table be back to index -1).
		// If we don't do any further stack manipulation in this function
		// then it becomes easy to remember how many values to pop
		// because it will always be one.
		// This is the strategy I'll take in this example
		// (look at the "OnExit" label):
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Uniform_data(io_luaState))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);  //Popping out the index dictionary

		return !wereThereErrors;
	}

	

	bool LoadTableValues_Uniform_data(lua_State& io_luaState)
	{

		std::cout << "Iterating through every texture path:"
			"\n";
		uniformCount = luaL_len(&io_luaState, -1);
		uniformdata = new sParameterToSet[uniformCount];
		uniformnames = new std::string [uniformCount];
		for (i = 1; i <= uniformCount; i++)
		{
			uniformdata[i - 1].uniformHandle = NULL;
			lua_pushinteger(&io_luaState, i); //Pusing the each vertex data
			lua_gettable(&io_luaState, -2);

			if (!LoadTableValues_Uniform_data_Name(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Uniform_data_Values(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Uniform_data_Type(io_luaState))
			{
				return false;
			}
			lua_pop(&io_luaState, 1); //Popping out the vertex data
	}
		return true;

	}

	bool LoadTableValues_Uniform_data_Name(lua_State& io_luaState)
	{

		const char* const type = "Name";
		lua_pushstring(&io_luaState, type);
		lua_gettable(&io_luaState, -2);
		uniformnames[i-1] = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		return true;
	}

	bool LoadTableValues_Uniform_data_Values(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Values"; // Pusing vertices data
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);
		// It can be hard to remember where the stack is at
		// and how many values to pop.
		// One strategy I would suggest is to always call a new function
		// When you are at a new level:
		// Right now we know that we have an original table at -2,
		// and a new one at -1,
		// and so we _know_ that we always have to pop at least _one_
		// value before leaving this function
		// (to make the original table be back to index -1).
		// If we don't do any further stack manipulation in this function
		// then it becomes easy to remember how many values to pop
		// because it will always be one.
		// This is the strategy I'll take in this example
		// (look at the "OnExit" label):
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Uniform_data_Values_data(io_luaState))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);//Popping out the vertex data

		return !wereThereErrors;
	}
	bool LoadTableValues_Uniform_data_Values_data(lua_State& io_luaState)
	{
		std::cout << "Iterating through every texture path:"
			"\n";
		uniformdata[i-1].valueCountToSet = luaL_len(&io_luaState, -1);
		std::cerr << "\n check lfloat " << uniformdata[i-1].valueCountToSet;
		for (int j = 1; j <= uniformdata[i-1].valueCountToSet; j++)
		{
			lua_pushinteger(&io_luaState, j);
			lua_gettable(&io_luaState, -2);
			uniformdata[i-1].values[j-1] = (float)lua_tonumber(&io_luaState, -1);
			std::cerr << "\n check vfloat"<<uniformdata[i-1].values[j-1];
			std::cerr << "\n check cfloat " << uniformdata[i-1].valueCountToSet;
			lua_pop(&io_luaState, 1);
		}

		return true;

	}
	bool LoadTableValues_Uniform_data_Type(lua_State& io_luaState)
	{
		const char* const type = "Shadertype";
		lua_pushstring(&io_luaState, type);
		lua_gettable(&io_luaState, -2);
		std::string abc = lua_tostring(&io_luaState, -1);
		if( abc == "fragment")
		uniformdata[i-1].shaderType = fragmentshader;
		else
			uniformdata[i-1].shaderType =vertexshader;
		std::cerr << "\n check type" << uniformdata[i-1].shaderType;
		lua_pop(&io_luaState, 1);
		return true;

	}


	bool LoadTableValues_Texture(lua_State& io_luaState)
	{

		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Texture";
		lua_pushstring(&io_luaState, key);  //Pushing the index dictionary
		lua_gettable(&io_luaState, -2);
		// It can be hard to remember where the stack is at
		// and how many values to pop.
		// One strategy I would suggest is to always call a new function
		// When you are at a new level:
		// Right now we know that we have an original table at -2,
		// and a new one at -1,
		// and so we _know_ that we always have to pop at least _one_
		// value before leaving this function
		// (to make the original table be back to index -1).
		// If we don't do any further stack manipulation in this function
		// then it becomes easy to remember how many values to pop
		// because it will always be one.
		// This is the strategy I'll take in this example
		// (look at the "OnExit" label):
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Texture_data(io_luaState))
			{
				wereThereErrors = true;
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::cerr << "The value at \"" << key << "\" must be a table "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
			goto OnExit;
		}

	OnExit:

		// Pop the textures table
		lua_pop(&io_luaState, 1);  //Popping out the index dictionary

		return !wereThereErrors;
	}
	bool LoadTableValues_Texture_data(lua_State& io_luaState)
	{

		std::cout << "Iterating through every texture path:"
			"\n";
		
	
			
		
			if (!LoadTableValues_Texture_data_Name(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Texture_data_Path(io_luaState))
			{
				return false;
			}
			
		
		return true;

	}
	bool LoadTableValues_Texture_data_Name(lua_State& io_luaState)
	{

		const char* const type = "Name";
		lua_pushstring(&io_luaState, type);
		lua_gettable(&io_luaState, -2);
		texturename = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		return true;
	}
	bool LoadTableValues_Texture_data_Path(lua_State& io_luaState)
	{

		const char* const type = "Path";
		lua_pushstring(&io_luaState, type);
		lua_gettable(&io_luaState, -2);
		texturepath = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		return true;
	}
	

}

