LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS := -fno-strict-aliasing

LOCAL_SRC_FILES := \
    gui.cpp \
    resources.cpp \
    pages.cpp \
    text.cpp \
    image.cpp \
    action.cpp \
    console.cpp \
    fill.cpp \
    button.cpp \
    checkbox.cpp \
    fileselector.cpp \
    progressbar.cpp \
    object.cpp \
    slidervalue.cpp \
    listbox.cpp \
    keyboard.cpp \
    input.cpp \
    blanktimer.cpp \
    partitionlist.cpp \
    mousecursor.cpp \
    scrolllist.cpp \
    patternpassword.cpp \
    textbox.cpp \
    terminal.cpp \
    twmsg.cpp

ifneq ($(TW_DELAY_TOUCH_INIT_MS),)
    LOCAL_CFLAGS += -DTW_DELAY_TOUCH_INIT_MS=$(TW_DELAY_TOUCH_INIT_MS)
endif

ifneq ($(TWRP_CUSTOM_KEYBOARD),)
    LOCAL_SRC_FILES += $(TWRP_CUSTOM_KEYBOARD)
else
    LOCAL_SRC_FILES += hardwarekeyboard.cpp
endif

LOCAL_SHARED_LIBRARIES += libminuitwrp libc libstdc++ libaosprecovery libselinux
ifeq ($(shell test $(PLATFORM_SDK_VERSION) -ge 26; echo $$?),0)
    LOCAL_SHARED_LIBRARIES += libziparchive
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/../otautil/include
else
    LOCAL_SHARED_LIBRARIES += libminzip
    LOCAL_CFLAGS += -DUSE_MINZIP
endif
ifeq ($(TARGET_USERIMAGES_USE_EXT4), true)
    ifeq ($(shell test $(PLATFORM_SDK_VERSION) -le 28; echo $$?),0)
        LOCAL_C_INCLUDES += system/extras/ext4_utils \
            system/extras/ext4_utils/include \
            $(LOCAL_PATH)/../crypto/ext4crypt
        LOCAL_SHARED_LIBRARIES += libext4_utils
    endif
endif

LOCAL_MODULE := libguitwrp

#TWRP_EVENT_LOGGING := true
ifeq ($(TWRP_EVENT_LOGGING), true)
    LOCAL_CFLAGS += -D_EVENT_LOGGING
endif
ifneq ($(TW_USE_KEY_CODE_TOUCH_SYNC),)
    LOCAL_CFLAGS += -DTW_USE_KEY_CODE_TOUCH_SYNC=$(TW_USE_KEY_CODE_TOUCH_SYNC)
endif
ifneq ($(TW_OZIP_DECRYPT_KEY),)
    LOCAL_CFLAGS += -DTW_OZIP_DECRYPT_KEY=\"$(TW_OZIP_DECRYPT_KEY)\"
else
    LOCAL_CFLAGS += -DTW_OZIP_DECRYPT_KEY=0
endif
ifneq ($(TW_NO_SCREEN_BLANK),)
    LOCAL_CFLAGS += -DTW_NO_SCREEN_BLANK
endif
ifneq ($(TW_NO_SCREEN_TIMEOUT),)
    LOCAL_CFLAGS += -DTW_NO_SCREEN_TIMEOUT
endif
ifeq ($(TW_OEM_BUILD), true)
    LOCAL_CFLAGS += -DTW_OEM_BUILD
endif
ifneq ($(TW_X_OFFSET),)
    LOCAL_CFLAGS += -DTW_X_OFFSET=$(TW_X_OFFSET)
endif
ifneq ($(TW_Y_OFFSET),)
    LOCAL_CFLAGS += -DTW_Y_OFFSET=$(TW_Y_OFFSET)
endif
ifneq ($(TW_W_OFFSET),)
    LOCAL_CFLAGS += -DTW_W_OFFSET=$(TW_W_OFFSET)
endif
ifneq ($(TW_H_OFFSET),)
    LOCAL_CFLAGS += -DTW_H_OFFSET=$(TW_H_OFFSET)
endif
ifeq ($(TW_ROUND_SCREEN), true)
    LOCAL_CFLAGS += -DTW_ROUND_SCREEN
endif
ifeq ($(TW_SCREEN_BLANK_ON_BOOT), true)
    LOCAL_CFLAGS += -DTW_SCREEN_BLANK_ON_BOOT
