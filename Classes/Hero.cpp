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
    using K = cocos2d::EventKeyboard::KeyCode;// 全if不elseif可以实现同时按两个键

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

/*  更新玩家的信息，包括所在位置，所在位置是barrier/grass，以及镜头跟随 */
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
    /* 创造currentBullet并设置初始位置 */
    auto currentBullet = Sprite::create(filename);

    if (currentBullet == nullptr) {
        Director::getInstance()->pushScene(HelloWorld::createScene());
        return;
        // 子弹用完回到开始界面，自动退出
    }

    currentBullet->setPosition(playerPosition);// 初始位置是从currentPlayer出发

    /* 加入渲染树 */
    this->getParent()->addChild(currentBullet);

    Vec2 offset = touchWorldPosition - playerPosition;
    offset.normalize();// currentPlayer位置指向鼠标touch位置的单位向量

    /* 定义一些动作 */
    auto actionMove = MoveBy::create(1.5f, offset * ShootSpeed);// 1.5秒到达目的地
    auto actionRemove = RemoveSelf::create();// 删掉自身

    /* 让currentBullet完成上面的一系列动作 */
    currentBullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}


