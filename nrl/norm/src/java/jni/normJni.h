#ifndef __NORMJNI_H__
#define __NORMJNI_H__

#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <normApi.h>

/*
 * This version string is used to validate the compatibility between the Java
 * and C native libraries. Update this string along with it's counterpart in
 * the NormInstance.java file whenever the native API changes.
 */
#define VERSION "20110308-0834"

#define PKGNAME(str) Java_mil_navy_nrl_norm_##str

#ifdef __cplusplus
extern "C" {
#endif

extern jclass cid_InetAddress;
extern jmethodID mid_InetAddress_getByAddress;

extern jclass cid_InetSocketAddress;
extern jmethodID mid_InetSocketAddress_init;

extern jclass cid_IOException;

extern jclass cid_NormAckingStatus;
extern jmethodID mid_NormAckingStatus_values;

extern jclass cid_NormData;
extern jmethodID mid_NormData_init;

extern jclass cid_NormEvent;
extern jmethodID mid_NormEvent_init;
extern jfieldID fid_NormEvent_objectHandle;

extern jclass cid_NormEventType;
extern jmethodID mid_NormEventType_values;

extern jclass cid_NormFile;
extern jmethodID mid_NormFile_init;

extern jclass cid_NormFlushMode;
extern jmethodID mid_NormFlushMode_ordinal;

extern jclass cid_NormInstance;
extern jfieldID fid_NormInstance_handle;

extern jclass cid_NormNackingMode;
extern jmethodID mid_NormNackingMode_ordinal;

extern jclass cid_NormNode;
extern jfieldID fid_NormNode_handle;
extern jmethodID mid_NormNode_init;

extern jclass cid_NormObject;
extern jfieldID fid_NormObject_handle;

extern jclass cid_NormObjectType;
extern jmethodID mid_NormObjectType_values;

extern jclass cid_NormProbingMode;
extern jmethodID mid_NormProbingMode_ordinal;

extern jclass cid_NormRepairBoundary;
extern jmethodID mid_NormRepairBoundary_ordinal;

extern jclass cid_NormSession;
extern jfieldID fid_NormSession_handle;
extern jmethodID mid_NormSession_init;

extern jclass cid_NormStream;
extern jmethodID mid_NormStream_init;

extern jclass cid_NormSyncPolicy;
extern jmethodID mid_NormSyncPolicy_ordinal;

#ifdef __cplusplus
}
#endif

#endif
