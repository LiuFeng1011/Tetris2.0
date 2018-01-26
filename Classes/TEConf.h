//
//  TEConf.h
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#ifndef __Tetris__TEConf__
#define __Tetris__TEConf__

#include <stdio.h>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

//预备图形的数量
#define READY_SHAPE_COUNT 2

#define MAX_COL_COUNT 10
#define MAX_ROW_COUNT 20

#define PONIT_WIDTH 30
#define PONIT_HIGHT 30


#define WIN_SIZE_WIDTH Director::getInstance()->getVisibleSize().width
#define WIN_SIZE_HEIGHT Director::getInstance()->getVisibleSize().height

enum enAudioTag {
    k_audio_music,
    k_audio_clear,
    k_audio_move,
    k_audio_change,
    k_audio_toground,
};

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static const std::string g_gameAudio[5] = {
    "audio/music.mp3",
    "audio/clear.mp3",
    "audio/move.mp3",
    "audio/change.mp3",
    "audio/toground.mp3",
};
#else
static const std::string g_gameAudio[5] = {
    "music.mp3",
    "clear.mp3",
    "move.mp3",
    "change.mp3",
    "toground.mp3",
};
#endif

static std::function<void(Node *n)> g_ActionFun = [](Node*n){n->getParent()->removeChild(n,true);};

//触摸层
class WHTouchLayer : public Layer
{
public:
    WHTouchLayer();
    //触摸回调函数
    virtual bool TouchBegan(Touch* touch, Event* event){ return true; };
    virtual void TouchMoved(Touch* touch, Event* event){};
    virtual void TouchEnded(Touch* touch, Event* event){};
};
//屏蔽触摸t秒
void EatGameTouchForTime(float t);
#endif /* defined(__Tetris__TEConf__) */
