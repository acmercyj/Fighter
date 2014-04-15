//
//  lifeObj.h
//  Card
//
//  Created by cyj on 3/8/14.
//
//

#ifndef __Card__lifeObj__
#define __Card__lifeObj__

#include "cocos2d.h"
USING_NS_CC;

#define HIDE_COLLISION_RECT

enum Edirection
{
    EDIR_FORWARD,
    EDIR_BACKWARD
};

enum class EobjState{
    E_STAND,
    E_ATTACK,
    E_WALK,
    E_HURT,
    E_RUSH
};

class dataModel : public Ref
{
    dataModel(){};
    ~dataModel(){};
};

const int _hardDir[2][4] = {1, 1, -1, -1, -1, 1, 1, -1};

class baseObj
{
public:
    baseObj() {}
    ~baseObj(){
    }
public:
    virtual void createBaseObj(const char* file, const char* name){
    }
    
    virtual void attack() {};
    
    virtual void attacked(float atkVal) {
        if(invincible) return;
        hp -= (atkVal - defense);
        if(hp <= 0) die();
    }
    
    virtual void die(){};
    
protected:
    CC_SYNTHESIZE(float, hp, HP);
    // 魔法值
    CC_SYNTHESIZE(float, mana, Mana);
    CC_SYNTHESIZE(float, atk, ATK);
    CC_SYNTHESIZE(float, defense, Defense);
    CC_SYNTHESIZE(float, dodge, Dodge);
    CC_SYNTHESIZE(float, speed, Speed);
    // 无敌的
    CC_SYNTHESIZE(bool, invincible, Invincible);
};

class lifeObj : public Sprite , public baseObj
{
public:
    lifeObj() : objState(EobjState::E_STAND){
        __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(lifeObj::die), "die", NULL);
    }
    ~lifeObj() {
        CC_SAFE_RELEASE_NULL(rootObj);
    }
public:
    
    void createLifeObj(const char* file, const char* name);
    
    virtual void actionWalk(Point destination){};
    
    void actionJump();
    
    //void actionAttack();
    
    virtual void actionStand() = 0;
    
    virtual void turnAround();
    
    virtual void actionAttack() {};
    
    void moveAway(Rect rect);
    
    virtual void hurt(float deltaAngle = 15, float hp = 10);
    
    virtual void die(Ref* data);
    
    Animation* createAnimateWithFileNames(const char* str, int amount);
    
    virtual Rect getShadowRect() = 0;
    
    Point getPointInMap(Point pos);
    
    bool IsPointInCircularSector3(float cx, float cy, float ux, float uy, float squaredR, float cosTheta,
                                  float px, float py);
    
    //virtual void setCollisionRectVisible(bool visible){}
    
    virtual Point getKeyPoint(Point exP) {
        
        Point lp = Point(keyPoint_l.x + rootObj->getPositionX(), rootObj->getPositionY() + rootObj->getContentSize().height / 2);
        //map->convertToNodeSpace(keyPoint_l);
        Point rp = Point(keyPoint_l.x + rootObj->getPositionX(), rootObj->getPositionY() + rootObj->getContentSize().height / 2);
        //map->convertToNodeSpace(keyPoint_r);
        Point p = exP.x > rp.x ? rp : lp;
        return p;
    }
    
public:
    virtual void onExit();
    
public:
    static void setActiveRange(Size range){
        lifeObj::activeRange = range;
    }
    
    static void setMap(Sprite* spMap){
        lifeObj::map = spMap;
    }
protected:
    Edirection dir;
    
    CC_SYNTHESIZE(EobjState, objState, State);
    
    CC_SYNTHESIZE(Sprite*, shadow, Shadow);
    CC_SYNTHESIZE(Sprite*, rootObj, rootObj);
    CC_SYNTHESIZE(ProgressTimer*, hpProgress, hpBar);
    //CC_SYNTHESIZE(Size, activeRange, ActiveRange);
    
    static Size activeRange;
    //CC_SYNTHESIZE(int, dirIndex, DirIndex);
    
    static Sprite* map;
    
    CC_SYNTHESIZE(Point, keyPoint_l, KeyPoint_l);
    CC_SYNTHESIZE(Point, keyPoint_r, KeyPoint_r)
};

#endif /* defined(__Card__lifeObj__) */
