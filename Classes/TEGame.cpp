//
//  TEGame.cpp
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#include "TEGame.h"
#include "LFPay.h"

TEGameStartLayer *TEGameStartLayer::Create()
{
    TEGameStartLayer *l = new TEGameStartLayer();
    l->autorelease();
    l->MyInit();
    return l;
}
void TEGameStartLayer::MyInit()
{
    //背景
    LayerColor *layerbg = LayerColor::create(Color4B(0, 0, 0, 150), WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
    this->addChild(layerbg);
    
    //文字
    Label *textlabel = Label::createWithSystemFont("点击屏幕开始游戏", "Arial", 40);
    this->addChild(textlabel);
    textlabel->setPosition(Vec2(WIN_SIZE_WIDTH / 2, WIN_SIZE_HEIGHT / 2));
    
    
    auto listener = EventListenerTouchOneByOne::create();  //创建一个单点触摸事件 :EventListenerTouchAllAtOnce 为多点
    //设置这些事件的的回调方法
    listener->onTouchBegan = CC_CALLBACK_2(TEGameStartLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TEGameStartLayer::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(TEGameStartLayer::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //事件调度器
}

bool TEGameStartLayer::onTouchBegan(Touch* touch, Event  *event)
{
    GetGameInstance->m_gGame->GameStart(true);
    this->getParent()->removeChild(this,true);
    return true;
}




TEGame::~TEGame()
{
}
Scene* TEGame::createScene()
{
    Scene *s = Scene::create();
    TEGame *game = TEGame::Create();
    s->addChild(game);
    return s;
}
Scene* TEGame::createSceneBylv()
{
    Scene *s = Scene::create();
    TEGame *game = TEGame::CreateByLv();
    s->addChild(game);
    return s;
}
TEGame *TEGame::Create()
{
    TEGame *game = new TEGame();
    game->autorelease();
    game->islv = false;
    game->MyInit();
    return game;
}

TEGame *TEGame::CreateByLv()
{
    TEGame *game = new TEGame();
    game->autorelease();
    game->islv = true;
    game->MyInit();
    return game;
}
void TEGame::MyInit()
{
    m_nClearCount = 0;
    
    m_nBuyClear5Count = 0;
    m_bShowBuyClear5 = true;
    
    m_bistouch = true;
    GetGameInstance->m_gGame = this;
    //图形下落时间
    m_nShapeDownTime = 0;
    m_kGameState = k_gamestate_pause;
    
    ///大背景
    Sprite *bigbg = Sprite::create("bigbg.jpg");
    this->addChild(bigbg,-1);
    bigbg->setPosition(Vec2(WIN_SIZE_WIDTH / 2, WIN_SIZE_HEIGHT / 2));
    bigbg->setScale(WIN_SIZE_WIDTH / bigbg->getContentSize().width,
                    WIN_SIZE_HEIGHT / bigbg->getContentSize().height);
    //背景
    m_bg = Sprite::create("beijing.png");
    this->addChild(m_bg,-1);
    m_bg->setPosition(Vec2(WIN_SIZE_WIDTH / 2, WIN_SIZE_HEIGHT / 2));
    
    float widthrate = WIN_SIZE_WIDTH / m_bg->getContentSize().width ;
    float heightrate = WIN_SIZE_HEIGHT / m_bg->getContentSize().height ;
    if (widthrate < heightrate) {
        m_bg->setScale(widthrate);
    }else{
        m_bg->setScale(heightrate);
    }
    
    Sprite *t_point = Sprite::create("block_big.png");
    
    float pointsize = t_point->getContentSize().width+2;
    
    
    Vec2 windowStartPos = Vec2(m_bg->getContentSize().width * 0.223,
                               m_bg->getContentSize().height * 0.393);
    
    
    for (int i = 0; i < MAX_ROW_COUNT ; i++) {
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            m_gamePointArr[i][j] = 0;
            
            Sprite *point = Sprite::create("block_big.png");
            m_bg->addChild(point);
            point->setPosition(Vec2(windowStartPos.x + pointsize * j, windowStartPos.y + pointsize * i));
            pointArr.push_back(point);
            point->setOpacity(25);
        }
    }
    
    
    m_bIsPlaySound = UserDefault::getInstance()->getIntegerForKey("isplaysound") == 0 ? true : false;
    
    //Pad
    TEGamePad *pad = TEGamePad::Create();
    m_bg->addChild(pad,1);
    
//    TEGameStartLayer *startlayer = TEGameStartLayer::Create();
//    this->addChild(startlayer,10);
    
    m_nLv = 0;
    m_nSpeed = 0;
    m_nNextAddSpeedScores = 0;
    m_nNextCdTime = -1;
    //游戏模式
//    if (!islv) {
//        CreateMenuLayer();
//        pad->m_bstartgame = 0;
//    }else{
//        pad->m_bstartgame = 1;
//    }
    
    CreateMenuLayer();
    pad->m_bstartgame = 1;
    
    auto listener = EventListenerTouchOneByOne::create();  //创建一个单点触摸事件 :EventListenerTouchAllAtOnce 为多点
    //设置这些事件的的回调方法
    listener->onTouchBegan = CC_CALLBACK_2(TEGame::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TEGame::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(TEGame::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //事件调度器
    
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyReleased = CC_CALLBACK_2(TEGame::onKeyPressed,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener,this);
    
    if (m_bIsPlaySound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(g_gameAudio[k_audio_music].c_str());
    }
}

void TEGame::CreateMenuLayer()
{
    m_menuLayer = Layer::create();
    m_bg->addChild(m_menuLayer);
    m_menuLayer->setPosition(Vec2(m_bg->getContentSize().width * 0.19,
                                  m_bg->getContentSize().height * 0.39));
    
    m_menuLayer->setContentSize(Size(213,404));
    
    //标题
    Sprite *title = Sprite::create("text_selmode.png");
    m_menuLayer->addChild(title);
    title->setPosition(Vec2(m_menuLayer->getContentSize().width / 2, m_menuLayer->getContentSize().height * 0.84));
    
    //girl
    Sprite *girl = Sprite::create("girl1.png");
    
    Vector<cocos2d::SpriteFrame *> arrayOfSpriteFrameNames;
    char textname[16];
    for (int i = 1; i <= 3; i++) {
        sprintf(textname, "girl%d.png",i);
        Texture2D *t = Director::getInstance()->getTextureCache()->addImage(textname);
        SpriteFrame *s = SpriteFrame::createWithTexture(t, Rect(0, 0, t->getContentSize().width, t->getContentSize().height));
        arrayOfSpriteFrameNames.pushBack(s);
    }
    
    Animation *an = Animation::createWithSpriteFrames(arrayOfSpriteFrameNames,0.4);
    girl->runAction(RepeatForever::create(Animate::create(an)));
    m_menuLayer->addChild(girl);
    girl->setPosition(Vec2(m_menuLayer->getContentSize().width / 2, m_menuLayer->getContentSize().height * 0.55));
    
    //模式文字
    
    //普通
    Sprite *normalbutton_nor = Sprite::create("text_mode_normal.png");
    Sprite *normalbutton_sel = Sprite::create("text_mode_normal.png");
    normalbutton_sel->setScale(0.9);
    Sprite *yixingbutton_nor = Sprite::create("text_mode_yixing.png");
    Sprite *yixingbutton_sel = Sprite::create("text_mode_yixing.png");
    yixingbutton_sel->setScale(0.9);
    
    noritem     = MenuItemSprite::create(normalbutton_nor, normalbutton_sel, CC_CALLBACK_1(TEGame::ModeButtonCB, this));
    yixingitem  = MenuItemSprite::create(yixingbutton_nor, yixingbutton_sel, CC_CALLBACK_1(TEGame::ModeButtonCB, this));
    yixingitem->setOpacity(100);
    noritem->setTag(0);
    yixingitem->setTag(1);
    yixingitem->setPosition(Vec2(0, - yixingitem->getContentSize().height * 1.3));
    
    Menu *menu = Menu::create(noritem,yixingitem, NULL);
    m_menuLayer->addChild(menu);
    menu->setPosition(Vec2(m_menuLayer->getContentSize().width / 2, m_menuLayer->getContentSize().height * 0.29));
    
    
    //标签
    Sprite *seltag = Sprite::create("seltag.png");
    m_menuLayer->addChild(seltag,0,10);
    seltag->setPosition(Vec2(menu->getPositionX() - noritem->getContentSize().width / 2 - seltag->getContentSize().width,
                             menu->getPositionY()));
    
    if (GetGameInstance->m_gameMode == 1) {
        noritem->setOpacity(100);
        yixingitem->setOpacity(255);
        
        seltag->setPositionY(yixingitem->getParent()->getPositionY() + yixingitem->getPositionY());
    }else{
        noritem->setOpacity(255);
        yixingitem->setOpacity(100);
        
        seltag->setPositionY(noritem->getParent()->getPositionY() + noritem->getPositionY());
    }
}
void TEGame::ModeButtonCB(Ref *obj)
{
    MenuItemSprite *item = (MenuItemSprite *)obj;
    int tag = item->getTag();
    GetGameInstance->m_gameMode = tag;
    
    if (noritem == item) {
        noritem->setOpacity(255);
        yixingitem->setOpacity(100);
    }else{
        noritem->setOpacity(100);
        yixingitem->setOpacity(255);
    }
    
    //获取标签
    Sprite *seltag = (Sprite *)m_menuLayer->getChildByTag(10);
    seltag->setPositionY(item->getParent()->getPositionY() + item->getPositionY());
}

void TEGame::ChangeMode()
{
    MenuItemSprite *item;
    
    if (GetGameInstance->m_gameMode == 0) {
        GetGameInstance->m_gameMode = 1;
        
        noritem->setOpacity(100);
        yixingitem->setOpacity(255);
        item = yixingitem;
    }else{
        GetGameInstance->m_gameMode = 0;
        noritem->setOpacity(255);
        yixingitem->setOpacity(100);
        item = noritem;
    }
    
    //获取标签
    Sprite *seltag = (Sprite *)m_menuLayer->getChildByTag(10);
    seltag->setPositionY(item->getParent()->getPositionY() + item->getPositionY());
    
    //刷新最高分
    GetGameInstance->m_gGamePad->FlushHeightScores();
}
void TEGame::SetIsPlayVideo()
{
    m_bIsPlaySound = !m_bIsPlaySound;
    UserDefault::getInstance()->setIntegerForKey("isplaysound", m_bIsPlaySound ? 0:1);
    GetGameInstance->m_gGamePad->SetMusicIcon(m_bIsPlaySound);
    
    if (m_bIsPlaySound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(g_gameAudio[k_audio_music].c_str());
    }else{
        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

void TEGame::CreateRegLayer()
{
    Layer *reglayer = Layer::create();
    m_bg->addChild(reglayer,100,1112);
    
    m_bistouch = false;
    
    //背景
    Sprite *bg;
    
    std::string bname;
    if (GetGameInstance->m_gameMode == 1) {
        bg = Sprite::create("regbg_2.png");
        bname = "openbutton_2.png";
        
    }else{
        bg = Sprite::create("regbg_1.png");
        bname = "openbutton_1.png";
    }
    
    reglayer->addChild(bg);
    bg->setAnchorPoint(Vec2(0, 0));
    bg->setPosition(Vec2(m_bg->getContentSize().width * 0.19, m_bg->getContentSize().height * 0.375));
    
    //button
    Sprite *b_n = Sprite::create(bname);
    Sprite *b_s = Sprite::create(bname);
    b_s->setColor(Color3B(150, 150, 150));
    
    MenuItemSprite *item = MenuItemSprite::create(b_n, b_s, CC_CALLBACK_1(TEGame::RegCB, this));
    item->setPosition(Vec2(bg->getContentSize().width / 2, b_n->getContentSize().height * 0.7));
    
    //close;
    Sprite *cb_n = Sprite::create("close.png");
    Sprite *cb_s = Sprite::create("close.png");
    cb_s->setColor(Color3B(150, 150, 150));
    
    MenuItemSprite *citem = MenuItemSprite::create(cb_n,cb_s, CC_CALLBACK_1(TEGame::CancelCB, this));
    citem->setPosition(Vec2(bg->getContentSize().width-cb_n->getContentSize().width * 0.6, bg->getContentSize().height -cb_n->getContentSize().height * 0.6));
    
    Menu *m = Menu::create(item,citem, NULL);
    bg->addChild(m);
    m->setPosition(Vec2(0, 0));
    
}

void TEGame::RegCB(Ref*obj)
{
    PayCtrl::GetInstance()->Pay(GetGameInstance->m_gameMode,
                                [&](Ref *obj){
                                    RegSuccess(nullptr);
                                },
                                [&](Ref *obj){
                                    RegFailed(nullptr);
                                });
    
}
void TEGame::RegFailed(Ref*obj){
    m_bistouch = true;
    m_bg->removeChildByTag(1112,true);
    GetGameInstance->m_gGame->GameStart(false);
}
void TEGame::RegSuccess(Ref*obj)
{
    m_bistouch = true;
    std::string keyname;
    if (GetGameInstance->m_gameMode == 1 ) { //普通
        keyname = "isr2";
    }else{//异形
        keyname = "isr1";
    }
    
    UserDefault::getInstance()->setIntegerForKey(keyname.c_str(), 1);
    
    m_bg->removeChildByTag(1112,true);
    
    GetGameInstance->m_gGame->GameStart(false);
}
void TEGame::CancelCB(Ref*obj)
{
    m_bistouch = true;
    m_bg->removeChildByTag(1112,true);
    GetGameInstance->m_gGame->GameStart(false);
}

void TEGame::GameStart(bool isshowui)
{
    if(isshowui){
        if (GetGameInstance->m_gameMode == 1 ) { //普通
            int isreg1 = UserDefault::getInstance()->getIntegerForKey("isr2");
            if (isreg1 == 0) {
                if(PayCtrl::GetInstance()->isShowUI){
                    CreateRegLayer();
                }else{
                    PayCtrl::GetInstance()->Pay(GetGameInstance->m_gameMode,
                                                [&](Ref *obj){
                                                    RegSuccess(nullptr);
                                                },
                                                [&](Ref *obj){
                                                    
                                                });
                }
                return;
            }
            
        }else{//异形
            int isreg2 = UserDefault::getInstance()->getIntegerForKey("isr1");
            if (isreg2 == 0) {
                if(PayCtrl::GetInstance()->isShowUI){
                    CreateRegLayer();
                }else{
                    PayCtrl::GetInstance()->Pay(GetGameInstance->m_gameMode,
                                                [&](Ref *obj){
                                                    RegSuccess(nullptr);
                                                },
                                                [&](Ref *obj){
                                                    
                                                });
                }
                
                return;
            }
        }
    }
    
    GetGameInstance->m_gGamePad->m_bstartgame = 2;
    m_bg->removeChild(m_menuLayer, true);
    
    m_kGameState = k_gamestate_runing;
    //添加预备图形
    for (int i = 0; i < READY_SHAPE_COUNT; i++) {
        TEBaseShape *t_shape = new TEBaseShape();
        t_shape->MyInit(GetGameInstance->m_gameMode);
        m_Squares[i] = t_shape;
    }
    
    FlashMatrix();
    
    
    //预览窗口背景
    //预览窗口
    m_NextOverview = Node::create();
    m_bg->addChild(m_NextOverview);
    m_NextOverview->setPosition(Vec2(m_bg->getContentSize().width * 0.66,
                                     m_bg->getContentSize().height * 0.74));
    
    //显示下一个
    ShowNextShape();
    
    this->schedule(schedule_selector(TEGame::MyLoop));
    
    
    //根据等级填充
    
    for (int i = 0; i < MAX_ROW_COUNT ; i++) {
        bool isshow = false;
        bool ishidden = false;
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            if (i < m_nLv) {
                if (CCRANDOM_0_1() < 0.7) {
                    isshow = true;
                    m_gamePointArr[i][j] = 1;
                    pointArr[(i) * MAX_COL_COUNT + j]->setOpacity(255);
                }else{
                    ishidden = true;
                }
            }
            if (j == MAX_COL_COUNT - 1) {
                if (!isshow ) {
                    m_gamePointArr[i][j] = 1;
                    pointArr[(i) * MAX_COL_COUNT + j]->setOpacity(255);
                }
                if (!ishidden) {
                    m_gamePointArr[i][j] = 0;
                    pointArr[(i) * MAX_COL_COUNT + j]->setOpacity(25);
                }
            }
        }
    }
    
}

void TEGame::GamePause()
{
    m_kGameState = k_gamestate_pause;
    GetGameInstance->m_gGamePad->m_cafeiicon->setOpacity(255);
}
void TEGame::GameResume()
{
    m_kGameState = k_gamestate_runing;
    GetGameInstance->m_gGamePad->m_cafeiicon->setOpacity(50);
}
void TEGame::MyLoop(float dt)
{
    if (m_kGameState != k_gamestate_runing) {
        return;
    }
    m_nShapeDownTime += dt;
    if(m_nNextCdTime >= 0) {
        m_nNextCdTime += dt;
        
        if (m_nNextCdTime > m_nSpeed / 10.0) {
            m_nNextCdTime = -1;
            m_nShapeDownTime = 0;
            NextShape();
        }
        
    }
    //根据分数计算当前游戏速度
    float timestep = 1.1 - m_nSpeed / 10.0;
    timestep = timestep < 0.03 ?  0.03 : timestep;
    if (m_nShapeDownTime >= timestep) {
        m_nShapeDownTime = 0;
        m_Squares[0]->Move(m_shapeMoveDir_down);
    }
    
    GetGameInstance->m_gGamePad->MyLoop(dt);
}

//清除图形
void TEGame::ClearShapeColor()
{
    TEBaseShape *t_shape = m_Squares[0];
    if (t_shape == nullptr) {
        return;
    }
    //绘制出图形矩阵
    for (int i = 0; i < SHAPE_HEIGHT; i++) {
        for (int j =0 ; j < SHAPE_WIDTH; j++) {
            if (ShapeMatrix[t_shape->myType][t_shape->m_nMyDir][i][j] == 1) {
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->stopAllActions();
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->runAction(FadeTo::create(0.1/float(m_nSpeed+1), 25));
            }
        }
    }
}

//显示新的图形
void TEGame::FlashMatrix()
{
    if (m_kGameState != k_gamestate_runing) {
        return;
    }
    
    TEBaseShape *t_shape = m_Squares[0];
    
    //绘制出图形矩阵
    for (int i = 0; i < SHAPE_HEIGHT; i++) {
        for (int j =0 ; j < SHAPE_WIDTH; j++) {
            if (ShapeMatrix[t_shape->myType][t_shape->m_nMyDir][i][j] == 1) {
                
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->stopAllActions();
//                colorArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->runAction(TintTo::create(0.2, 255, 0, 0));
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->setOpacity(255);
            }
        }
    }
}

//测试是否可以移动
bool TEGame::TestNewShape(TEBaseShape *s)
{
    for (int i = 0; i < SHAPE_HEIGHT; i++) {
        for (int j =0 ; j < SHAPE_WIDTH; j++) {
            if (ShapeMatrix[s->myType][s->m_nMyDir][i][j] == 1) {
                //先检测左右边界
                if (s->posx + j < 0 || s->posx + j >= MAX_COL_COUNT) {
                    //已经到边界
                    return false;
                }
                
                if (s->posy - i < 0) {
                    //已经移动到最下面
                    return false;
                }
                
                if (m_gamePointArr[s->posy - i][s->posx + j] == 1) {
                    //与已有部分重叠
                    return false;
                }
                
            }
        }
    }
    
    return true;
}

//增加游戏速度
void TEGame::SetSpeed(int speed)
{
    m_nSpeed = speed;
    char text[8];
    sprintf(text, "%d",m_nSpeed);
    GetGameInstance->m_gGamePad->SetSpeedText(text);
}

//增加等级
void TEGame::SetLv(int lv){
    m_nLv = lv;
    char text[8];
    sprintf(text, "%d",m_nLv);
    GetGameInstance->m_gGamePad->SetLvText(text);
}
//图形落地
void TEGame::ShapeDown()
{
    if (m_nNextCdTime == -1) {
        m_nNextCdTime = 0;
    }
    log("");
}
//将图形保存到数组  删除当前图形
void TEGame::NextShape()
{
    TEBaseShape *t_shape = m_Squares[0];
    for (int i = 0; i < SHAPE_HEIGHT; i++) {
        for (int j =0 ; j < SHAPE_WIDTH; j++) {
            if (ShapeMatrix[t_shape->myType][t_shape->m_nMyDir][i][j] == 1) {
                m_gamePointArr[t_shape->posy - i][t_shape->posx + j] = 1;
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->stopAllActions();
                pointArr[(t_shape->posy - i) * MAX_COL_COUNT + t_shape->posx + j]->setOpacity(255);
            }
        }
    }
    
    
    
    
    //删除图形
    delete m_Squares[0];
    //重新排列数组
    for (int i = 0; i < READY_SHAPE_COUNT - 1;  i++) {
        m_Squares[i] =  m_Squares[i+1];
    }
    
    //创建图形
    TEBaseShape *newshape = new TEBaseShape();
    newshape->MyInit(GetGameInstance->m_gameMode);
    m_Squares[READY_SHAPE_COUNT - 1] = newshape;
    if (!TestNewShape(newshape)) {
        //gameover
        GameOver();
        return;
    }
    
    //下一个预览
    ShowNextShape();
    
    //消除已经填满的行
    bool ishavefullline = false;
    int maxHeight = 0;
    for (int i = 0; i < MAX_ROW_COUNT ; i++) {
        bool isfull = true; //纪录当前行是否已经填满
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            if (m_gamePointArr[i][j] == 0) {
                isfull = false;
            }else{
                if (i > maxHeight) {
                    maxHeight = i;
                }
            }
        }
        if (isfull) {
            //消除动画
            ishavefullline = true;
            float actiontime = 0.03;
            float delaytime = 0.08;
            for (int j = 0; j < MAX_COL_COUNT; j++) {
                pointArr[(i) * MAX_COL_COUNT + j]->stopAllActions();
                //pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(DelayTime::create(0.04 * j),FadeTo::create(0.03, 50), NULL));
                pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(FadeTo::create(actiontime, 25),
                                                                              DelayTime::create(delaytime),
                                                                              FadeTo::create(actiontime, 255),
                                                                              DelayTime::create(delaytime),
                                                                              FadeTo::create(actiontime, 25),
                                                                              DelayTime::create(delaytime),
                                                                              FadeTo::create(actiontime, 255),
                                                                              DelayTime::create(delaytime),
                                                                              FadeTo::create(actiontime, 25),
                                                                              DelayTime::create(delaytime),NULL));
            }
        }
    }
    
    if (maxHeight < MAX_ROW_COUNT - 6) {
        m_bShowBuyClear5 = true;
    }
    
    if (m_bShowBuyClear5) {
        if (m_nBuyClear5Count < 2) {
            if (maxHeight >= MAX_ROW_COUNT - 6) {
                m_nBuyClear5Count ++;
                Clear5Line();
                m_bShowBuyClear5 = false;
            }
        }
    }
    
    if (ishavefullline) {
        //this->runAction(Sequence::create(DelayTime::create(0.04 * MAX_COL_COUNT),CallFunc::create(CC_CALLBACK_0(TEGame::ClearFullLine, this)), NULL));
        this->runAction(Sequence::create(DelayTime::create(0.65),CallFunc::create(CC_CALLBACK_0(TEGame::ClearFullLine, this)), NULL));
        
        
        if (m_bIsPlaySound) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_clear].c_str());
        }
    }else{
        AddScores(10);
    }
    
}

