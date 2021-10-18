#ifndef __FGEndLevelSensor__H__
#define __FGEndLevelSensor__H__

class FGEndLevelSensor :
	public GameObject
{
public:
    FGEndLevelSensor(const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
    virtual ~FGEndLevelSensor();
	void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
};

#endif