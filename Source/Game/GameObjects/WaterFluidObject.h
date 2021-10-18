#pragma once

class WaterFluidObject :
	public GameObject
{
public:
	WaterFluidObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 aUVScale, vec2 aUVOffset,
		const char* aCameraName, unsigned int aDrawRenderOrder);
	virtual ~WaterFluidObject();
};

