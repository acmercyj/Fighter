//
//  GameOver.cpp
//  Fighter
//
//  Created by cyj on 4/21/14.
//
//

#include "GameOver.h"
#include "Util.h"

bool LGameOver::init()
{
    if(!Layer::init()) {
        return false;
    }
    Label* l = Label::create("100", "fzcy.ttf", 50);
    l->setString("Game Over");
    l->setPosition(Util::getCenterPos());
    l->setAnchorPoint(Point(0.5, 0.5));
    this->addChild(l);
    
    MenuItemFont *pCloseItem = MenuItemFont::create("exit",
                                                          this,
                                                          menu_selector(LGameOver::exitClicked) );
    
    Size size = Director::getInstance()->getVisibleSize();
    Point p = Point(size.width - pCloseItem->getContentSize().width, pCloseItem->getContentSize().height / 2);
    pCloseItem->setPosition(p);
    
    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, 1);
    
    //Director::getInstance()->replaceScene();
    
    return true;
}

Scene* LGameOver::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LGameOver::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void LGameOver::exitClicked(Ref* obj){
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}