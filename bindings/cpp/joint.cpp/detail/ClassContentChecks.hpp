#ifndef JOINT_CPP_DETAIL_CLASSCONTENTCHECKS_HPP
#define JOINT_CPP_DETAIL_CLASSCONTENTCHECKS_HPP


#include <joint.cpp/MetaProgramming.hpp>


#define JOINT_CPP_DECLARE_NESTED_TYPE_CHECK(NestedType_) \
    template < typename T > \
    class HasNestedType_##NestedType_ \
    { \
        template < typename U > static YesType deduce(IntConstant<sizeof(typename RemoveReference<typename U::NestedType_>::ValueT*)>*); \
        template < typename U > static NoType deduce(...); \
        \
    public: \
        static const bool Value = sizeof(deduce<T>(0)) == sizeof(YesType); \
    }

#define JOINT_CPP_DECLARE_METHOD_CHECK(Method_) \
    template < typename T > \
    struct HasMethod_##Method_ \
    { \
        template <typename Type_> \
        class Impl \
        { \
            struct BaseMixin { void Method_(){} }; \
            struct Base : public Type_, public BaseMixin { Base(); }; \
            \
            template <typename V, V t> class Helper { }; \
            \
            template <typename U> static NoType deduce(U*, Helper<void (BaseMixin::*)(), &U::Method_>* = 0); \
            static YesType deduce(...); \
            \
        public: \
            static const bool Value = (sizeof(YesType) == sizeof(deduce((Base*)(0)))); \
        }; \
        static const bool Value = \
            IfElse< \
                    IsClass<T>::Value, \
                    Impl<T>, \
                    FalseConstant \
                >::ValueT::Value; \
    }

#endif
