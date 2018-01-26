//
//  LFPay.h
//  ProEarth
//
//  Created by reddog on 14/9/11.
//
//

#ifndef __ProEarth__LFPay__
#define __ProEarth__LFPay__

#include <iostream>

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif
#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"
class PayCtrl {
    int m_pid;
    cocos2d::ccMenuCallback m_paySuccessCB;
    cocos2d::ccMenuCallback m_payFailCB;
    PayCtrl();
public:
    static PayCtrl *GetInstance();
    
    bool isShowUI;
    
    void Pay(int pid,const cocos2d::ccMenuCallback& paySuccess ,const cocos2d::ccMenuCallback& payFailCB);
    void CreateEnterLayer();
    
    void CancelCB(cocos2d::Ref *obj);
    void EnterCB(cocos2d::Ref *obj);
    
    void ErrAlert();
    void OkAlert();
    void Alert(bool isSuccess);
    
    
    void PaySuccess();
    void PayFail();
};

#ifdef __cplusplus
extern "C" {
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /*
     * Class:     com_reddog
     * Method:    sendPurchaseOK
     * Signature: (ILjava/lang/String;)V
     */
    void Java_org_cocos2dx_cpp_AppActivity_sendPurchaseOK(JNIEnv *env, jobject jobj);
    /*
     * Class:     com_reddog
     * Method:    sendPurchaseError
     * Signature: (I)V
     */
    void Java_org_cocos2dx_cpp_AppActivity_sendPurchaseError(JNIEnv *env, jobject jobj);
    
#endif
#ifdef __cplusplus
}
#endif
#endif /* defined(__ProEarth__LFPay__) */
