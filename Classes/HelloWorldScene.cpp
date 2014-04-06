#include "HelloWorldScene.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDraw(true); //此句仅3.0 alpha0 有效
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    //layer->setPhyWorld(scene->getPhysicsWorld());
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    
    auto edgeSp = Sprite::create();
    //auto body = PhysicsBody::createEdgeBox(visibleSize,3); //此句仅3.0 alpha0 有效
    auto body = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);
    edgeSp->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    edgeSp->setPhysicsBody(body);this->addChild(edgeSp);edgeSp->setTag(0);
    
    setTouchEnabled(true);
    
    return true;
}

void HelloWorld::setPhysicsBody(PhysicsBody* body)
{
    _physicsBody = body;
    _physicsBody->retain();
    
    
    //_physicsBody->setPositionOffset(Point(0, 0));
    //_physicsBody->setRotationOffset(Point(0, 0));
    //_physicsBody->setRotationEnable(true);
   // _physicsBody->set = this;
   // auto contactListener = EventListenerPhysicsContactWithBodies::create(platform->getPhysicsBody(), ball->getPhysicsBody());
   // contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
   // _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool HelloWorld::onContactBegin(EventCustom* event, const PhysicsContact& contact)
{
    auto spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
    auto spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
    
    if (spriteA->getTag() == 1)
    {
        spriteA->removeFromParentAndCleanup(true);
    }
    
    if (spriteB->getTag() == 1)
    {
        spriteB->removeFromParentAndCleanup(true);
    }
    
    return true;
}

//void HelloWorld::onEnter()
//{
    //auto contactListener = EventListenerPhysicsContactWithBodies::create(platform->getPhysicsBody(), ball->getPhysicsBody());
    //contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    //auto contactListener = EventListenerPhysicsContact::create();
    //contactListener->onContactBegin = onContactBegin;//CC_CALLBACK_2(HelloWorld::onContactBegin, this);
    
    //auto dispatcher = Director::getInstance()->getEventDispatcher();
    
    //dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
//}

void HelloWorld::addNewSpriteAtPosition(Point p)
{
    auto sp = Sprite::create("CloseNormal.png");
    sp->setTag(1);
    auto body = PhysicsBody::createBox(Size(80, 40));
    sp->setPhysicsBody(body);
    sp->setPosition(p);
    this->addChild(sp);
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
    for(auto touch:touches)
    {
        auto location = touch->getLocation();
        addNewSpriteAtPosition(location);
    }
}
//
//void HelloWorld::menuCloseCallback(Ref* pSender)
//{
//    if(m_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_NONE)
//    {
//        m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
//    }
//    else
//    {
//        m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//    }
//    
////    Director::getInstance()->end();
////
////#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
////    exit(0);
////#endif
//}