static const int AddSpeedScores[9] = {15,20,25,30,35,40,40,40,40};

//清除已经填满的行
void TEGame::ClearFullLine()
{
    int linecount = 0;
    for (int i = 0; i < MAX_ROW_COUNT ; i++) {
        bool isfull = true; //纪录当前行是否已经填满
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            if (m_gamePointArr[i][j] == 0) {
                isfull = false;
            }
        }
        if (isfull) {
            linecount ++;
            //下落
            LineDown(i);
            //重新从当前行检测
            i--;
        }
    }
    if (linecount >= 1) {
        m_nClearCount ++;
        if (m_nSpeed < 9) {
            if (m_nClearCount >= AddSpeedScores[m_nSpeed]) {
                m_nClearCount = 0;
                SetSpeed(m_nSpeed + 1);
            }
        }
    }
    switch (linecount) {
        case 1:
            AddScores(100);
            break;
        case 2:
            AddScores(300);
            break;
        case 3:
            AddScores(700);
            break;
        case 4:
            AddScores(1500);
            break;
        default:
            AddScores(1500);
            break;
    }
}
void TEGame::LineDown(int startLine)
{
    //startLine以上所有行下落
    for (int i = startLine; i < MAX_ROW_COUNT -1; i++) {
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            m_gamePointArr[i][j] = m_gamePointArr[i + 1][j];
            if (m_gamePointArr[i][j] == 0) {
                pointArr[(i) * MAX_COL_COUNT + j]->stopAllActions();
                pointArr[(i) * MAX_COL_COUNT + j]->runAction(FadeTo::create(0.1/float(m_nSpeed + 1), 25) );
            }else{
                pointArr[(i) * MAX_COL_COUNT + j]->stopAllActions();
                pointArr[(i) * MAX_COL_COUNT + j]->runAction(FadeTo::create(0.1/float(m_nSpeed + 1), 255) );
            }
        }
    }
    //清除最上面一行
    for (int i = 0; i < MAX_COL_COUNT; i++) {
        m_gamePointArr[MAX_ROW_COUNT -1][i] = 0;
        pointArr[(MAX_ROW_COUNT -1) * MAX_COL_COUNT + i]->stopAllActions();
        pointArr[(MAX_ROW_COUNT -1) * MAX_COL_COUNT + i]->runAction(FadeTo::create(0.1/float(m_nSpeed + 1), 25) );
    }
    //显示图形
    FlashMatrix();
    
}

