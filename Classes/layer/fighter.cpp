//
//  fighter.cpp
//  Card
//
//  Created by cyj on 3/1/14.
//
//

#include "fighter.h"
#include "Util.h"

LCBattleScene::LCBattleScene():
//activeRange(nullptr),
hero(NULL)
{
    monsterArr = __Array::create();
    monsterArr->retain();
    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = CC_CALLBACK_1(LCBattleScene::onMouseMove, this);
    _mouseListener->onMouseUp = CC_CALLBACK_1(LCBattleScene::onMouseUp, this);
    _mouseListener->onMouseDown = CC_CALLBACK_1(LCBattleScene::onMouseDown, this);
    _mouseListener->onMouseScroll = CC_CALLBACK_1(LCBattleScene::onMouseScroll, this);
    
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = CC_CALLBACK_2(LCBattleScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = CC_CALLBACK_2(LCBattleScene::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
}

LCBattleScene::~LCBattleScene()
{
    CC_SAFE_RELEASE_NULL(hero);
    CC_SAFE_RELEASE_NULL(monsterArr);
    CC_SAFE_DELETE(tree);
    _eventDispatcher->removeEventListener(_mouseListener);
    _eventDispatcher->removeEventListener(_keyboardListener);
}

Scene* LCBattleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LCBattleScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LCBattleScene::init()
{
    if(!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Point origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    
    auto edgeSp = Sprite::create();
    //auto body = PhysicsBody::createEdgeBox(visibleSize,3); //此句仅3.0 alpha0 有效
    auto body = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);
    edgeSp->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    edgeSp->setPhysicsBody(body);this->addChild(edgeSp);edgeSp->setTag(0);
    //setTouchEnabled(true);
    setKeyboardEnabled(true);
    
    initView();
    
    Rect rect = Rect(0, 0, backGround->getContentSize().width, backGround->getContentSize().height);
    tree = new QuadTree(4, rect);
    
    
    return true;
}

void LCBattleScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    CCLOG("%d", keyCode);
}

void LCBattleScene::update(float duration){
    // set z order start
    for(int i = 0; i < monsterArr->count(); ++i){
        ObjMonster* monster_1 = (ObjMonster*)monsterArr->getObjectAtIndex(i);
        for(int j = i + 1; j < monsterArr->count(); ++j){
            ObjMonster* monster_2 = (ObjMonster*)monsterArr->getObjectAtIndex(j);
            if(monster_1->getrootObj()->getPositionY() < monster_2->getrootObj()->getPositionY()){
                monster_1->getrootObj()->setZOrder(monster_2->getrootObj()->getZOrder() + 1);
            }else{
                monster_2->getrootObj()->setZOrder(monster_1->getrootObj()->getZOrder() + 1);
            }
        }
    }
    
    for(int i = 0; i < monsterArr->count(); ++i){
        ObjMonster* monster_1 = (ObjMonster*)monsterArr->getObjectAtIndex(i);
        //int order = monster_1->getrootObj()->getZOrder();
        if(monster_1->getrootObj()->getPositionY() < hero->getrootObj()->getPositionY()){
            monster_1->getrootObj()->setZOrder(hero->getrootObj()->getZOrder() + 1);
        }else{
            hero->getrootObj()->setZOrder(monster_1->getrootObj()->getZOrder() + 1);
        }
    }
    // set z order end
    
    tree->clear();
    for(int i = 0; i < monsterArr->count(); ++i){
        lifeObj* obj = (lifeObj*)monsterArr->getObjectAtIndex(i);
        tree->addObject(obj);
    }
    tree->addObject(hero);

    __Array* arr = __Array::create();
    
    for(int i = 0; i < monsterArr->count(); ++i){
        lifeObj* monster = (lifeObj*)monsterArr->getObjectAtIndex(i);
        tree->getCollisionObjects(monster, arr);
        for(int j = 0; j < arr->count(); ++j){
            lifeObj* monster_1 = (lifeObj*)arr->getObjectAtIndex(j);
            
            if(monster == monster_1) continue;
            
            Rect rec = monster->getShadowRect();
            Rect rec_1 = ((lifeObj*)monster_1)->getShadowRect();
            if(rec.intersectsRect(rec_1)){
                ((lifeObj*)monster_1)->moveAway(monster->getShadowRect());//(ccp(flag * 20, flag * 20));
                CCLOG("cllo");
                
                monster->actionAttack();
                monster_1->actionAttack();
            }
        }
    }
}

void LCBattleScene::test(float d){
    
//    Label* l = Label::create("100", "fzcy.ttf", 30);
//    //Label* l = Label::create();
//    l->setString("100");
//    l->setPosition(Point(10, 20));
//    this->addChild(l);
//    //sp->addChild(l);
//    
//    Sprite* sp = Sprite::create("Hero1.png");
//    
//    sp->setAnchorPoint(Point(0.5, 0));
//    sp->setPosition(getCenterPos());
//    
//    this->addChild(sp);
//    
//    RotateBy* rotateBy_1 = RotateBy::create(1, 90);
//    RotateBy* rotateBy_2 = RotateBy::create(1, -90);
//    Sequence* seq = Sequence::create(rotateBy_1, rotateBy_2, NULL);
//    //Repeat* repeat = Repeat::create(seq, 1);
//    
//    sp->stopAllActions();
//    sp->runAction(seq);
    
}

void LCBattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
    CCLOG("%c", keyCode);
    if((int)keyCode == 32){
        
        hero->actionRusn();
        
//        hero->hurt();
//        ObjMonster* monster = (ObjMonster*)monsterArr->getObjectAtIndex(0);
//        monster->hurt();
        
        //hero->getrootObj()->setRotation(45);
    }else if ((int)keyCode == 's'){
        hero->die();
        ObjMonster* monster = (ObjMonster*)monsterArr->getObjectAtIndex(0);
        monster->die();
    }
}

