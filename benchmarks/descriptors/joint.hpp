#ifndef SRC_BENCHMARKS_DESCRIPTORS_SIGNALS_WIGWAG_HPP
#define SRC_BENCHMARKS_DESCRIPTORS_SIGNALS_WIGWAG_HPP


#include <Benchmarks_adapters.hpp>


namespace descriptors {
namespace joint
{

	struct Desc
	{
		class Module
		{
		private:
			::joint::Context	_ctx;
			::joint::Module		_module;

		public:
			Module(const std::string& bindingName, const std::string& moduleName)
				: _module(_ctx.LoadModule(bindingName, moduleName))
			{ }

			benchmarks::IBenchmarks_Ptr CreateBenchmarks() const
			{ return _module.GetRootObject<benchmarks::IBenchmarks>("GetBenchmarks"); }
		};

		static std::string GetName() { return "joint"; }
	};

}}

#endif
