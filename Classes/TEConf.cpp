//
//  TEConf.cpp
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#include "TEConf.h"
#pragma mark -
#pragma mark 触摸层

WHTouchLayer::WHTouchLayer()
{
    //============触摸=========
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(WHTouchLayer::TouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WHTouchLayer::TouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WHTouchLayer::TouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void EatGameTouchForTime(float t)
{
    WHTouchLayer *l = new WHTouchLayer();
    l->autorelease();
    Director::getInstance()->getRunningScene()->addChild(l,INT_MAX);
    
    l->runAction(Sequence::create(DelayTime::create(t),CallFuncN::create(g_ActionFun), NULL));
}