//下一个预览
void TEGame::ShowNextShape()
{
    m_NextOverview->removeAllChildren();
    
    for (int k = 1; k < READY_SHAPE_COUNT; k++) {
        
        TEBaseShape *t_shape = m_Squares[k] ;
        
        //绘制出图形矩阵
        for (int i = 0; i < SHAPE_HEIGHT; i++) {
            for (int j =0 ; j < SHAPE_WIDTH; j++) {
                Sprite *block = Sprite::create("block_small.png");
                m_NextOverview->addChild(block);
                block->setPosition(Vec2(block->getContentSize().width * j, - block->getContentSize().height * (i + 1) - ((k-1) * 100)));
                block->setOpacity(25);
                if (ShapeMatrix[t_shape->myType][t_shape->m_nMyDir][i][j] == 1) {
                    block->setOpacity(255);
                }
            }
        }
    }
}

void TEGame::FustDown()
{
    while (GetNowShape()->Move(m_shapeMoveDir_down)) {
        
    }
}

void TEGame::GameOver()
{
    m_kGameState = k_gamestate_over;
    OverAction();
//    TEGameOverLayer *overlayer = TEGameOverLayer::Create();
//    this->addChild(overlayer,10);
    
    //最高分数
    
    if (GetGameInstance->m_gameMode == 1) {
        int bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores1");
        if (bestscores < GetGameInstance->m_gGame->GetScores()) {
            bestscores = GetGameInstance->m_gGame->GetScores();
            UserDefault::getInstance()->setIntegerForKey("bestscores1", bestscores);
        }
    }else{
        int bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores2");
        if (bestscores < GetGameInstance->m_gGame->GetScores()) {
            bestscores = GetGameInstance->m_gGame->GetScores();
            UserDefault::getInstance()->setIntegerForKey("bestscores2", bestscores);
        }
    }
    
    
}
void TEGame::AddScores(int s){
    m_nScores += s;
    GetGameInstance->m_gGamePad->FlushGameScores();
};


