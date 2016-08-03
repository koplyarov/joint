#ifndef JOINT_C_DETAIL_PREPROCESSOR_H
#define JOINT_C_DETAIL_PREPROCESSOR_H


#define JOINT_C_PP_PARAM(...)	__VA_ARGS__


#define JOINT_C_PP_CAT(A, B)	DETAIL_JOINT_C_PP_CAT_IMPL(A, B)
#define DETAIL_JOINT_C_PP_CAT_IMPL(A, B)	A##B



#define JOINT_C_PP_COUNT(...) DETAIL_JOINT_C_PP_COUNT_SEQ(Dummy_, ##__VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define DETAIL_JOINT_C_PP_COUNT_SEQ(Dummy_, _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,N,...) N


#define JOINT_C_PP_REPEAT(Count_, MacroName_, ...) JOINT_C_PP_CAT(DETAIL_JOINT_C_PP_REPEAT_IMPL_, Count_)(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__))

#define DETAIL_JOINT_C_PP_REPEAT_IMPL_0(MacroName_, ...)
#define DETAIL_JOINT_C_PP_REPEAT_IMPL_1(MacroName_, ...) DETAIL_JOINT_C_PP_REPEAT_IMPL_0(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__)) MacroName_( 0, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_REPEAT_IMPL_2(MacroName_, ...) DETAIL_JOINT_C_PP_REPEAT_IMPL_1(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__)) MacroName_( 1, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_REPEAT_IMPL_3(MacroName_, ...) DETAIL_JOINT_C_PP_REPEAT_IMPL_2(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__)) MacroName_( 2, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_REPEAT_IMPL_4(MacroName_, ...) DETAIL_JOINT_C_PP_REPEAT_IMPL_3(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__)) MacroName_( 3, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_REPEAT_IMPL_5(MacroName_, ...) DETAIL_JOINT_C_PP_REPEAT_IMPL_4(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(__VA_ARGS__)) MacroName_( 4, __VA_ARGS__)


#define JOINT_C_PP_FOREACH(MacroName_, UserArg_, ...) JOINT_C_PP_CAT(DETAIL_JOINT_C_PP_FOREACH_IMPL_, JOINT_C_PP_COUNT(__VA_ARGS__))(JOINT_C_PP_PARAM(MacroName_), JOINT_C_PP_PARAM(UserArg_), __VA_ARGS__)

#define DETAIL_JOINT_C_PP_FOREACH_IMPL_0(MacroName_, UserArg_, ...)
#define DETAIL_JOINT_C_PP_FOREACH_IMPL_1(MacroName_, UserArg_, Element_, ...)  MacroName_(Element_, UserArg_) DETAIL_JOINT_C_PP_FOREACH_IMPL_0(MacroName_, UserArg_, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_FOREACH_IMPL_2(MacroName_, UserArg_, Element_, ...)  MacroName_(Element_, UserArg_) DETAIL_JOINT_C_PP_FOREACH_IMPL_1(MacroName_, UserArg_, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_FOREACH_IMPL_3(MacroName_, UserArg_, Element_, ...)  MacroName_(Element_, UserArg_) DETAIL_JOINT_C_PP_FOREACH_IMPL_2(MacroName_, UserArg_, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_FOREACH_IMPL_4(MacroName_, UserArg_, Element_, ...)  MacroName_(Element_, UserArg_) DETAIL_JOINT_C_PP_FOREACH_IMPL_3(MacroName_, UserArg_, __VA_ARGS__)
#define DETAIL_JOINT_C_PP_FOREACH_IMPL_5(MacroName_, UserArg_, Element_, ...)  MacroName_(Element_, UserArg_) DETAIL_JOINT_C_PP_FOREACH_IMPL_4(MacroName_, UserArg_, __VA_ARGS__)


#endif
