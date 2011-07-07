LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := gs2d

GS2D_SOURCE_RELATIVE_PATH = ../../../../../src
GS2D_SOURCE_PATH = $(LOCAL_PATH)$(GS2D_SOURCE_RELATIVE_PATH)
VENDORS_PATH = $(LOCAL_PATH)../../../../../../vendors

LOCAL_C_INCLUDES := \
	$(VENDORS_PATH)/BoostSDK/ \
	$(VENDORS_PATH)/libzip/ \
	$(VENDORS_PATH)/soil/

LOCAL_STATIC_LIBRARIES := libzip libsoil

LOCAL_CFLAGS    := -Werror -DANDROID=1

LOCAL_DEFAULT_CPP_EXTENSION := cpp 

LOCAL_SRC_FILES := \
	$(GS2D_SOURCE_RELATIVE_PATH)/gs2d.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/gs2dGLES2.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/gs2dGLES2Shader.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/gs2dGLES2Sprite.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/gs2dGLES2Texture.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Input/Android/gs2dAndroidInput.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Audio/Android/gs2dAndroidAudio.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/Platform.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/Logger.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/FileLogger.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/android/Platform.android.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/GLES2RectRenderer.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/android/ZipFileManager.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/android/NativeCommandForwarder.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Platform/android/NativeCommandAssembler.cpp \
	$(GS2D_SOURCE_RELATIVE_PATH)/Video/GLES2/GLES2UniformParameter.cpp

LOCAL_LDLIBS := -ldl -lz -llog -lGLESv2

# include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)