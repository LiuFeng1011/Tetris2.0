#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto keylistener = EventListenerKeyboard::create();
    keylistener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyReleased,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener,this);
    
    return true;
}


void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
    switch (keyCode)
    {
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
void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
