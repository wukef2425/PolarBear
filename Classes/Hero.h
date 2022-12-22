#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Consts.h"
#include "cocos2d.h"

USING_NS_CC;

class Player :public cocos2d::Sprite
{
private:

    enum Key { W, A, S, D, E, C };

    bool keyPressed[6] = {};

    /* ���ò��� */
    float step = PlayerStep;

    /* ��ȡ��Ļ��С������������playerλ�� */
    cocos2d::Size winSize = Director::getInstance()->getVisibleSize();

    float x = winSize.width * 0.5f;
    float y = winSize.height * 0.5f;

public:

    static Player* create(const std::string& filename);
    void attack(Vec2 playerPosition, Vec2 touchWorldPosition, const std::string& filename);
    /* ���ڼ������� ʹplayer�ƶ� */
    void listenToKeyPresses(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void listenToKeyReleases(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void update(float dt) override;
};

#endif // #define __PLAYER_H__