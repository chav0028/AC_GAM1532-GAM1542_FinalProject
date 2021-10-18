#ifndef __PlaneRippleObject_H__
#define __PlaneRippleObject_H__

const Vector4 PLANE_DEFAULT_COLOR = Vector4(0, 0, 20, 255);

const float PLANE_RIPPLE_DEFAULT_DURATION = 6;
const float DEFAULT_COLLISION_RADIUS_MODIFIER = 5.0f;

class PlaneRippleObject : public GameObject
{
protected:
    std::vector<Vector4> m_collisionPointsPositionAndRadius;
    std::vector<float> m_collisionPointsTimer;
    std::vector<float>m_timersPercentageLeft;
    int m_numberActiveCollisionPoints;
	Sound* m_impactSound;

public:
    PlaneRippleObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));
    virtual ~PlaneRippleObject();

    virtual void Update(double TimePassed);
    virtual void Draw(int renderorder);

    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);


    virtual void Reset();
};

#endif //__PlayerObject_H__
