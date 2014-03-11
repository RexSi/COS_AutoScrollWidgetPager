LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libMainPage


LOCAL_GAIA_SYSTEM_RES := true
LOCAL_GAIA_APP_LIBRARY := true

LOCAL_PACKAGE_NAME := AutoScrollViewPagerTest
LOCAL_PRELINK_MODULE := false

LOCAL_GAIA_R_PACKAGES := \
	AutoScrollViewPagerTest \

LOCAL_SRC_FILES:= \
	AutoScrollWidgetPager.cpp \
	MainPage.cpp \


LOCAL_SHARED_LIBRARIES := \
	libapi.$(strip $(SDK_VERSION)) \

LOCAL_C_INCLUDES += \
	$(COMMON_EXTERNAL_DIR)/stlport/stlport \

LOCAL_MODULE_TAGS := eng debug

common_CFLAGS := -gstabs -O0 -g -c
LOCAL_CPPFLAGS += $(common_CFLAGS)

include $(BUILD_SHARED_LIBRARY)
