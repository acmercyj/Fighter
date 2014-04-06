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
    return hero;
    //actionStand();
}

void ObjHero::actionStand(){
    assert(rootObj);
    rootObj->stopAllActions();
    Animation* stand = createAnimateWithFileNames("Hero%d.png", 3);
    auto action = Animate::create(stand);
    rootObj->runAction(RepeatForever::create(action));
}

void ObjHero::actionWalk(Node* target, Point destination){
    
    assert(rootObj);
    rootObj->stopAllActions();
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

void ObjHero::actionAttack(){
    assert(rootObj);
    rootObj->stopAllActions();
    Animation* attack = createAnimateWithFileNames("HeroAttack%d.png", 5);
    attack->setDelayPerUnit(0.1);
    auto action = Animate::create(attack);
    
    rootObj->runAction(CCSequence::create(action, CallFunc::create( CC_CALLBACK_0(ObjHero::actionStand,this)), NULL));
    
}