void TEGame::OverAction()
{
    m_kGameState = k_gamestate_over;
    EatGameTouchForTime(5.0);
    for (int i = 0; i < MAX_ROW_COUNT ; i++) {
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            pointArr[(i) * MAX_COL_COUNT + j]->stopAllActions();
            if (i == 0 && j == 0) {
                pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(DelayTime::create(i*0.06),FadeTo::create(0.01, 255),DelayTime::create((MAX_ROW_COUNT - i) * 0.06 * 2),FadeTo::create(0.1, 25),DelayTime::create(0.1f) ,CallFunc::create([](){
                    
                    ChangeScene::Create(k_scenetag_lv);
                }) , NULL));
            }else{
                pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(DelayTime::create(i*0.06),FadeTo::create(0.1, 255),DelayTime::create((MAX_ROW_COUNT - i) * 0.06 * 2),FadeTo::create(0.1, 25) , NULL));
            }
            
        }
    }
}

//消除5行
void TEGame::Clear5Line()
{
    m_kGameState = k_gamestate_pause;
    PayCtrl::GetInstance()->Pay(k_pay_clear5line,
                                [&](Ref *obj){
                                    Clear5LineBuyFun();
                                },
                                [&](Ref *obj){
                                    m_kGameState = k_gamestate_runing;
                                });
}

