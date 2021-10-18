#include "CommonHeader.h"

CameraObject::CameraObject() :
m_targetPosition(0, 0),
m_targetLookAtOffset(Vector3(0, 0, 0)),
m_maxTargetLookAtOffset(CAMERA_DEFAULT_MAX_LOOK_AT_OFFSET),
m_cameraOffset(Vector3(0, 0, 0)),
m_minVelocityThreshold(Vector3(0, 0, 0)),
m_maxVelocityThreshold(Vector3(0, 0, 0)),
m_minPositionsLimit(Vector3(0, 0, 0)),
m_maxPositionsLimit(Vector3(0, 0, 0)),
m_limitXMovement(false),
m_limitYMovement(false),
m_limitZMovement(false)
{
    m_speed = CAMERA_DEFAULT_CONTROLS_SPEED;
}

CameraObject::CameraObject(const char* aName) :GameObject(aName),
m_targetPosition(0, 0),
m_targetLookAtOffset(Vector3(0, 0, 0)),
m_maxTargetLookAtOffset(CAMERA_DEFAULT_MAX_LOOK_AT_OFFSET),
m_cameraOffset(Vector3(0, 0, 0)),
m_minVelocityThreshold(Vector3(0, 0, 0)),
m_maxVelocityThreshold(Vector3(0, 0, 0)),
m_minPositionsLimit(Vector3(0, 0, 0)),
m_maxPositionsLimit(Vector3(0, 0, 0)),
m_limitXMovement(false),
m_limitYMovement(false),
m_limitZMovement(false)
{
    m_speed = CAMERA_DEFAULT_CONTROLS_SPEED;
}

CameraObject::CameraObject(Scene* pScene, std::string name, Vector3 pos, Vector3 rot, Vector3 scale, Mesh* pMesh, ShaderProgram* pShader, GLuint texture, bool aUseLookAt) :
m_targetPosition(0, 0),
m_targetLookAtOffset(Vector3(0, 0, 0)),
m_maxTargetLookAtOffset(CAMERA_DEFAULT_MAX_LOOK_AT_OFFSET),
m_cameraOffset(Vector3(0, 0, 0)),
m_minVelocityThreshold(Vector3(0, 0, 0)),
m_maxVelocityThreshold(Vector3(0, 0, 0)),
m_minPositionsLimit(Vector3(0, 0, 0)),
m_maxPositionsLimit(Vector3(0, 0, 0)),
m_limitXMovement(false),
m_limitYMovement(false),
m_limitZMovement(false)
{
    Init(pScene, name, pos, rot, scale, pMesh, pShader, texture);
    m_useLookAt = aUseLookAt;
    m_speed = CAMERA_DEFAULT_CONTROLS_SPEED;
}


CameraObject::~CameraObject()
{
}

void CameraObject::Update(double aTimePassed)
{
    GameObject::Update(aTimePassed);


    if (m_useLookAt == true)
    {
        LookAt();
    }
    else
    {
        //Set the camera
        m_viewMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);//Create the transform for the camera
        m_viewMatrix.Inverse();//Inverse the camera
    }

}

mat4 CameraObject::GetRotationMatrix()
{
    mat4 rotationMatrix;
    rotationMatrix.CreateSRT(vec3(1, 1, 1), m_Rotation, vec3(0, 0, 0));//Make a new matrix only taking into account the rotation

    return rotationMatrix;
}

//This function uses the object Box2D body velocity for movement. So if the object is ever bound or captured by something, its velocity has to reflect that.
//Otherwise the camera will keep moving in its velocity.
void CameraObject::FollowObjectXY(GameObject * aObjectToFollow, double aDelta)
{
    if (aObjectToFollow != nullptr)
    {
        if (CheckTargetInDeadZoneXY(aObjectToFollow->GetPosition()) == false)//If the target is not in the dead zone, follow him
        {
            float distanceFromTarget = aObjectToFollow->Calculate2DDistanceBetween(vec2(m_Position.x + m_cameraOffset.x, m_Position.y + m_cameraOffset.y));//Get how far the camera is from the target

            vec2 velocity = aObjectToFollow->GetPhysicsBodyVelocity();

            m_speed = velocity.Length();

            vec2 direction = velocity;
            direction.Normalize();

            float finalSpeed = 0;//Temporary variable to hold the speed, according to the calculations from camera speed and framerate, etc.

            float stepX;
            float stepY;

            if (distanceFromTarget < CAMERA_DISTANCE_FROM_OBJECT_SMOOTHING)//Smooth speed
            {
                finalSpeed = (m_speed*(float)aDelta)*(distanceFromTarget*CAMERA_SMOOTHING_SPEED_REDUCTION);//Get the speed according to the target, and reduce it as the camera gets close to target

            }
            else//Normal speed
            {
                finalSpeed = m_speed*(float)aDelta;//Get speed according to frame rate
            }

            stepX = direction.x*finalSpeed;//Calculate the movement on X axis
            stepY = direction.y*finalSpeed;//Calculate movement on Y axis

            vec2 newPosition;
            newPosition.x = m_Position.x + stepX;
            newPosition.y = m_Position.y + stepY;

            //Check limits of camera position
            if (m_limitXMovement == true)
            {
                if (newPosition.x<m_minPositionsLimit.x || newPosition.x>m_maxPositionsLimit.x)
                {
                    stepX = 0;
                }
            }

            if (m_limitYMovement == true)
            {
                if (newPosition.y<m_minPositionsLimit.y || newPosition.y>m_maxPositionsLimit.y)
                {
                    stepY = 0;
                }
            }


            m_Position.x += stepX;//Move  the camera toward the target
            m_Position.y += stepY;
        }
    }
}

