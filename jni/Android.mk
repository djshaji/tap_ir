LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := ir.lv2 
LOCAL_LDLIBS := -llog -lsndfile -lfftw3 -lzita-convolver -lsamplerate -lfftw3f
LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := ir.cc 
include $(BUILD_SHARED_LIBRARY) 
LOCAL_C_INCLUDES := dsp/ plugin/
