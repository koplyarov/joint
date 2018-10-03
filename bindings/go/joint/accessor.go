package joint

/*
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <joint/Joint.h>

typedef struct { uint64_t Words[2]; } JointGo_ValueStorage;
typedef struct { uint64_t Words[2]; JointCore_ReleaseRetValue_Func* ReleaseValue; } JointGo_RetValueStorage;

static JointCore_Error JointGo_ReleaseRetValue(JointCore_Type type, JointCore_Value value)
{
    int32_t i;
    switch (type.id)
    {
    case JOINT_CORE_TYPE_UTF8:
        free((void*)value.utf8);
        return JOINT_CORE_ERROR_NONE;
    case JOINT_CORE_TYPE_STRUCT:
        for (i = 0; i < type.payload.structDescriptor->membersCount; ++i)
            JointGo_ReleaseRetValue(type.payload.structDescriptor->memberTypes[i], value.members[i]);
        free(value.members);
        return JOINT_CORE_ERROR_NONE;
    default:
        return JOINT_CORE_ERROR_NONE;
    }
}

static JointCore_ReleaseRetValue_Func* JointGo_GetReleaseRetValueFunc()
{ return &JointGo_ReleaseRetValue; }

typedef struct JointGo_Accessor_s JointGo_Accessor;

typedef struct
{
    void (*IncRef)(intptr_t component);
    void (*DecRef)(intptr_t component);
    JointCore_Error (*CastObject)(intptr_t component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointGo_Accessor* outAccessor);
    JointCore_Error (*InvokeMethod)(intptr_t component, JointCore_SizeT methodId, JointGo_ValueStorage* params, JointCore_SizeT paramsCount, JointGo_RetValueStorage* outRetValue);
} JointGo_AccessorVTable;

struct JointGo_Accessor_s
{
    JointGo_AccessorVTable*   VTable;
    intptr_t                  Instance;
};
static JointGo_AccessorVTable g_go_obj_vtable;

extern void _JointGo_IncRef(intptr_t component);
extern void _JointGo_DecRef(intptr_t component);
extern JointCore_Error _JointGo_CastObject(intptr_t component, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointGo_Accessor* outAccessor);
extern JointCore_Error _JointGo_InvokeMethod(intptr_t component, JointCore_SizeT methodId, JointGo_ValueStorage* params, JointCore_SizeT paramsCount, JointGo_RetValueStorage* outRetValue);

static void Init_Accessor_VTable()
{
    g_go_obj_vtable.IncRef = &_JointGo_IncRef;
    g_go_obj_vtable.DecRef = &_JointGo_DecRef;
    g_go_obj_vtable.InvokeMethod = &_JointGo_InvokeMethod;
    g_go_obj_vtable.CastObject = &_JointGo_CastObject;
}

static void Accessor_Init(intptr_t component, JointGo_Accessor* accessor)
{
    Init_Accessor_VTable();
    accessor->VTable = &g_go_obj_vtable;
    accessor->Instance = component;
}

static JointCore_Error Accessor_InvokeMethod(JointGo_Accessor accessor, JointCore_SizeT methodId, JointGo_ValueStorage* params, JointCore_SizeT paramsCount, JointGo_RetValueStorage* outRetValue)
{ return accessor.VTable->InvokeMethod(accessor.Instance, methodId, params, paramsCount, outRetValue); }

static void Accessor_IncRef(JointGo_Accessor accessor)
{ accessor.VTable->IncRef(accessor.Instance); }

static void Accessor_DecRef(JointGo_Accessor accessor)
{ accessor.VTable->DecRef(accessor.Instance); }

static JointCore_Error Accessor_CastObject(JointGo_Accessor accessor, JointCore_InterfaceId interfaceId, JointCore_InterfaceChecksum checksum, JointGo_Accessor* outAccessor)
{ return accessor.VTable->CastObject(accessor.Instance, interfaceId, checksum, outAccessor); }
*/
import "C"

import "unsafe"

func toCAccessor(accessor Internal_AccessorNative) (result C.JointGo_Accessor) {
    result.VTable = (*C.JointGo_AccessorVTable)(unsafe.Pointer(accessor.VTable))
    result.Instance = (C.intptr_t)(accessor.Instance)
    return
}

