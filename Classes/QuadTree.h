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
#include <CCNotificationCenter.h>
USING_NS_CC;
//#include "GleeCommon.h"
//#include "CardCommon.h"
//using namespace GLEE;


class QuadTree : public Object{
public:
    QuadTree(int deep,Rect rect);
    ~QuadTree();
    bool addObject(Node *node);//build tree
    void clear();
    void getCollisionObjects(Node *node, __Array *result);
    static QuadTree *sharedQuadTree();
    private:
    void initChild(int deep, Rect rect);
    void deleteTree();
    void clearTree(QuadTree *tree);
    bool isRecAContainsRecB(Rect recA, Rect recB);
    void onObjectMoved(Object *obj);
    
    static QuadTree *m_Instance;
    __Array *mObjArray;
    QuadTree *mChild[BRANCH];
    Rect mRect;
};

#endif /* defined(__Card__QuadTree__) */
