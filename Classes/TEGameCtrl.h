//
//  TEGameCtrl.h
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#ifndef __Tetris__TEGameCtrl__
#define __Tetris__TEGameCtrl__

#include <stdio.h>
#include "TEConf.h"

#include "TEGame.h"
#include "TEShape.h"
#include "TEMenu.h"



//游戏单例
class TEGame;
class TEGamePad;
class TEGameCtrl  {
    
public:
    int m_gameMode;
    
    TEGameCtrl();
    ~TEGameCtrl();
    static TEGameCtrl *GetGameCtrl();
    
    
    TEGame *m_gGame;
    TEGamePad *m_gGamePad;
    
    void Clear();
};

enum enGameSceneTag{
    k_scenetag_menu,
    k_scenetag_game,
    k_scenetag_lv,
    
};
class ChangeScene : public Layer {
    
    enGameSceneTag m_sceneTag;
public:
    static ChangeScene *Create(enGameSceneTag tag);
    void MyInit(enGameSceneTag tag);
    
    
};
#define GetGameInstance TEGameCtrl::GetGameCtrl()

#endif /* defined(__Tetris__TEGameCtrl__) */
