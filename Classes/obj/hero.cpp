//
//  hero.cpp
//  Card
//
//  Created by cyj on 3/9/14.
//
//

#include "hero.h"

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
    
    hero->setHP(1000);
    hero->autorelease();
    
    
    Sprite* sp = Sprite::create("HeroState3.png");
    hero->hpProgress  = ProgressTimer::create(sp);
    hero->hpProgress->setType(ProgressTimer::Type::BAR);
    hero->hpProgress->setMidpoint(Point(0, 0.5));
    hero->hpProgress->setPosition(Point(hero->getrootObj()->getContentSize().width / 2, hero->getrootObj()->getContentSize().height + sp->getContentSize().height / 2));
    hero->hpProgress->setPercentage(100);
    hero->getrootObj()->addChild(hero->hpProgress);
    
    
#ifndef HIDE_COLLISION_RECT
    hero->shadow = Sprite::create();
    target->addChild(hero->shadow);
#endif
    //hero->shadow->setTextureRect(Rect(0, 0, 122, 30));
    
    //hero->shadow->setPosition(Point(hero->rootObj->getContentSize().width / 2 - 15, 0));
    //hero->rootObj->addChild(hero->shadow);
    
    
    return hero;
    //actionStand();
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
    rootObj->stopAllActions();
    Animation* stand = createAnimateWithFileNames("Hero%d.png", 3);
    auto action = Animate::create(stand);
    rootObj->runAction(RepeatForever::create(action));
}

void ObjHero::actionWalk(Node* target, Point des){
    
    Point destination = getPointInMap(des);
    
    assert(rootObj);
    rootObj->stopAllActions();
    if((destination.x < rootObj->getPosition().x && dir == EDIR_FORWARD) || (destination.x > rootObj->getPosition().x && dir == EDIR_BACKWARD)){
        turnAround();
    }
    Animation* walk = createAnimateWithFileNames("HeroRun%d.png", 11);
    walk->setDelayPerUnit(0.1);
    auto action = Animate::create(walk);
    
    float distance = sqrtf((destination.x - rootObj->getPositionX()) * (destination.x - rootObj->getPositionX()) +
                           (destination.y - rootObj->getPositionY()) * (destination.y - rootObj->getPositionY()));
    float dur = distance / speed;
    
    auto moveTo = MoveTo::create(dur, destination);
    rootObj->runAction(RepeatForever::create(action));
    rootObj->runAction(CCSequence::create(moveTo, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
    
    //target->runAction(Follow::create(rootObj));
}

void ObjHero::actionRusn(){
    
    assert(rootObj);
    rootObj->stopAllActions();
    
    int flag = 1;
    if(dir == EDIR_FORWARD) flag = 1;
    else flag = -1;
    int distance = 40;
    
    Point p = ccpAdd(rootObj->getPosition(), ccp(distance * flag, 0));
    
    Animation* rush = createAnimateWithFileNames("HeroAttackT%d.png", 3);
    rush->setDelayPerUnit(0.1);
    auto actionRush = Animate::create(rush);
    
    rootObj->runAction(actionRush);
    
    MoveTo* moveTo = MoveTo::create(0.2, getPointInMap(p));
    
    rootObj->runAction(CCSequence::create(moveTo, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
}

void ObjHero::actionAttack(){
    assert(rootObj);
    rootObj->stopAllActions();
    Animation* attack = createAnimateWithFileNames("HeroAttack%d.png", 5);
    attack->setDelayPerUnit(0.1);
    auto action = Animate::create(attack);
    
    rootObj->runAction(CCSequence::create(action, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
}