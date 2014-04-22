//
//  Util.h
//  Fighter
//
//  Created by cyj on 3/22/14.
//
//

#ifndef __Fighter__Util__
#define __Fighter__Util__

#include "cocos2d.h"
USING_NS_CC;

class Util //: public cocos2d::Layer
{
public:
    Util() {}
    ~Util() {}
    
    Size getWinsize();
    
    static Point getCenterPos();
    
    static Util* _util;
    
    static Util* getInstance();
    void destroyInstance();
    
    static bool IsPointInCircularSector3(float cx, float cy, float ux, float uy, float squaredR, float cosTheta,
                                         float px, float py);
    
    static bool getCosTheta(Point origin, Point p);
    
    //static Util* instance;
};

enum E_tag
{
    ETagMonster,
    ETagHero
};


#endif /* defined(__Fighter__Util__) */