void TEGame::Clear5LineBuyFun()
{
    m_kGameState = k_gamestate_runing;
    
    for (int i = 0; i < 5 ; i++) {
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            if (m_gamePointArr[i][j] == 0) {
                m_gamePointArr[i][j] = 1;
            }
        }
        float actiontime = 0.03;
        float delaytime = 0.08;
        for (int j = 0; j < MAX_COL_COUNT; j++) {
            pointArr[(i) * MAX_COL_COUNT + j]->stopAllActions();
            //pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(DelayTime::create(0.04 * j),FadeTo::create(0.03, 50), NULL));
            pointArr[(i) * MAX_COL_COUNT + j]->runAction(Sequence::create(FadeTo::create(actiontime, 25),
                                                                          DelayTime::create(delaytime),
                                                                          FadeTo::create(actiontime, 255),
                                                                          DelayTime::create(delaytime),
                                                                          FadeTo::create(actiontime, 25),
                                                                          DelayTime::create(delaytime),
                                                                          FadeTo::create(actiontime, 255),
                                                                          DelayTime::create(delaytime),
                                                                          FadeTo::create(actiontime, 25),
                                                                          DelayTime::create(delaytime),NULL));
        }
    }
    
    this->runAction(Sequence::create(DelayTime::create(0.65),CallFunc::create(CC_CALLBACK_0(TEGame::ClearFullLine, this)), NULL));
    
    if (m_bIsPlaySound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_clear].c_str());
    }
    
}

//获取竖条
void TEGame::GetLine()
{
    m_kGameState = k_gamestate_pause;
    PayCtrl::GetInstance()->Pay(k_pay_getline,
                                 [&](Ref *obj){
                                     GetLineBuyFun();
                                 },
                                 [&](Ref *obj){
                                     m_kGameState = k_gamestate_runing;
                                 });
    
}
void TEGame::GetLineBuyFun()
{
    m_kGameState = k_gamestate_runing;
    GetGameInstance->m_gGame->ClearShapeColor();
    m_Squares[0]->myType = k_ShapeType_line;
    GetGameInstance->m_gGame->FlashMatrix();
    
}

