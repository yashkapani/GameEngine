# GameEngine
Custom 3D Game Engine for OpenGL and DirectX

The Engine is not a Code Sample but to demonstrate my software engineering skills in Graphics, Architecture, 3D Math, Tools and Networking

Structure:
The engine is structured to resemble a 3D Engine like Unity or Unreal.

The Engine is Devided into Components: 

Assets a storage place for all meshes,material, textures and shaders.

Code: Contains Engine: comprising of Graphics, Math, User Input, User Output and windows.<br> External: has exterbal libraries for Lua and OpenGl. <br> Game: All code uing various Engine components to load engine and gameplay functionalitiesare.<br>

Tools: components of the engine that can be run indepedently and are not needed by the engine at runtime, main func is to convert all the assets, meshes, meterial, shaders and textures to a binary format into assets folder that can be easily used by the game when needed.

MayaMeshExporter: Loads the Meshes and Vertices and Indices in the correct format from Maya. which is the accesed by the Mesh builder

Files:
Graphics.h, Graphics.cpp header/common implementation for the Graphics class
Direct3D/Graphics.cpp Direct3D implementation for the Graphics class
OpenGL/Graphics.cpp OpenGL implementation for the Graphics class

The function of the Graphic class: 
The definition of interfaces of Graphics module to the others parts of the engine

The function of Graphics project: load and render meshes. 
Users could customize Shaders, Effects, and Materials used when rendering meshes.
It works under both Direct3D and OpenGL.