endif
ifeq ($(TW_EXCLUDE_NANO), true)
    LOCAL_CFLAGS += -DTW_EXCLUDE_NANO
endif
ifeq ($(AB_OTA_UPDATER),true)
    LOCAL_CFLAGS += -DAB_OTA_UPDATER=1
endif

#SHRP Build Flags
ifeq ($(SHRP_CUSTOM_FLASHLIGHT),true)
    LOCAL_CFLAGS += -DSHRP_CUSTOM_FLASHLIGHT
endif
ifeq ($(SHRP_LITE),true)
    LOCAL_CFLAGS += -DSHRP_LITE
endif
ifeq ($(SHRP_AB),true)
    LOCAL_CFLAGS += -DSHRP_AB
endif
ifeq ($(SHRP_EXPRESS),true)
	LOCAL_CFLAGS += -DSHRP_EXPRESS
endif
#Default Addon Disabler
ifeq ($(SHRP_EXCLUDE_DEFAULT_ADDONS),true)
	LOCAL_CFLAGS += -DSHRP_EXCLUDE_DEFAULT_ADDON
endif
ifeq ($(SHRP_SKIP_DEFAULT_ADDON_1),true)
	LOCAL_CFLAGS += -DSHRP_SKIP_DEFAULT_ADDON_1
endif
ifeq ($(SHRP_SKIP_DEFAULT_ADDON_2),true)
	LOCAL_CFLAGS += -DSHRP_SKIP_DEFAULT_ADDON_2
endif
ifeq ($(SHRP_SKIP_DEFAULT_ADDON_3),true)
	LOCAL_CFLAGS += -DSHRP_SKIP_DEFAULT_ADDON_3
endif
ifeq ($(SHRP_SKIP_DEFAULT_ADDON_4),true)
	LOCAL_CFLAGS += -DSHRP_SKIP_DEFAULT_ADDON_4
endif
#SHRP External Addon Integration
#Exclude magisk flash
ifeq ($(SHRP_EXCLUDE_MAGISK_FLASH),true)
	LOCAL_CFLAGS += -DSHRP_EXCLUDE_MAGISK_FLASH
