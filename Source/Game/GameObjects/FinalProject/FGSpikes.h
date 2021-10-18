#ifndef __FGSpikes_H__
#define __FGSpikes_H__

class FGPSpikes : public LivingObject
{

protected:

public:
    FGPSpikes(int aCollisionDamage,bool destructible=false,int maxHealth=20);
    virtual ~FGPSpikes();
    virtual void OnDeath();
};

#endif
