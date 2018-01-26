//
//  TEGame.h
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#ifndef __Tetris__TEGame__
#define __Tetris__TEGame__

#include <stdio.h>
#include "TEGameCtrl.h"
enum enGameState{
    k_gamestate_runing,
    k_gamestate_pause,
    k_gamestate_over,
    
};
enum enPayTag{
    k_pay_normalgame = 0,       //激活正常模式
    k_pay_othergame = 1,        //激活异形模式
    k_pay_clear5line = 2,       //消除5行
    k_pay_getline = 3,          //获取竖条
    
};
class TEBaseShape;

class TEGameStartLayer : public Layer {
    
public:
    static TEGameStartLayer *Create();
    void MyInit();
    
    bool onTouchBegan(Touch* touch, Event  *event);
    void onTouchMoved(Touch *touch, Event *event){};
    void onTouchEnded(Touch* touch, Event  *event){};
};

class TEGame : public Layer{
    int m_gamePointArr[MAX_ROW_COUNT][MAX_COL_COUNT];
    std::vector<Sprite *> pointArr;
    TEBaseShape *m_Squares[READY_SHAPE_COUNT];
    
    //图形下落时间
    float m_nShapeDownTime;
    
    //游戏状态
    enGameState m_kGameState;
    Node *m_NextOverview;
    //游戏分数
    int m_nScores;
    //游戏速度
    int m_nLv;
    int m_nSpeed;
    float m_nNextAddSpeedScores;
    
    
    MenuItemSprite *noritem;
    MenuItemSprite *yixingitem;
    Layer *m_menuLayer;
    
    float m_nNextCdTime;
    
    bool m_bistouch;
    
    int m_nClearCount;
    
    int m_nBuyClear5Count;
    int m_bShowBuyClear5;
public:
    bool islv;
    bool m_bIsPlaySound;
    Sprite *m_bg;
    ~TEGame();
    static Scene* createScene();
    static Scene* createSceneBylv();
    static TEGame *Create();
    static TEGame *CreateByLv();
    void MyInit();
    //开始菜单
    void CreateMenuLayer();
    void ModeButtonCB(Ref *obj);
    void ChangeMode();
    //开始游戏
    void GameStart(bool isshowui);
    
    //注册界面
    void CreateRegLayer();
    void RegCB(Ref*obj);
    void CancelCB(Ref*obj);
    void RegSuccess(Ref*obj);
    void RegFailed(Ref*obj);
    
    void GamePause();
    void GameResume();
    enGameState getGmaeState(){return m_kGameState;};
    void MyLoop(float dt);
    
    void FlashMatrix();
    void ClearShapeColor();
    
    
    bool TestNewShape(TEBaseShape *s);
    
    void NextShape();
    void ShapeDown();
    
    void SetIsPlayVideo();
    
    //增加游戏速度
    void SetSpeed(int speed);
    //当前游戏速度
    int getGameSpeed(){return m_nSpeed;};
    
    //增加等级
    void SetLv(int lv);
    //当前等级
    int getLv(){return m_nLv;};
    
    //清除已经填满的行
    void ClearFullLine();
    //整体下落
    void LineDown(int startLine);
    
    //获取当前的图形
    TEBaseShape * GetNowShape(){return m_Squares[0];};
    
    //下一个预览
    void ShowNextShape();
    
    void GameOver();
    
    void FustDown();
    
    void AddScores(int s);
    int GetScores(){return m_nScores;};
    
    void OverAction();
    
    //消除5行
    void Clear5Line();
    void Clear5LineBuyFun();
    //获取竖条
    void GetLine();
    void GetLineBuyFun();
public:
    void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);
    bool onTouchBegan(Touch* touch, Event  *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch* touch, Event  *event);
    
};

class TEGameOverLayer : public Layer {
    
    
public:
    static TEGameOverLayer *Create();
    void MyInit();
    
    void ReplayCB(Ref *obj);
    
public:
    bool onTouchBegan(Touch* touch, Event  *event);
    void onTouchMoved(Touch *touch, Event *event){};
    void onTouchEnded(Touch* touch, Event  *event){};
};

class TEGamePad : public Layer {
    
private :
    //gamepad
    Sprite *m_padLeft;
    Sprite *m_padRight;
    Sprite *m_padDown;
    Sprite *m_padUp;
    Sprite *m_padRot;
    
    Sprite *m_padStart;
    Sprite *m_padRest;
    Sprite *m_padSound;
    Sprite *m_padMenu;
    
    Sprite *m_padClear5;
    Sprite *m_padGetLine;
    
    
    int m_nTouchPadTag;
    
    //下落间隔时间
    float m_nDownTime;
    
    //分数
    LabelAtlas *m_scoreLabel;
    LabelAtlas *m_hiscoreLabel;
    LabelAtlas *m_speedLabel;
    LabelAtlas *m_levelLabel;
    
    Sprite *m_musicicon;
    
public:
    int m_bstartgame;
    Sprite *m_cafeiicon;
    static TEGamePad *Create();
    void MyInit();
    void MyLoop(float dt);
    
    void SetMusicIcon(bool isshow);
    
    void FlushGameScores();
    
    void FlushHeightScores();
    
    void SetSpeedText(std::string text);
    void SetLvText(std::string text);
    
    void ButtonFunction(int buttonTag);
public:
    bool onTouchBegan(Touch* touch, Event  *event);
    void onTouchMoved(Touch *touch, Event *event);
    void onTouchEnded(Touch* touch, Event  *event);
};
#endif /* defined(__Tetris__TEGame__) */
