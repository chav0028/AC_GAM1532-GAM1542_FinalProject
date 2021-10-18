#ifndef __GameObject_H__
#define __GameObject_H__

const char* const GAME_DEFAULT_CAMERA_NAME = "GameCamera";
const char* const HUD_DEFAULT_CAMERA_NAME = "HUDCamera";

const char* const GAMEOBJECT_POSITION_TWEEN_NAME = "TweenGameObjectPosition";
const char* const GAMEOBJECT_X_POSITION_TWEEN_NAME = "TweenGameObjectXPosition";
const char* const GAMEOBJECT_Y_POSITION_TWEEN_NAME = "TweenGameObjectYPosition";
const char* const GAMEOBJECT_Z_POSITION_TWEEN_NAME = "TweenGameObjectZPosition";
const char* const GAMEOBJECT_ROTATION_TWEEN_NAME = "TweenGameObjectRotation";
const char* const GAMEOBJECT_ROTATION_X_TWEEN_NAME = "TweenGameObjectXRotation";
const char* const GAMEOBJECT_ROTATION_Y_TWEEN_NAME = "TweenGameObjectYRotation";
const char* const GAMEOBJECT_ROTATION_Z_TWEEN_NAME = "TweenGameObjectZRotation";
const char* const GAMEOBJECT_SCALE_TWEEN_NAME = "TweenGameObjectScale";
const char* const GAMEOBJECT_SCALE_X_TWEEN_NAME = "TweenGameObjectPosition";
const char* const GAMEOBJECT_SCALE_Y_TWEEN_NAME = "TweenGameObjectPosition";
const char* const GAMEOBJECT_SCALE_Z_TWEEN_NAME = "TweenGameObjectPosition";

class ShaderProgram;
class Mesh;
class Scene;

class GameObject
{

private:
	bool m_enabled;
    bool m_elementOfHUD;
    const char* m_cameraDrawerName;
    unsigned int m_drawRenderOrder;

protected:
	Scene* m_pScene;

	std::string m_Name;

	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

	Mesh* m_pMesh;  

	ShaderProgram* m_pShaderProgram;
	GLuint m_TextureHandle;
    GLuint m_secondaryTexture;
	vec2 m_UVScale;
	vec2 m_UVOffset;

	Vector4 m_Color;
    float m_speed;

    GameObject* m_pParentObject;
    std::vector<GameObject*>m_pChildObjects;

	//Variables used when resetting an object
	Vector3 m_defaultPosition;
	Vector3 m_defaultRotation;
	Vector3 m_defaultScale;

	//Tweening
	std::vector <Tween<float>*>m_pfloatTweens;
	std::vector < Tween<Vector3>*>m_pVec3Tweens;

    //Lighting
    float m_reflectivity;

    //Box2D
	b2Body* m_pPhysicsBody;
	std::vector<b2Fixture*> m_pFixtures;
    bool m_isCheckingCollisionDepth;
    
