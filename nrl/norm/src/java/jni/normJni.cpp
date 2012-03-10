#include "normJni.h"

#ifdef WIN32
#define snprintf _snprintf
#endif

jclass cid_NormInstance;
jfieldID fid_NormInstance_handle;

jclass cid_NormAckingStatus;
jmethodID mid_NormAckingStatus_values;

jclass cid_NormData;
jmethodID mid_NormData_init;

jclass cid_NormEvent;
jmethodID mid_NormEvent_init;
jfieldID fid_NormEvent_objectHandle;

jclass cid_NormEventType;
jmethodID mid_NormEventType_values;

jclass cid_NormFile;
jmethodID mid_NormFile_init;

jclass cid_NormFlushMode;
jmethodID mid_NormFlushMode_ordinal;

jclass cid_NormNackingMode;
jmethodID mid_NormNackingMode_ordinal;

jclass cid_NormNode;
jfieldID fid_NormNode_handle;
jmethodID mid_NormNode_init;

jclass cid_NormObject;
jfieldID fid_NormObject_handle;

jclass cid_NormObjectType;
jmethodID mid_NormObjectType_values;

jclass cid_NormProbingMode;
jmethodID mid_NormProbingMode_ordinal;

jclass cid_NormRepairBoundary;
jmethodID mid_NormRepairBoundary_ordinal;

jclass cid_NormSession;
jfieldID fid_NormSession_handle;
jmethodID mid_NormSession_init;

jclass cid_NormStream;
jmethodID mid_NormStream_init;

jclass cid_NormSyncPolicy;
jmethodID mid_NormSyncPolicy_ordinal;

jclass cid_InetAddress;
jmethodID mid_InetAddress_getByAddress;

jclass cid_InetSocketAddress;
jmethodID mid_InetSocketAddress_init;

jclass cid_IOException;

void check_version(JNIEnv *env);

/* Called by the JVM when the library is loaded */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env;

  if (vm->GetEnv((void **)&env, JNI_VERSION_1_4)) {
    return JNI_ERR; /* JNI version not supported */
  }

  /* Load the NormInstance first so we can compare the versions */
  cid_NormInstance = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormInstance"));
  fid_NormInstance_handle = env->GetFieldID(cid_NormInstance, "handle", "J");

  /* Check the version */
  check_version(env);

  cid_NormAckingStatus = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormAckingStatus"));
  mid_NormAckingStatus_values = env->GetStaticMethodID(cid_NormAckingStatus,
    "values", "()[Lmil/navy/nrl/norm/enums/NormAckingStatus;");

  cid_NormData = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormData"));
  mid_NormData_init = env->GetMethodID(cid_NormData, "<init>", "(J)V");

  cid_NormEvent = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormEvent"));
  fid_NormEvent_objectHandle = env->GetFieldID(cid_NormEvent,
    "objectHandle", "J");
  mid_NormEvent_init = env->GetMethodID(cid_NormEvent, "<init>",
    "(Lmil/navy/nrl/norm/enums/NormEventType;JJJ)V");

  cid_NormEventType = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormEventType"));
  mid_NormEventType_values = env->GetStaticMethodID(cid_NormEventType,
    "values", "()[Lmil/navy/nrl/norm/enums/NormEventType;");

  cid_NormFile = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormFile"));
  mid_NormFile_init = env->GetMethodID(cid_NormFile, "<init>", "(J)V");

  cid_NormFlushMode = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormFlushMode"));
  mid_NormFlushMode_ordinal = env->GetMethodID(cid_NormFlushMode,
    "ordinal", "()I");

  cid_NormNackingMode = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormNackingMode"));
  mid_NormNackingMode_ordinal = env->GetMethodID(cid_NormNackingMode,
    "ordinal", "()I");

  cid_NormNode = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormNode"));
  fid_NormNode_handle = env->GetFieldID(cid_NormNode, "handle", "J");
  mid_NormNode_init = env->GetMethodID(cid_NormNode, "<init>", "(J)V");

  cid_NormObject = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormObject"));
  fid_NormObject_handle = env->GetFieldID(cid_NormObject, "handle", "J");

  cid_NormObjectType = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormObjectType"));
  mid_NormObjectType_values = env->GetStaticMethodID(cid_NormObjectType,
    "values", "()[Lmil/navy/nrl/norm/enums/NormObjectType;");

  cid_NormProbingMode = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormProbingMode"));
  mid_NormProbingMode_ordinal = env->GetMethodID(cid_NormProbingMode,
    "ordinal", "()I");

  cid_NormRepairBoundary = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormRepairBoundary"));
  mid_NormRepairBoundary_ordinal = env->GetMethodID(cid_NormRepairBoundary,
    "ordinal", "()I");

  cid_NormSession = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormSession"));
  fid_NormSession_handle = env->GetFieldID(cid_NormSession, "handle", "J");
  mid_NormSession_init = env->GetMethodID(cid_NormSession, "<init>", "(J)V");

  cid_NormStream = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/NormStream"));
  mid_NormStream_init = env->GetMethodID(cid_NormStream, "<init>", "(J)V");

  cid_NormSyncPolicy = (jclass)env->NewWeakGlobalRef(
    env->FindClass("mil/navy/nrl/norm/enums/NormSyncPolicy"));
  mid_NormSyncPolicy_ordinal = env->GetMethodID(cid_NormSyncPolicy,
    "ordinal", "()I");

  cid_InetAddress = (jclass)env->NewWeakGlobalRef(
    env->FindClass("java/net/InetAddress"));
  mid_InetAddress_getByAddress = env->GetStaticMethodID(cid_InetAddress,
    "getByAddress", "([B)Ljava/net/InetAddress;");

  cid_InetSocketAddress = (jclass)env->NewWeakGlobalRef(
    env->FindClass("java/net/InetSocketAddress"));
  mid_InetSocketAddress_init = env->GetMethodID(cid_InetSocketAddress,
    "<init>", "(Ljava/net/InetAddress;I)V");

  cid_IOException = (jclass)env->NewWeakGlobalRef(
    env->FindClass("java/io/IOException"));

  return JNI_VERSION_1_4;
}

