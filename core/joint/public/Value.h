#ifndef JOINT_PUBLIC_VALUE_H
#define JOINT_PUBLIC_VALUE_H


#include <joint/public/BasicTypes.h>
#include <joint/public/Error.h>
#include <joint/public/Exception.h>
#include <joint/public/JointConfig.h>
#include <joint/public/ObjectAccessor.h>
#include <joint/public/Type.h>


JOINT_CORE_EXTERN_C_BEGIN

	typedef struct JointCore_Parameter_s JointCore_Parameter;
	typedef struct JointCore_RetValue_s JointCore_RetValue;


	typedef union _JointCore_Value_t
	{
		JointCore_Bool              b;
		int8_t                      i8;
		uint8_t                     u8;
		int16_t                     i16;
		uint16_t                    u16;
		int32_t                     i32;
		uint32_t                    u32;
		int64_t                     i64;
		uint64_t                    u64;
		float                       f32;
		double                      f64;
		const char*                 utf8;
		int32_t                     e;
		union _JointCore_Value_t*   members;
		JointCore_ObjectAccessor    obj;
		struct Joint_Array*         array;
	} JointCore_Value;


	struct JointCore_Parameter_s
	{
		JointCore_Value value;
	};


	typedef JointCore_Error JointCore_ReleaseRetValue_Func(JointCore_Type type, JointCore_Value value);

	struct JointCore_RetValue_s
	{
		union
		{
			JointCore_Value               value;
			JointCore_Exception_Handle    ex;
		} result;
		JointCore_ReleaseRetValue_Func*   releaseValue;
	};

JOINT_CORE_EXTERN_C_END

#endif
