//
//  TEMenu.h
//  Tetris
//
//  Created by 刘 峰 on 14/12/7.
//
//

#ifndef __Tetris__TEMenu__
#define __Tetris__TEMenu__

#include <stdio.h>
#include "TEGameCtrl.h"

class TEMenu : public Layer {
    
    
public:
    static Scene* createScene();
    static TEMenu *Create();
    void MyInit();
};

#endif /* defined(__Tetris__TEMenu__) */
