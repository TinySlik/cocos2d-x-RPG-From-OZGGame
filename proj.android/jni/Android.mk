LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libs/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libs/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../libs/JsonBox/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../libs
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../libs/JsonBox/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, D:\root\Temp\cocos2d-x-2.2.5)
$(call import-add-path, D:\root\Temp\cocos2d-x-2.2.5\cocos2dx\platform\third_party\android\prebuilt)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
