//
//  fighter.cpp
//  Card
//
//  Created by cyj on 3/1/14.
//
//

#include "fighter.h"
//#include "IOSRootContrller.h"

//------------------------------------------ controler -----------------------------------
bool isCBattleSceneregister = GleeCore::registerController("CBattleScene", CBattleScene::create);

CBattleScene::CBattleScene()
{
    
}

CBattleScene::~CBattleScene()
{
    
}

void CBattleScene::onInit()
{
    //BottomLayer::shared()->updateMenu(NULL);
}

void CBattleScene::onEnter()
{
    GleeCCLayer* layer = (GleeCCLayer *)getLayer();
    if(!layer)
    {
        layer = GleeCore::getCCLayer("LCBattleScene");
        setLayer(layer->getCCLayer());
        layer->updateCCLayer(NULL);
    }
}

void CBattleScene::onInTransitionStart()
{
    GleeCCLayer* layer = (GleeCCLayer *)getLayer();
    if(layer) {
        CCFiniteTimeAction* action = layer->getInTransition();;
        if(action) {
            this->setInTransition(action);
        }
    }
}

void CBattleScene::onInTransitionEnd()
{
    
}

void CBattleScene::onBack()
{
    
}

void CBattleScene::onLeave()
{
    GleeCCLayer* layer = (GleeCCLayer *)getLayer();
    layer->setLayerEnabled(false);
}

void CBattleScene::onOutTransitionStart()
{
    GleeCCLayer* layer = (GleeCCLayer *)getLayer();
    if(layer) {
        CCFiniteTimeAction* action = layer->getOutTransition();;
        if(action) {
            this->setOutTransition(action);
        }
    }
}

void CBattleScene::onOutTransitionEnd()
{
    
}

void CBattleScene::onRelease()
{
    
}

void CBattleScene::didReceiveMemoryWarning()
{
    
}

bool CBattleScene::isKeepAlive()
{
    return false;
}

void CBattleScene::setEnabled(bool enabled){
    GleeCCLayer* layer = (GleeCCLayer *)getLayer();
    layer->setLayerEnabled(enabled);
}

//------------------------------------------ layer -------------------------------------
bool isLCBattleSceneregister = GleeCore::registerLayer("LCBattleScene", LCBattleScene::create);

LCBattleScene::LCBattleScene():
_set(NULL),
_document(NULL),
dir(forward),
hero(NULL)
{
    //mapSize = CCSize(600, 250);
    monsterArr = CCArray::create();
    monsterArr->retain();
}

LCBattleScene::~LCBattleScene()
{
    CC_SAFE_RELEASE_NULL(hero);
    CC_SAFE_RELEASE_NULL(_set);
    CC_SAFE_RELEASE_NULL(_document);
    CC_SAFE_RELEASE_NULL(monsterArr);
    //CC_SAFE_RELEASE_NULL(_model);
    //CC_SAFE_RELEASE_NULL(formationArr);
    CC_SAFE_DELETE(tree);
}

void LCBattleScene::setLayerEnabled(bool enabled)
{
    NodeHelper::setTouchable(this, enabled);
}

bool LCBattleScene::init()
{
    if(!GleeCCLayer::init()) {
        return false;
    }
    
    XMLFactory::getInstance()->setZipFilePath(CCString::createWithFormat("%sCBattleScene.cocos.zip", COCOS_ZIP_HEAD_PATH)->getCString());
    _document = XMLFactory::getInstance()->createDocument("CBattleScene.cocos");
    ElfXMLElement * element = XMLFactory::getInstance()->findElementByName(_document, "@root");
    
    _set = XMLFactory::getInstance()->createWithElement(element);
    _set->addOtherNodeSet( XMLFactory::getInstance()->createWithElement( XMLFactory::getInstance()->getRootElement(_document) ) );
    
    //@@@@[[[[
    //// [[[[ auto assigned at Fri Jan 17 14:52:52 GMT+08:00 2014
	this->_root = _set->getElfNode("@root");
    
	this->backGround = _set->getElfNode("##root_#backGround");
    
    this->activeRange = _set->getElfNode("##root_backGround_range");
    //]]]]@@@@
    
    activeRange->setDebug(true);
    
    CC_SAFE_RETAIN(_set);
    CC_SAFE_RETAIN(_document);
    
    this->addChild(_set->getRootElfNode());
    _set->getRootElfNode()->setPosition(ccp(0,0));
    
    this->initView();
    
    test = new GeometricRecognizer();
    test->loadTemplates();
    
    setTouchEnabled(true);
    
    testODClient();
    
    CCRect rect = CCRectMake(0, 0, activeRange->getContentSize().width, activeRange->getContentSize().height);
    tree = new QuadTree(4, rect);
    
    return true;
}

void LCBattleScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
    //if(pTouches->count() == 1) singleTouch = true;
    moved = false;
}

void LCBattleScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
    moved = true;
    CCSetIterator it = pTouches->begin();
    CCTouch *touch = (CCTouch*)(*it);
    CCPoint location = touch->getLocationInView();
    Point2D pt;
    pt.x = location.x;
    pt.y = location.y;
    mPath.push_back(pt);
}

void LCBattleScene::update(float duration){
    tree->clear();
    
    for(int i = 0; i < monsterArr->count(); ++i){
        tree->addObject((ObjMonster*)monsterArr->objectAtIndex(i));
    }
    tree->addObject(hero);
    
    CCArray* arr = CCArray::create();
    
    for(int i = 0; i < monsterArr->count(); ++i){
        ObjMonster* monster = (ObjMonster*)monsterArr->objectAtIndex(i);
        //if(monster->getrootObj() == NULL) continue;
        tree->getCollisionObjects(monster, arr);
        
        for(int j = 0; j < arr->count(); ++j){
            ObjMonster* monster_1 = (ObjMonster*)arr->objectAtIndex(j);
            CCRect rec = CCRectMake(monster->getShadow()->getPositionX(), monster->getShadow()->getPositionY(), monster->getShadow()->getContentSize().width, monster->getShadow()->getContentSize().height);
            CCRect rec_1 = CCRectMake(monster_1->getShadow()->getPositionX(), monster_1->getShadow()->getPositionY(), monster_1->getShadow()->getContentSize().width, monster_1->getShadow()->getContentSize().height);
            if(rec.intersectsRect(rec_1)){
                //int flag = j % 2 == 0 ? 1 : -1;
                monster_1->moveAway();//(ccp(flag * 20, flag * 20));
            }
        }
    }
}

void LCBattleScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {
    if(!moved){
        CCTouch* touch = dynamic_cast<CCTouch*>(*pTouches->begin());
        CCPoint position = NodeHelper::getPositionToELFDesigner(hero->getrootObj(), touch->getLocation());
        
        //sendPosition(position);
        hero->actionWalk(getPointInMap(position));
        return;
    }
    if (mPath.size() < 1) {
        return;
    }
    RecognitionResult result = test->recognize(mPath);
    CCLog("%s %lf",result.name.c_str(), result.score);
    
    if(strcmp(result.name.c_str(), "V") == 0){
        hero->actionJump();
    }else if(strcmp(result.name.c_str(), "LeftToRightLine") == 0){
        hero->actionRusn(forward);
    }else if(strcmp(result.name.c_str(), "RightToLeftLine") == 0){
        hero->actionRusn(backward);
    }
    mPath.clear();
}

void LCBattleScene::sendPosition(CCPoint pos){
    
    OBJ_GPoint point;
    point.x = pos.x;
    point.y = pos.y;
    
    
    Json::Value root;
    Json::Value jsPos;
    
    jsPos["x"] = pos.x;
    jsPos["y"] = pos.y;
    
    root["success"] = true;
    root["msg"] = "hello";
    root["type"] = OBJ_TYPE_GPOINT;

    root["jsondata"] = jsPos.toStyledString().c_str();

    
    std::string str = root.toStyledString();
    str = str + "\r\n";
    
    //CCLOG("%s %s %s", root.toStyledString().c_str(), jsPos.toStyledString().c_str(), str.c_str());
    
    OD_BYTEPTR buffer = (OD_BYTEPTR)malloc(1024);
    memcpy(buffer, str.c_str() , 1024);
    con->Send(buffer);
}

void LCBattleScene::createHero(){
    hero = new ObjHero();
    hero->create();
    activeRange->addChild(hero->getset()->getRootElfNode());
}
//do initialization here
void LCBattleScene::initView()
{
    createHero();
    
    addMonster(3);
    
    schedule(schedule_selector(LCBattleScene::followHero), 3, kCCRepeatForever, 1);
    //this->schedule();
    
    //schedule(schedule_selector(LCBattleScene::updatess), 2, kCCRepeatForever, 3);
    
    //this->scheduleUpdate();
    //createHero();
    //_root_elf_title->setString(Language("FORMATION$_TITLE"));
    //_root_elf_btn_back_normal_text->setString(Language("GLOBAL$_BTN_RETURN"));
    //_root_elf_btn_back_pressed_text->setString(Language("GLOBAL$_BTN_RETURN"));
    //_root_elf_upgrage_text->setString(Language("FORMATION$_UPGRADE"));
    
    //createTeam();
    //_root_elf_startup->setTarget(this, menu_selector(LCBattleScene::enableClicked));
    //_root_elf_upgrage->setTarget(this, menu_selector(LCBattleScene::upgradeClicked));
    //_root_elf_btn_back->setTarget(this, menu_selector(LCBattleScene::backClicked));
    //_root_walk->setTarget(this, menu_selector(LCBattleScene::actionWalk));
    //_root_walk_des->setString("walk");
    
    //_root_rush->setTarget(this, menu_selector(LCBattleScene::actionRusn));
    //_root_jump->setTarget(this, menu_selector(LCBattleScene::actionJump));
    //_root_attack->setTarget(this, menu_selector(LCBattleScene::actionAttack));
    
    //_root_turn->setTarget(this, menu_selector(LCBattleScene::turnAround));
    
}

