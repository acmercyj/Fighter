//
//  QuadTree.h
//  Card
//
//  Created by Roton_Lin on 14-3-20.
//
//

#ifndef __Card__QuadTree__
#define __Card__QuadTree__

#define BRANCH 4

//#include <cocos2d.h>
//USING_NS_CC;
#include "GleeCommon.h"
#include "CardCommon.h"
using namespace GLEE;


class QuadTree {
public:
    QuadTree(int deep,CCRect rect);
    ~QuadTree();
    bool addObject(CCNode *node);//build tree
    void clear();
    void getCollisionObjects(CCNode *node, CCArray *result);
private:
//    void split();
    void initChild(int deep, CCRect rect);
    void deleteTree();
    void clearTree(QuadTree *tree);
    bool isRecAContainsRecB(CCRect recA, CCRect recB);
    
    CCArray *mObjArray;
    QuadTree *mChild[BRANCH];
    CCRect mRect;
};

#endif /* defined(__Card__QuadTree__) */
