//
//  lifeObj.cpp
//  Card
//
//  Created by cyj on 3/8/14.
//
//

#include "lifeObj.h"
#include "QuadTree.h"
#include "Util.h"
Size lifeObj::activeRange = Size(0, 0);

Sprite* lifeObj::map = NULL;

void lifeObj::createLifeObj(const char* file, const char* name){
    createBaseObj(file, name);
}

void lifeObj::turnAround(){
    if(dir == EDIR_FORWARD){
        rootObj->setScaleX(-1);
        dir = EDIR_BACKWARD;
    }else{
        rootObj->setScaleX(1);
        dir = EDIR_FORWARD;
    }
}

void lifeObj::actionJump(){
    //jump->setVisible(true);
    
    //int distance = 40;
    //Point origin = rootObj->getPosition();
    
    //JumpTo* jumpTo = JumpTo::cre
    //JumpBy* jumpBy = JumpBy::create(0.3, rootObj->getPosition(), distance, 1);
    //Point destination = pAdd(rootObj->getPosition(), p(0, distance));
    //MoveTo* moveTo = MoveTo::create(0.2, destination);
    //MoveTo* moveBack = MoveTo::create(0.1, origin);
    //Sequence* seq = Sequence::create(jumpBy, CallFunc::create(this, callfunc_selector(lifeObj::actionStand)), NULL);
    
    
}

void lifeObj::moveAway(Rect rect){
    Point pt = getrootObj()->getPosition();
    
    Point ot = Point(rect.origin.x + rect.size.width / 2.0f, rect.origin.y + rect.size.height / 2.0f);
    Rect selfRec = getShadowRect();
 //   Point pt = selfRec.origin;
//    float lx = rect.size.width / 2 + selfRec.size.width / 2;
//    float ly = rect.size.height / 2 + selfRec.size.width / 2;
//    
//    float dx = selfRec.origin.x - rect.origin.x;
//    //dx = dx < 0 ? -dx : dx;
//    float dy = selfRec.origin.y - rect.origin.y;
//    //dy = dy < 0 ? -dy : dy;
//    
//    Point des = Point(dx - absf rect.origin.x - selfRec.origin.x)
//    
//    Point selfLeftB = Point(selfRec.origin.x - selfRec.size.width / 2, selfRec.origin.y - selfRec.size.height / 2);
    
    if (pt.x > rect.origin.x) {
        pt.x = rect.origin.x + rect.size.width + selfRec.size.width / 2.0f;
    } else if(pt.x < rect.origin.x){
        pt.x = rect.origin.x - rect.size.width - selfRec.size.width / 2.0f;
    }
    if (pt.y > rect.origin.y) {
        pt.y = rect.origin.y + rect.size.height + selfRec.size.height / 2.0f;
    } else if(pt.y < rect.origin.y){
        pt.y = rect.origin.y - rect.size.height - selfRec.size.height / 2.0f;
    }
    
    Point des = getPointInMap(pt);
    
    this->getrootObj()->setPosition(des);
    //CCLOG("%f %f",pt.x, pt.y);
    //CCLOG("%f %f",rect.origin.x, rect.origin.y);
}

void lifeObj::hurt(float deltaAngle, float hp){
    
    //setState(EobjState::E_HURT);
    //RotateTo* rotateBy_1 = RotateTo::create(0.2, deltaAngle);
    //RotateTo* rotateBy_2 = RotateTo::create(0.2, 0);
    //Sequence* seq = Sequence::create(rotateBy_1, rotateBy_2, NULL);
    //Repeat* repeat = Repeat::create(seq, 1);
    
    //float te = hpProgress->getPercentage();
    float totalHp = (1 + 1 - hpProgress->getPercentage() / 100) * this->hp;
    //ProgressTo* progressTo = ProgressTo::create(0.2, (this->hp - hp) / totalHp * 100);
    this->hp -= hp;
    //hpProgress->runAction(progressTo);
    
    hpProgress->setPercentage((this->hp - hp) / totalHp * 100);
    
    if(getHP() <= 0){
        die(NULL);
        return;
    }
    //rootObj->stopAllActions();
    //rootObj->runAction(CCSequence::create(seq, CallFunc::create( CC_CALLBACK_0(lifeObj::actionStand,this)), NULL));
}

void lifeObj::removeRootObj(){
    __NotificationCenter::getInstance()->postNotification("deleteObj", this);
    if(rootObj) rootObj->removeFromParentAndCleanup(true);
}

void lifeObj::die(Ref* data){
    if(getState() == EobjState::E_DEAD) return;
    setState(EobjState::E_DEAD);
    int flag = 1;
    rootObj->stopAllActions();
    
    SkewTo* sk = SkewTo::create(0.2, 0, 10);
    
    RotateTo* rotateTo = RotateTo::create(0.2, 90 * flag);
    
    Spawn* spawn = Spawn::create(sk, rotateTo, NULL);
    
    hpProgress->setPercentage(0);
    
    rootObj->runAction(CCSequence::create(spawn, DelayTime::create(2.0f), CallFunc::create( CC_CALLBACK_0(lifeObj::removeRootObj,this)), NULL));
}

Point lifeObj::getPointInMap(Point pos){
    float minX = 0;
    float maxX = activeRange.width;
    
    float minY = 0;
    float maxY = activeRange.height / 2;
    
    pos.x = pos.x > maxX ? maxX : pos.x;
    pos.x = pos.x < minX ? minX : pos.x;
    
    pos.y = pos.y > maxY ? maxY : pos.y;
    pos.y = pos.y < minY ? minY : pos.y;
    
    return  pos;
}

Animation* lifeObj::createAnimateWithFileNames(const char* str, int amount){
    Vector<SpriteFrame*> frames;
    for(auto i = 1; i <= amount; ++i){
        __String* fileName = String::createWithFormat(str, i);
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(fileName->getCString());
        if (texture)
        {
            Rect rect = Rect::ZERO;
            rect.size = texture->getContentSize();
            SpriteFrame* frame = SpriteFrame::create(fileName->getCString(), rect);
            frames.pushBack(frame);
        }
    }
    Animation* animate = Animation::createWithSpriteFrames(frames, 0.2);
    
    return animate;
}

//void lifeObj::onExit(){
//    __NotificationCenter::getInstance()->removeAllObservers(this);
//}

void lifeObj::attackEffect(){
    Point centerP = Point(rootObj->getPositionX(), rootObj->getPositionY() + rootObj->getContentSize().height / 2);
    getDebugLabel()->setPosition(centerP);
    float CR = rootObj->getContentSize().width / 2 + 100;
    Point up = Point(1.0f, 0);
    float cosTheta = 1.0f / sqrtf(2.0f);
    
    __Array* objList = __Array::create();
    QuadTree::getInstance()->getCollisionObjects(this, objList);
    for(int i = 0; i < objList->count(); ++i){
        lifeObj* obj = dynamic_cast<lifeObj*>(objList->getObjectAtIndex(i));
        if(obj->getObjType() == getObjType() || obj->getState() == EobjState::E_HURT) continue;
        Point kp = obj->getKeyPoint(rootObj->getPosition());
        if(kp.x < centerP.x) up.x = -1;
        obj->getDebugLabel()->setPosition(kp);
        
        Rect rec = obj->getShadowRect();
        Rect rec_1 = getShadowRect();
            
        if(rec.intersectsRect(rec_1) || Util::IsPointInCircularSector3(centerP.x, centerP.y, up.x, up.y, CR * CR, cosTheta,
                                    kp.x, kp.y)){
            obj->hurt(getATK());
            CCLOG("hit!!");
        }
    }
}

