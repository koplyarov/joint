#ifndef SRC_BENCHMARKS_DESCRIPTORS_SIGNALS_WIGWAG_HPP
#define SRC_BENCHMARKS_DESCRIPTORS_SIGNALS_WIGWAG_HPP


#include <Benchmarks_adapters.hpp>


namespace descriptors {
namespace joint
{

	struct Desc
	{
		class Invokable
		{
		public:
			typedef ::joint::TypeList<benchmarks::IInvokable>	JointInterfaces;

			void NoParamsMethod() { }
		};

		class BenchmarkCtx
		{
		private:
			::joint::Context	_ctx;
			::joint::Module		_module;

		public:
			BenchmarkCtx(const std::string& bindingName, const std::string& moduleName)
				: _module(_ctx.LoadModule(bindingName, moduleName))
			{ }

			::joint::Context& GetContext() { return _ctx; }

			benchmarks::IBenchmarks_Ptr CreateBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IBenchmarks>("GetBenchmarks"); }

			benchmarks::IInvokable_Ptr CreateLocalInvokable()
			{ return _ctx.MakeComponent<benchmarks::IInvokable, Invokable>(); }
		};


		static std::string GetName() { return "joint"; }
	};

}}

#endif