void TEGame::onKeyPressed(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_ESCAPE:   //返回键听
            Director::getInstance()->end();
            break;
        case EventKeyboard::KeyCode::KEY_BACKSPACE:   //返回键听
            Director::getInstance()->end();
            break;
        case EventKeyboard::KeyCode::KEY_MENU:      //菜单监听
            break;
        case::EventKeyboard::KeyCode::KEY_HOME:
            break;
        default:
            break;
    }
}
bool TEGame::onTouchBegan(Touch* touch, Event  *event)
{
    if (!m_bistouch) {
        return true;
    }
    GetGameInstance->m_gGamePad->onTouchBegan(touch, event);
    return true;
}
void TEGame::onTouchMoved(Touch *touch, Event *event)
{
    
    if (!m_bistouch) {
        return ;
    }
    GetGameInstance->m_gGamePad->onTouchMoved(touch, event);
}
void TEGame::onTouchEnded(Touch* touch, Event  *event)
{
    if (!m_bistouch) {
        return ;
    }
    GetGameInstance->m_gGamePad->onTouchEnded(touch, event);
}


#pragma mark - TEGameOverLayer
TEGameOverLayer *TEGameOverLayer::Create()
{
    TEGameOverLayer *l = new TEGameOverLayer();
    l->autorelease();
    l->MyInit();
    return l;
}
void TEGameOverLayer::MyInit()
{
    //大背景
    LayerColor *overbg = LayerColor::create(Color4B(0, 0, 0, 150), WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT);
    this->addChild(overbg,-1);
    
    //小背景
    LayerColor *desbg = LayerColor::create(Color4B(1, 77, 103, 255), WIN_SIZE_WIDTH / 2, WIN_SIZE_HEIGHT / 2);
    this->addChild(desbg);
    desbg->setPosition(Vec2(WIN_SIZE_WIDTH / 2 - desbg->getContentSize().width / 2,
                            WIN_SIZE_HEIGHT / 2 - desbg->getContentSize().height / 2));
    desbg->setScale(0);
    desbg->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.1, 1.0), NULL));
    
    
    //游戏结束
    Label *overtext = Label::createWithSystemFont("游戏结束", "Arial", 70);
    desbg->addChild(overtext);
    
    overtext->setPosition(Vec2(desbg->getContentSize().width / 2, desbg->getContentSize().height - overtext->getContentSize().height ));
    
    
    char scores[32];
    //最高分数
    int bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores");
    if (bestscores < GetGameInstance->m_gGame->GetScores()) {
        bestscores = GetGameInstance->m_gGame->GetScores();
        UserDefault::getInstance()->setIntegerForKey("bestscores", bestscores);
    }
    sprintf(scores, "最高分数 : %d",bestscores);
    Label *bestscoresLabel = Label::createWithSystemFont(scores, "Arial", 35);
    desbg->addChild(bestscoresLabel);
    bestscoresLabel->setPosition(Vec2(desbg->getContentSize().width / 2, desbg->getContentSize().height * 0.5));
    
    //当前分数
    sprintf(scores, "本局分数 : %d",GetGameInstance->m_gGame->GetScores());
    Label *nowscores = Label::createWithSystemFont(scores, "Arial", 35);
    desbg->addChild(nowscores);
    nowscores->setPosition(Vec2(desbg->getContentSize().width / 2, desbg->getContentSize().height * 0.6));
    
    //重新开始按钮
    Label *replaylabel = Label::createWithSystemFont("重新开始", "Arial", 40);
    MenuItemLabel *itemlabel = MenuItemLabel::create(replaylabel, CC_CALLBACK_1(TEGameOverLayer::ReplayCB, this));
    Menu *replaymenu = Menu::create(itemlabel, NULL);
    desbg->addChild(replaymenu,1);
    replaymenu->setPosition(Vec2(desbg->getContentSize().width / 2,
                                 replaylabel->getContentSize().height * 1.2));
    
    LayerColor *buttonbg = LayerColor::create(Color4B(96,143,159,255), 200, replaylabel->getContentSize().height * 1.1);
    desbg->addChild(buttonbg );
    buttonbg->setPosition(Vec2(replaymenu->getPositionX() - buttonbg->getContentSize().width / 2,
                               replaymenu->getPositionY() - buttonbg->getContentSize().height / 2));
    
    auto listener = EventListenerTouchOneByOne::create();  //创建一个单点触摸事件 :EventListenerTouchAllAtOnce 为多点
    listener->setSwallowTouches(true);
    //设置这些事件的的回调方法
    listener->onTouchBegan = CC_CALLBACK_2(TEGameOverLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TEGameOverLayer::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(TEGameOverLayer::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this); //事件调度器
}

void TEGameOverLayer::ReplayCB(Ref *obj)
{
    ChangeScene::Create(k_scenetag_game);
}

bool TEGameOverLayer::onTouchBegan(Touch* touch, Event  *event)
{
    return true;
    
}

