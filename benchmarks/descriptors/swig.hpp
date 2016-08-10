#ifndef BENCHMARKS_DESCRIPTORS_SWIG_HPP
#define BENCHMARKS_DESCRIPTORS_SWIG_HPP


#include <swig/ISwigBenchmarks.hpp>

#if defined(_DEBUG)
#	undef _DEBUG
#	include <Python.h>
#	define _DEBUG
#else
#	include <Python.h>
#endif


namespace descriptors {
namespace swig
{

	struct Desc
	{
		class Invokable : public IInvokable
		{
		public:
			void NoParamsMethod() { }
		};

		class BenchmarkCtx
		{
		private:
			PyObject*	_pyBenchmarks;

		public:
			BenchmarkCtx(const std::string& bindingName, const std::string& moduleName)
				: _pyBenchmarks(nullptr)
			{
				if (bindingName == "python")
				{
					Py_Initialize();
					PyObject* py_module_name = PyUnicode_FromString(moduleName.c_str());
					PyObject* py_module = PyImport_Import(py_module_name);
					if (!py_module)
						throw std::runtime_error("Could not get import " + moduleName);
					const char* getter_name = "GetBenchmarks";
					PyObject* py_getter = PyObject_GetAttrString(py_module, getter_name);
					if (!py_getter)
						throw std::runtime_error("Could not find " + std::string(getter_name) + " function in " + moduleName);
					_pyBenchmarks = PyObject_CallObject(py_getter, nullptr);
					if (!_pyBenchmarks)
						throw std::runtime_error("Could not get benchmarks object!");
				}
				else
					throw std::runtime_error("Binding not supported!");
			}

			~BenchmarkCtx()
			{
				if (_pyBenchmarks)
				{
					Py_DECREF(_pyBenchmarks);
					Py_Finalize();
				}
			}

			IBenchmarks* CreateBenchmarks() const
			{
				auto result = GetGlobalBenchmarks();
				if (!result)
					throw std::runtime_error("Global benchmarks not set!");
				return result;
			}
			//{ return _module.GetRootObject<benchmarks::IBenchmarks>("GetBenchmarks"); }

			IInvokable* CreateLocalInvokable()
			{ return new Invokable; }
			//{ return _ctx.MakeComponent<benchmarks::IInvokable, Invokable>(); }
		};


		static std::string GetName() { return "swig"; }
	};

}}

#endif