endif
#FirstAddon
ifneq ($(SHRP_EXTERNAL_ADDON_1_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_1_NAME=$(SHRP_EXTERNAL_ADDON_1_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_1_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_1_INFO=$(SHRP_EXTERNAL_ADDON_1_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_1_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_1_FILENAME=$(SHRP_EXTERNAL_ADDON_1_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_1_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_1_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_1_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_1_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_1_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_1_SUCCESSFUL_TEXT)
endif
#SecondAddon
ifneq ($(SHRP_EXTERNAL_ADDON_2_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_2_NAME=$(SHRP_EXTERNAL_ADDON_2_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_2_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_2_INFO=$(SHRP_EXTERNAL_ADDON_2_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_2_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_2_FILENAME=$(SHRP_EXTERNAL_ADDON_2_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_2_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_2_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_2_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_2_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_2_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_2_SUCCESSFUL_TEXT)
endif
#ThirdAddon
ifneq ($(SHRP_EXTERNAL_ADDON_3_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_3_NAME=$(SHRP_EXTERNAL_ADDON_3_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_3_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_3_INFO=$(SHRP_EXTERNAL_ADDON_3_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_3_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_3_FILENAME=$(SHRP_EXTERNAL_ADDON_3_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_3_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_3_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_3_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_3_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_3_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_3_SUCCESSFUL_TEXT)
endif
#ForthAddon
ifneq ($(SHRP_EXTERNAL_ADDON_4_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_4_NAME=$(SHRP_EXTERNAL_ADDON_4_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_4_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_4_INFO=$(SHRP_EXTERNAL_ADDON_4_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_4_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_4_FILENAME=$(SHRP_EXTERNAL_ADDON_4_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_4_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_4_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_4_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_4_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_4_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_4_SUCCESSFUL_TEXT)
endif
#FifthAddon
ifneq ($(SHRP_EXTERNAL_ADDON_5_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_5_NAME=$(SHRP_EXTERNAL_ADDON_5_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_5_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_5_INFO=$(SHRP_EXTERNAL_ADDON_5_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_5_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_5_FILENAME=$(SHRP_EXTERNAL_ADDON_5_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_5_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_5_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_5_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_5_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_5_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_5_SUCCESSFUL_TEXT)
endif
#SixthAddon
ifneq ($(SHRP_EXTERNAL_ADDON_6_NAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_6_NAME=$(SHRP_EXTERNAL_ADDON_6_NAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_6_INFO),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_6_INFO=$(SHRP_EXTERNAL_ADDON_6_INFO)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_6_FILENAME),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_6_FILENAME=$(SHRP_EXTERNAL_ADDON_6_FILENAME)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_6_BTN_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_6_BTN_TEXT=$(SHRP_EXTERNAL_ADDON_6_BTN_TEXT)
endif
ifneq ($(SHRP_EXTERNAL_ADDON_6_SUCCESSFUL_TEXT),)
	LOCAL_CFLAGS += -DSHRP_EXTERNAL_ADDON_6_SUCCESSFUL_TEXT=$(SHRP_EXTERNAL_ADDON_6_SUCCESSFUL_TEXT)
endif
#Addon Flags Ended
ifeq ($(TW_EXCLUDE_ENCRYPTED_BACKUPS), true)
    LOCAL_CFLAGS += -DTW_EXCLUDE_ENCRYPTED_BACKUPS
endif
ifeq ($(TW_USE_TOOLBOX), true)
    LOCAL_CFLAGS += -DTW_USE_TOOLBOX
endif

LOCAL_C_INCLUDES += \
    bionic \
    system/core/base/include \
    system/core/include \
    system/core/libpixelflinger/include

ifeq ($(shell test $(PLATFORM_SDK_VERSION) -lt 23; echo $$?),0)
    LOCAL_C_INCLUDES += external/stlport/stlport
    LOCAL_CFLAGS += -DUSE_FUSE_SIDELOAD22
endif

LOCAL_CFLAGS += -DTWRES=\"$(TWRES_PATH)\"

include $(BUILD_STATIC_LIBRARY)

# Transfer in the resources for the device
include $(CLEAR_VARS)
LOCAL_MODULE := twrp
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE_CLASS := RECOVERY_EXECUTABLES
LOCAL_MODULE_PATH := $(TARGET_RECOVERY_ROOT_OUT)$(TWRES_PATH)

# The extra blank line before *** is intentional to ensure it ends up on its own line
define TW_THEME_WARNING_MSG

****************************************************************************
  Could not find ui.xml for TW_THEME: $(TW_THEME)
  Set TARGET_SCREEN_WIDTH and TARGET_SCREEN_HEIGHT to automatically select
  an appropriate theme, or set TW_THEME to one of the following:
    $(notdir $(wildcard $(LOCAL_PATH)/theme/*_*))
****************************************************************************
endef
define TW_CUSTOM_THEME_WARNING_MSG

****************************************************************************
  Could not find ui.xml for TW_CUSTOM_THEME: $(TW_CUSTOM_THEME)
  Expected to find custom theme's ui.xml at:
    $(TWRP_THEME_LOC)/ui.xml
  Please fix this or set TW_THEME to one of the following:
    $(notdir $(wildcard $(LOCAL_PATH)/theme/*_*))
****************************************************************************
endef

TWRP_RES := $(LOCAL_PATH)/theme/shrp_portrait_hdpi/fonts
TWRP_RES += $(LOCAL_PATH)/theme/shrp_portrait_hdpi/languages
ifeq ($(TW_EXTRA_LANGUAGES),true)
    TWRP_RES += $(LOCAL_PATH)/theme/extra-languages/fonts
    TWRP_RES += $(LOCAL_PATH)/theme/extra-languages/languages
endif


TW_THEME := shrp_portrait_hdpi


TWRP_THEME_LOC := $(LOCAL_PATH)/theme/$(TW_THEME)

TWRP_RES += $(TW_ADDITIONAL_RES)

TWRP_RES_GEN := $(intermediates)/twrp
$(TWRP_RES_GEN):
	mkdir -p $(TARGET_RECOVERY_ROOT_OUT)$(TWRES_PATH)
	cp -fr $(TWRP_RES) $(TARGET_RECOVERY_ROOT_OUT)$(TWRES_PATH)
	cp -fr $(TWRP_THEME_LOC)/* $(TARGET_RECOVERY_ROOT_OUT)$(TWRES_PATH)

LOCAL_GENERATED_SOURCES := $(TWRP_RES_GEN)
LOCAL_SRC_FILES := twrp $(TWRP_RES_GEN)
include $(BUILD_PREBUILT)