#pragma mark - Pad
TEGamePad *TEGamePad::Create()
{
    TEGamePad *pad = new TEGamePad();
    pad->autorelease();
    pad->MyInit();
    return pad;
}
void TEGamePad::MyInit()
{
    GetGameInstance->m_gGamePad = this;
    m_bstartgame = 0;
    
    m_nTouchPadTag = -1;
    m_nDownTime = 0;
    
//    Label *buttonLabel;
    
    /***************左**************/
    m_padLeft = Sprite::create("button_mid.png");
    this->addChild(m_padLeft,0,1);
    m_padLeft->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.076,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.16));
    
    /***************右**************/
    m_padRight = Sprite::create("button_mid.png");
    this->addChild(m_padRight,0,2);
    m_padRight->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.386,
                                 GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.16));
    
    /***************下**************/
    m_padDown = Sprite::create("button_mid.png");
    this->addChild(m_padDown,0,3);
    m_padDown->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.23,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.08));
    
    /***************上**************/
    m_padUp = Sprite::create("button_mid.png");
    this->addChild(m_padUp,0,5);
    m_padUp->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.23,
                                   GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.25));
    
    /***************旋转**************/
    m_padRot = Sprite::create("button_big.png");
    this->addChild(m_padRot,0,4);
    m_padRot->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.87,
                               GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.13));
    
    
    
    //消除5行
    m_padClear5 = Sprite::create("clear5btn.png");//Label::createWithSystemFont("清除5行", "Arial", 25);
    this->addChild(m_padClear5,0,10);
    m_padClear5->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.5 + 50,
                               60));
    m_padClear5->setAnchorPoint(Vec2(0.5, 0.5));
    //获取竖条
    m_padGetLine = Sprite::create("getlinebtn.png");//Label::createWithSystemFont("获得竖条", "Arial", 25);
    this->addChild(m_padGetLine,0,11);
    m_padGetLine->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.5+50,
                                   110));
    m_padGetLine->setAnchorPoint(Vec2(0.5, 0.5));
    
    
    //分数标签
    Sprite *scorestext = Sprite::create("score.png");
    this->addChild(scorestext);
    scorestext->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.74,
                                 GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.87));
    
    LabelAtlas *tscoreLabel = LabelAtlas::create("000000", "num.png", 12, 21, '0');
    this->addChild(tscoreLabel);
    tscoreLabel->setOpacity(50);
    tscoreLabel->setAnchorPoint(Vec2(1, 0.5));
    tscoreLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                  scorestext->getPositionY()-scorestext->getContentSize().height / 2 - 17));
    
    m_scoreLabel = LabelAtlas::create("0", "num.png", 12, 21, '0');
    this->addChild(m_scoreLabel);
    m_scoreLabel->setAnchorPoint(Vec2(1, 0.5));
    m_scoreLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                   scorestext->getPositionY()-scorestext->getContentSize().height / 2 - 17));
    
    
    //最高分
    Sprite *heightscores = Sprite::create("hi_score.png");
    this->addChild(heightscores);
    heightscores->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.73,
                                   GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.79));
    
    int bestscores;
    
    if (GetGameInstance->m_gameMode == 1) {
        bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores1");
    }else{
        bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores2");
    }
    
    char hiscores[32];
    sprintf(hiscores, "%d",bestscores);
    
    LabelAtlas *thiscoreLabel = LabelAtlas::create("000000", "num.png", 12, 21, '0');
    this->addChild(thiscoreLabel);
    thiscoreLabel->setOpacity(50);
    thiscoreLabel->setAnchorPoint(Vec2(1, 0.5));
    thiscoreLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                    heightscores->getPositionY()-heightscores->getContentSize().height / 2 - 17));
    
    m_hiscoreLabel = LabelAtlas::create(hiscores, "num.png", 12, 21, '0');
    this->addChild(m_hiscoreLabel);
    m_hiscoreLabel->setAnchorPoint(Vec2(1, 0.5));
    m_hiscoreLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                   heightscores->getPositionY()-heightscores->getContentSize().height / 2 - 17));
    
    
    //速度
    Sprite *speed = Sprite::create("speed.png");
    this->addChild(speed);
    speed->setAnchorPoint(Vec2(1, 0.5));
    speed->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                   GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.635));
    
    m_speedLabel = LabelAtlas::create("0", "num.png", 12, 21, '0');
    this->addChild(m_speedLabel);
    m_speedLabel->setAnchorPoint(Vec2(1, 0.5));
    m_speedLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                   GetGameInstance->m_gGame->m_bg->getContentSize().height *0.6));
    
    //等级
    Sprite *lv = Sprite::create("level.png");
    this->addChild(lv);
    lv->setAnchorPoint(Vec2(1, 0.5));
    lv->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                            GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.53));
    
    m_levelLabel = LabelAtlas::create("0", "num.png", 12, 21, '0');
    this->addChild(m_levelLabel);
    m_levelLabel->setAnchorPoint(Vec2(1, 0.5));
    m_levelLabel->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.8,
                                   GetGameInstance->m_gGame->m_bg->getContentSize().height *0.56));
    
    /***************开始**************/
    m_padStart = Sprite::create("button_small.png");
    this->addChild(m_padStart,0,6);
    m_padStart->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.56,
                               GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.245));
    
    /***************声音**************/
    m_padSound = Sprite::create("button_small.png");
    this->addChild(m_padSound,0,8);
    m_padSound->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.68,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.245));
    
    /***************菜单**************/
    m_padMenu = Sprite::create("button_small.png");
    this->addChild(m_padMenu,0,9);
    m_padMenu->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.81,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.245));
    
    /***************重置**************/
    m_padRest = Sprite::create("button_small.png");
    this->addChild(m_padRest,0,7);
    m_padRest->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.94,
                                 GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.245));
    
    
    
    
    //音乐图标
    m_musicicon = Sprite::create("musicicon.png");
    m_cafeiicon = Sprite::create("coffee.png");
    this->addChild(m_musicicon);
    this->addChild(m_cafeiicon);
    m_musicicon->setPosition(Vec2(GetGameInstance->m_gGame->m_bg->getContentSize().width * 0.77,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.41));
    m_cafeiicon->setPosition(Vec2(m_musicicon->getPositionX() - m_musicicon->getContentSize().width * 1.8,
                                GetGameInstance->m_gGame->m_bg->getContentSize().height * 0.413));
    m_cafeiicon->setOpacity(50);
    SetMusicIcon(GetGameInstance->m_gGame->m_bIsPlaySound);
}

void TEGamePad::SetMusicIcon(bool isshow)
{
    m_musicicon->setOpacity(isshow ? 255 : 50);
}

void TEGamePad::FlushGameScores()
{
    char scores[32];
    if (GetGameInstance->m_gGame->GetScores() < 999999) {
        sprintf(scores, "%d", GetGameInstance->m_gGame->GetScores());
    }else{
        sprintf(scores, "%d", 999999);
    }
    
    m_scoreLabel->setString(scores);
    
    if (GetGameInstance->m_gGame->GetScores() > atoi(m_hiscoreLabel->getString().c_str())) {
        m_hiscoreLabel->setString(scores);
        
        if (GetGameInstance->m_gameMode == 1) {
            UserDefault::getInstance()->setIntegerForKey("bestscores1", GetGameInstance->m_gGame->GetScores());
        }else{
            UserDefault::getInstance()->setIntegerForKey("bestscores2", GetGameInstance->m_gGame->GetScores());
        }
    }
}

void TEGamePad::FlushHeightScores()
{
    
    int bestscores;
    
    if (GetGameInstance->m_gameMode == 1) {
        bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores1");
    }else{
        bestscores = UserDefault::getInstance()->getIntegerForKey("bestscores2");
    }
    
    char hiscores[32];
    sprintf(hiscores, "%d",bestscores);
    m_hiscoreLabel->setString(hiscores);
}

