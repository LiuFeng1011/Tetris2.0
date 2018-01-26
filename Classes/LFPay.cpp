//
//  LFPay.cpp
//  ProEarth
//
//  Created by reddog on 14/9/11.
//
//

#include "LFPay.h"
using namespace cocos2d;
static PayCtrl *m_gPayInstance = nullptr;

PayCtrl *PayCtrl::GetInstance(){
    if (m_gPayInstance == nullptr) {
        m_gPayInstance = new PayCtrl();
    }
    return m_gPayInstance;
};

PayCtrl::PayCtrl(){
    
    isShowUI = true;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,CLASS_NAME,"isShowPayUI", "()Z");
    if (isHave)
    {
        jboolean boolTemp = minfo.env->CallStaticBooleanMethod(minfo.classID, minfo.methodID);
        
        if(boolTemp == JNI_FALSE )
        {
            isShowUI = false;
        }
    }
#endif
    
}

void PayCtrl::Pay(int pid,const ccMenuCallback& paySuccessCB,const ccMenuCallback& payFailCB)
{
    m_pid = pid;
    m_paySuccessCB = paySuccessCB;
    m_payFailCB = payFailCB;
    
//    k_pay_clear5line = 2,
//    k_pay_getline = 3,
    if ( isShowUI && (m_pid == 2 || m_pid == 3)) {
        CreateEnterLayer();
    }else{
        EnterCB(nullptr);
    }
    
    return;

}


void PayCtrl::CreateEnterLayer()
{
    Layer *l = Layer::create();
    Director::getInstance()->getRunningScene()->addChild(l,0,12345);
    
    Size winsize = Director::getInstance()->getVisibleSize();
    
    std::string texturename;
    
    if (m_pid == 2) {
        texturename = "clear5text.png";
    }else if(m_pid == 3){
        texturename = "getlinetext.png";
    }
    
    
    Sprite *n = Sprite::create(texturename);
    l->addChild(n);
    n->setPosition(Vec2(winsize.width /2, winsize.height / 2));
    
    //button
    
    Sprite *btn1 = Sprite::create("close.png");
    Sprite *btn1_sel = Sprite::create("close.png");
    btn1_sel->setColor(Color3B(150, 150, 150));
    MenuItemSprite *item1 = MenuItemSprite::create(btn1, btn1_sel, CC_CALLBACK_1(PayCtrl::CancelCB, this));
    item1->setPosition(Vec2(n->getContentSize().width - btn1->getContentSize().width *0.65,
                            n->getContentSize().height - btn1->getContentSize().height *0.65));
    
    
    Sprite *btn2 = Sprite::create("enter.png");
    Sprite *btn2_sel = Sprite::create("enter.png");
    btn2_sel->setColor(Color3B(150, 150, 150));
    MenuItemSprite *item2 = MenuItemSprite::create(btn2, btn2_sel, CC_CALLBACK_1(PayCtrl::EnterCB, this));
    item2->setPosition(Vec2(n->getContentSize().width*0.5, 0));
    
//    Sprite *b_n = Sprite::create(bname);
//    Sprite *b_s = Sprite::create(bname);
//    b_s->setColor(Color3B(150, 150, 150));
//    
//    MenuItemSprite *item = MenuItemSprite::create(b_n, b_s, CC_CALLBACK_1(TEGame::RegCB, this));
//    item->setPosition(Vec2(bg->getContentSize().width / 2, b_n->getContentSize().height * 0.7));
    
    //close;
//    Sprite *cb_n = Sprite::create("close.png");
//    Sprite *cb_s = Sprite::create("close.png");
//    cb_s->setColor(Color3B(150, 150, 150));
//    
//    MenuItemSprite *citem = MenuItemSprite::create(cb_n,cb_s, CC_CALLBACK_1(TEGame::CancelCB, this));
//    citem->setPosition(Vec2(bg->getContentSize().width-cb_n->getContentSize().width * 0.6, bg->getContentSize().height -cb_n->getContentSize().height * 0.6));
    
    Menu *m = Menu::create(item1,item2, NULL);
    n->addChild(m);
    m->setPosition(Vec2(0,0));
    
    
}
void PayCtrl::CancelCB(Ref *obj)
{
    PayFail();
    Director::getInstance()->getRunningScene()->removeChildByTag(12345,true);
}
void PayCtrl::EnterCB(Ref *obj)
{
    Director::getInstance()->getRunningScene()->removeChildByTag(12345,true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    JniMethodInfo JNIinfo;//定义Jni函数信息结构体
    //    bool isHave = JniHelper::getMethodInfo(JNIinfo,CLASS_NAME,"purchaseGoods", "(Ljava/lang/String;)V");
    CCLog("jni:InAppPurchase");
    bool isHave = JniHelper::getStaticMethodInfo(JNIinfo,CLASS_NAME,"getActivity","()Ljava/lang/Object;");
    jobject jobj;
    if (!isHave) {
        CCLog("jni:此函数不存在");
        return ;
    }else{
        jobj = JNIinfo.env->CallStaticObjectMethod(JNIinfo.classID, JNIinfo.methodID);
        if (jobj!=NULL) {
            CCLog("正确获取到jobj");
        }else
        {
            CCLog("未正确获取到jobj");
            return ;
        }
        
        isHave = JniHelper::getMethodInfo(JNIinfo,CLASS_NAME,"Context", "(I)V");
        if (!isHave) {
            CCLog("purchaseGoods:此函数不存在");
            return ;
        }else
        {
            JNIinfo.env->CallVoidMethod(jobj, JNIinfo.methodID,m_pid);
            CCLog("jni-purchaseGoods函数执行完毕");
            return ;
        }
        return ;
    }
#else
    PaySuccess();
    //PayFail();
#endif
}


void PayCtrl::PaySuccess()
{
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create(CC_CALLBACK_0(PayCtrl::OkAlert, this)), NULL));
}

