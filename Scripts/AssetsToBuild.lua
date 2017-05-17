return
{
  
	meshes=
	{
		source ="",
		target="",
		tool = "MeshBuilder.exe",
		assets=
		{
			"ceiling.mesh",
			"cement.mesh",
			"walls.mesh",
			"metal.mesh",
			"floor.mesh",
			"lambert3.mesh",
			"railing.mesh",
			"Player.mesh",
			"Player1.mesh",
			"collisiondata.mesh",
		},
		dependencies = 
		 {
		 },
	},
	fragmentshader = 
	{
		source= "",
		target= "",
		tool = "FragmentShaderBuilder.exe",
		assets=
		{
			"fragment.shader",
			"fragmenttransperent.shader",
		},
		dependencies = 
		 {
		 "shaders.inc",
		 },
	},
	vertexshader = 
	{
		source= "",
		target= "",
		tool = "VertexShaderBuilder.exe",
		assets=
		{
			"vertex.shader",
		},
		dependencies = 
		 {
		 "shaders.inc",
		 },
	},
	Material = 
	{
		source= "",
		target= "",
		tool = "MaterialBuilder.exe",
		assets=
		{
			"Green.material",
			"Blue.material",
			"Red.material",
			"Yellow.material",
			"Default.material",
			"Playermat.material",
			"cylinder.material",
			

		},
		dependencies = 
		 {
		 
		 },
	},
	effect = 
	{
	source = "",
	target = "",
	tool ="EffectBuilder.exe",
	assets =
		{
		"Shader.effect",
		"ShaderTransp.effect",
		},
		dependencies = 
		 {
		 },
	},
	Texture =
	{
	source = "",
	target = "",
	tool ="TextureBuilder.exe",
	assets =
		{
		"cement_wall_D.png",
		"floor_D.png",
		"metal_brace_D.png",
		"railing_D.png",
		"alpha.png",
		"wall_D.png",
		"eae6320.png",
		"rock.JPG",
		},
		dependencies = 
		 {
		 },
	},
	
}
