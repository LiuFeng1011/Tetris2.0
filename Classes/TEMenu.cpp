//
//  TEMenu.cpp
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#include "TEMenu.h"

Scene* TEMenu::createScene()
{
    Scene *s = Scene::create();
    TEMenu *menu = TEMenu::Create();
    s->addChild(menu);
    return s;
}
TEMenu *TEMenu::Create()
{
    TEMenu * menu = new TEMenu();
    menu->autorelease();
    menu->MyInit();
    return menu;
}
void TEMenu::MyInit()
{
    
}