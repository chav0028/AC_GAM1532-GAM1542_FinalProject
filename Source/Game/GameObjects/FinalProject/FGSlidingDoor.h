#ifndef __FGSlidingDoor_H__
#define __FGSlidingDoor_H__

class FGSlidingDoor : public LivingObject
{

protected:
    b2PrismaticJoint* m_pJoint;

    bool m_isStationary;
    double m_waitTime;
    double m_Timerwait;
    double m_warningTime;
    Vector4 m_stationaryColor;
    Vector4 m_warningColor;
    Vector4 m_movingColor;

public:
    FGSlidingDoor(double waitTime,double warningTime,Vector4 stationaryColor, Vector4 warningColor,Vector4 movingColor);
    virtual ~FGSlidingDoor();

    virtual void Update(double TimePassed);
    void SetJoint(b2PrismaticJoint* joint);
};

#endif