func toCAccessorPtr(accessorPtr *Internal_AccessorNative) *C.JointGo_Accessor {
    return (*C.JointGo_Accessor)(unsafe.Pointer(accessorPtr))
}

//export _JointGo_IncRef
func _JointGo_IncRef(component C.intptr_t) {
    c := Internal_GetObject(Internal_ComponentHandle(component))
    c.IncRef()
}

//export _JointGo_DecRef
func _JointGo_DecRef(component C.intptr_t) {
    c := Internal_GetObject(Internal_ComponentHandle(component))
    if (c.DecRef() == 0) {
        Internal_UnregisterObject(Internal_ComponentHandle(component))
    }
}

//export _JointGo_CastObject
func _JointGo_CastObject(
    component C.intptr_t,
    interfaceId C.JointCore_InterfaceId,
    checksum C.JointCore_InterfaceChecksum,
    outAccessor *C.JointGo_Accessor,
) C.JointCore_Error {
    c := Internal_GetObject(Internal_ComponentHandle(component))
    var castable = c.GetCastable()
    var result = castable.Cast(C.GoString(interfaceId))
    if result == nil {
        return C.JOINT_CORE_ERROR_CAST_FAILED
    }
    *outAccessor = toCAccessor(result.GetAccessor())
    return C.JOINT_CORE_ERROR_NONE
}

//export _JointGo_InvokeMethod
func _JointGo_InvokeMethod(
    component C.intptr_t,
    methodId C.JointCore_SizeT,
    params *C.JointGo_ValueStorage,
    paramsCount C.JointCore_SizeT,
    outRetValue *C.JointGo_RetValueStorage,
) Internal_ErrorCode {
    c := Internal_GetObject(Internal_ComponentHandle(component))
    return c.InvokeMethod(
        Internal_Size(methodId),
        (*Internal_ValueStorage)(params),
        Internal_Size(paramsCount),
        (*Internal_RetValueStorage)(outRetValue),
    )
}

func Internal_GetValueFirstPtr(params *Internal_ValueStorage, index Internal_Size) unsafe.Pointer {
    return unsafe.Pointer(uintptr(unsafe.Pointer(params)) + uintptr(index) * unsafe.Sizeof(*params))
}

func Internal_GetValueSecondPtr(params *Internal_ValueStorage, index Internal_Size) unsafe.Pointer {
    return unsafe.Pointer(uintptr(unsafe.Pointer(params)) + unsafe.Sizeof(params.Words[0]) + uintptr(index) * unsafe.Sizeof(*params))
}

func Internal_GetRetValueFirstPtr(params *Internal_RetValueStorage, index Internal_Size) unsafe.Pointer {
    return unsafe.Pointer(uintptr(unsafe.Pointer(params)) + uintptr(index) * unsafe.Sizeof(*params))
}

func Internal_GetRetValueSecondPtr(params *Internal_RetValueStorage, index Internal_Size) unsafe.Pointer {
    return unsafe.Pointer(uintptr(unsafe.Pointer(params)) + unsafe.Sizeof(params.Words[0]) + uintptr(index) * unsafe.Sizeof(*params))
}

type Internal_AccessorNativeVTable C.JointGo_AccessorVTable
//type Internal_AccessorNative C.JointGo_Accessor
type Internal_ErrorCode C.JointCore_Error
type Internal_InterfaceId C.JointCore_InterfaceId
type Internal_InterfaceChecksum C.JointCore_InterfaceChecksum
type Internal_ComponentHandle C.intptr_t
type Internal_Size C.JointCore_SizeT
type Internal_ValueStorage C.JointGo_ValueStorage
type Internal_RetValueStorage C.JointGo_RetValueStorage

type Internal_AccessorNative struct {
    VTable uintptr
    Instance uintptr
}

var Internal_ReleaseRetValueFunc = C.JointGo_GetReleaseRetValueFunc()

