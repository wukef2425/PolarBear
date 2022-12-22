#ifndef __FIGHT_SCENE_H__
#define __FIGHT_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "MinHeap.h"
using namespace std;

class FightScene : public cocos2d::Scene
{
private:

    Player* currentPlayer;

    Bullet myBag[10] = { {1,"Bullet/1.png"},{2,"Bullet/2.png"},{3,"Bullet/3.png"} };

    MinHeap<Bullet>* bulletHeap = nullptr;

public:

    static cocos2d::Scene* createScene();

    virtual bool init();

    void addWeapon(float dt);

    static void bindPhysicsBodyAndTag(cocos2d::Sprite*& sprite, int bitmask, int tag);
    static void FightScene::bindPhysicsBodyAndTag(Player*& sprite, int bitmask, int tag);

    void showSpark(const std::string& filename, cocos2d::Node*& node);

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    // callbacks
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(FightScene);
};

#endif // __FIGHT_SCENE_H__

