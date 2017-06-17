#ifndef JOINT_C_ARRAY_H
#define JOINT_C_ARRAY_H


#include <joint/Joint.h>

#include <joint.c/detail/Exception.h>
#include <joint.c/detail/String.h>


#define JOINT_C_INCREF_ARRAY(Array_) Joint_IncRefArray((Array_).handle)
#define JOINT_C_DECREF_ARRAY(Array_) Joint_DecRefArray((Array_).handle)

#define DETAIL_JOINT_C_DEFINE_ARRAY(Name_, CppType_, JointType_, GetCode_, SetCode_) \
	typedef struct { JointCore_ArrayHandle handle; } Name_##__Array; \
	JointCore_Error Name_##__Array_Create(size_t size, Name_##__Array* array, JointCore_ExceptionHandle* ex) \
	{ \
		JointCore_Type t; \
		t.id = JointType_; \
		JointCore_Error ret = Joint_MakeArray(t, size, &array->handle); \
		if (ret != JOINT_CORE_ERROR_NONE) \
			return JOINT_THROW("Joint_MakeArray failed!", ex); \
		return JOINT_CORE_ERROR_NONE; \
	} \
	JointCore_Error Name_##__Array_GetSize(Name_##__Array array, size_t* result, JointCore_ExceptionHandle* ex) \
	{ \
		JointCore_SizeT size; \
		JointCore_Error ret = Joint_ArrayGetSize(array.handle, &size); \
		if (ret != JOINT_CORE_ERROR_NONE) \
			return JOINT_THROW("Joint_ArrayGet failed!", ex); \
		*result = size; \
		return JOINT_CORE_ERROR_NONE; \
	} \
	JointCore_Error Name_##__Array_Get(Name_##__Array array, size_t index, CppType_* result, JointCore_ExceptionHandle* ex) \
	{ \
		JointCore_Value result_v; \
		JointCore_Error ret = Joint_ArrayGet(array.handle, index, &result_v); \
		if (ret != JOINT_CORE_ERROR_NONE) \
			return JOINT_THROW("Joint_ArrayGet failed!", ex); \
		{ GetCode_; } \
		return JOINT_CORE_ERROR_NONE; \
	} \
	JointCore_Error Name_##__Array_Set(Name_##__Array array, size_t index, CppType_ value, JointCore_ExceptionHandle* ex) \
	{ \
		JointCore_Value value_v; \
		{ SetCode_; } \
		JointCore_Error ret = Joint_ArraySet(array.handle, index, value_v); \
		if (ret != JOINT_CORE_ERROR_NONE) \
			return JOINT_THROW("Joint_ArraySet failed!", ex); \
		return JOINT_CORE_ERROR_NONE; \
	}


#define DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(Name_, CppType_, JointType_, VariantMember_) \
	DETAIL_JOINT_C_DEFINE_ARRAY(Name_, CppType_, JointType_, \
			*result = result_v.VariantMember_, \
			value_v.VariantMember_ = value \
		)

DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(bool, JointCore_Bool, JOINT_CORE_TYPE_BOOL, i64)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(u8, uint8_t, JOINT_CORE_TYPE_U8, u8)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(i8, int8_t, JOINT_CORE_TYPE_I8, i8)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(u16, uint16_t, JOINT_CORE_TYPE_U16, u16)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(i16, int16_t, JOINT_CORE_TYPE_I16, i16)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(u32, uint32_t, JOINT_CORE_TYPE_U32, u32)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(i32, int32_t, JOINT_CORE_TYPE_I32, i32)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(u64, uint64_t, JOINT_CORE_TYPE_U64, u64)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(i64, int64_t, JOINT_CORE_TYPE_I64, i64)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(f32, float, JOINT_CORE_TYPE_F32, f32)
DETAIL_JOINT_C_DEFINE_BASIC_ARRAY(f64, double, JOINT_CORE_TYPE_F64, f64)

DETAIL_JOINT_C_DEFINE_ARRAY(string, const char*, JOINT_CORE_TYPE_UTF8,
		*result = Joint_CopyString(result_v.utf8),
		value_v.utf8 = value
	)

#undef DETAIL_JOINT_C_DEFINE_BASIC_ARRAY
#undef DETAIL_JOINT_C_DEFINE_ARRAY


#endif
