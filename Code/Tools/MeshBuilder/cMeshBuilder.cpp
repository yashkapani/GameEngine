// Header Files
//=============

#include "cMeshBuilder.h"
#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>
#include "../../External/Lua/Includes.h"
#include "../../Engine/Windows/WindowsFunctions.h"

// Interface
//==========

// Build
//------
namespace eae6320 {
	
	uint32_t i;
	uint32_t verticesCount;
	uint32_t indicesCount;
	uint32_t* indexData;
	Graphics::Mesh::sVertex *vertexData;
	bool ReadValues(const char* const path);
	 bool LoadAsset(const char* const i_path);
	bool LoadTableValues(lua_State& io_luaState);
	bool LoadTableValues_Vertices(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Position(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Position_paths(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Normals(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Normals_paths(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Color(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Color_paths(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Texture(lua_State& io_luaState);
	bool LoadTableValues_Vertices_paths_Texture_paths(lua_State& io_luaState);
	bool LoadTableValues_Indices(lua_State& io_luaState);
	bool LoadTableValues_Indices_values(lua_State& io_luaState);
	bool LoadTableValues_Indices_values_data(lua_State& io_luaState);


	bool eae6320::cGenericBuilder::Build(const std::vector<std::string>&)
	{
		std::ofstream outfile(m_path_target, std::ofstream::binary);
		bool wereThereErrors = false;
		ReadValues(m_path_source);
		//char* buffer = reinterpret_cast<char*>(verticesCount);
		
		outfile.write(reinterpret_cast<char*>(&verticesCount), sizeof(uint32_t));
		outfile.write(reinterpret_cast<char*>(vertexData), (sizeof(Graphics::Mesh::sVertex) * verticesCount));
		outfile.write(reinterpret_cast<char*>(&indicesCount), sizeof(uint32_t));
		outfile.write(reinterpret_cast<char*>(indexData), (sizeof(uint32_t) * indicesCount));

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
		if (!LoadTableValues_Vertices(io_luaState))
		{
			return false;
		}
		if (!LoadTableValues_Indices(io_luaState))
		{
			return false;
		}

		return true;
	}

	bool LoadTableValues_Vertices(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Vertices"; // Pusing vertices data
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
			if (!LoadTableValues_Vertices_paths(io_luaState))
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

	bool LoadTableValues_Vertices_paths(lua_State& io_luaState)
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

		std::cout << "Iterating through every texture path:"
			"\n";
		verticesCount = luaL_len(&io_luaState, -1);
		vertexData = new Graphics::Mesh::sVertex[verticesCount];
		for (i = 1; i <= verticesCount; ++i)
		{
			lua_pushinteger(&io_luaState, i); //Pusing the each vertex data
			lua_gettable(&io_luaState, -2);

			if (!LoadTableValues_Vertices_paths_Position(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Vertices_paths_Normals(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Vertices_paths_Color(io_luaState))
			{
				return false;
			}
			if (!LoadTableValues_Vertices_paths_Texture(io_luaState))
			{
				return false;
			}
			lua_pop(&io_luaState, 1); //Popping out the vertex data
		}

		return true;
	}

	bool LoadTableValues_Vertices_paths_Position(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Position";
		lua_pushstring(&io_luaState, key); //Pushinh posiiotn
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Vertices_paths_Position_paths(io_luaState))
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

		// Poping out the position table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;


	}



	bool LoadTableValues_Vertices_paths_Normals(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Normals";
		lua_pushstring(&io_luaState, key); //Pushinh posiiotn
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Vertices_paths_Normals_paths(io_luaState))
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

		// Poping out the position table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;


	}





	bool LoadTableValues_Vertices_paths_Position_paths(lua_State& io_luaState)
	{

		// Right now the parameters table is at -1.
		// Every time the while() statement is executed it will be at -2
		// and the next key will be at -1.
		// Inside the block the table will be at -3,
		// the current key will be at -2,
		// and the value will be at -1.
		// (You may want to review LoadTableValues_allKeys()
		// in the ReadTopLevelTableValues example,
		// but remember that you don't need to know how to do this
		// for Assignment 03.)


		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].x = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].y = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].z = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		return true;
	}





	bool LoadTableValues_Vertices_paths_Normals_paths(lua_State& io_luaState)
	{

		// Right now the parameters table is at -1.
		// Every time the while() statement is executed it will be at -2
		// and the next key will be at -1.
		// Inside the block the table will be at -3,
		// the current key will be at -2,
		// and the value will be at -1.
		// (You may want to review LoadTableValues_allKeys()
		// in the ReadTopLevelTableValues example,
		// but remember that you don't need to know how to do this
		// for Assignment 03.)


		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].nx = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].ny = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].nz = (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		return true;
	}




	bool LoadTableValues_Vertices_paths_Color(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Color";
		lua_pushstring(&io_luaState, key); //Pushing in the color table
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Vertices_paths_Color_paths(io_luaState))
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

		// Pop the color table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;


	}

	bool LoadTableValues_Vertices_paths_Color_paths(lua_State& io_luaState)
	{

		// Right now the parameters table is at -1.
		// Every time the while() statement is executed it will be at -2
		// and the next key will be at -1.
		// Inside the block the table will be at -3,
		// the current key will be at -2,
		// and the value will be at -1.
		// (You may want to review LoadTableValues_allKeys()
		// in the ReadTopLevelTableValues example,
		// but remember that you don't need to know how to do this
		// for Assignment 03.)

		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].r = static_cast<uint8_t>(255 * (float)lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].g = static_cast<uint8_t>(255 * (float)lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].b = static_cast<uint8_t>(255 * (float)lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 4);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].a = static_cast<uint8_t>(255 * (float)lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);

