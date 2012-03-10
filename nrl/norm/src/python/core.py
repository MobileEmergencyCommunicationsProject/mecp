"""
pynorm - Python wrapper for NRL's libnorm
By: Tom Wambold <wambold@itd.nrl.navy.mil>
"""

from __future__ import absolute_import

import ctypes
from ctypes.util import find_library

# Exception classes
class NormError(Exception): pass

# ctypes libnorm structures
class NormEventStruct(ctypes.Structure):
    _fields_ = [
            ("type", ctypes.c_int),
            ("session", ctypes.c_void_p),
            ("sender", ctypes.c_void_p),
            ("object", ctypes.c_void_p)]

# ctypes error checkers
def errcheck_bool(result, func, args):
    """Checks the return value of functions that return bools.  Raises an
    exception if they return false."""
    if result == False:
        raise NormError("Error calling %s" % func.__name__)
    return bool(result)

def errcheck_instance(result, func, args):
    """Checks the return value of NormCreateInstance"""
    if result == ctypes.c_void_p.in_dll(libnorm, "NORM_INSTANCE_INVALID"):
        raise NormError("Error creating instance.")
    return result

def errcheck_descriptor(result, func, args):
    """Checks the return value of NormGetDescriptor"""
    if result == ctypes.c_int.in_dll(libnorm, "NORM_DESCRIPTOR_INVALID").value:
        raise NormError("Error getting descriptor.")
    return result

def errcheck_session(result, func, args):
    """Checks the return value of NormGetDescriptor"""
    if result == ctypes.c_void_p.in_dll(libnorm, "NORM_SESSION_INVALID"):
        raise NormError("Error starting session.")
    return result

def errcheck_object(result, func, args):
    """Checks the return value of NormFileEnqueue and NormDataEnqueue"""
    if result == ctypes.c_void_p.in_dll(libnorm, "NORM_OBJECT_INVALID"):
        raise NormError(
                "Error creating object from function '%s'" % func.__name__)
    return result

def return_bool(result, func, args):
    """Converts the return type of a C function to a Python bool"""
    return bool(result)

