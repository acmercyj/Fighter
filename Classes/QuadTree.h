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

#include <cocos2d.h>
USING_NS_CC;
//#include "GleeCommon.h"
//#include "CardCommon.h"
//using namespace GLEE;


class QuadTree {
public:
    QuadTree(int deep,Rect rect);
    ~QuadTree();
    bool addObject(Node *node);//build tree
    void clear();
    void getCollisionObjects(Node *node, __Array *result);
private:
//    void split();
    void initChild(int deep, Rect rect);
    void deleteTree();
    void clearTree(QuadTree *tree);
    bool isRecAContainsRecB(Rect recA, Rect recB);
    
    __Array *mObjArray;
    QuadTree *mChild[BRANCH];
    Rect mRect;
};

#endif /* defined(__Card__QuadTree__) */