void PayCtrl::PayFail()
{
    Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.1),CallFunc::create(CC_CALLBACK_0(PayCtrl::ErrAlert, this)), NULL));
}

void PayCtrl::ErrAlert()
{
    Alert(false);
}
void PayCtrl::OkAlert()
{
    Alert(true);
}
void PayCtrl::Alert(bool isSuccess)
{
//    Size winsize = Director::getInstance()->getVisibleSize();
//    
//    std::string str = "";
    
    if (isSuccess) {
        m_paySuccessCB(nullptr);
//        str ="支付成功!";
    }else{
        m_payFailCB(nullptr);
//        str ="支付失败!";
    }
    
//    Label *l = Label::createWithSystemFont(str,"Arial",25);
//    Director::getInstance()->getRunningScene()->addChild(l,INT_MAX);
//    l->setPosition(Vec2(winsize.width /2, winsize.height / 2));
//    l->runAction(Sequence::create(DelayTime::create(1.4), CallFuncN::create([](Node *n){n->getParent()->removeChild(n, true); }), NULL));
//    
//    LayerColor *labelbg = LayerColor::create(Color4B(0, 0, 0, 150), l->getContentSize().width + 10, l->getContentSize().height + 5);
//    Director::getInstance()->getRunningScene()->addChild(labelbg,INT_MAX - 1);
//    labelbg->setPosition(Vec2(winsize.width /2 - labelbg->getContentSize().width / 2, winsize.height / 2 - labelbg->getContentSize().height / 2));
//    labelbg->runAction(Sequence::create(DelayTime::create(1.4), CallFuncN::create([](Node *n){n->getParent()->removeChild(n, true); }), NULL));
//    
//    l->setOpacity(0);
//    l->runAction(Sequence::create(FadeIn::create(0.2),DelayTime::create(1),FadeOut::create(0.2), NULL));
//    
//    labelbg->setOpacity(0);
//    labelbg->runAction(Sequence::create(FadeIn::create(0.2),DelayTime::create(1),FadeOut::create(0.2), NULL));
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void Java_org_cocos2dx_cpp_AppActivity_sendPurchaseOK(JNIEnv *env, jobject jobj)
{
    PayCtrl::GetInstance()->PaySuccess();
}

void Java_org_cocos2dx_cpp_AppActivity_sendPurchaseError(JNIEnv *env, jobject jobj)
{
    PayCtrl::GetInstance()->PayFail();
}
#endif
