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
#include <core/gltools.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <core/gworldview.h>
#include <core/gworldmodel.h>
#include <shapes/gobjcircle.h>
#include <core/gobjcamera.h>
#include <shapes/gobjrect.h>

std::shared_ptr<GWorldModel> gWorldModel;
std::shared_ptr<GWorldView> gWorldView;

/// Moving circle for test scene
class TestCircle : public GObjCircle {
public:
    using GObjCircle::GObjCircle;

    void recalc(DeltaT) override {
        n += 0.001;
        this->getPosR().x += std::sin(n) / 50.0;
        this->getPosR().y += std::cos(n) / 50.0;
        if (intersectObjList().size() > 0)
            setColorRGBA({1.0f, 0, 0, 0});
        else
            setColorRGBA({1.0f, 1.0f, 1.0f, 1.0f});
    }
private:
    float n = 0;
};

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    gWorldModel = std::make_shared<GWorldFlappy>();
    gWorldView = std::make_shared<GWorldView>(gWorldModel);

    //Fill scene with objects
    auto gObjSubContainer1 = gWorldModel->getRoot()->ADD_CHILD(GObj,POS(0,0,0));
    auto gObjSubContainer2 = gObjSubContainer1->ADD_CHILD(GObj,POS(-15,0,0));
    gObjSubContainer1->ADD_CHILD(GObjRect,20,20,POS(-10,-10,0));
    gObjSubContainer2->ADD_CHILD(TestCircle,4,POS(-20,-20,0));
    gObjSubContainer2->ADD_CHILD(TestCircle,6,POS(20,-20,0));
    gObjSubContainer2->ADD_CHILD(TestCircle,8,POS(-20,20,0));
    gObjSubContainer2->ADD_CHILD(TestCircle,10,POS(20,20,0));
    auto gObjCamera = std::make_shared<GObjCamera>(100,1.0,GObj::Pos({0,0,0}));
    gWorldModel->getRoot()->addChild<GObjCamera>(gObjCamera);
    gWorldModel->setActiveCamera(gObjCamera);

    gWorldView->resize(w, h);
    gWorldView->init();

    return true;
}

void renderFrame() {
    if (gWorldView != nullptr) {
        gWorldView->redraw();
    }
    if (gWorldModel != nullptr)
        gWorldModel->run(); //only for test
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    if (gWorldView != nullptr)
        gWorldView->resize(width, height);
    else
        setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}
