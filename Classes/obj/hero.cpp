//
//  hero.cpp
//  Card
//
//  Created by cyj on 3/9/14.
//
//

#include "hero.h"
#include "Util.h"
#include "QuadTree.h"

float ObjHero::speed = 230;

ObjHero* ObjHero::create(Node* target, Point pos){
    assert(target);
    if(!target) return NULL;
    //createLifeObj("OHero", "@hero");
    ObjHero* hero = new ObjHero();
    hero->rootObj = Sprite::create("Hero1.png");
    //hero->rootObj->setContentSize(Size(100, 100));
    hero->rootObj->retain();
    hero->rootObj->setAnchorPoint(Point(0.5, 0));
    target->addChild(hero->rootObj);
    hero->rootObj->setPosition(pos);
    
    hero->setHP(300);
    hero->autorelease();
    
    
    Sprite* sp = Sprite::create("HeroState3.png");
    hero->hpProgress  = ProgressTimer::create(sp);
    hero->hpProgress->setType(ProgressTimer::Type::BAR);
    hero->hpProgress->setBarChangeRate(ccp(1, 0));
    hero->hpProgress->setMidpoint(Point(0, 0.5));
    hero->hpProgress->setPosition(Point(hero->getrootObj()->getContentSize().width / 2, hero->getrootObj()->getContentSize().height + sp->getContentSize().height / 2));
    hero->hpProgress->setPercentage(100);
    hero->getrootObj()->addChild(hero->hpProgress);
    
    hero->setKeyPoint_l(Point(0, hero->rootObj->getContentSize().height / 2));
    
    hero->setKeyPoint_r(Point(hero->rootObj->getContentSize().width, hero->rootObj->getContentSize().height / 2));
    hero->setObjType(EObjType::E_HERO);
    
    hero->setDebugLabel(Label::create("0", "fzcy.ttf", 30));
    target->addChild(hero->getDebugLabel(), 10);
    
#ifndef HIDE_COLLISION_RECT
    hero->shadow = Sprite::create();
    target->addChild(hero->shadow);
#endif
    //hero->shadow->setTextureRect(Rect(0, 0, 122, 30));
    
    //hero->shadow->setPosition(Point(hero->rootObj->getContentSize().width / 2 - 15, 0));
    //hero->rootObj->addChild(hero->shadow);
    
    
    return hero;
}

Rect ObjHero::getShadowRect(){
    
    Rect rec = Rect(rootObj->getPositionX() - rootObj->getContentSize().width / 2.0f - 15, rootObj->getPositionY(), 122, 30);
    
#ifndef HIDE_COLLISION_RECT
    shadow->setTextureRect(rec);
    shadow->setPosition(Point(rec.origin.x + rec.size.width / 2.0f, rec.origin.y + rec.size.height / 2.0f));
#endif
    return rec;
}

void ObjHero::actionStand(){
    assert(rootObj);
    if(getHP() <= 0){
        die();
        return;
    }
    rootObj->stopAllActions();
    setDesPoin(rootObj->getPosition());
    setState(EobjState::E_STAND);
    Animation* stand = createAnimateWithFileNames("Hero%d.png", 3);
    auto action = Animate::create(stand);
    rootObj->runAction(RepeatForever::create(action));
}

void ObjHero::actionWalk(Node* target, Point des){
    
    Point destination = getPointInMap(des);
    
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_WALK);
    if((destination.x < rootObj->getPosition().x && dir == EDIR_FORWARD) || (destination.x > rootObj->getPosition().x && dir == EDIR_BACKWARD)){
        turnAround();
    }
    Animation* walk = createAnimateWithFileNames("HeroRun%d.png", 11);
    walk->setDelayPerUnit(0.1);
    auto action = Animate::create(walk);
    
    float distance = sqrtf((destination.x - rootObj->getPositionX()) * (destination.x - rootObj->getPositionX()) +
                           (destination.y - rootObj->getPositionY()) * (destination.y - rootObj->getPositionY()));
    float dur = distance / speed;
    
    setDesPoin(des);
    auto moveTo = MoveTo::create(dur, destination);
    rootObj->runAction(RepeatForever::create(action));
    rootObj->runAction(CCSequence::create(moveTo, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
    
    //target->runAction(Follow::create(rootObj));
}

void ObjHero::actionRusn(){
    if(getState() == EobjState::E_RUSH) return;
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_RUSH);
    int flag = 1;
    if(dir == EDIR_FORWARD) flag = 1;
    else flag = -1;
    int distance = 60;
    
    Point p = ccpAdd(rootObj->getPosition(), ccp(distance * flag, 0));
    
    Animation* rush = createAnimateWithFileNames("HeroAttackT%d.png", 3);
    rush->setDelayPerUnit(0.1);
    auto actionRush = Animate::create(rush);
    
    rootObj->runAction(actionRush);
    
    MoveTo* moveTo = MoveTo::create(0.3, getPointInMap(p));
    
    rootObj->runAction(CCSequence::create(moveTo, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
}

void ObjHero::actionAttack(){
    if(getState() == EobjState::E_ATTACK) return;
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_ATTACK);
    Animation* attack = createAnimateWithFileNames("HeroAttack%d.png", 5);
    attack->setDelayPerUnit(0.1);
    auto action = Animate::create(attack);
    
    rootObj->runAction(CCSequence::create(action, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
    
    //scheduleOnce(schedule_selector(ObjHero::attackEffect), 0.2f);
    attackEffect();
}

//void ObjHero::attackEffect(){
//    //__Array* objList = objList;
//    //if(objList == NULL || objList->count() <= 0) return;
//    
//    Point centerP = Point(rootObj->getPositionX(), rootObj->getPositionY() + rootObj->getContentSize().height / 2);
//    
//    getDebugLabel()->setPosition(centerP);
//    float CR = rootObj->getContentSize().width / 2 + 100;
//    Point up = Point(1.0f, 0);
//    float cosTheta = 1.0f / sqrtf(2.0f);
//    
//    __Array* objList = __Array::create();
//    //lifeObj* monster = (lifeObj*)monsterArr->getObjectAtIndex(i);
//    QuadTree::getInstance()->getCollisionObjects(this, objList);
//    //for(int j = 0; j < arr->count(); ++j){
//            
//    for(int i = 0; i < objList->count(); ++i){
//        lifeObj* obj = dynamic_cast<lifeObj*>(objList->getObjectAtIndex(i));
//        if(obj->getState() == EobjState::E_HURT) continue;
//        Point kp = obj->getKeyPoint(rootObj->getPosition());
//        if(kp.x < centerP.x) up.x = -1;
//        obj->getDebugLabel()->setPosition(kp);
//        
//        if(IsPointInCircularSector3(centerP.x, centerP.y, up.x, up.y, CR * CR, cosTheta,
//                                    kp.x, kp.y)){
//            obj->hurt(getATK());
//            CCLOG("hit!!");
//        }
//    }
//}
