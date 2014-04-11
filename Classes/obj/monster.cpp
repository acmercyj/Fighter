//
//  monster.cpp
//  Card
//
//  Created by cyj on 3/9/14.
//
//

#include "monster.h"

float ObjMonster::speed = 230;

ObjMonster* ObjMonster::create(Node* target, Point pos, int id){
    assert(target);
    if(!target) return NULL;
    ObjMonster* monster = new ObjMonster();
    
    monster->rootObj = Sprite::create("Robot1.png");
    //monster->rootObj->setContentSize(Size(100, 100));
    monster->rootObj->retain();
    monster->rootObj->setAnchorPoint(Point(0.5, 0));
    target->addChild(monster->rootObj);
    monster->rootObj->setPosition(pos);
    
    monster->setHP(100);
    monster->autorelease();
    
    Sprite* sp = Sprite::create("RobotState1.png");
    monster->hpProgress = ProgressTimer::create(sp);
    monster->hpProgress->setType(ProgressTimer::Type::BAR);
    monster->hpProgress->setMidpoint(Point(0, 0.5));
    monster->hpProgress->setPosition(Point(monster->getrootObj()->getContentSize().width / 2, monster->getrootObj()->getContentSize().height - 30));
    monster->hpProgress->setPercentage(100);
    monster->getrootObj()->addChild(monster->hpProgress);
    
    //Label* l = Label::create(String::createWithFormat("%d", id)->getCString(), "fzcy.ttf", 30);
    //l->setString(String::createWithFormat("%d", id)->getCString());
    //l->setPosition(Point(0, 200));
    //monster->getrootObj()->addChild(l);
#ifndef HIDE_COLLISION_RECT
    monster->shadow = Sprite::create();
    target->addChild(monster->shadow);
#endif
    return monster;
}

void ObjMonster::actionStand(){
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_STAND);
    Animation* stand = createAnimateWithFileNames("Robot%d.png", 3);
    auto action = Animate::create(stand);
    rootObj->runAction(RepeatForever::create(action));
}

void ObjMonster::turnAround(){
    if(dir == EDIR_FORWARD){
        rootObj->setScaleX(1);
        dir = EDIR_BACKWARD;
    }else{
        rootObj->setScaleX(-1);
        dir = EDIR_FORWARD;
    }
}

Rect ObjMonster::getShadowRect(){
    Rect rec = Rect(rootObj->getPositionX() - rootObj->getContentSize().width / 2.0f + 100, rootObj->getPositionY(), 75, 28);
    
#ifndef HIDE_COLLISION_RECT
    shadow->setTextureRect(rec);
    shadow->setPosition(Point(rec.origin.x + rec.size.width / 2.0f, rec.origin.y + rec.size.height / 2.0f));
#endif
    
    return rec;//Rect(rootObj->getPositionX() + rootObj->getContentSize().width / 2 + 20, rootObj->getPositionY(), 75, 28);
}

void ObjMonster::actionWalk(Point des){
    
    Point destination = getPointInMap(des);
    
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_WALK);
    if((destination.x < rootObj->getPosition().x && dir == EDIR_FORWARD) || (destination.x > rootObj->getPosition().x && dir == EDIR_BACKWARD)){
        turnAround();
    }
    Animation* walk = createAnimateWithFileNames("RobotRun%d.png", 6);
    walk->setDelayPerUnit(0.1);
    auto action = Animate::create(walk);
    
    float distance = sqrtf((destination.x - rootObj->getPositionX()) * (destination.x - rootObj->getPositionX()) +
                           (destination.y - rootObj->getPositionY()) * (destination.y - rootObj->getPositionY()));
    float dur = distance / speed;
    
    auto moveTo = MoveTo::create(dur, destination);
    rootObj->runAction(RepeatForever::create(action));
    rootObj->runAction(CCSequence::create(moveTo, CallFunc::create( CC_CALLBACK_0(ObjMonster::actionStand,this)), NULL));
    
    //target->runAction(Follow::create(rootObj));
}

void ObjMonster::actionAttack(){
    if(getState() == EobjState::E_ATTACK) return;
    assert(rootObj);
    rootObj->stopAllActions();
    setState(EobjState::E_ATTACK);
    Animation* attack = createAnimateWithFileNames("RobotAttack%d.png", 5);
    attack->setDelayPerUnit(0.1);
    auto action = Animate::create(attack);
    
    rootObj->runAction(CCSequence::create(action, CallFunc::create( CC_CALLBACK_0(ObjMonster::actionStand,this)), NULL));
}