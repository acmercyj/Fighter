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
