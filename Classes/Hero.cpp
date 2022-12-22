#include "Hero.h"
#include "Consts.h"
#include "FightScene.h"
#include "HelloWorldScene.h"

Player* Player::create(const std::string& filename)
{
    auto player = new(std::nothrow) Player;

    if (!player)
    {
        return nullptr;
    }

    auto chosenHero = Sprite::create(filename);

    if (chosenHero)
    {
        FightScene::bindPhysicsBodyAndTag(player, EnemyBulletAndPlayerBitmask, PlayerTag);

        player->addChild(chosenHero);

        player->setPosition(player->x, player->y);

        player->autorelease();

        return player;
    }

    return nullptr;
}

void Player::listenToKeyPresses(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    using K = cocos2d::EventKeyboard::KeyCode;// ȫif��elseif����ʵ��ͬʱ��������

    if (keyCode == K::KEY_D)
    {
        keyPressed[D] = true;
    }
    if (keyCode == K::KEY_A)
    {
        keyPressed[A] = true;
    }
    if (keyCode == K::KEY_W)
    {
        keyPressed[W] = true;
    }
    if (keyCode == K::KEY_S)
    {
        keyPressed[S] = true;
    }
}

void Player::listenToKeyReleases(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    using K = cocos2d::EventKeyboard::KeyCode;

    if (keyCode == K::KEY_W)
    {
        keyPressed[W] = false;
    }
    if (keyCode == K::KEY_A)
    {
        keyPressed[A] = false;
    }
    if (keyCode == K::KEY_S)
    {
        keyPressed[S] = false;
    }
    if (keyCode == K::KEY_D)
    {
        keyPressed[D] = false;
    }
}

/*  ������ҵ���Ϣ����������λ�ã�����λ����barrier/grass���Լ���ͷ���� */
void Player::update(float dt)
{
    if (keyPressed[W])
    {
        y += step;
    }
    if (keyPressed[A])
    {
        x -= step;
    }
    if (keyPressed[S])
    {
        y -= step;
    }
    if (keyPressed[D])
    {
        x += step;
    }
    setPosition(x, y);
}

void Player::attack(Vec2 playerPosition, Vec2 touchWorldPosition, const std::string& filename)
{
    /* ����currentBullet�����ó�ʼλ�� */
    auto currentBullet = Sprite::create(filename);

    if (currentBullet == nullptr) {
        Director::getInstance()->pushScene(HelloWorld::createScene());
        return;
        // �ӵ�����ص���ʼ���棬�Զ��˳�
    }

    currentBullet->setPosition(playerPosition);// ��ʼλ���Ǵ�currentPlayer����

    /* ������Ⱦ�� */
    this->getParent()->addChild(currentBullet);

    Vec2 offset = touchWorldPosition - playerPosition;
    offset.normalize();// currentPlayerλ��ָ�����touchλ�õĵ�λ����

    /* ����һЩ���� */
    auto actionMove = MoveBy::create(1.5f, offset * ShootSpeed);// 1.5�뵽��Ŀ�ĵ�
    auto actionRemove = RemoveSelf::create();// ɾ������

    /* ��currentBullet��������һϵ�ж��� */
    currentBullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}


