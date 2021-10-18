#include "CommonHeader.h"


ScoreSensor::ScoreSensor(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 aUVScale, vec2 aUVOffset,
	const char* aCameraName, unsigned int aDrawRenderOrder):
	GameObject(pScene,name,pos,rot,scale,pMesh,pShader,texture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
	//Create physics body so it can only collide with player
 	CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_staticBody, vec2(0, 0), true);
}


ScoreSensor::~ScoreSensor()
{
}

//Merely set the game over state in a collsion
void  ScoreSensor::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
	if (aTouching == true)
	{
		if (aObjectCollidedagainst != nullptr)
		{
			CannonScene* gameScene = dynamic_cast<CannonScene*>(aObjectCollidedagainst->GetScene());

			if (gameScene != nullptr)
			{
				gameScene->AddToScore(1);//Add to score
			}
		}
	}
}