void check_version(JNIEnv *env) {
  jfieldID fid;
  jstring versionString;
  const char *version;

  fid = env->GetStaticFieldID(cid_NormInstance,
    "VERSION", "Ljava/lang/String;");
  if (fid == NULL) {
    env->FatalError("Could not obtain version from NormInstance class."
      "  The native library and jar file are not compatible.\n");
  }

  versionString = (jstring)env->GetStaticObjectField(cid_NormInstance, fid);
  version = env->GetStringUTFChars(versionString, NULL);

  if (strcmp(VERSION, version) != 0) {
    char str[1024];
    snprintf(str, 1024, "Warning: NORM JNI versions do not match %s != %s."
      "  The native library and jar file are not compatible.\n",
      VERSION, version);
    env->FatalError(str);
  }

  env->ReleaseStringUTFChars(versionString, version);
}

/* Called by the JVM when the library is unloaded */
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
  JNIEnv *env;

  if (vm->GetEnv((void **)&env, JNI_VERSION_1_2)) {
    return;
  }

  env->DeleteWeakGlobalRef(cid_InetAddress);
  env->DeleteWeakGlobalRef(cid_InetSocketAddress);
  env->DeleteWeakGlobalRef(cid_IOException);
  env->DeleteWeakGlobalRef(cid_NormAckingStatus);
  env->DeleteWeakGlobalRef(cid_NormData);
  env->DeleteWeakGlobalRef(cid_NormEvent);
  env->DeleteWeakGlobalRef(cid_NormEventType);
  env->DeleteWeakGlobalRef(cid_NormFile);
  env->DeleteWeakGlobalRef(cid_NormFlushMode);
  env->DeleteWeakGlobalRef(cid_NormInstance);
  env->DeleteWeakGlobalRef(cid_NormNackingMode);
  env->DeleteWeakGlobalRef(cid_NormNode);
  env->DeleteWeakGlobalRef(cid_NormObject);
  env->DeleteWeakGlobalRef(cid_NormObjectType);
  env->DeleteWeakGlobalRef(cid_NormProbingMode);
  env->DeleteWeakGlobalRef(cid_NormRepairBoundary);
  env->DeleteWeakGlobalRef(cid_NormSession);
  env->DeleteWeakGlobalRef(cid_NormStream);
  env->DeleteWeakGlobalRef(cid_NormSyncPolicy);
}

