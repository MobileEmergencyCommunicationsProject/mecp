LOCAL_PATH := $(call my-dir)

NORM_CFLAGS := \
	-DPROTO_DEBUG \
	-DNORM_DEBUG \
	-DUNIX \
	-DHAVE_DIRFD \
	-DHAVE_ASSERT \
	-DHAVE_GETLOGIN \
	-DLINUX \
	-D_FILE_OFFSET_BITS=64 \
	-DHAVE_OLD_SIGNALHANDLER \
	-DHAVE_SCHED \
	-DNO_SCM_RIGHTS

NORM_C_INCLUDES := \
	$(LOCAL_PATH)/../../../protolib/include \
	$(LOCAL_PATH)/../../../include

include $(CLEAR_VARS)
LOCAL_MODULE := norm
LOCAL_C_INCLUDES := $(NORM_C_INCLUDES)
LOCAL_CFLAGS := $(NORM_CFLAGS)
LOCAL_SRC_FILES := \
	../../../protolib/src/common/protoAddress.cpp \
	../../../protolib/src/common/protoBitmask.cpp \
	../../../protolib/src/common/protoChannel.cpp \
	../../../protolib/src/common/protoDebug.cpp \
	../../../protolib/src/common/protoDispatcher.cpp \
	../../../protolib/src/common/protoList.cpp \
	../../../protolib/src/common/protoNet.cpp \
	../../../protolib/src/common/protoPipe.cpp \
	../../../protolib/src/common/protoPkt.cpp \
	../../../protolib/src/common/protoPktETH.cpp \
	../../../protolib/src/common/protoPktIP.cpp \
	../../../protolib/src/common/protoSocket.cpp \
	../../../protolib/src/common/protoTime.cpp \
	../../../protolib/src/common/protoTimer.cpp \
	../../../protolib/src/common/protoTree.cpp \
	../../../protolib/src/linux/linuxNet.cpp \
	../../../protolib/src/unix/unixNet.cpp \
	../../../src/common/galois.cpp \
	../../../src/common/normApi.cpp \
	../../../src/common/normEncoder.cpp \
	../../../src/common/normEncoderMDP.cpp \
	../../../src/common/normEncoderRS16.cpp \
	../../../src/common/normEncoderRS8.cpp \
	../../../src/common/normFile.cpp \
	../../../src/common/normMessage.cpp \
	../../../src/common/normNode.cpp \
	../../../src/common/normObject.cpp \
	../../../src/common/normSegment.cpp \
	../../../src/common/normSession.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := norm-jni
LOCAL_C_INCLUDES := $(NORM_C_INCLUDES)
LOCAL_CFLAGS := $(NORM_CFLAGS)
LOCAL_SHARED_LIBRARIES := norm
LOCAL_SRC_FILES := \
	../../../src/java/jni/normDataJni.cpp \
	../../../src/java/jni/normEventJni.cpp \
	../../../src/java/jni/normFileJni.cpp \
	../../../src/java/jni/normInstanceJni.cpp \
	../../../src/java/jni/normJni.cpp \
	../../../src/java/jni/normNodeJni.cpp \
	../../../src/java/jni/normObjectJni.cpp \
	../../../src/java/jni/normSessionJni.cpp \
	../../../src/java/jni/normStreamJni.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := norm-app
LOCAL_C_INCLUDES := $(NORM_C_INCLUDES)
LOCAL_CFLAGS := $(NORM_CFLAGS)
LOCAL_SHARED_LIBRARIES := norm
LOCAL_SRC_FILES := \
	../../../protolib/src/common/protoApp.cpp \
	../../../src/common/normApp.cpp \
	../../../src/common/normPostProcess.cpp \
	../../../src/unix/unixPostProcess.cpp
include $(BUILD_EXECUTABLE)
