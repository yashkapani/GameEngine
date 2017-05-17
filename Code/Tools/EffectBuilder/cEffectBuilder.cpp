// Header Files
//=============

#include "cEffectBuilder.h"

#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>
#include <string.h>
#include "../../External/Lua/Includes.h"
#include "../../Engine/Windows/WindowsFunctions.h"

// Interface
//==========

// Build
//------

namespace eae6320
{
	 enum  renderstates:uint8_t
	 {
		alpha = 1 << 0,
		depthTest = 1 << 1,
		depthwrite = 1 << 2,
		};
	 uint8_t renderstatevalue =0;
	int shaderCount;
	char *vertex;
	char *fragment;
	bool EffectReadValues(const char* const path);
	bool EffectLoadAsset(const char* const i_path);
	bool EffectLoadTableValues(lua_State& io_luaState);
	bool EffectLoadTableValues_Shaders_paths(lua_State& io_luaState);
	bool EffectLoadTableValues_Shader(lua_State& io_luaState);
	bool EffectLoadTableValues_renderstates(lua_State& io_luaState);
	bool EffectLoadTableValues_renderstates_values(lua_State& io_luaState);
	bool cEffectBuilder::Build(const std::vector<std::string>&)
	{
		bool wereThereErrors = false;

		// Copy the source to the target
		

			std::ofstream outfile(m_path_target, std::ofstream::binary);
			//bool wereThereErrors = false;
			EffectReadValues(m_path_source);
			outfile.write(reinterpret_cast<char*>(vertex), strlen(vertex));
			outfile.write("\0",1);
			outfile.write(reinterpret_cast<char*>(fragment), strlen(fragment));
			outfile.write("\0", 1);
			outfile.write(reinterpret_cast<char*>(&renderstatevalue), sizeof(uint8_t));
			
		//	outfile.write(reinterpret_cast<char*>(&verticesCount), sizeof(uint32_t));
		//	outfile.write(reinterpret_cast<char*>(vertexData), (sizeof(Graphics::Mesh::sVertex) * verticesCount));
		//	outfile.write(reinterpret_cast<char*>(&indicesCount), sizeof(uint32_t));
		//	outfile.write(reinterpret_cast<char*>(indexData), (sizeof(uint32_t) * indicesCount));

			return !wereThereErrors;
		
	}

	bool EffectReadValues(const char* const path)
	{

		//const char* const path = "data/square.mesh";
		if (!EffectLoadAsset(path))
		{
			return false;
		}
		// Writing values to the table
		// allocate memory for file content


		return true;
	}

	bool EffectLoadAsset(const char* const i_path)
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
		if (!EffectLoadTableValues(*luaState))
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

	bool EffectLoadTableValues(lua_State& io_luaState)
	{
		if (!EffectLoadTableValues_Shader(io_luaState))
		{
			return false;
		}
		if (!EffectLoadTableValues_renderstates(io_luaState))
		{
			return false;
		}

		return true;
	}
	bool EffectLoadTableValues_Shader(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Shaders"; // Pusing vertices data
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
			if (!EffectLoadTableValues_Shaders_paths(io_luaState))
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

	bool EffectLoadTableValues_Shaders_paths(lua_State& io_luaState)
	{
		// Right now the asset table is at -2
		// and the textures table is at -1.
		// NOTE, however, that it doesn't matter to me in this function
		// that the asset table is at -2.
		// Because I've carefully called a new function for every "stack level"
		// The only thing I care about is that the textures table that I care about
		// is at the top of the stack.
		// As long as I make sure that when I leave this function it is _still_
		// at -1 then it doesn't matter to me at all what is on the stack below it.
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertex = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		fragment = _strdup(lua_tostring(&io_luaState, -1));
		lua_pop(&io_luaState, 1);

		return true;
	}

	bool EffectLoadTableValues_renderstates(lua_State& io_luaState)
	{
		bool wereThereErrors = false;
		const char* const key = "renderstates"; // Pusing vertices data
		lua_pushstring(&io_luaState, key);
		lua_gettable(&io_luaState, -2);

		if (lua_istable(&io_luaState, -1))
		{
			if (!EffectLoadTableValues_renderstates_values(io_luaState))
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
	bool EffectLoadTableValues_renderstates_values(lua_State& io_luaState)
	{
		const char* const key_alpha = "Alpha_transperency";
		lua_pushstring(&io_luaState, key_alpha);
		lua_gettable(&io_luaState, -2);
		bool alpha_value = static_cast<bool>(lua_toboolean(&io_luaState, -1));
			if(alpha_value == true)
			{
				renderstatevalue |= alpha;
			}
		lua_pop(&io_luaState, 1);
		const char* const key_test = "Depth_testing";
		lua_pushstring(&io_luaState, key_test);
		lua_gettable(&io_luaState, -2);
		bool test_value = static_cast<bool>(lua_toboolean(&io_luaState, -1));
		if (test_value == true)
		{
			renderstatevalue |= depthTest;
		}
		lua_pop(&io_luaState, 1);
		const char* const key_write = "Depth_writing";
		lua_pushstring(&io_luaState, key_write);
		lua_gettable(&io_luaState, -2);
		bool write_value = static_cast<bool>(lua_toboolean(&io_luaState, -1));
		if (write_value == true)
		{
			renderstatevalue |= depthwrite;
		}
		lua_pop(&io_luaState, 1);
		return true;
	}
}

