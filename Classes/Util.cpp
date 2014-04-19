//
//  Util.cpp
//  Fighter
//
//  Created by cyj on 3/22/14.
//
//

#include "Util.h"

USING_NS_CC;

static Util *_util = nullptr;

Size Util::getWinsize(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return visibleSize;
}

Point Util::getCenterPos(){
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Point origin = Director::getInstance()->getVisibleOrigin();
//    Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//          origin.y + closeItem->getContentSize().height/2)
}

Util* Util::getInstance()
{
    if(!_util)
    {
        _util = new Util();
    }
    return _util;
}

void Util::destroyInstance()
{
    if (_util)
    {
        delete _util;
        _util = nullptr;
    }
}

bool Util::getCosTheta(Point origin, Point p){
    
    if(p.x > origin.x) return 1.0f / sqrtf(2.0f);
    else return -1.0f / sqrtf(2.0f);
}

// Bit trick
bool Util::IsPointInCircularSector3(float cx, float cy, float ux, float uy, float squaredR, float cosTheta,
                                       float px, float py)
{
    assert(cosTheta > -1 && cosTheta < 1);
    assert(squaredR > 0.0f);
    
    // D = P - C
    float dx = px - cx;
    float dy = py - cy;
    
    // |D|^2 = (dx^2 + dy^2)
    float squaredLength = dx * dx + dy * dy;
    
    // |D|^2 > r^2
    if (squaredLength > squaredR)
        return false;
    
    // D dot U
    float DdotU = dx * ux + dy * uy;
    
    // D dot U > |D| cos(theta)
    // <=>
    // (D dot U)^2 > |D|^2 (cos(theta))^2 if D dot U >= 0 and cos(theta) >= 0
    // (D dot U)^2 < |D|^2 (cos(theta))^2 if D dot U <  0 and cos(theta) <  0
    // true                               if D dot U >= 0 and cos(theta) <  0
    // false                              if D dot U <  0 and cos(theta) >= 0
    const unsigned cSignMask = 0x80000000;
    union {
        float f;
        unsigned u;
    }a, b, lhs, rhs;
    a.f = DdotU;
    b.f = cosTheta;
    unsigned asign = a.u & cSignMask;
    unsigned bsign = b.u & cSignMask;
    if (asign == bsign) {
        lhs.f = DdotU * DdotU;
        rhs.f = squaredLength * cosTheta * cosTheta;
        lhs.u |= asign;
        rhs.u |= asign;
        return lhs.f > rhs.f;
    }
    else
        return asign == 0;
}