def get_libnorm():
    """Sets up the return and argument types for the functions in the NORM
    library"""
    libnorm = getattr(ctypes.LibraryLoader(ctypes.CDLL), find_library("norm"))

    ## libnorm initializations
    # Instance functions
    libnorm.NormCreateInstance.restype = ctypes.c_void_p
    libnorm.NormCreateInstance.argtypes = [ctypes.c_byte]
    libnorm.NormCreateInstance.errcheck = errcheck_instance

    libnorm.NormDestroyInstance.restype = None
    libnorm.NormDestroyInstance.argtypes = [ctypes.c_void_p]

    libnorm.NormStopInstance.restype = None
    libnorm.NormStopInstance.argtypes = [ctypes.c_void_p]

    libnorm.NormRestartInstance.argtypes = [ctypes.c_void_p]
    libnorm.NormRestartInstance.errcheck = errcheck_bool

    libnorm.NormSetCacheDirectory.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    libnorm.NormSetCacheDirectory.errcheck = errcheck_bool

    libnorm.NormGetNextEvent.argtypes = [ctypes.c_void_p,
            ctypes.POINTER(NormEventStruct)]
    libnorm.NormGetNextEvent.errcheck = errcheck_bool

    libnorm.NormGetDescriptor.argtypes = [ctypes.c_void_p]
    libnorm.NormGetDescriptor.errcheck = errcheck_descriptor

    # Session Functions
    libnorm.NormCreateSession.restype = ctypes.c_void_p
    libnorm.NormCreateSession.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.c_uint16, ctypes.c_uint32]
    libnorm.NormCreateSession.errcheck = errcheck_session

    libnorm.NormDestroySession.restype = None
    libnorm.NormDestroySession.argtypes = [ctypes.c_void_p]

    libnorm.NormSetUserData.restype = None
    libnorm.NormSetUserData.argtypes = [ctypes.c_void_p, ctypes.c_char_p]

    libnorm.NormGetUserData.restype = ctypes.c_char_p
    libnorm.NormGetUserData.argtypes = [ctypes.c_void_p]

    libnorm.NormGetLocalNodeId.restype = ctypes.c_uint32
    libnorm.NormGetLocalNodeId.argtypes = [ctypes.c_void_p]

    libnorm.NormSetTxPort.restype = None
    libnorm.NormSetTxPort.argtypes = [ctypes.c_void_p, ctypes.c_uint16]

    libnorm.NormSetRxPortReuse.restype = None
    libnorm.NormSetRxPortReuse.argtypes = [
      ctypes.c_void_p, ctypes.c_byte, ctypes.c_byte]

    libnorm.NormSetEcnSupport.restype = None
    libnorm.NormSetEcnSupport.argtypes = [ctypes.c_void_p, ctypes.c_byte,
            ctypes.c_byte]

    libnorm.NormSetMulticastInterface.argtypes = [ctypes.c_void_p,
            ctypes.c_char_p]
    libnorm.NormSetMulticastInterface.errcheck = errcheck_bool

    libnorm.NormSetTTL.argtypes = [ctypes.c_void_p, ctypes.c_uint8]
    libnorm.NormSetTTL.errcheck = errcheck_bool

    libnorm.NormSetTOS.argtypes = [ctypes.c_void_p, ctypes.c_uint8]
    libnorm.NormSetTOS.errcheck = errcheck_bool

    libnorm.NormSetLoopback.argtypes = [ctypes.c_void_p, ctypes.c_byte]
    libnorm.NormSetLoopback.errcheck = errcheck_bool

    # Debug functions
    libnorm.NormSetMessageTrace.restype = None
    libnorm.NormSetMessageTrace.argtypes = [ctypes.c_void_p, ctypes.c_byte]

    libnorm.NormSetTxLoss.restype = None
    libnorm.NormSetTxLoss.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormSetRxLoss.restype = None
    libnorm.NormSetRxLoss.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormOpenDebugLog.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    libnorm.NormOpenDebugLog.errcheck = errcheck_bool

    libnorm.NormCloseDebugLog.restype = None
    libnorm.NormCloseDebugLog.argtypes = [ctypes.c_void_p]

    libnorm.NormOpenDebugPipe.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    libnorm.NormOpenDebugPipe.errcheck = errcheck_bool

    libnorm.NormSetDebugLevel.restype = None
    libnorm.NormSetDebugLevel.argtypes = [ctypes.c_uint]

    libnorm.NormGetDebugLevel.restype = ctypes.c_uint

    libnorm.NormSetReportInterval.restype = None
    libnorm.NormSetReportInterval.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormGetReportInterval.restype = ctypes.c_double

    # Sender functions
    libnorm.NormStartSender.argtypes = [ctypes.c_void_p, ctypes.c_uint16,
            ctypes.c_uint32, ctypes.c_uint16, ctypes.c_uint8, ctypes.c_uint8]
    libnorm.NormStartSender.errcheck = errcheck_bool

    libnorm.NormStopSender.restype = None
    libnorm.NormStopSender.argtypes = [ctypes.c_void_p]

    libnorm.NormGetTxRate.restype = ctypes.c_double
    libnorm.NormGetTxRate.argtypes = [ctypes.c_void_p]

    libnorm.NormSetTxRate.restype = None
    libnorm.NormSetTxRate.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormSetTxSocketBuffer.argtypes = [ctypes.c_void_p, ctypes.c_uint]
    libnorm.NormSetTxSocketBuffer.errcheck = errcheck_bool

    libnorm.NormSetCongestionControl.restype = None
    libnorm.NormSetCongestionControl.argtypes = [ctypes.c_void_p,
            ctypes.c_byte]

    libnorm.NormSetTxRateBounds.restype = None
    libnorm.NormSetTxRateBounds.argtypes = [
      ctypes.c_void_p, ctypes.c_double, ctypes.c_double]

    libnorm.NormSetTxCacheBounds.restype = None
    libnorm.NormSetTxCacheBounds.argtypes = [ctypes.c_void_p,
            ctypes.c_int64, ctypes.c_uint32, ctypes.c_uint32]

    libnorm.NormSetAutoParity.restype = None
    libnorm.NormSetAutoParity.argtypes = [ctypes.c_void_p, ctypes.c_uint8]

    libnorm.NormSetGrttEstimate.restype = None
    libnorm.NormSetGrttEstimate.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormGetGrttEstimate.restype = ctypes.c_double
    libnorm.NormGetGrttEstimate.argtypes = [ctypes.c_void_p]

    libnorm.NormSetGrttMax.restype = None
    libnorm.NormSetGrttMax.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormSetGrttProbingMode.restype = None
    libnorm.NormSetGrttProbingMode.argtypes = [ctypes.c_void_p, ctypes.c_int]

    libnorm.NormSetGrttProbingInterval.restype = None
    libnorm.NormSetGrttProbingInterval.argtypes = [ctypes.c_void_p,
            ctypes.c_double, ctypes.c_double]

    libnorm.NormSetBackoffFactor.restype = None
    libnorm.NormSetBackoffFactor.argtypes = [ctypes.c_void_p, ctypes.c_double]

    libnorm.NormSetGroupSize.restype = None
    libnorm.NormSetGroupSize.argtypes = [ctypes.c_void_p, ctypes.c_uint]

    libnorm.NormSetTxRobustFactor.restype = None
    libnorm.NormSetTxRobustFactor.argtypes = [ctypes.c_void_p, ctypes.c_int]

    libnorm.NormFileEnqueue.restype = ctypes.c_void_p
    libnorm.NormFileEnqueue.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.c_char_p, ctypes.c_uint]
    libnorm.NormFileEnqueue.errcheck = errcheck_object

    libnorm.NormDataEnqueue.restype = ctypes.c_void_p
    libnorm.NormDataEnqueue.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.c_uint32, ctypes.c_char_p, ctypes.c_uint]
    libnorm.NormDataEnqueue.errcheck = errcheck_object

    libnorm.NormRequeueObject.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
    libnorm.NormRequeueObject.errcheck = errcheck_bool

    libnorm.NormStreamOpen.restype = ctypes.c_void_p
    libnorm.NormStreamOpen.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    libnorm.NormStreamOpen.errcheck = errcheck_object

    libnorm.NormStreamClose.restype = None
    libnorm.NormStreamClose.argtypes = [ctypes.c_void_p, ctypes.c_byte]

    libnorm.NormStreamWrite.restype = ctypes.c_uint
    libnorm.NormStreamWrite.argtypes = [
      ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint]

    libnorm.NormStreamFlush.restype = None
    libnorm.NormStreamFlush.argtypes = [
      ctypes.c_void_p, ctypes.c_byte, ctypes.c_int]

    libnorm.NormStreamSetAutoFlush.restype = None
    libnorm.NormStreamSetAutoFlush.argtypes = [ctypes.c_void_p, ctypes.c_int]

    libnorm.NormStreamSetPushEnable.restype = None
    libnorm.NormStreamSetPushEnable.argtypes = [ctypes.c_void_p, ctypes.c_byte]

    libnorm.NormStreamHasVacancy.argtypes = [ctypes.c_void_p]
    libnorm.NormStreamHasVacancy.errcheck = return_bool

    libnorm.NormStreamMarkEom.restype = None
    libnorm.NormStreamMarkEom.argtypes = [ctypes.c_void_p]

    libnorm.NormSetWatermark.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
    libnorm.NormSetWatermark.errcheck = errcheck_bool

    libnorm.NormCancelWatermark.restype = None
    libnorm.NormCancelWatermark.argtypes = [ctypes.c_void_p]

    libnorm.NormAddAckingNode.restype = None
    libnorm.NormAddAckingNode.argtypes = [ctypes.c_void_p, ctypes.c_uint32]

    libnorm.NormRemoveAckingNode.restype = None
    libnorm.NormRemoveAckingNode.argtypes = [ctypes.c_void_p, ctypes.c_uint32]

    libnorm.NormGetAckingStatus.argtypes = [ctypes.c_void_p, ctypes.c_uint32]

    # Receiver functions
    libnorm.NormStartReceiver.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    libnorm.NormStartReceiver.errcheck = errcheck_bool

    libnorm.NormStopReceiver.restype = None
    libnorm.NormStopReceiver.argtypes = [ctypes.c_void_p]

    libnorm.NormSetRxSocketBuffer.argtypes = [ctypes.c_void_p, ctypes.c_uint]
    libnorm.NormSetRxSocketBuffer.errcheck = errcheck_bool

    libnorm.NormSetSilentReceiver.restype = None
    libnorm.NormSetSilentReceiver.argtypes = [ctypes.c_void_p, ctypes.c_byte,
            ctypes.c_int]

    libnorm.NormSetDefaultUnicastNack.restype = None
    libnorm.NormSetDefaultUnicastNack.argtypes = [ctypes.c_void_p,
            ctypes.c_byte]

    libnorm.NormNodeSetUnicastNack.restype = None
    libnorm.NormNodeSetUnicastNack.argtypes = [ctypes.c_void_p, ctypes.c_byte]

    libnorm.NormSetDefaultNackingMode.restype = None
    libnorm.NormSetDefaultNackingMode.argtypes = [ctypes.c_void_p,
            ctypes.c_int]

    libnorm.NormNodeSetNackingMode.restype = None
    libnorm.NormNodeSetNackingMode.argtypes = [ctypes.c_void_p, ctypes.c_int]

    libnorm.NormObjectSetNackingMode.restype = None
    libnorm.NormObjectSetNackingMode.argtypes = [ctypes.c_void_p, ctypes.c_int]

    libnorm.NormSetDefaultRepairBoundary.restype = None
    libnorm.NormSetDefaultRepairBoundary.argtypes = [ctypes.c_void_p,
            ctypes.c_int]

    libnorm.NormNodeSetRepairBoundary.restype = None
    libnorm.NormNodeSetRepairBoundary.argtypes = [ctypes.c_void_p,
            ctypes.c_int]

    libnorm.NormSetDefaultRxRobustFactor.restype = None
    libnorm.NormSetDefaultRxRobustFactor.argtypes = [ctypes.c_void_p,
            ctypes.c_int]

    libnorm.NormNodeSetRxRobustFactor.restype = None
    libnorm.NormNodeSetRxRobustFactor.argtypes = [ctypes.c_void_p,
            ctypes.c_int]

    libnorm.NormStreamRead.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.POINTER(ctypes.c_uint)]
    libnorm.NormStreamRead.errcheck = return_bool

    libnorm.NormStreamSeekMsgStart.argtypes = [ctypes.c_void_p]
    libnorm.NormStreamSeekMsgStart.errcheck = return_bool

    libnorm.NormStreamGetReadOffset.restype = ctypes.c_uint32
    libnorm.NormStreamGetReadOffset.argtypes = [ctypes.c_void_p]

    # Object functions
    libnorm.NormObjectGetType.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectHasInfo.argtypes = [ctypes.c_void_p]
    libnorm.NormObjectHasInfo.errcheck = return_bool

    libnorm.NormObjectGetInfoLength.restype = ctypes.c_uint16
    libnorm.NormObjectGetInfoLength.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectGetInfo.restype = ctypes.c_uint16
    libnorm.NormObjectGetInfo.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.c_uint16]

    libnorm.NormObjectGetSize.restype = ctypes.c_int64
    libnorm.NormObjectGetSize.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectGetBytesPending.restype = ctypes.c_int64
    libnorm.NormObjectGetBytesPending.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectCancel.restype = None
    libnorm.NormObjectCancel.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectRetain.restype = None
    libnorm.NormObjectRetain.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectRelease.restype = None
    libnorm.NormObjectRelease.argtypes = [ctypes.c_void_p]

    libnorm.NormFileGetName.argtypes = [ctypes.c_void_p, ctypes.c_char_p,
            ctypes.c_uint]
    libnorm.NormFileGetName.errcheck = errcheck_bool

    libnorm.NormFileRename.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
    libnorm.NormFileRename.errcheck = errcheck_bool

    libnorm.NormDataAccessData.restype = ctypes.c_void_p
    libnorm.NormDataAccessData.argtypes = [ctypes.c_void_p]

    libnorm.NormDataDetachData.restype = ctypes.c_void_p
    libnorm.NormDataDetachData.argtypes = [ctypes.c_void_p]

    libnorm.NormObjectGetSender.restype = ctypes.c_void_p
    libnorm.NormObjectGetSender.argtypes = [ctypes.c_void_p]

    # Node functions
    libnorm.NormNodeGetId.restype = ctypes.c_uint32
    libnorm.NormNodeGetId.argtypes = [ctypes.c_void_p]

    libnorm.NormNodeGetAddress.argtypes = [ ctypes.c_void_p, ctypes.c_char_p,
            ctypes.POINTER(ctypes.c_uint), ctypes.POINTER(ctypes.c_uint16)]
    libnorm.NormNodeGetAddress.errcheck = errcheck_bool

    libnorm.NormNodeGetGrtt.restype = ctypes.c_double
    libnorm.NormNodeGetGrtt.argtypes = [ctypes.c_void_p]

    libnorm.NormNodeRetain.restype = None
    libnorm.NormNodeRetain.argtypes = [ctypes.c_void_p]

    libnorm.NormNodeRelease.restype = None
    libnorm.NormNodeRelease.argtypes = [ctypes.c_void_p]

    return libnorm

# Global libnorm refrence that everyone should use
libnorm = get_libnorm()