		return true;
	}


	bool LoadTableValues_Vertices_paths_Texture(lua_State& io_luaState)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Texture";
		lua_pushstring(&io_luaState, key); //Pushing in the color table
		lua_gettable(&io_luaState, -2);
		if (lua_istable(&io_luaState, -1))
		{
			if (!LoadTableValues_Vertices_paths_Texture_paths(io_luaState))
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

		// Pop the color table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;


	}


	bool LoadTableValues_Vertices_paths_Texture_paths(lua_State& io_luaState)
	{

		// Right now the parameters table is at -1.
		// Every time the while() statement is executed it will be at -2
		// and the next key will be at -1.
		// Inside the block the table will be at -3,
		// the current key will be at -2,
		// and the value will be at -1.
		// (You may want to review LoadTableValues_allKeys()
		// in the ReadTopLevelTableValues example,
		// but remember that you don't need to know how to do this
		// for Assignment 03.)

		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].u =  (float)lua_tonumber(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		vertexData[i - 1].v = (float)(1.0f - lua_tonumber(&io_luaState, -1));
		lua_pop(&io_luaState, 1);

		return true;
	}

	bool LoadTableValues_Indices(lua_State& io_luaState)
	{

		bool wereThereErrors = false;

		// Right now the asset table is at -1.
		// After the following table operation it will be at -2
		// and the "textures" table will be at -1:
		const char* const key = "Index";
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
			if (!LoadTableValues_Indices_values(io_luaState))
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

	bool LoadTableValues_Indices_values(lua_State& io_luaState)
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

		std::cout << "Iterating through every texture path:"
			"\n";
		indicesCount = luaL_len(&io_luaState, -1) * 3;
		indexData = new uint32_t[indicesCount];
		for (i = 1; i <= indicesCount / 3; ++i)
		{
			lua_pushinteger(&io_luaState, i); //Pushing each triangle indices
			lua_gettable(&io_luaState, -2);

			if (!LoadTableValues_Indices_values_data(io_luaState))
			{
				return false;
			}

			lua_pop(&io_luaState, 1); //Popping out the triangle index
		}

		return true;
	}

	bool LoadTableValues_Indices_values_data(lua_State& io_luaState)
	{
#if defined( EAE6320_PLATFORM_D3D )


		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3 + 2] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3 + 1] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
#elif defined( EAE6320_PLATFORM_GL )
		lua_pushinteger(&io_luaState, 1);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 2);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3 + 1] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
		lua_pushinteger(&io_luaState, 3);
		lua_gettable(&io_luaState, -2);
		indexData[(i - 1) * 3 + 2] = (uint32_t)lua_tointeger(&io_luaState, -1);
		lua_pop(&io_luaState, 1);
#endif
		return true;

	}


}
