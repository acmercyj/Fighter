//
//  b2test.h
//  Fighter
//
//  Created by cyj on 3/30/14.
//
//

#ifndef __Fighter__b2test__
#define __Fighter__b2test__

#include "cocos2d.h"
#include "VisibleRect.h"

USING_NS_CC;

class Test : public b2ContactListener
{
public:
    
	Test();
	virtual ~Test();
    
    void DrawTitle(const char *string);
	virtual void Step(Settings* settings);
	virtual void Keyboard(unsigned char key) { B2_NOT_USED(key); }
	virtual void KeyboardUp(unsigned char key) { B2_NOT_USED(key); }
	void ShiftMouseDown(const b2Vec2& p);
	virtual bool MouseDown(const b2Vec2& p);
	virtual void MouseUp(const b2Vec2& p);
	void MouseMove(const b2Vec2& p);
	void LaunchBomb();
	void LaunchBomb(const b2Vec2& position, const b2Vec2& velocity);
	
	void SpawnBomb(const b2Vec2& worldPt);
	void CompleteBombSpawn(const b2Vec2& p);
    
	// Let derived tests know that a joint was destroyed.
	virtual void JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }
    
	// Callbacks for derived classes.
	virtual void BeginContact(b2Contact* contact) { B2_NOT_USED(contact); }
	virtual void EndContact(b2Contact* contact) { B2_NOT_USED(contact); }
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}
    
	void ShiftOrigin(const b2Vec2& newOrigin);
    
protected:
	friend class DestructionListener;
	friend class BoundaryListener;
	friend class ContactListener;
    friend class Box2DView;
    
	b2Body* m_groundBody;
	b2AABB m_worldAABB;
	ContactPoint m_points[k_maxContactPoints];
	int32 m_pointCount;
	DestructionListener m_destructionListener;
	GLESDebugDraw m_debugDraw;
	int32 m_textLine;
	b2World* m_world;
	b2Body* m_bomb;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_bombSpawnPoint;
	bool m_bombSpawning;
	b2Vec2 m_mouseWorld;
	int32 m_stepCount;
    
	b2Profile m_maxProfile;
	b2Profile m_totalProfile;
};

class Box2DView : public Layer
{
    EventListenerTouchOneByOne* _touchListener;
    TestEntry*    m_entry;
    Test*        m_test;
    int            m_entryID;
public:
    Box2DView(void);
    virtual ~Box2DView(void);
    
    bool initWithEntryID(int entryId);
    std::string title() const;
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    
    //    virtual void registerWithTouchDispatcher();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    //virtual void accelerometer(UIAccelerometer* accelerometer, Acceleration* acceleration);
    
    static Box2DView* viewWithEntryID(int entryId);
protected:
    void onDraw(const kmMat4 &transform, bool transformUpdated);
    
    CustomCommand _customCmd;
};

#endif /* defined(__Fighter__b2test__) */