	void CreateFixture(b2BodyType bodyType, b2Shape* shape, bool sensor = false, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
    void CreatePhysicsBody(b2BodyType bodyType);

public:
    GameObject(const char* cameraName=GAME_DEFAULT_CAMERA_NAME,unsigned int drawRenderOrder=0);
    GameObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0), 
        const char* cameraName = GAME_DEFAULT_CAMERA_NAME, unsigned int drawRenderOrder = 0);
	virtual ~GameObject();

	virtual void Init(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 UVScale = vec2(1, 1), vec2 UVOffset = vec2(0, 0));

	virtual void Update(double TimePassed);
    virtual void Draw(int renderorder);

    virtual bool HandleInput(InputEvent& inputevent, double aDelta);

    //Helper math functions
    float Calculate3DDistanceBetween(Vector3 position);
    float Calculate2DDistanceBetween(Vector2 position);
    bool DepthCollisionCheck(float aTop, float aBottom);//Only works in objects with centered pivot points
    float Get2DAngleBetween(Vector2 position);
    virtual bool CheckPointIsInSquare2D(Vector2 position);

    //Physic body fixtures
    void CreatePhysicsBox(vec2 size, b2BodyType bodyType = b2_dynamicBody, vec2 offset = 0, bool sensor = false, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
    void CreatePhysicsCircle(float radius, b2BodyType bodyType = b2_dynamicBody, vec2 offset = 0, bool sensor = false, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
    void CreatePhysicsLine(vec2 lineStart, vec2 lineEnd, b2BodyType bodyType = b2_dynamicBody, vec2 offset = 0, bool sensor = false, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
    void CreatePhysicsLineStrip(b2Vec2* Vertices, unsigned int numLines, bool loop = false, b2BodyType = b2_dynamicBody, vec2 offset = 0, bool sensor = false, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);

    //Contact listener
    virtual void BeginCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void EndCollision(b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, b2Vec2 contactPoint, bool touching);
    virtual void CollsionPostSolve(b2Contact* aContact, b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal, const b2ContactImpulse* impulse);
    virtual void CollsionPreSolve(b2Contact* aContact, b2Fixture* fixtureCollided, b2Fixture* fixtureCollidedAgainst, GameObject* objectCollidedagainst, b2Vec2 collisionNormal);

    //Collision filtering
    virtual void SetFixtureCollisionsFiltering(int fixtureIndex = 0, signed short fixtureGroup = 0, unsigned short fixtureCategory = 1, unsigned short fixtureMasks = 1);
    virtual void SetFixtureCollisionsFilteringGroup(int fixtureIndex = 0, signed short fixtureGroup = 0);
    virtual void SetFixtureCollisionsFilteringCategory(int fixtureIndex = 0, unsigned short fixtureCategory = 1);
    virtual void SetFixtureCollisionsFilteringMask(int fixtureIndex = 0, unsigned short fixtureMasks = 1);

    //reset functions
    virtual void Reset();
    virtual void ResetPhysicsBody();
    virtual void ResetPhysicsBodyVelocities();
	void ResetPhysicsBodyXVelocity();
	void ResetPhysicsBodyYVelocity();
	void SetActivePhysicsBody(bool status);

    //Resource management
    void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
    Mesh* GetMesh() { return m_pMesh; }

    void SetShader(ShaderProgram* pShader) { m_pShaderProgram = pShader; }
    ShaderProgram* GetShader() { return m_pShaderProgram; }

    virtual void SetTexture(GLuint texturehandle) { m_TextureHandle = texturehandle; }
    virtual GLuint GetTexture() { return m_TextureHandle; }

    virtual void SetSecondaryTexture(GLuint textureHandle){ m_secondaryTexture = textureHandle; }
    virtual GLuint GetSecondaryTexture(){ return m_secondaryTexture; }

    //Setters and Getters
	virtual void SetTransform(Vector3 position, Vector3 rotation, Vector3 scale);
	virtual void SetDefaultTransform(Vector3 position, Vector3 rotation, Vector3 scale);

    virtual void SetPhysicsAndGameObjectPosition(vec3 aPosition);
	virtual void SetPosition(Vector3 pos) { m_Position = pos; }
	virtual void SetPositionX(float xPos) { m_Position.x = xPos; }
	virtual void SetPositionY(float yPos) { m_Position.y = yPos; }
	virtual void SetPositionZ(float zPos) { m_Position.z = zPos; }
    virtual void SetDefaultPosition(Vector3 pos){ m_defaultPosition = pos; }
    virtual Vector3 GetPosition() { return m_Position; }
	virtual float GetPositionX() { return m_Position.x; }
	virtual float GetPositionY() { return m_Position.y; }
	virtual float GetPositionZ() { return m_Position.z; }
		    
	virtual void SetRotation(Vector3 rot) { m_Rotation = rot; }
	virtual void SetRotationX(float xRot) { m_Rotation.x = xRot; }
	virtual void SetRotationY(float yRot) { m_Rotation.y = yRot; }
	virtual void SetRotationZ(float zRot) { m_Rotation.z = zRot; }
    virtual void SetDefaultRotation(Vector3 rot){ m_defaultRotation = rot; }
	virtual Vector3 GetRotation() { return m_Rotation; }
	virtual Vector3 GetRotationRadians() { return m_Rotation*PI / 180.0f; }
	virtual float GetRotationX() { return m_Rotation.x; }
	virtual float GetRotationXRadians(){return  m_Rotation.x*PI / 180.0f;}
	virtual float GetRotationY() { return m_Rotation.y; }
	virtual float GetRotationYRadians() { return  m_Rotation.y*PI / 180.0f; }
	virtual float GetRotationZ() { return m_Rotation.z; }
	virtual float GetRotationZRadians() { return  m_Rotation.z*PI / 180.0f; }

	virtual void SetScale(Vector3 scale) { m_Scale = scale; }
    virtual void SetDefaultScale(Vector3 scale){ m_defaultScale = scale; }
    virtual void SetScaleX(float xScale) { m_Scale.x = xScale; }
    virtual void SetScaleY(float yScale) { m_Scale.y = yScale; }
    virtual void SetScaleZ(float zScale) { m_Scale.z = zScale; }
	virtual Vector3 GetScale() { return m_Scale; }

	Vector3 GetDirection3D();
	Vector2 GetDirectionXY();
	Vector2	GetDirectionXZ();

	void SetColor(Vector4 aColor) { m_Color = aColor; }
    Vector4 GetColor() { return m_Color; }

	virtual void SetEnabled(bool enabled);
	virtual bool GetEnabled();

    virtual void SetElementOfHUD(bool aStat){ m_elementOfHUD = aStat; }

    void SetName(std::string aName){ m_Name = aName; }
	virtual std::string GetName() { return m_Name; }

	virtual void SetScene(Scene* scene);
	virtual Scene* GetScene() { return m_pScene; }

	virtual void SetCameraDrawerName(const char* aCameraName){ m_cameraDrawerName = aCameraName; }
	virtual const char* GetCameraDrawerName(){ return m_cameraDrawerName; }

	virtual void SetRenderOrder(unsigned int aRenderOrder);
	virtual unsigned int GetRenderOrder(){ return m_drawRenderOrder; }

    virtual void SetUVScale(vec2 aScale) { m_UVScale = aScale; }
    virtual void SetUVOffset(vec2 aOffset) { m_UVOffset = aOffset; }

    //Object Parenting
    MyMatrix GetWorldMatrix();
    Vector3 GetWorldPosition();

    void SetParentObject(GameObject* pParent);

    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);

    //Bounding boxes values
    virtual Vector3 GetSizes();
    virtual float GetXLength();
    virtual float GetYLength();
    virtual float GetZLength();
    virtual float GetXYRadius();

    virtual Vector3 GetMinXYZ();
    virtual Vector3 GetMaxXYZ();

    //Lighting
    void SetReflectivity(float aReflectivity);
    float GetReflectivity(){ return m_reflectivity; }

	//Tweening
	Tween<float>* GetFloatTweener(unsigned int index = 0);
    Tween<float>* GetFloatTweenerByName(std::string tweenName);
	void AddFloatTweener(Tween<float>* aTweener);
    bool GetFloatTweenerOver(std::string tweenName);

	Tween<vec3>* GetVec3Tweener(unsigned int aIndex);
    Tween<vec3>* GetVec3TweenerByName(std::string tweenName);
	void AddVec3Tweener(Tween<vec3>* aTweener);
    bool GetVec3TweenerOver(std::string tweenName);
	
	virtual void SetTweenPosition(Vector3 aEndValue, double aEndTime, TweeningType aType, double aDelay=0);
	virtual void SetTweenXPosition(float aEndValue, double aEndTime, TweeningType aTypedouble, double aDelay=0);
	virtual void SetTweenYPosition(float aEndValue, double aEndTime, TweeningType aTypedouble, double aDelay=0);
	virtual void SetTweenZPosition(float aEndValue, double aEndTime, TweeningType aTypedouble, double aDelay=0);

	virtual void SetTweenRotation(Vector3 aEndValue, double aEndTime, TweeningType aType, double aDelay = 0);
	virtual void SetTweenXRotation(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);
	virtual void SetTweenYRotation(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);
	virtual void SetTweenZRotation(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);

	virtual void SetTweenScale(Vector3 aEndValue, double aEndTime, TweeningType aType, double aDelay = 0);
	virtual void SetTweenXScale(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);
	virtual void SetTweenYScale(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);
	virtual void SetTweenZScale(float aEndValue, double aEndTime, TweeningType aType,double aDelay=0);

    //Use the fixture vertices
    virtual std::vector<b2Vec2> GetShapeVertices(b2Shape* aShape);
    virtual vec2 GetMinFixtureXY(int aFixtureIndex = -1);
    virtual vec2 GetMaxFixtureXY(int aFixtureIndex = -1);

    //Functions to modify the physics body
	virtual void SetPhysicsBodyType(b2BodyType aType);
	virtual b2BodyType GetPhysicsBodyType();

	virtual void SetPhysicsBodyIsSensor(bool aState, int fixtureIndex = -1);
	virtual bool GetPhysicsBodyIsSensor(int fixtureIndex = 0);

    virtual b2Body* GetPhysicsBody() { return m_pPhysicsBody; }
    virtual vec2 GetPhysicsBodyVelocity();

    virtual void SetPhysicsBodyTransform(vec2 position, float angle);
    virtual void SetPhysicsBodyRestitution(float restitution, int fixtureIndex = -1);
    virtual void SetPhysicsBodyDensity(float Density, int fixtureIndex = -1);
    virtual void SetPhysicsBodyFriction(float friction, int fixtureIndex = -1);

	//Functions to simulate bouncing, not the msot effective
	virtual float GetSimulatedRestitution(float restitutionCollidingObject = 0.5f, int fixtureIndex = -1);
    virtual float GetSimulatedFriction(float frictionCollidingObject = 0.5f, int fixtureIndex = -1);
    virtual void BounceObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
    virtual void BounceXObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
    virtual void BounceYObjectBody(float restitutionCollidingObject = 0.5f, float frictionCollidingObject = 0.5f);
    virtual void BounceAngleZ(float frictionCollidingObject = 0.5f);

    bool GetIsCheckingCollisionDepth(){ return m_isCheckingCollisionDepth; }
    void SetIsCheckingCollisionDepth(bool aStat){ m_isCheckingCollisionDepth = aStat; }

    //The screen position is top left up.
    virtual bool RayCastPositionIn2d(Vector2 aScreenPosition);

	void AddReflectivityToQuickWatch(QuickWatchManager* aQuickWatch);
};

#endif //__GameObject_H__