//
//  monster.h
//  Card
//
//  Created by cyj on 3/9/14.
//
//

#ifndef __Card__monster__
#define __Card__monster__

#include "lifeObj.h"

class ObjMonster : public lifeObj
{
public:
    ObjMonster() : id(0){}
    ~ObjMonster() {}
    
    static float speed;
    static ObjMonster* create(Node* target, Point pos, int id);
    
    virtual void actionStand();
    
    virtual void actionWalk(Point destination);
    
    void actionAttack();
    
    virtual Rect getShadowRect();
    
    CC_SYNTHESIZE(int, id, ID);
};

#endif /* defined(__Card__monster__) */
