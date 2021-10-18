#ifndef __Box2DContactListener__H__
#define  __Box2DContactListener__H__


class Box2DContactListener :public b2ContactListener
{
protected:
   
public:
    Box2DContactListener();
    ~Box2DContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif