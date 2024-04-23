LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := ir.lv2 
LOCAL_SRC_FILES := ir.cc 
include $(BUILD_SHARED_LIBRARY) 
LOCAL_C_INCLUDES := dsp/ plugin/
