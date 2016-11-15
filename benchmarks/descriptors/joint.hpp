#ifndef BENCHMARKS_DESCRIPTORS_JOINT_HPP
#define BENCHMARKS_DESCRIPTORS_JOINT_HPP


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

			void NoParamsToVoid() { }
			void I32ToVoid(int32_t) { }
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

			benchmarks::IBenchmarks_Ptr CreateBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IBenchmarks>("GetBenchmarks"); }

			benchmarks::IInvokable_Ptr CreateLocalInvokable()
			{ return _ctx.MakeComponent<benchmarks::IInvokable, Invokable>(); }
		};


		static std::string GetName() { return "joint"; }
	};

}}

#endif