//NOT POLISHED, JUST CODE FOR SIMPLE 3D TRACKING
void CameraObject::FollowObject(GameObject * aObjectToFollow, double aDelta)
{
    if (aObjectToFollow != nullptr)
    {
        Vector3 objectPositon = aObjectToFollow->GetPosition();
        m_Position = objectPositon + m_cameraOffset;
    }
}

//Check if the target has moved from his designed "dead zone"
bool CameraObject::CheckTargetInDeadZoneXY(Vector3 aTargetPosition)
{
    //Divide the size of the deadzone by 2, so that we can center it
    float halfWidth = CAMERA_DEAD_ZONE_DEFAULT_SIZE.x / 2.0f;
    float halfHeight = CAMERA_DEAD_ZONE_DEFAULT_SIZE.y / 2.0f;

    //Horizontal bounds
    if ((aTargetPosition.x >= m_Position.x - halfWidth &&//Check left side
        aTargetPosition.x <= m_Position.x + halfWidth) &&//Check right side
        (aTargetPosition.y >= m_Position.y - halfHeight &&//Check bottom side
        aTargetPosition.y <= m_Position.y + halfHeight)//Check top side
        )
    {
        return true;//Return that the player is in the deadzone
    }

    return false;//Return that the target is not the deadzone
}

void CameraObject::LookAt()
{
    m_viewMatrix.CreateLookAtViewLeftHanded(m_Position, vec3(0, 1, 0), m_targetPosition + m_targetLookAtOffset);
}

bool CameraObject::HandleInput(InputEvent& aInputevent, double aDelta)
{
    if ((aInputevent.state == InputEventState_Down || aInputevent.state == InputEventState_Held) && aInputevent.type == InputEventType_Key)//Check for keyboard events
    {
        float cameraTranslateSpeed = CAMERA_DEFAULT_CONTROLS_SPEED*(float)aDelta;
        float cameraRotateSpeed = CAMERA_DEFAULT_CONTROLS_ROTATION_SPEED*(float)aDelta;

        vec3 cameraPosition = GetPosition();
        vec3 cameraRotation = GetRotation();

        //Translation
        //Y axis
        if (aInputevent.keycode == VK_UP)
        {
            SetPositionY(cameraPosition.y + cameraTranslateSpeed);
        }

        if (aInputevent.keycode == VK_DOWN)
        {
            SetPositionY(cameraPosition.y - cameraTranslateSpeed);
        }

        //X axis
        if (aInputevent.keycode == VK_RIGHT)
        {
            SetPositionX(cameraPosition.x + cameraTranslateSpeed);
        }

        if (aInputevent.keycode == VK_LEFT)
        {
            SetPositionX(cameraPosition.x - cameraTranslateSpeed);
        }

        //Z axis
        if (aInputevent.keycode == 'O')
        {
            SetPositionZ(cameraPosition.z + cameraTranslateSpeed);
        }

        if (aInputevent.keycode == 'L')
        {
            SetPositionZ(cameraPosition.z - cameraTranslateSpeed);
        }


        //Rotation
        //Y axis
        if (aInputevent.keycode == 'I')
        {
            SetRotationY(cameraRotation.y + cameraRotateSpeed);
        }

        if (aInputevent.keycode == 'J')
        {
            SetRotationY(cameraRotation.y - cameraRotateSpeed);
        }

        //X axis
        if (aInputevent.keycode == 'L')
        {
            SetRotationX(cameraRotation.x + cameraRotateSpeed);
        }

        if (aInputevent.keycode == 'K')
        {
            SetRotationX(cameraRotation.x - cameraRotateSpeed);
        }

        //Z axis
        if (aInputevent.keycode == 'M')
        {
            SetRotationZ(cameraRotation.z + cameraRotateSpeed);
        }

        if (aInputevent.keycode == 'N')
        {
            SetRotationZ(cameraRotation.z - cameraRotateSpeed);
        }
    }

    return false;
}

