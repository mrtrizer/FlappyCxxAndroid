/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <gl/gltools.h>
#include <gl/gltexture.h>
#include <gl/glviewfactory.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <ctrl.h>

JNIEnv * gEnv = 0;
jclass gClass;

class GLViewFactoryAndroid : public GLViewFactory {
public:
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string name) const override {
        char const *path = "bird.png";
        jmethodID mid;
        mid = gEnv->GetStaticMethodID(gClass, "loadBitmap",
                                 "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
        jstring pathUTF = gEnv->NewStringUTF(path);
        jobject bitmap = gEnv->CallStaticObjectMethod(gClass, mid, pathUTF);
        gEnv->DeleteLocalRef(pathUTF);
        gEnv->NewGlobalRef(bitmap);

        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getWidth","()I");
        int width = gEnv->CallIntMethod(bitmap,mid);
        LOGI("Width: %d",width);
        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getHeight","()I");
        int height = gEnv->CallIntMethod(bitmap,mid);
        LOGI("Height: %d",width);
        jintArray array = gEnv->NewIntArray(width * height);
        gEnv->NewGlobalRef(array);
        //getPixels(int[] pixels, int offset, int stride, int x, int y, int width, int height)
        mid = gEnv->GetMethodID(gEnv->GetObjectClass(bitmap),"getPixels","([IIIIIII)V");
        gEnv->CallVoidMethod(bitmap,mid,array,0,width,0,0,width,height);
        jint *pixels = gEnv->GetIntArrayElements(array, 0);
//        gEnv->ReleaseIntArrayElements(array, pixels, 0);
//        gEnv->DeleteGlobalRef(array);
//        gEnv->DeleteGlobalRef(png);

        char * bits = new char[64 * 64 * 4]();
        for (int i = 0; i < 64 * 64; i++)
            *(unsigned int *)&bits[i * 4] = 0xff00ffff;
        auto result = std::make_shared<GLTexture>((char*)pixels, width, height);
        return result;
    }
};

std::shared_ptr<GLWorldView> gWorld;
std::shared_ptr<Ctrl> flappyCtrl;

void setupGraphics(int w, int h) {
    gWorld = std::make_shared<GLWorldView>(std::make_shared<GLViewFactoryAndroid>());
    flappyCtrl = std::make_shared<Ctrl>(gWorld);

    static bool initFlag = false;

    if (initFlag)
        return;

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    flappyCtrl->init();
    flappyCtrl->resize(w, h);
    initFlag = true;
}

void renderFrame() {
    flappyCtrl->glRedraw();
    flappyCtrl->step();
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jclass obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_click(JNIEnv * env, jobject obj, jint x, jint y);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    LOGI("1");
    setupGraphics(width, height);
    flappyCtrl->resize(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jclass cla)
{
    gEnv = env;
    gClass = cla;
    LOGI("1");
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_click(JNIEnv * env, jobject obj, jint x, jint y)
{
    LOGI("2");
    flappyCtrl->mouseClick(x,y);
}
//
////https://habrahabr.ru/post/222997/
//JNIEXPORT void JNICALL JavaCritical_com_android_gl2jni_GL2JNILib_createTexture(jint width, jint height, jbyte* buf) {
//
//}