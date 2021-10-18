#include "CommonHeader.h"

FGEndLevelSensor::FGEndLevelSensor(const char* aCameraName, unsigned int aDrawRenderOrder):
	GameObject(aCameraName,aDrawRenderOrder)
{
    m_isCheckingCollisionDepth = false;
}

FGEndLevelSensor::~FGEndLevelSensor()
{
}

//Merely set the game over state in a collsion
void  FGEndLevelSensor::BeginCollision(b2Fixture* aFixtureCollided, b2Fixture* aFixtureCollidedAgainst, GameObject* aObjectCollidedagainst, b2Vec2 aCollisionNormal, b2Vec2 aContactPoint, bool aTouching)
{
	if (aTouching == true)
	{
		//Don't Check for Z depth in this case (since its a sensor, the presolve collision depth won't affect it). Presolve isn't called in sensors
		if (aObjectCollidedagainst != nullptr)
		{
            FinalGameScene* gameScene = dynamic_cast<FinalGameScene*>(m_pScene);

			if (gameScene != nullptr)
			{
				gameScene->EndLevel(FGEndStates_Completed);//Set the game as over
			}
		}
	}
}
