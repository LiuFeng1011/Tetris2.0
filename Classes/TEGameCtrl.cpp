//
//  TEGameCtrl.cpp
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#include "TEGameCtrl.h"

static TEGameCtrl *m_gGameCtrl = nullptr;
TEGameCtrl *TEGameCtrl::GetGameCtrl()
{
    if (m_gGameCtrl == nullptr) {
        m_gGameCtrl = new TEGameCtrl();
    }
    return m_gGameCtrl;
}
TEGameCtrl::TEGameCtrl()
{
    m_gGame = nullptr;
    m_gGamePad = nullptr;
    m_gameMode = 0;
}
TEGameCtrl::~TEGameCtrl()
{
    
}

void TEGameCtrl::Clear()
{
    m_gGame = nullptr;
    m_gGamePad = nullptr;
}


#pragma mark - change scene


ChangeScene *ChangeScene::Create(enGameSceneTag tag)
{
    
    GetGameInstance->Clear();
    switch (tag) {
        case k_scenetag_menu:
            Director::getInstance()->replaceScene(TEMenu::createScene());
            break;
        case k_scenetag_game:
            Director::getInstance()->replaceScene(TEGame::createScene());
            break;
        case k_scenetag_lv:
            Director::getInstance()->replaceScene(TEGame::createSceneBylv());
            break;
        default:
            break;
    }
    
    
//    Scene *s = Scene::create();
//    ChangeScene *c = new ChangeScene();
//    c->MyInit(tag);
//    s->addChild(c);
//    Director::getInstance()->replaceScene(s);
    return nullptr;
}
void ChangeScene::MyInit(enGameSceneTag tag)
{
    m_sceneTag = tag;
    std::function<void ()> f = [&](){
        switch (m_sceneTag) {
            case k_scenetag_menu:
                Director::getInstance()->replaceScene(TEMenu::createScene());
                break;
            case k_scenetag_game:
                Director::getInstance()->replaceScene(TEGame::createScene());
                break;
            case k_scenetag_lv:
                Director::getInstance()->replaceScene(TEGame::createSceneBylv());
                break;
            default:
                break;
        }
    };
    this->runAction(Sequence::create(DelayTime::create(0.02),CallFunc::create(f), NULL));
}










