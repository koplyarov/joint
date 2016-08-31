#ifndef JOINT_DEVKIT_STACKSTORAGE_HPP
#define JOINT_DEVKIT_STACKSTORAGE_HPP


#include <joint/devkit/Holder.hpp>

#include <array>


namespace joint {
namespace devkit
{

	template < typename T_ >
	T_ AlignUp(T_ value, T_ alignment)
	{ return alignment * ((value + alignment - 1) / alignment); }

	template < typename T_ >
	T_ AlignDown(T_ value, T_ alignment)
	{ return alignment * (value / alignment); }


	template < typename T_, size_t BytesSize_ >
	class StackStorage
	{
		using ElementStorage = typename std::aligned_storage<sizeof(T_), alignof(T_)>::type;
		using BufArray = std::array<ElementStorage, BytesSize_ / sizeof(T_)>;

		struct BlockDescriptor
		{
			size_t  			Count;
			ElementStorage*     Pointer;
		};

		using BlocksVector = std::vector<BlockDescriptor>;

	private:
		BufArray              _buf;
		size_t                _freeOfs;
		BlocksVector          _fallbackBlocks;

	public:
		StackStorage()
			: _freeOfs()
		{ }

		StackStorage(const StackStorage&) = delete;
		StackStorage* operator = (const StackStorage&) = delete;

		~StackStorage()
		{
			while (_freeOfs--)
				reinterpret_cast<T_&>(_buf[_freeOfs]).~T_();

			for (auto block : _fallbackBlocks)
			{
				auto count = block.Count;
				auto ptr = block.Pointer;
				for (size_t i = 0; i < count; ++i)
					reinterpret_cast<T_&>(ptr[i]).~T_();
				delete[] ptr;
			}
		}

		T_* Make(size_t count)
		{
			return AllocateAndConstruct(count, [&](T_* p) {
				size_t i = 0;
				auto sg(ScopeExit([&]() { while (i--) p[i].~T_(); }));
				for (; i < count; ++i)
					new(p + i) T_;
				sg.Cancel();
			});
		}

		template < typename... Args_ >
		T_& MakeSingle(Args_&&... args)
		{ return *AllocateAndConstruct(1, [&](T_* p, Args_&&... a) { new(p) T_(std::forward<Args_>(a)...); }, std::forward<Args_>(args)...); }

	private:
		template < typename ConstructFunc_, typename... Args_ >
		T_* AllocateAndConstruct(size_t count, ConstructFunc_&& constructFunc, Args_&&... args)
		{
			T_* result;
			if (_buf.size() >= _freeOfs + count)
			{
				auto ptr = &_buf[_freeOfs];
				_freeOfs += count;

				auto sg(ScopeExit([&]{ _freeOfs -= count; }));
				result = reinterpret_cast<T_*>(ptr);
				constructFunc(result, std::forward<Args_>(args)...);
				sg.Cancel();
			}
			else
			{
				static_assert(sizeof(ElementStorage) >= sizeof(T_), "sdfsdfgsdfg");
				auto ptr = new ElementStorage[count];
				_fallbackBlocks.push_back({ count, ptr });

				auto sg(ScopeExit([&]{ _fallbackBlocks.pop_back(); delete[] ptr; }));
				result = reinterpret_cast<T_*>(ptr);
				constructFunc(result, std::forward<Args_>(args)...);
				sg.Cancel();
			}

			return result;
		}
	};

}}

#endif
