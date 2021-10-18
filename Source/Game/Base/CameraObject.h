#ifndef __CameraObject_H__
#define __CameraObject_H__

const float CAMERA_SPEED_PCT_FROM_FOLLOWING_TARGET = 0.2f;//The speed percentage the camera speed will have in comparison to the target it is following
const Vector3 CAMERA_DEAD_ZONE_DEFAULT_SIZE = Vector3(30,30,35);
const float CAMERA_DISTANCE_FROM_OBJECT_SMOOTHING = 2;
const float CAMERA_SMOOTHING_SPEED_REDUCTION = 0.2f;

const Vector3 CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT = Vector3(1, 0.5f, 0.5f);
const float GAME_CAMERA_LOOK_AT_SPEED = 2.0f;
const Vector3 CAMERA_DEFAULT_MAX_LOOK_AT_OFFSET = Vector3(4, 2,4);

const float CAMERA_DEFAULT_CONTROLS_SPEED = 30.0f;
const float CAMERA_DEFAULT_CONTROLS_ROTATION_SPEED = 50.0f;

class CameraObject : public GameObject
{
protected:
    mat4 m_viewMatrix;
    
    Vector3 m_targetPosition;
    Vector3 m_targetLookAtOffset;
    Vector3 m_maxTargetLookAtOffset;
	Vector3 m_cameraOffset;
    Vector3 m_maxVelocityThreshold;
    Vector3 m_minVelocityThreshold;
    Vector3 m_minPositionsLimit;
    Vector3 m_maxPositionsLimit;
    bool m_useLookAt;

    bool m_limitXMovement;
    bool m_limitYMovement;
    bool m_limitZMovement;
    
public:
    CameraObject();
	CameraObject(const char* name);
    CameraObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, bool useLookAt = true);
    virtual ~CameraObject();

    virtual void Update(double TimePassed);
    virtual bool HandleInput(InputEvent& inputevent, double aDelta);

    mat4* GetViewMatrix(){ return &m_viewMatrix; }//Pointer for efficiency, less bytes being passed
	mat4 GetRotationMatrix();

    void FollowObjectXY(GameObject* objectToFollow, double delta);//This functions would need to be call in an update
    void FollowObject(GameObject* objectToFollow, double delta);
    bool CheckTargetInDeadZoneXY(Vector3 aTargetPosition);
    void SetCameraSpeed(float aSpeed){ m_speed = aSpeed; }

    void LookAt();
    void SetTargetPosition(Vector3 aTargetPosition){ m_targetPosition = aTargetPosition; }
    void CalculateLookAtOffset(Vector3 aVelocity,double aDelta);
    void SetUseLookAt(bool aStatus){ m_useLookAt = aStatus; }

    void SetTargetLookAtOffset(Vector3 aLookAtOffset){ m_targetLookAtOffset = aLookAtOffset; }
    Vector3 GetTargetLookAtOffset(){ return m_targetLookAtOffset; }

    void SetCameraOffset(Vector3 aOffset) { m_cameraOffset = aOffset; }
    Vector3 GetCameraOffset(){ return m_cameraOffset; }

    void SetMinVelocityThreshold(Vector3 aThreshold){ m_minVelocityThreshold = aThreshold; }
    void SetMaxVelocityThreshold(Vector3 aThreshold){ m_maxVelocityThreshold = aThreshold; }

    void SetMinPositionsLimit(Vector3 aLimit){ m_minPositionsLimit = aLimit; }
    void SetMaxPositionsLimit(Vector3 aLimit){ m_maxPositionsLimit = aLimit; }


    void SetLimitXMovement(bool aStat){ m_limitXMovement = aStat; }
    void SetLimitYMovement(bool aStat){ m_limitYMovement = aStat; }
    void SetLimitZMovement(bool aStat){ m_limitZMovement = aStat; }
};

#endif //__CameraObject_H__
