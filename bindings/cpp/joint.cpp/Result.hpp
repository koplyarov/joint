#ifndef JOINT_CPP_RESULT_HPP
#define JOINT_CPP_RESULT_HPP


#include <joint/public/Assert.h>

#include <joint.cpp/Exception.hpp>
#include <joint.cpp/detail/Config.hpp>


namespace joint
{

    template < typename T_ >
    class Result
    {
        union Storage
        {
            T_                 _val;
            joint::Exception   _ex;

            Storage() { }
            ~Storage() { }
        };

    private:
        bool    _ok;
        Storage _storage;

    public:
        template < typename U_ >
        Result(const U_& val)
            : _ok(true)
        { new (ValueStorage()) T_(val); }

        Result(const joint::Exception& ex)
            : _ok(false)
        { new (ExceptionStorage()) joint::Exception(ex); }


        Result(const Result& other)
            : _ok(other._ok)
        {
            if (_ok)
                new (ValueStorage()) T_(*other.ValueStorage());
            else
                new (ExceptionStorage()) joint::Exception(*other.ExceptionStorage());
        }

        Result& operator=(const Result&) = delete;

        ~Result()
        {
            if (_ok)
                ValueStorage()->~T_();
            else
                ExceptionStorage()->~Exception();
        }

        bool Ok() const
        { return _ok; }

        const T_& Value() const
        {
            if (!_ok)
                JOINT_CORE_FATAL("Joint.C++.Result", "An exception occured: %s", ExceptionStorage()->what());
            return *ValueStorage();
        }

        const joint::Exception& Exception() const
        {
            if (_ok)
                JOINT_CORE_FATAL("Joint.C++.Result", "Result has no exception");
            return *ExceptionStorage();
        }

    private:
        T_* ValueStorage() { return &_storage._val; }
        const T_* ValueStorage() const { return &_storage._val; }
        joint::Exception* ExceptionStorage() { return &_storage._ex; }
        const joint::Exception* ExceptionStorage() const { return &_storage._ex; }
    };

    template < >
    class Result<void>
    {
        union Storage
        {
            joint::Exception   _ex;

            Storage() { }
            ~Storage() { }
        };

    private:
        bool    _ok;
        Storage _storage;

    public:
        Result()
            : _ok(true)
        { }

        Result(const joint::Exception& ex)
            : _ok(false)
        { new (ExceptionStorage()) joint::Exception(ex); }


        Result(const Result& other)
            : _ok(other._ok)
        {
            if (!_ok)
                new (ExceptionStorage()) joint::Exception(*other.ExceptionStorage());
        }

        Result& operator=(const Result&) = delete;

        ~Result()
        {
            if (!_ok)
                ExceptionStorage()->~Exception();
        }

        bool Ok() const
        { return _ok; }

        void Value() const
        {
            if (!_ok)
                JOINT_CORE_FATAL("Joint.C++.Result", "An exception occured!");
        }

        const joint::Exception& Exception() const
        {
            if (_ok)
                JOINT_CORE_FATAL("Joint.C++.Result", "Result has no exception");
            return *ExceptionStorage();
        }

    private:
        joint::Exception* ExceptionStorage() { return &_storage._ex; }
        const joint::Exception* ExceptionStorage() const { return &_storage._ex; }
    };


#if JOINT_CPP_CONFIG_NO_EXCEPTIONS
    namespace Detail
    {
        template < typename T_ >
        class ReturnProxy
        {
        private:
            T_    _val;

        public:
            ReturnProxy(const T_& val)
                : _val(val)
            { }

            template < typename U_ >
            operator Result<U_>() const
            { return Result<U_>(_val); }
        };

    }
#endif


#if JOINT_CPP_CONFIG_NO_EXCEPTIONS
#   define JOINT_CPP_RET_TYPE(...) ::joint::Result<__VA_ARGS__ >
#   define JOINT_CPP_THROW(...) return (__VA_ARGS__)
#   define JOINT_CPP_RETURN(...) return (__VA_ARGS__)
#   define JOINT_CPP_RETURN_VOID() return ::joint::Result<void>()
#   define JOINT_CPP_RET_VALUE(...) (__VA_ARGS__).Value()
#   define JOINT_CPP_RET_OK(...) (__VA_ARGS__).Ok()
#   define JOINT_CPP_RETHROW(...) \
        do { \
            if (!JOINT_CPP_RET_OK(__VA_ARGS__)) \
                JOINT_CPP_THROW((__VA_ARGS__).Exception()); \
        } while (false)

#   define DETAIL_JOINT_CPP_INVOKE_METHOD(Type_, MethodCall_) ::joint::Result<Type_> result(MethodCall_)
#   define DETAIL_JOINT_CPP_INVOKE_VOID_METHOD(MethodCall_) ::joint::Result<void> result(MethodCall_)
#   define DETAIL_JOINT_CPP_METHOD_RESULT result.Value()
#else
#   define JOINT_CPP_RET_TYPE(...) __VA_ARGS__
#   define JOINT_CPP_THROW(...) throw (__VA_ARGS__)
#   define JOINT_CPP_RETURN(...) return (__VA_ARGS__)
#   define JOINT_CPP_RETURN_VOID() return
#   define JOINT_CPP_RET_VALUE(...) (__VA_ARGS__)
#   define JOINT_CPP_RET_OK(...) true
#   define JOINT_CPP_RETHROW(...) do { (void)(__VA_ARGS__); } while (false)

#   define DETAIL_JOINT_CPP_INVOKE_METHOD(Type_, MethodCall_) Type_ result(MethodCall_)
#   define DETAIL_JOINT_CPP_INVOKE_VOID_METHOD(MethodCall_) MethodCall_
#   define DETAIL_JOINT_CPP_METHOD_RESULT result
#endif

}

#endif
