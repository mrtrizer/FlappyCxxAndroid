/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.gl2jni;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;

import java.io.File;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        GL2JNILib.click((int)event.getX(), (int)event.getY());
        return true;
    }

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        GL2JNILib.amgr = getResources();
        GL2JNILib.packageName = getPackageName();
        GL2JNILib.preinit();
        mView = new GL2JNIView(getApplication());
	setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