void LCBattleScene::onMouseDown(Event *event)
{
//    EventMouse* e = (EventMouse*)event;
//    std::string str = "Mouse Down detected, Key: ";
//    str += tostr(e->getMouseButton());
//    _labelAction->setString(str.c_str());
}

void LCBattleScene::onMouseUp(Event *event)
{
    EventMouse* e = (EventMouse*)event;
    //world->setGravity(Point(0 ,-98));
    //world->setSpeed(1);
//    Vect vct = world->getGravity();
//    float speed = world->getSpeed();
    // right clicked move hero
    if(e->getMouseButton() == 1){
        // retain screen double length
        //Point pos = CCDirector::getInstance()->convertToGL(Point(e->getCursorX(), e->getCursorY()));//glLocation, origin(0,0)
        //pos.y = CCDirector::getInstance()->getWinSize().height - pos.y / 2;
        //CCLOG("%f %f", pos.x, pos.y);
        
        //Point origin = hero->getrootObj()->getPosition();
        
        //CCLOG("%f %f", origin.x, origin.y);
        
        //Point convertedToNodeLocation = backGround->convertToNodeSpace(pos);
        
        //Point p = getPointInMap();
        hero->actionWalk(this, backGround->convertToNodeSpace(Point(e->getCursorX(), e->getCursorY())));
    }else if(e->getMouseButton() == 0){
        hero->actionAttack();
    }
}
template <typename T> std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); }

void LCBattleScene::onMouseMove(Event *event)
{
//    EventMouse* e = (EventMouse*)event;
    // move map if beyond screen
//    std::string str = "MousePosition X:";
//    str = str + tostr(e->getCursorX()) + " Y:" + tostr(e->getCursorY());
//    
//    CCLOG("mouse pos:%s", str.c_str());
//    
//    Point origin = hero->getrootObj()->getPosition();
//    
//    CCLOG("hero pos:%f %f", origin.x, origin.y);
}

void LCBattleScene::onMouseScroll(Event *event)
{
//    EventMouse* e = (EventMouse*)event;
//    std::string str = "Mouse Scroll detected, X: ";
//    str = str + tostr(e->getScrollX()) + " Y: " + tostr(e->getScrollY());
//    _labelAction->setString(str.c_str());
}

void LCBattleScene::sendPosition(Point pos){
}

void LCBattleScene::createHero(){
    hero = ObjHero::create(backGround, getCenterPos());
    hero->retain();
    hero->setActiveRange(backGround->getContentSize());
    hero->actionStand();
    
    hero->getrootObj()->setTag(ETagHero);
}

Point LCBattleScene::getCenterPos(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    return Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);
}

//do initialization here
void LCBattleScene::initView()
{
    backGround = Sprite::create("BackGround.png");
    //backGround->setScale(3);
    this->addChild(backGround);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    backGround->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    createHero();
    
    addMonster(3);
    
    schedule(schedule_selector(LCBattleScene::followHero), 3.0f, -1, 1);
    
    scheduleOnce(schedule_selector(LCBattleScene::test), 1);
    scheduleUpdate();
 //   schedule(, 2, 2);
}

Point LCBattleScene::getPointInMap(Point pos){
    float minX = 0;
    float maxX = backGround->getContentSize().width;
    
    float minY = 0;
    float maxY = backGround->getContentSize().height / 2;
    
    pos.x = pos.x > maxX ? maxX : pos.x;
    pos.x = pos.x < minX ? minX : pos.x;
    
    pos.y = pos.y > maxY ? maxY : pos.y;
    pos.y = pos.y < minY ? minY : pos.y;
    
    return  pos;
}

void LCBattleScene::addMonster(int amount){
    while(amount--) {
        addMonsterAtPosition(ccpAdd(getCenterPos(), Point(rand() % 50, rand() % 60)));
    }
}

void LCBattleScene::followHero(float duration){
    for(int i = 0; i < monsterArr->count(); ++i){
        ObjMonster * monster = (ObjMonster*)monsterArr->getObjectAtIndex(i);
        monster->actionWalk(hero->getrootObj()->getPosition());
    }
}

void LCBattleScene::backClicked(){
}

void LCBattleScene::addMonsterAtPosition(Point p)
{
    ObjMonster* monster = ObjMonster::create(backGround, p, monsterArr->count() + 1);//new ObjMonster();
    
    monster->setActiveRange(backGround->getContentSize());
    monster->actionStand();
    monster->setID(monsterArr->count() + 1);
    monsterArr->addObject(monster);
    
    monster->getrootObj()->setTag(ETagMonster);
}
