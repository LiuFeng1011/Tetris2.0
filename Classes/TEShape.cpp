//
//  TEShape.cpp
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#include "TEShape.h"

TEBaseShape::TEBaseShape(){};
TEBaseShape::~TEBaseShape(){};
void TEBaseShape::MyInit(int mode){
    if (mode == 0) {
        myType = (TEShapeType)(CCRANDOM_0_1() * 7);
    }else{
        myType = (TEShapeType)(CCRANDOM_0_1() * SHAPE_TYPE_COUNT);
    }
    
    posx = MAX_COL_COUNT / 2 - 1;
    posy = MAX_ROW_COUNT - 1;
    
    m_nMyDir = 0;
}


//旋转
void TEBaseShape::Rotation(){
    
    //清除当前图形
    GetGameInstance->m_gGame->ClearShapeColor();
    //首先判断是否可以旋转
    
    TEBaseShape *testshape = new TEBaseShape() ;
    testshape->MyInit(0);
    testshape->posx = posx;
    testshape->posy = posy;
    testshape->myType = myType;
    testshape->m_nMyDir = m_nMyDir+1;
    //只有4个方向
    if (testshape->m_nMyDir > 3) {
        testshape->m_nMyDir = 0;
    }
    
    if (GetGameInstance->m_gGame->TestNewShape(testshape)) {
        m_nMyDir = testshape->m_nMyDir ;
    }
    delete testshape;
    //显示当前图形
    GetGameInstance->m_gGame->FlashMatrix();
    
    
    if (GetGameInstance->m_gGame->m_bIsPlaySound) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_change].c_str());
    }
    
};
//移动
bool TEBaseShape::Move(ShapeMoveDir dir)
{
    bool ret = true;
    //清除当前图形
    GetGameInstance->m_gGame->ClearShapeColor();
    
    
    TEBaseShape *testshape = new TEBaseShape() ;
    testshape->MyInit(0);
    testshape->posx = posx;
    testshape->posy = posy;
    testshape->myType = myType;
    testshape->m_nMyDir = m_nMyDir;
    
    if (dir == m_shapeMoveDir_down) {
        testshape->posy = posy - 1;
        
        //首先判断是否可以移动
        if (!GetGameInstance->m_gGame->TestNewShape(testshape)) {
            GetGameInstance->m_gGame->NextShape();
            
            if (GetGameInstance->m_gGame->m_bIsPlaySound) {
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(g_gameAudio[k_audio_toground].c_str());
            }
            
            ret = false;
        }else{
            posy -= 1;
            
        }
        
    }else if (dir == m_shapeMoveDir_left) {
        testshape->posx = posx - 1;
        
        //首先判断是否可以移动
        if (GetGameInstance->m_gGame->TestNewShape(testshape)) {
            posx -= 1;
        }
    }else if (dir == m_shapeMoveDir_right) {
        testshape->posx = posx + 1;
        
        //首先判断是否可以移动
        if (GetGameInstance->m_gGame->TestNewShape(testshape)) {
            posx += 1;
        }
    }
    
    delete testshape;
    //显示当前图形
    GetGameInstance->m_gGame->FlashMatrix();
    
    return ret;
};



