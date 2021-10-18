#include "CommonHeader.h"
#include "EndLevelSensor.h"


EndLevelSensor::EndLevelSensor(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 aUVScale, vec2 aUVOffset,
	const char* aCameraName, unsigned int aDrawRenderOrder):
	GameObject(pScene,name,pos,rot,scale,pMesh,pShader,texture,aUVScale,aUVOffset,aCameraName,aDrawRenderOrder)
{
	//Create physics body so it can only collide with player
 	CreatePhysicsBox(vec2(GetXLength(), GetYLength()), b2_staticBody, vec2(0, 0), true, 0, CollisionFilteringCategory_EndLevel, CollisionFilteringCategory_Player);

}


EndLevelSensor::~EndLevelSensor()
{
}

//Merely set the game over state in a collsion
void  EndLevelSensor::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
	if (aTouching == true)
	{
		//Don't Check for Z depth in this case (since its a sensor, the presolve collision depth won't affect it). Presolve isn't called in sensors
		if (aObjectCollidedagainst != nullptr)
		{
			ThrustersGame* gameScene = dynamic_cast<ThrustersGame*>(aObjectCollidedagainst->GetScene());

			if (gameScene != nullptr)
			{
				gameScene->SetGameOver(true);//Set the game as over
			}
		}
	}
}
