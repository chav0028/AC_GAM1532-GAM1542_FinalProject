#include "CommonHeader.h"

WaterFluidObject::WaterFluidObject(Scene * apScene, std::string aName, Vector3 aPos, Vector3 aRot, Vector3 aScale, Mesh * apMesh, ShaderProgram * apShader, GLuint aTexture, vec2 aUVScale, vec2 aUVOffset, const char * aCameraName, unsigned int aDrawRenderOrder) :
	GameObject(apScene, aName, aPos, aRot,aScale, apMesh,apShader, aTexture, aUVScale,aUVOffset,aCameraName, aDrawRenderOrder)
{
}

WaterFluidObject::~WaterFluidObject()
{
}
