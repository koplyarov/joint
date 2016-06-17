#include <iostream>
#include <memory>

#include <../bindings/python/PythonBinding.h>
#include <../bindings/python/PythonModule.hpp>


#define JOINT_CALL(...) \
		do { \
			Joint_Error ret = (__VA_ARGS__); \
			if (ret != JOINT_ERROR_NONE) \
				throw std::runtime_error(std::string(#__VA_ARGS__ " failed: ") + Joint_ErrorToString(ret)); \
		} while (false)

struct IOtherInterface
{
	virtual ~IOtherInterface() { }

	virtual void Func() = 0;
};

struct ISomeInterface
{
	virtual ~ISomeInterface() { }

	virtual void Method1() = 0;
	virtual void Method2() = 0;
	virtual std::string ToString() = 0;
	virtual void PrintInt(int i) = 0;
	virtual void PrintString(const std::string& s) = 0;
	virtual IOtherInterface* ReturnOther() = 0;
};


class OtherInterfaceWrapper : public IOtherInterface
{
private:
	Joint_ObjectHandle		_obj;

public:
	OtherInterfaceWrapper(Joint_ObjectHandle obj)
		: _obj(obj)
	{ }

	~OtherInterfaceWrapper()
	{ JOINT_CALL( Joint_DecRefObject(_obj) ); }

	virtual void Func()
	{
		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(_obj, 0, nullptr, 0, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
	}
};


class SomeInterfaceWrapper : public ISomeInterface
{
private:
	Joint_ObjectHandle		_obj;

public:
	SomeInterfaceWrapper(Joint_ObjectHandle obj)
		: _obj(obj)
	{ }

	~SomeInterfaceWrapper()
	{ JOINT_CALL( Joint_DecRefObject(_obj) ); }

	virtual void Method1()
	{
		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(_obj, 0, nullptr, 0, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
	}

	virtual void Method2()
	{
		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(_obj, 1, nullptr, 0, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
	}

	virtual std::string ToString()
	{
		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(_obj, 2, nullptr, 0, JOINT_TYPE_UTF8, &ret_val) );
		std::string result = ret_val.variant.value.utf8;
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
		return result;
	}

	virtual void PrintInt(int i)
	{
		Joint_RetValue ret_val;
		Joint_Variant params[1];
		params[0].value.i32 = i;
		params[0].type = JOINT_TYPE_I32;
		JOINT_CALL( Joint_InvokeMethod(_obj, 3, params, 1, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
	}

	virtual void PrintString(const std::string& s)
	{
		Joint_RetValue ret_val;
		Joint_Variant params[1];
		params[0].value.utf8 = s.c_str();
		params[0].type = JOINT_TYPE_UTF8;
		JOINT_CALL( Joint_InvokeMethod(_obj, 4, params, 1, JOINT_TYPE_VOID, &ret_val) );
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
	}

	virtual IOtherInterface* ReturnOther()
	{
		Joint_RetValue ret_val;
		JOINT_CALL( Joint_InvokeMethod(_obj, 5, nullptr, 0, JOINT_TYPE_OBJ, &ret_val) );
		std::unique_ptr<IOtherInterface> result(new OtherInterfaceWrapper(ret_val.variant.value.obj));;
		JOINT_CALL( Joint_ReleaseRetValue(ret_val) );
		return result.release();
	}
};


class JointModule
{
private:
	Joint_ModuleHandle		_module;
	std::string				_bindingName;
	std::string				_moduleName;

public:
	JointModule(std::string bindingName, std::string moduleName)
		: _bindingName(std::move(bindingName)), _moduleName(std::move(moduleName))
	{ JOINT_CALL( Joint_LoadModule(_bindingName.c_str(), _moduleName.c_str(), &_module) ); }

	JointModule(JointModule&& other)
		: _module(other._module)
	{ other._module = JOINT_NULL_HANDLE; }

	~JointModule()
	{
		if (_module != JOINT_NULL_HANDLE)
		{
			Joint_Error ret = Joint_UnloadModule(_module);
			if (ret != JOINT_ERROR_NONE)
				Joint_Log(JOINT_LOGLEVEL_WARNING, "Joint.C++", "Could not unload module");
		}
	}

	ISomeInterface* GetRootObject(const std::string& getterName) const
	{
		Joint_ObjectHandle obj;
		JOINT_CALL( Joint_GetRootObject(_module, "func", &obj) );
		return new SomeInterfaceWrapper(obj);
	}

	JointModule(const JointModule& other) = delete;
	JointModule& operator = (const JointModule& other) = delete;
};


int main()
{
	try
	{
		JOINT_CALL( JointPython_Register() );

		JointModule m("python", "test_module");

		ISomeInterface* obj = m.GetRootObject("func");
		obj->Method1();
		obj->Method2();
		auto s = obj->ToString();
		std::cout << "obj.ToString(): " << s << std::endl;

		obj->PrintInt(42);
		obj->PrintString("qwe");

		IOtherInterface* other = obj->ReturnOther();
		other->Func();

		delete other;
		delete obj;

		JOINT_CALL( JointPython_Unregister() );
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Uncaught exception: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
