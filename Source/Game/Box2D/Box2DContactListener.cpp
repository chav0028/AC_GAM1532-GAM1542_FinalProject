#include "CommonHeader.h"

Box2DContactListener::Box2DContactListener()
{

}

Box2DContactListener::~Box2DContactListener()
{
}

void Box2DContactListener::BeginContact(b2Contact* aContact)
{
    //Get the game objects involved in contact
    GameObject* objectA = (GameObject*)aContact->GetFixtureA()->GetBody()->GetUserData();
    GameObject* objectB = (GameObject*)aContact->GetFixtureB()->GetBody()->GetUserData();

    //Get fixtures involved in contact
    b2Fixture* fixtureA = aContact->GetFixtureA();
    b2Fixture* fixtureB = aContact->GetFixtureB();

    //Get the world manifold
    b2WorldManifold worldManifold;
    aContact->GetWorldManifold(&worldManifold);


    //Call both objects begin collision methods
    objectA->BeginCollision(fixtureA, fixtureB, objectB, -worldManifold.normal, worldManifold.points[0], aContact->IsTouching());///Inverse normal so regardless of the fixture, the normal is always pointing toward the object
    objectB->BeginCollision(fixtureB, fixtureA, objectA, worldManifold.normal, worldManifold.points[0], aContact->IsTouching());

}

void Box2DContactListener::EndContact(b2Contact* aContact)
{
    //Get the game objects involved in contact
    GameObject* objectA = (GameObject*)aContact->GetFixtureA()->GetBody()->GetUserData();
    GameObject* objectB = (GameObject*)aContact->GetFixtureB()->GetBody()->GetUserData();

    //Get fixtures involved in contact
    b2Fixture* fixtureA = aContact->GetFixtureA();
    b2Fixture* fixtureB = aContact->GetFixtureB();

    //Get the world manifold
    b2WorldManifold worldManifold;
    aContact->GetWorldManifold(&worldManifold);
    

    //Call both objects end collision methods
    objectA->EndCollision(fixtureA, fixtureB, objectB, -worldManifold.normal, worldManifold.points[0], aContact->IsTouching());
    objectB->EndCollision(fixtureB, fixtureA, objectA, worldManifold.normal, worldManifold.points[0],aContact->IsTouching());

}

void Box2DContactListener::PostSolve(b2Contact* aContact,const  b2ContactImpulse* aImpulse)
{
    //Get the game objects involved in contact
    GameObject* objectA = (GameObject*)aContact->GetFixtureA()->GetBody()->GetUserData();
    GameObject* objectB = (GameObject*)aContact->GetFixtureB()->GetBody()->GetUserData();

    //Get fixtures involved in contact
    b2Fixture* fixtureA = aContact->GetFixtureA();
    b2Fixture* fixtureB = aContact->GetFixtureB();

    //Get the world manifold
    b2WorldManifold worldManifold;
    aContact->GetWorldManifold(&worldManifold);

    //Call both objects post solve methods
    objectA->CollsionPostSolve(aContact,fixtureA, fixtureB, objectB, -worldManifold.normal, aImpulse);///Inverse normal so regardless of the fixture, the normal is always pointing toward the object
    objectB->CollsionPostSolve(aContact,fixtureB, fixtureA, objectA, worldManifold.normal, aImpulse);

}


void Box2DContactListener::PreSolve(b2Contact* aContact, const b2Manifold* aOldManifold)
{
    //Get the game objects involved in contact
    GameObject* objectA = (GameObject*)aContact->GetFixtureA()->GetBody()->GetUserData();
    GameObject* objectB = (GameObject*)aContact->GetFixtureB()->GetBody()->GetUserData();

    //Get fixtures involved in contact
    b2Fixture* fixtureA = aContact->GetFixtureA();
    b2Fixture* fixtureB = aContact->GetFixtureB();

    //Get the world manifold
    b2WorldManifold worldManifold;
    aContact->GetWorldManifold(&worldManifold);
    
    //Check if objects collide in the Z depth.
    vec3 objectBPos = objectB->GetPosition();
    float objectBHalfDepth = objectB->GetZLength() / 2.0f;

    //Check if the objects care about depth
    if (objectA->GetIsCheckingCollisionDepth() == true && objectB->GetIsCheckingCollisionDepth() == true)
    {
        if (objectA->DepthCollisionCheck(objectBPos.z + objectBHalfDepth, objectBPos.z - objectBHalfDepth) == false)
        {
            //If there are is no collision, disable contact
            aContact->SetEnabled(false);
            return;
        }
    }

    //Call both object presolve method
    objectA->CollsionPreSolve(aContact,fixtureA, fixtureB, objectB, -worldManifold.normal);///Inverse normal so regardless of the fixture, the normal is always pointing toward the object
    objectB->CollsionPreSolve(aContact,fixtureB, fixtureA, objectA, worldManifold.normal);

}
