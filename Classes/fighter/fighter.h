//
//  fighter.h
//  Card
//
//  Created by cyj on 3/1/14.
//
//

#ifndef __Card__fighter__
#define __Card__fighter__

#include "GleeCommon.h"
#include "CardCommon.h"
#include "QuadTree.h"
using namespace GLEE;

#include "hero.h"
#include "monster.h"

#include "elf.h"
USING_NS_ELF;
#include "GeometricRecognizer.h"
#include "ODClient.h"
using namespace DollarRecognizer;
//#include "SwipeGestureRecognizer.h"

//enum Edirection
//{
//    forward,
//    backward
//};

//
// 数据结构
//
#define OBJ_FIG '\\'          // 转义字符
#define OBJ_SP '-'           // 分隔符
#define OBJ_MSGSIZE 128     // 信息说明长度
#define OBJ_JSONSIZE 1024   // 数据长度
#define OBJ_MAXSIZE 1160       // 消息最大长度
/// 数据类型
#define OBJ_TYPE_GPOINT 0

/**
 * 消息结构
 */
struct OBJ_Msg {
    bool success;
    char msg[OBJ_MSGSIZE];
    int type;
    char* jsondata;
};

/**
 * 游戏坐标数据
 */
struct OBJ_GPoint {
    float x;
    float y;
};

class CBattleScene : public GleeController
{
public:
    CBattleScene();
    ~CBattleScene();
    
    virtual void onInit();
    virtual void onEnter();
    virtual void onInTransitionStart();
    virtual void onInTransitionEnd();
    virtual void onBack();
    virtual void onLeave();
    virtual void onOutTransitionStart();
    virtual void onOutTransitionEnd();
    virtual void onRelease();
    virtual void didReceiveMemoryWarning();
    virtual bool isKeepAlive();
    
    virtual void setEnabled(bool enabled);
    
    static GleeController * create() {
        return new CBattleScene();
    };
};

class LCBattleScene : public GleeCCLayer
{
public:
    LCBattleScene();
    ~LCBattleScene();
    
    virtual bool init();
    
    static GleeCCLayer * create() {
        LCBattleScene * ret = new LCBattleScene();
        ret->init();
        ret->autorelease();
        return ret;
    };
    
    virtual void setLayerEnabled(bool enabled);
    virtual void updateCCLayer(GleeObject* data);
    virtual void clickListener(CCObject * object);
    virtual void initView();
    virtual CCFiniteTimeAction* getInTransition();
    virtual CCFiniteTimeAction* getOutTransition();
    
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    
private:
    void backClicked();
    
public:
    //void createTeam();
    void createHero();
//    
//    void excuteAction(CCAction* action);
//    
//    void actionWalk(CCPoint destination);
//    
//    void actionJump();
//    
//    void actionRusn(Edirection opDir);
//    
//    void actionAttack();
//    
//    void actionStand();
//    
//    void hideHeroAction();
//    
//    void turnAround();
    
    CCPoint getPointInMap(CCPoint pos);
    
    void testODClient();
    
    void sendPosition(CCPoint pos);
    
private:
    void addMonster(int amount = 1);
    
    void followHero();
    
    CCArray* monsterArr;
    
private:
    NodeSet * _set;
    ElfXMLDocument * _document;
    
    //@@@@[[[[
    //// [[[[ auto declared at Fri Jan 17 14:52:52 GMT+08:00 2014
	ElfNode * _root;
//    ElfNode* hero;
//    SimpleAnimateNode* stand;
//    SimpleAnimateNode* walk;
//    SimpleAnimateNode* jump;
//    SimpleAnimateNode* attack;
//    SimpleAnimateNode* rush;
	ElfNode * backGround;
    ElfNode * activeRange;
    
    Edirection dir;
    
    GeometricRecognizer* test;
    Path2D mPath;
    
    bool moved;
    
    ODClient* con;
    //CCSize mapSize;
    
    virtual void update(float duration);
    
public:
    ObjHero* hero;
    
private:
    QuadTree* tree;
};

LCBattleScene* l;

#endif /* defined(__Card__fighter__) */
