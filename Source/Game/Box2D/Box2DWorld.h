#ifndef __Box2DWorld__H__
#define  __Box2DWorld__H__

class Box2DDebugDraw;
class Box2DContactListener;
class ShaderProgram;

const int32 BOX2D_WORLD_VELOCITY_ITERATIONS = 8;
const int32 BOX2D_WORLD_POSITION_ITERATIONS = 3;
const float BOX2D_FRAMES_PER_SECOND = 60;

class Box2DWorld
{
protected:
	b2World* m_pWorld;
	Box2DDebugDraw* m_pDebugDrawer;
	Box2DContactListener* m_pContactListener;
	b2Body* m_pCenterWorldBody;

	double m_worldStepTimer;

public:
	void DrawDebugDrawer();

	Box2DWorld(b2Vec2 gravity);
	virtual ~Box2DWorld();

	b2World* GetWorld() { return m_pWorld; }

	void SetWorldGravity(b2Vec2 aGravity);
	b2Vec2 GetWorldGravity();

	void Update(double TimePassed);

	//Functions to manage the debug draw
	Box2DDebugDraw* GetDebugDraw() { return m_pDebugDrawer; }

	void SetDebugDrawShader(ShaderProgram* shader);

	uint32 GetDebugDrawFlags();
	void SetDebugDrawFlags(uint32 flags);

	void SetDebugDrawViewMatrix(mat4* viewMatrix);
	void SetDebugDrawProjectionMatrix(mat4* projectionMatrix);

	b2Joint* CreateJoint(b2JointDef* jointDefinition);
	void RayCast(b2RayCastCallback* callback, const b2Vec2& point1, const b2Vec2& point2);

	b2Body* GetCenterWorldBody() { return m_pCenterWorldBody; }

};

#endif