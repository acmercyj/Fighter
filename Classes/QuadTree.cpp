//
//  QuadTree.cpp
//  Card
//
//  Created by Roton_Lin on 14-3-20.
//
//

#include "QuadTree.h"
#include "lifeObj.h"

//build the tree
QuadTree::QuadTree(int deep, Rect rect) {
    mRect = rect;
    mObjArray = CCArray::create();
    mObjArray->retain();
    initChild(deep, rect);
}

QuadTree::~QuadTree() {
    mObjArray->removeAllObjects();
    mObjArray->release();
    deleteTree();
}

void QuadTree::deleteTree() {
    for (int i = 0; i < BRANCH; i++) {
        if (mChild[i] != NULL) {
            delete mChild[i];
        }
    }
}

void QuadTree::initChild(int deep, Rect rect) {
    for (int i = 0; i < BRANCH; i ++) {
        mChild[i] = NULL;
    }
    if (deep == 1) {
        return;
    }
    float width = rect.size.width / 2;
    float height = rect.size.height / 2;
    int curdeep = deep - 1;
    mChild[0] = new QuadTree(curdeep, Rect(rect.origin.x, rect.origin.y, width, height));
    mChild[1] = new QuadTree(curdeep, Rect(rect.origin.x + width, rect.origin.y, width, height));
    mChild[2] = new QuadTree(curdeep, Rect(rect.origin.x, rect.origin.y + height, width, height));
    mChild[3] = new QuadTree(curdeep, Rect(rect.origin.x + width, rect.origin.y + height, width, height));
}

void QuadTree::clear() {
    clearTree(this);
}

void QuadTree::clearTree(QuadTree *tree) {
    for (int i = 0; i < BRANCH; i ++) {
        if (tree->mChild[i] != NULL) {
            clearTree(tree->mChild[i]);
        }
    }
    (tree->mObjArray)->removeAllObjects();
}

bool QuadTree::addObject(Node* node) {
    lifeObj* obj = (lifeObj*)node;
    Rect rec = obj->getrootObj()->boundingBox();
    bool isInsert = false;
    if (isRecAContainsRecB(mRect, rec)) {
        for (int i = 0 ; i < BRANCH; i++) {
            if (mChild[i] == NULL) {
                break;
            }
            isInsert |= (mChild[i]->addObject(node));
        }
        if (!isInsert) {
            mObjArray->addObject(node);
            isInsert = true;
        }
    }
    return isInsert;
}

bool QuadTree::isRecAContainsRecB(Rect recA, Rect recB) {
    bool withFit = recB.origin.x + recB.size.width <= recA.origin.x + recA.size.width;
    bool heightFit = recB.origin.y + recB.size.height <= recA.origin.y + recA.size.height;
    if (withFit && heightFit && recA.origin.x <= recB.origin.x && recB.origin.y >= recA.origin.y) {
        return true;
    }
    return false;
}

void QuadTree::getCollisionObjects(Node *node, __Array *result) {
    lifeObj* obj = (lifeObj*)node;
    Rect rec = obj->getrootObj()->boundingBox();
    //CCRect rec = node->boundingBox();
    if (isRecAContainsRecB(mRect, rec)) {
        result->addObjectsFromArray(mObjArray);
        for (int i = 0; i < BRANCH; i++) {
            if (mChild[i] == NULL) {
                break;
            }
            mChild[i]->getCollisionObjects(node, result);
        }
    }
}
