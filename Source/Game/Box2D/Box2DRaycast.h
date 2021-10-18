#ifndef __Box2DRayCast__H__
#define  __Box2DRayCast__H__

#include <typeinfo>

enum Box2DRayCastType
{
    Box2DRayCastType_Closest,
    Box2DRayCastType_AllFixtures,
    Box2DRayCastType_Anything,
    Box2DRayCastType_Ignore
};

struct RayCastInformation
{
    b2Body* m_pBody;
    b2Fixture* m_pFixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;
};

class Box2DRayCast : public b2RayCastCallback
{

protected:
    bool m_hit;
    std::vector<RayCastInformation*> m_rayCastInformation;
    Box2DRayCastType m_rayCastType;
	std::vector<b2BodyType> m_bodyTypesToIgnore;
    
public:
    
    Box2DRayCast(Box2DRayCastType aRayCastType = Box2DRayCastType_Closest);
    float32 ReportFixture(b2Fixture* aFixture, const b2Vec2& aPoint, const b2Vec2 &aNormal, float32 aFraction);

    RayCastInformation* GetRayCastInformation(unsigned int aIndex = 0);
    std::vector<RayCastInformation*> GetAllRayCastInformation();
	void ClearAllRayCastInformation();

    bool GetHit() {return m_hit; }
    void SetHit(bool aStatus){ m_hit = aStatus; }

	void AddBodyTypeToIgnore(b2BodyType aBodyType);
	void RemoveBodyTypeToIgnore(b2BodyType aBodyType);

	void SetRayCastType(Box2DRayCastType aRayCastType) { m_rayCastType = aRayCastType; }

	void ResetRayCast();

};



#endif