LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_image_PATH := ../SDL_image
SDL_mixer_PATH := ../SDL_mixer
SDL_ttf_PATH := ../SDL_ttf
BOOST_PATH := ../boost
LUACPPINTERFACE_PATH := ../luacppinterface/LuaCppInterface
LUA_PATH := ../luacppinterface/lua/src
BOX2D_PATH := ../box2d/

LOCAL_CFLAGS := -std=gnu++11 -fexceptions -frtti

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(BOOST_PATH) \
	$(LOCAL_PATH)/$(SDL_image_PATH) \
	$(LOCAL_PATH)/$(SDL_mixer_PATH) \
	$(LOCAL_PATH)/$(SDL_ttf_PATH) \
	$(LOCAL_PATH)/$(LUACPPINTERFACE_PATH) \
	$(LOCAL_PATH)/$(LUA_PATH) \
	$(LOCAL_PATH)/$(BOX2D_PATH)

# Add your application source files here...

FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
	$(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf
LOCAL_STATIC_LIBRARIES := luacppinterface lua box2d

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
