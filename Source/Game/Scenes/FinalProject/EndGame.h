#ifndef __EndGame_H__
#define __EndGame_H__

class EndGame: public Scene
{
protected:
    FGEndStates m_endState;
    int m_finalScore;

public:
    EndGame(int score, FGEndStates aEndState);
    virtual ~EndGame();

    virtual void LoadContent(b2Vec2 worldGravity = b2Vec2(0.0f, -9.8f));
    virtual void LoadResources(b2Vec2 worldGravity);
    virtual void LoadGameObjects();
    virtual bool HandleInput(InputEvent& inputevent, double delta);
    virtual void Update(double TimePassed);

};

#endif //__Scene_H__