const (
    Internal_ErrorCode_None                      Internal_ErrorCode = C.JOINT_CORE_ERROR_NONE
    Internal_ErrorCode_CastFailed                Internal_ErrorCode = C.JOINT_CORE_ERROR_CAST_FAILED
    Internal_ErrorCode_Exception                 Internal_ErrorCode = C.JOINT_CORE_ERROR_EXCEPTION
    Internal_ErrorCode_Generic                   Internal_ErrorCode = C.JOINT_CORE_ERROR_GENERIC
    Internal_ErrorCode_NoSuchBinding             Internal_ErrorCode = C.JOINT_CORE_ERROR_NO_SUCH_BINDING
    Internal_ErrorCode_NoSuchModule              Internal_ErrorCode = C.JOINT_CORE_ERROR_NO_SUCH_MODULE
    Internal_ErrorCode_NotImplemented            Internal_ErrorCode = C.JOINT_CORE_ERROR_NOT_IMPLEMENTED
    Internal_ErrorCode_InvalidParameter          Internal_ErrorCode = C.JOINT_CORE_ERROR_INVALID_PARAMETER
    Internal_ErrorCode_OutOfMemory               Internal_ErrorCode = C.JOINT_CORE_ERROR_OUT_OF_MEMORY
    Internal_ErrorCode_ImplementationError       Internal_ErrorCode = C.JOINT_CORE_ERROR_IMPLEMENTATION_ERROR
    Internal_ErrorCode_InvalidInterfaceChecksum  Internal_ErrorCode = C.JOINT_CORE_ERROR_INVALID_INTERFACE_CHECKSUM
    Internal_ErrorCode_IndexOutOfRange           Internal_ErrorCode = C.JOINT_CORE_ERROR_INDEX_OUT_OF_RANGE
    Internal_ErrorCode_IoError                   Internal_ErrorCode = C.JOINT_CORE_ERROR_IO_ERROR
    Internal_ErrorCode_InvalidManifest           Internal_ErrorCode = C.JOINT_CORE_ERROR_INVALID_MANIFEST
)


type Internal_Accessor interface {
    IncRef()
    DecRef() int32
    GetCastable() Internal_Castable
    InvokeMethod(methodId Internal_Size, params *Internal_ValueStorage, paramsCount Internal_Size, outRetValue *Internal_RetValueStorage) Internal_ErrorCode
}

type Internal_Proxy interface {
    GetAccessor() Internal_AccessorNative
}

type Internal_Castable interface {
    Cast(interfaceId string) Internal_Proxy
}


func Internal_Accessor_Init(component Internal_ComponentHandle, accessor *Internal_AccessorNative) {
    C.Accessor_Init(C.intptr_t(component), toCAccessorPtr(accessor))
}

func Internal_Accessor_InvokeMethod(
    accessor Internal_AccessorNative,
    methodId Internal_Size,
    params *Internal_ValueStorage,
    paramsCount Internal_Size,
    outRetValue *Internal_RetValueStorage,
) Internal_ErrorCode {
    return Internal_ErrorCode(
        C.Accessor_InvokeMethod(
            toCAccessor(accessor),
            C.JointCore_SizeT(methodId),
            (*C.JointGo_ValueStorage)(params),
            C.JointCore_SizeT(paramsCount),
            (*C.JointGo_RetValueStorage)(outRetValue),
        ),
    )
}

func Internal_Accessor_IncRef(accessor Internal_AccessorNative) {
    C.Accessor_IncRef(toCAccessor(accessor))
}

func Internal_Accessor_DecRef(accessor Internal_AccessorNative) {
    C.Accessor_DecRef(toCAccessor(accessor))
}

func Internal_Accessor_CastObject(
    accessor Internal_AccessorNative,
    interfaceId Internal_InterfaceId,
    checksum Internal_InterfaceChecksum,
    outAccessor *Internal_AccessorNative,
) Internal_ErrorCode {
    return Internal_ErrorCode(
        C.Accessor_CastObject(
            toCAccessor(accessor),
            interfaceId,
            C.JointCore_InterfaceChecksum(checksum),
            toCAccessorPtr(outAccessor),
        ),
    )
}

func Internal_MakeCString(str string) uintptr {
    return uintptr(unsafe.Pointer(C.CString(str)))
}
func Internal_DeleteCString(str uintptr) {
    C.free(unsafe.Pointer(str))
}