void CameraObject::CalculateLookAtOffset(Vector3 aVelocity, double aDelta)
{
    Vector3 addedLookAtOffset = Vector3(0, 0, 0);

    //Calculate look at offset using the object velocity
    //X axis
    if (aVelocity.x > m_maxVelocityThreshold.x && aVelocity.x > 0)//Ensure the velocity is significant enough
    {
        addedLookAtOffset.x = CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.x;
    }
    else if (aVelocity.x < m_minVelocityThreshold.x && aVelocity.x < 0)
    {
        addedLookAtOffset.x = -CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.x;
    }

    //y axis
    if (aVelocity.y > m_maxVelocityThreshold.y && aVelocity.y > 0)
    {
        addedLookAtOffset.y = CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.y;
    }
    else if (aVelocity.y < m_minVelocityThreshold.y && aVelocity.y < 0)
    {
        addedLookAtOffset.y = -CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.y;
    }

    //z axis
    if (aVelocity.z > m_maxVelocityThreshold.z && aVelocity.z > 0)
    {
        addedLookAtOffset.z = CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.z;
    }
    else if (aVelocity.z < m_minVelocityThreshold.z && aVelocity.z < 0)
    {
        addedLookAtOffset.z = -CAMERA_LOOK_AT_OFFSET_MOVE_AMOUNT.z;
    }

    //Calculate the distance between the current offset and the offset we want
    float distanceBetweenOffsets;

    //calculate distance using Pythagorean Theorem 
    //distance=Sqrt(x^2+y^2+z^2)
    //distance=sqrt((x-x2)^2+(y-y2)^2+(z-z2)^2)
    float xPowerOfTwo = (addedLookAtOffset.x - m_targetLookAtOffset.x)*(addedLookAtOffset.x - m_targetLookAtOffset.x);
    float yPowerOfTwo = (addedLookAtOffset.y - m_targetLookAtOffset.y)*(addedLookAtOffset.y - m_targetLookAtOffset.y);
    float zPowerOfTwo = (addedLookAtOffset.z - m_targetLookAtOffset.z)*(addedLookAtOffset.z - m_targetLookAtOffset.z);

    distanceBetweenOffsets = sqrtf(xPowerOfTwo + yPowerOfTwo + zPowerOfTwo);

    float finalSpeed = 0;//Temporary variable to hold the speed, according to the calculations from camera speed and framerate, etc.

    //Get the direction vector
    Vector3 directionVector = addedLookAtOffset.Normalize();

    //Move the camera
    if (distanceBetweenOffsets < CAMERA_DISTANCE_FROM_OBJECT_SMOOTHING)//Smooth camera movement
    {

        finalSpeed = (GAME_CAMERA_LOOK_AT_SPEED*(float)aDelta)*(distanceBetweenOffsets*CAMERA_SMOOTHING_SPEED_REDUCTION);//Get the speed according to the target, and reduce it as the camera gets close to target

        float stepX = directionVector.x*finalSpeed;//Calculate the movement on X axis
        float stepY = directionVector.y*finalSpeed;//Calculate movement on Y axis
        float stepZ = directionVector.z*finalSpeed;//Calculate movement on Z axis

        m_targetLookAtOffset += Vector3(stepX, stepY, stepZ);//Move  the offset toward the target
    }
    else if (distanceBetweenOffsets != 0)//Move the camera normally, if we have to move it
    {
        finalSpeed = GAME_CAMERA_LOOK_AT_SPEED*(float)aDelta;//Get speed according to frame rate

        float stepX = directionVector.x*finalSpeed;//Calculate the movement on X axis
        float stepY = directionVector.y*finalSpeed;//Calculate movement on Y axis
        float stepZ = directionVector.z*finalSpeed;//Calculate movement on Z axis

        m_targetLookAtOffset += Vector3(stepX, stepY, stepZ);//Move  the offset toward the target
    }

    //Check the look at offset isn't over the limit
    //X axis
    if (m_targetLookAtOffset.x > m_maxTargetLookAtOffset.x)
    {
        m_targetLookAtOffset.x = m_maxTargetLookAtOffset.x;
    }
    else if (m_targetLookAtOffset.x < -m_maxTargetLookAtOffset.x)
    {
        m_targetLookAtOffset.x = -m_maxTargetLookAtOffset.x;
    }

    //Y axis
    if (m_targetLookAtOffset.y > m_maxTargetLookAtOffset.y)
    {
        m_targetLookAtOffset.y = m_maxTargetLookAtOffset.y;
    }
    else if (m_targetLookAtOffset.y < -m_maxTargetLookAtOffset.y)
    {
        m_targetLookAtOffset.y = -m_maxTargetLookAtOffset.y;
    }

    //Z axis
    if (m_targetLookAtOffset.z > m_maxTargetLookAtOffset.z)
    {
        m_targetLookAtOffset.z = m_maxTargetLookAtOffset.z;
    }
    else if (m_targetLookAtOffset.z < -m_maxTargetLookAtOffset.z)
    {
        m_targetLookAtOffset.z = -m_maxTargetLookAtOffset.z;
    }

}