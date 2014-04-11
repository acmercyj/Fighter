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
    E_HURT,
    E_RUSH
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
    lifeObj() : objState(EobjState::E_STAND){}
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
    
    void moveAway(Rect rect);
    
    virtual void hurt(float deltaAngle = 15, float hp = 10);
    
    virtual void die(int flag = 1);
    
    Animation* createAnimateWithFileNames(const char* str, int amount);
    
    virtual Rect getShadowRect() = 0;
    
    Point getPointInMap(Point pos);
    
    //virtual void setCollisionRectVisible(bool visible){}
    
public:
    void setActiveRange(Size range){
        lifeObj::activeRange = range;
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
};

#endif /* defined(__Card__lifeObj__) */
