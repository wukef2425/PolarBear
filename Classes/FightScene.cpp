#include "FightScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* FightScene::createScene()
{
    return FightScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool FightScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init() || !Scene::initWithPhysics())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // set background color
    auto background = DrawNode::create();
    background->drawSolidRect(origin, origin + visibleSize, cocos2d::Color4F(0.5f, 0.6f, 0.7f, 1.0f));
    this->addChild(background, 0);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(FightScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);    // menu的这些标签都加在第一层

    /////////////////////////////
    // 3. add your codes below...

    this->currentPlayer = Player::create("polarbear.png");
    this->addChild(this->currentPlayer);
    this->currentPlayer->scheduleUpdate();

    bulletHeap = new MinHeap<Bullet>(myBag, 3);

    /* 监听键盘WASD实现移动 */
    auto keyboardListener = cocos2d::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::listenToKeyPresses, this->currentPlayer);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::listenToKeyReleases, this->currentPlayer);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    /* 监听鼠标 实现点击攻击 */
    auto eventListener = cocos2d::EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(FightScene::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

    /* 碰撞监听 */
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FightScene::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    srand(static_cast<unsigned>(time(nullptr)));
    this->schedule(CC_SCHEDULE_SELECTOR(FightScene::addWeapon), 2.0f);// 跑一次武器生成的频率

    return true;
}

bool FightScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unusedEvent)
{
    /* touch转世界坐标 */
    cocos2d::Size winSize = Director::getInstance()->getVisibleSize();
    Vec2 touchWorldPosition = touch->getLocation() + currentPlayer->getPosition() - Vec2(winSize.width * 0.5f, winSize.height * 0.5f);

    Bullet currentBullet;
    bulletHeap->RemoveMin(currentBullet);
    currentPlayer->attack(currentPlayer->getPosition(), touchWorldPosition, currentBullet.name);

    unusedEvent->stopPropagation();// 停止事件传播，避免按下去抬起来执行两次

    return false;// 不把事件向下层分发
}

