#ifndef __EndLevelSensor__H__
#define __EndLevelSensor__H__

class EndLevelSensor :
	public GameObject
{
public:
	EndLevelSensor(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0),
		const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	virtual ~EndLevelSensor();
	void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
};

#endif