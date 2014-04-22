//
//  fighter.h
//  Card
//
//  Created by cyj on 3/1/14.
//
//

#ifndef __Card__fighter__
#define __Card__fighter__

#include "cocos2d.h"
USING_NS_CC;

#include "hero.h"
#include "monster.h"
#include "QuadTree.h"

class LCBattleScene : public Layer
{
public:
    LCBattleScene();
    ~LCBattleScene();
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void initView();
    
    void onMouseDown(Event* event);
    void onMouseUp(Event* event);
    void onMouseMove(Event* event);
    void onMouseScroll(Event* event);
    
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    virtual void update(float duration);
    
public:
    void addMonsterAtPosition(cocos2d::Point p);
    
    void test(float d);

private:
    void backClicked();
    
public:
    //void createTeam();
    void createHero();
    
    Point getPointInMap(Point pos);
    
    void testODClient();
    
    void sendPosition(Point pos);
    
    void caculateCollitionPerSec(float dur);
    
private:
    void addMonster(int amount = 1);
    
    void followHero(float duration);
    
    __Array* monsterArr;
    
    void delteMonster(Ref* obj);
    
private:
	Sprite * backGround;
    //Sprite * activeRange;
    
    //Edirection dir;
    
    bool moved;
    
    EventListenerMouse* _mouseListener;
    EventListenerKeyboard* _keyboardListener;
    
public:
    ObjHero* hero;
private:
    CREATE_FUNC(LCBattleScene);
};

#endif /* defined(__Card__fighter__) */