void FightScene::addWeapon(float dt)
{
    srand(static_cast<unsigned>(time(nullptr)));

    auto weapon = Sprite::create("Bullet/1.png");

    switch (rand() % 5 + 1)
    {
        case WeaponTag1:
            weapon = Sprite::create("Bullet/1.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag1);
            break;

        case WeaponTag2:
            weapon = Sprite::create("Bullet/2.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag2);
            break;

        case WeaponTag3:
            weapon = Sprite::create("Bullet/3.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag3);
            break;

        case WeaponTag4:
            weapon = Sprite::create("Bullet/4.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag4);
            break;

        case WeaponTag5:
            weapon = Sprite::create("Bullet/5.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag5);
            break;

        default:
            weapon = Sprite::create("Bullet/1.png");
            FightScene::bindPhysicsBodyAndTag(weapon, EnemyBulletAndPlayerBitmask, WeaponTag1);
            break;
    }

    auto weaponSize = weapon->getContentSize();
    auto conSize = this->getContentSize();

    int minY = weaponSize.height / 2;
    int maxY = conSize.height - minY;
    int rangeY = maxY - minY;

    int randomY = (rand() % rangeY) + minY;

    weapon->setPosition(Vec2(conSize.width + weaponSize.width/2, randomY));
    this->addChild(weapon);

    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    int randomDuration = (rand() % rangeDuration) + minDuration;


    auto actionMove = MoveTo::create(randomDuration, Vec2(-weaponSize.width / 2, randomY));

    auto actionRemove = RemoveSelf::create();
    weapon->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

bool FightScene::onContactBegin(cocos2d::PhysicsContact& contact)
{
    /* 获取两个碰撞的物体 */
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    /* 火花特效 */
    if (nodeA && nodeB)
    {
        if (nodeA->getTag() == PlayerTag && nodeB->getTag() == WeaponTag1)
        {
            bulletHeap->Insert(Bullet{ WeaponTag1,"Bullet/1.png" });
            showSpark("Bullet/1.png", nodeB);
        }
        else if (nodeB->getTag() == PlayerTag && nodeA->getTag() == WeaponTag1)
        {
            bulletHeap->Insert(Bullet{ WeaponTag1,"Bullet/1.png" });
            showSpark("Bullet/1.png", nodeA);
        }
        else if (nodeA->getTag() == PlayerTag && nodeB->getTag() == WeaponTag2)
        {
            bulletHeap->Insert(Bullet{ WeaponTag2,"Bullet/2.png" });
            showSpark("Bullet/2.png", nodeB);
        }
        else if (nodeB->getTag() == PlayerTag && nodeA->getTag() == WeaponTag2)
        {
            bulletHeap->Insert(Bullet{ WeaponTag2,"Bullet/2.png" });
            showSpark("Bullet/2.png", nodeA);
        }
        else if (nodeA->getTag() == PlayerTag && nodeB->getTag() == WeaponTag3)
        {
            bulletHeap->Insert(Bullet{ WeaponTag3,"Bullet/3.png" });
            showSpark("Bullet/3.png", nodeB);
        }
        else if (nodeB->getTag() == PlayerTag && nodeA->getTag() == WeaponTag3)
        {
            bulletHeap->Insert(Bullet{ WeaponTag3,"Bullet/3.png" });
            showSpark("Bullet/3.png", nodeA);
        }
        else if (nodeA->getTag() == PlayerTag && nodeB->getTag() == WeaponTag4)
        {
            bulletHeap->Insert(Bullet{ WeaponTag4,"Bullet/4.png" });
            showSpark("Bullet/4.png", nodeB);
        }
        else if (nodeB->getTag() == PlayerTag && nodeA->getTag() == WeaponTag4)
        {
            bulletHeap->Insert(Bullet{ WeaponTag4,"Bullet/4.png" });
            showSpark("Bullet/4.png", nodeA);
        }
        else if (nodeA->getTag() == PlayerTag && nodeB->getTag() == WeaponTag5)
        {
            bulletHeap->Insert(Bullet{ WeaponTag5,"Bullet/5.png" });
            showSpark("Bullet/5.png", nodeB);
        }
        else if (nodeB->getTag() == PlayerTag && nodeA->getTag() == WeaponTag5)
        {
            bulletHeap->Insert(Bullet{ WeaponTag5,"Bullet/5.png" });
            showSpark("Bullet/5.png", nodeA);
        }
    }
    return true;
}

void FightScene::showSpark(const std::string& filename, cocos2d::Node*& node)// 创造图片为filename的火花，删除节点node
{
    auto spark = Sprite::create(filename);
    spark->setPosition(node->getPosition());
    this->addChild(spark);
    auto scaleTo = ScaleTo::create(0.5f, 0.5f, 0.5f);
    auto actionRemove = RemoveSelf::create();
    spark->runAction(Sequence::create(scaleTo, actionRemove, nullptr));
    node->removeFromParentAndCleanup(true);
}

void FightScene::bindPhysicsBodyAndTag(cocos2d::Sprite*& sprite, int bitmask, int tag)// 传引用，否则会被释放掉
{
    auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.f, 0.f, 0.f));
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(bitmask);// bitmask是碰撞的body种类 这一句是设置种类
    physicsBody->setContactTestBitmask(bitmask);// 这一句是在这个种类碰撞的时候通知
    sprite->setPhysicsBody(physicsBody);
    sprite->setTag(tag);
}

void FightScene::bindPhysicsBodyAndTag(Player*& sprite, int bitmask, int tag)// 传引用，否则会被释放掉
{
    auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.f, 0.f, 0.f));
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(bitmask);// bitmask是碰撞的body种类 这一句是设置种类
    physicsBody->setContactTestBitmask(bitmask);// 这一句是在这个种类碰撞的时候通知
    sprite->setPhysicsBody(physicsBody);
    sprite->setTag(tag);
}

void FightScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->pushScene(HelloWorld::createScene());
}