void TEGamePad::SetSpeedText(std::string text)
{
    m_speedLabel->setString(text);
}
void TEGamePad::SetLvText(std::string text)
{
    m_levelLabel->setString(text);
}

void TEGamePad::MyLoop(float dt)
{
    m_nDownTime -= dt;
    //是否点击按钮
    if (m_nTouchPadTag != -1 && m_nTouchPadTag <= 3) {
        //达到间隔时间
        if (m_nDownTime <= 0) {
            m_nDownTime = 0.1;
            ButtonFunction(m_nTouchPadTag);
            
        }
    }
}

void TEGamePad::ButtonFunction(int buttonTag)
{
    if (buttonTag == 8) {
        GetGameInstance->m_gGame->SetIsPlayVideo();
        return;
    }else if (buttonTag == 7){
        GetGameInstance->m_gGame->OverAction();
//        ChangeScene::Create(k_scenetag_game);
        return;
    }
//    else if (buttonTag == 9){
//        ChangeScene::Create(k_scenetag_lv);
//        return;
//    }
    
    if (m_bstartgame == 0) {
        switch (buttonTag) {
            case 3:
                GetGameInstance->m_gGame->ChangeMode();
                break;
            case 5:
                GetGameInstance->m_gGame->ChangeMode();
                break;
            case 6:
                ChangeScene::Create(k_scenetag_lv);
                break;
            case 4:
                ChangeScene::Create(k_scenetag_lv);
                break;
            default:
                break;
        }
        return;
    }else if (m_bstartgame == 1) {
        switch (buttonTag) {
            case 1:
                GetGameInstance->m_gGame->SetLv(GetGameInstance->m_gGame->getLv() - 1 < 0 ? 9 : GetGameInstance->m_gGame->getLv() - 1);
                break;
            case 2:
                GetGameInstance->m_gGame->SetLv(GetGameInstance->m_gGame->getLv() + 1 >9 ? 0 : GetGameInstance->m_gGame->getLv() + 1);
                break;
            case 3:
                GetGameInstance->m_gGame->ChangeMode();
                break;
            case 5:
                GetGameInstance->m_gGame->ChangeMode();
                break;
            case 6:
                GetGameInstance->m_gGame->GameStart(true);
                break;
            case 4:
                GetGameInstance->m_gGame->GameStart(true);
                break;
            case 9:
                GetGameInstance->m_gGame->SetSpeed(GetGameInstance->m_gGame->getGameSpeed() + 1 > 9 ? 0 : GetGameInstance->m_gGame->getGameSpeed() + 1);
                break;
            default:
                break;
        }
        
        if (GetGameInstance->m_gGame->m_bIsPlaySound) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_move].c_str());
        }
        return;
    }
    
    if (GetGameInstance->m_gGame->getGmaeState() == k_gamestate_runing) {
        
        switch (buttonTag) {
            case 1: GetGameInstance->m_gGame->GetNowShape()->Move(m_shapeMoveDir_left);break;
            case 2: GetGameInstance->m_gGame->GetNowShape()->Move(m_shapeMoveDir_right);break;
            case 3: GetGameInstance->m_gGame->GetNowShape()->Move(m_shapeMoveDir_down);break;
            case 4: GetGameInstance->m_gGame->GetNowShape()->Rotation(); break;
            case 5: GetGameInstance->m_gGame->FustDown(); return;break;
            case 6: GetGameInstance->m_gGame->GamePause(); return;break;
            case 10: GetGameInstance->m_gGame->Clear5Line(); return;break;
            case 11: GetGameInstance->m_gGame->GetLine(); return;break;
                
            default:
                break;
        }
        
        if (GetGameInstance->m_gGame->m_bIsPlaySound) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_move].c_str());
        }
    }else{
        if (buttonTag == 6){
            GetGameInstance->m_gGame->GameResume();
            return;
        }
    }
}
bool TEGamePad::onTouchBegan(Touch* touch, Event  *event)
{
    Vec2 touchpos = touch->getLocation();
    touchpos = this->convertToNodeSpace(touchpos);
    
    Rect t_rect;
    
    //遍历4个按钮
    for (int i = 1; i <= 11; i++) {
        //获取按钮碰撞框
        LayerColor *button = (LayerColor*)this->getChildByTag(i);
        t_rect.origin = button->getPosition() - button->getContentSize() / 2;;
        t_rect.size = button->getContentSize();
        //是否被点击
        if (t_rect.containsPoint(touchpos)) {
            m_nTouchPadTag = i;
            button->setPositionY(button->getPositionY() - 3);
            
            //下落按钮属于持续效果  按下即激活
            if (m_nTouchPadTag <= 3) {
                ButtonFunction(m_nTouchPadTag);
            }
            break;
        }
    }
    m_nDownTime = 0.5;
    return true;
}
void TEGamePad::onTouchMoved(Touch *touch, Event *event)
{
    if (m_nTouchPadTag == -1 ) {
        return;
    }
    
    Vec2 touchpos = touch->getLocation();
    touchpos = this->convertToNodeSpace(touchpos);
    Rect rect;
    
    LayerColor *button = (LayerColor*)this->getChildByTag(m_nTouchPadTag);
    
    rect.origin = button->getPosition() - button->getContentSize() / 2;;
    rect.size = button->getContentSize();
    
    if (!rect.containsPoint(touchpos)) {
        m_nTouchPadTag = -1;
        button->setPositionY(button->getPositionY() + 3);
    }
}
void TEGamePad::onTouchEnded(Touch* touch, Event  *event)
{
    if (m_nTouchPadTag == -1 ) {
        return;
    }
    Vec2 touchpos = touch->getLocation();
    touchpos = this->convertToNodeSpace(touchpos);
    
    Rect rect;
    
    LayerColor *button = (LayerColor*)this->getChildByTag(m_nTouchPadTag);
    
    rect.origin = button->getPosition() - button->getContentSize() / 2;
    rect.size = button->getContentSize();
    
    if (rect.containsPoint(touchpos)) {
        if (m_nTouchPadTag > 3) {
            ButtonFunction(m_nTouchPadTag);
        }
    }
    
    
    m_nTouchPadTag = -1;
    button->setPositionY(button->getPositionY() + 3);
}