void LCBattleScene::clickListener(CCObject * object)
{
    
}

void LCBattleScene::updateCCLayer(GleeObject* data)
{
    //if(NULL == data) return;
    
    //setModel((FormationModel*)data);
    
    //refreshView();
    //refreshMid();
    //refreshView();
}

CCPoint LCBattleScene::getPointInMap(CCPoint pos){
    float halfWidth = activeRange->getContentSize().width / 2;
    float halfHeight = activeRange->getContentSize().height / 2;
    
    pos.x = pos.x > halfWidth ? halfWidth : pos.x;
    pos.x = pos.x < -halfWidth ? -halfWidth : pos.x;
    
    pos.y = pos.y > halfHeight ? halfHeight : pos.y;
    pos.y = pos.y < -halfHeight ? -halfHeight : pos.y;
    
    return  pos;
}

void LCBattleScene::addMonster(int amount){
    while(amount--) {
        ObjMonster* monster = new ObjMonster();
        monster->create();
        monster->autorelease();
        monsterArr->addObject(monster);
        
        activeRange->addChild(monster->getset()->getRootElfNode());
        monster->getrootObj()->setPosition(ccp(amount * 50, amount* 50));
        
        monster->setDirIndex((amount % 8) % 2 ? amount % 8 -1 : amount % 8);
    }
}

void LCBattleScene::followHero(){
    for(int i = 0; i < monsterArr->count(); ++i){
        ObjMonster * monster = (ObjMonster*)monsterArr->objectAtIndex(i);
        monster->actionWalk(hero->getrootObj()->getPosition());
    }
}

void LCBattleScene::backClicked()
{
    //SoundManager::shared()->playEffect(SFX_BUTTON);
    GleeCore::popController(NULL,GleeTransitionCustom::create(1.0));
}

CCFiniteTimeAction* LCBattleScene::getInTransition()
{
    return NULL;//CardUtil::getSceneInAction(_root_elf);
}

CCFiniteTimeAction* LCBattleScene::getOutTransition(){
    return NULL;//CardUtil::getSceneOutAction(_root_elf);
}

static void *testODClientProc(OD_BYTEPTR buffer)
{
    //CCLOG("%s", buffer);
    Json::Reader reader;
    Json::Value value;
    std::vector<char>* data;
    data = new std::vector<char>();
    for(int i = 0; i < 1024; ++i){
        data->push_back(*(buffer + i));
    }
    
    if(reader.parse(std::string(&*(data->begin())), value)){
        GLEELOG("%s", value.toStyledString().c_str());
    }
    //CCLog("receive : %s", buffer);
    OBJ_Msg msg;
    msg.success = value["success"].asBool();
    strcpy(msg.msg, value["msg"].asCString());
    msg.type = value["type"].asInt();
    
    std::string posStr = value["jsondata"].asCString();
    //msg.jsondata;
    
    CCLOG("%s", posStr.c_str());
    Json::Value temp;
    if(reader.parse(std::string(&*(posStr.begin())), temp)){
        GLEELOG("%s", temp.toStyledString().c_str());
    }
    CCPoint p;
    p.x = temp["x"].asDouble();
    p.y = temp["y"].asDouble();
    
    l->hero->actionWalk(p);
    //wal
    //Json::Value posStr;
    //posStr.value()
    //strcpy(msg.msg, value["msg"].asCString());
    
    //msg.msg = value["msg"].as
    //(OBJ_Msg*)buffer;
    delete data;
    
    return NULL;
}

void LCBattleScene::testODClient()
{
    con = new ODClient("192.168.2.101", 8888, 1024, testODClientProc);
    bool isconn = con->Connect();
    if (isconn) {
        //OD_BYTEPTR buffer = (OD_BYTEPTR)malloc(1024);
        //memcpy(buffer, "hello world", 1024);
        //c->Send(buffer);
        l = this;
    } else {
        CCLog("connect failed!");
    }
    
}
