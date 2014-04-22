//
//  GameOver.h
//  Fighter
//
//  Created by cyj on 4/21/14.
//
//

#ifndef __Fighter__GameOver__
#define __Fighter__GameOver__
#include "cocos2d.h"
USING_NS_CC;

//#include "hero.h"
//#include "monster.h"
//#include "QuadTree.h"

class LGameOver : public Layer
{
public:
    LGameOver(){};
    ~LGameOver(){};
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void exitClicked(Ref* obj);
    
    CREATE_FUNC(LGameOver);
};
#endif /* defined(__Fighter__GameOver__) */
