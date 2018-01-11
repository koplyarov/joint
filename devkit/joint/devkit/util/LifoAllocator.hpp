#ifndef JOINT_DEVKIT_UTIL_LIFOALLOCATOR_HPP
#define JOINT_DEVKIT_UTIL_LIFOALLOCATOR_HPP


#include <joint/Joint.h>
#include <joint/devkit/log/Logger.hpp>
#include <joint/devkit/meta/IntToType.hpp>
#include <joint/devkit/util/Alignment.hpp>
#include <joint/devkit/util/JointException.hpp>

#include <tuple>

#include <stdlib.h>


namespace joint {
namespace devkit
{

    namespace detail_LifoAllocator
    {
        class Arena
        {
            JOINT_DEVKIT_LOGGER("Joint.DevKit.LifoAllocator.Arena")

        private:
            char* _begin;
            char* _end;
            char* _freeSpacePtr;

        public:
            Arena(char* begin, char* end)
                : _begin(begin), _end(end), _freeSpacePtr(begin)
            { }

            bool Contains(void* ptr)
            { return ptr >= _begin && ptr < _end; }

            void* Allocate(size_t size, size_t alignment)
            {
                const auto& LoggerName = GetLogger().GetName();

                JOINT_DEVKIT_ASSERT(alignment > 0);

                char* beginPtr = AlignDown(_freeSpacePtr, alignment);
                char* endPtr = beginPtr + size;

                if (endPtr > _end)
                    return nullptr;

                _freeSpacePtr = endPtr;
                return beginPtr;
            }

            void* Shrink(void* ptr, size_t oldSize, size_t newSize, size_t alignment)
            {
                const auto& LoggerName = GetLogger().GetName();

                JOINT_DEVKIT_ASSERT(ptr >= _begin && ptr + oldSize <= _end);
                JOINT_DEVKIT_ASSERT(oldSize >= newSize);
                JOINT_DEVKIT_ASSERT(reinterpret_cast<char*>(ptr) + oldSize == _freeSpacePtr); // The last allocated block

                _freeSpacePtr -= oldSize - newSize;

                return ptr;
            }

            void Deallocate(void* ptr, size_t size)
            {
                const auto& LoggerName = GetLogger().GetName();

                JOINT_DEVKIT_ASSERT(ptr >= _begin && ptr + oldSize <= _end);
                JOINT_DEVKIT_ASSERT(reinterpret_cast<char*>(ptr) + size == _freeSpacePtr); // The last allocated block

                _freeSpacePtr -= size;
            }
        };


        template < typename Allocator_ >
        class AllocatorOps
        {
        public:
            bool Contains(void* ptr)
            { return Instance().GetArena().Contains(ptr); }

            void* Allocate(size_t size, size_t alignment)
            { return Instance().GetArena().Allocate(size, alignment); }

            void* Shrink(void* ptr, size_t oldSize, size_t newSize, size_t alignment)
            { return Instance().GetArena().Shrink(ptr, oldSize, newSize, alignment); }

            void Deallocate(void* ptr, size_t size)
            { Instance().GetArena().Deallocate(ptr, size); }

        protected:
            ~AllocatorOps() { }

        private:
            Allocator_& Instance() { return *static_cast<Allocator_*>(this); }
        };
    }


    template < size_t ArenaSize_ >
    class InplaceLifoAllocator : public detail_LifoAllocator::AllocatorOps<InplaceLifoAllocator<ArenaSize_>>
    {
        using Arena = detail_LifoAllocator::Arena;
        using ArenaBuf = std::array<char, ArenaSize_>;

    private:
        ArenaBuf                        _arenaBuf;
        detail_LifoAllocator::Arena     _arena;

    public:
        InplaceLifoAllocator()
            : _arena(_arenaBuf.data(), _arenaBuf.data() + _arenaBuf.size())
        { }

        Arena& GetArena()
        { return _arena; }
    };


    template < size_t ArenaSize_ >
    class ThreadLocalLifoAllocator : public detail_LifoAllocator::AllocatorOps<ThreadLocalLifoAllocator<ArenaSize_>>
    {
        using Arena = detail_LifoAllocator::Arena;

    public:
        static Arena& GetArena()
        {
            thread_local std::vector<char> arenaBuf(ArenaSize_);
            thread_local Arena arena(arenaBuf.data(), arenaBuf.data() + arenaBuf.size());
            return arena;
        }
    };


    class LargeBlockFallbackLifoAllocator
    {
        using Arena = detail_LifoAllocator::Arena;

    public:
        bool Contains(void* ptr)
        { return true; } // TODO: Is there a better way?

        void* Allocate(size_t size, size_t alignment)
        {
            JOINT_CORE_UNUSED(alignment);
            return malloc(size);
        }

        void* Shrink(void* ptr, size_t oldSize, size_t newSize, size_t alignment)
        {
            JOINT_CORE_UNUSED(oldSize);
            JOINT_CORE_UNUSED(newSize);
            JOINT_CORE_UNUSED(alignment);
            return ptr;
        }

        void Deallocate(void* ptr, size_t size)
        {
            JOINT_CORE_UNUSED(size);
            free(ptr);
        }
    };


    namespace detail_LifoAllocator
    {
        template < typename IndexType_, typename... Allocators_ >
        struct CombinedLifoAllocatorOps
        {
            static const int Index = IndexType_::Value;
            using Tail = CombinedLifoAllocatorOps<meta::IntToType<Index + 1>, Allocators_...>;

            static bool Contains(std::tuple<Allocators_...>& allocators, void* ptr)
            { return std::get<Index>(allocators).Contains(ptr) || Tail::Contains(allocators, ptr); }

            static void *Allocate(std::tuple<Allocators_...>& allocators, size_t size, size_t alignment)
            {
                void* ptr = std::get<Index>(allocators).Allocate(size, alignment);
                return ptr != nullptr ? ptr : Tail::Allocate(allocators, size, alignment);
            }

            static void* Shrink(std::tuple<Allocators_...>& allocators, void* ptr, size_t oldSize, size_t newSize, size_t alignment)
            {
                auto& a = std::get<Index>(allocators);
                if (a.Contains(ptr))
                    return a.Shrink(ptr, oldSize, newSize, alignment);
                return Tail::Shrink(allocators, ptr, oldSize, newSize, alignment);
            }

            static void Deallocate(std::tuple<Allocators_...>& allocators, void* ptr, size_t size)
            {
                auto& a = std::get<Index>(allocators);
                if (a.Contains(ptr))
                    a.Deallocate(ptr, size);
                else
                    return Tail::Deallocate(allocators, ptr, size);
            }
        };

        template < typename... Allocators_ >
        struct CombinedLifoAllocatorOps<meta::IntToType<sizeof...(Allocators_)>, Allocators_...>
        {
            JOINT_DEVKIT_LOGGER("Joint.DevKit.CombinedLifoAllocator");
            static bool Contains(std::tuple<Allocators_...>& allocators, void* ptr)
            {
                JOINT_CORE_UNUSED(allocators);
                JOINT_CORE_UNUSED(ptr);
                return false;
            }

            static void *Allocate(std::tuple<Allocators_...>& allocators, size_t size, size_t alignment)
            {
                JOINT_CORE_UNUSED(allocators);
                JOINT_CORE_UNUSED(size);
                JOINT_CORE_UNUSED(alignment);
                return nullptr;
            }

            static void* Shrink(std::tuple<Allocators_...>& allocators, void* ptr, size_t oldSize, size_t newSize, size_t alignment)
            {
                const auto& LoggerName = GetLogger().GetName();

                JOINT_CORE_UNUSED(allocators);
                JOINT_CORE_UNUSED(ptr);
                JOINT_CORE_UNUSED(oldSize);
                JOINT_CORE_UNUSED(newSize);
                JOINT_CORE_UNUSED(alignment);
                JOINT_DEVKIT_ASSERT(false);
                return nullptr;
            }

            static void Deallocate(std::tuple<Allocators_...>& allocators, void* ptr, size_t size)
            {
                const auto& LoggerName = GetLogger().GetName();

                JOINT_CORE_UNUSED(allocators);
                JOINT_CORE_UNUSED(ptr);
                JOINT_CORE_UNUSED(size);
                JOINT_DEVKIT_ASSERT(false);
            }
        };
    }


    template < typename... Allocators_ >
    class CombinedLifoAllocator
    {
        using AllocatorsTuple = std::tuple<Allocators_...>;
        using Ops = detail_LifoAllocator::CombinedLifoAllocatorOps<meta::IntToType<0>, Allocators_...>;

    private:
        AllocatorsTuple    _allocators;

    public:
        bool Contains(void* ptr)
        { return Ops::Contains(_allocators, ptr); }

        void* Allocate(size_t size, size_t alignment)
        { return Ops::Allocate(_allocators, size, alignment); }

        void* Shrink(void* ptr, size_t oldSize, size_t newSize, size_t alignment)
        { return Ops::Shrink(_allocators, ptr, oldSize, newSize, alignment); }

        void Deallocate(void* ptr, size_t size)
        { Ops::Deallocate(_allocators, ptr, size); }
    };


    template < typename Wrapped_ >
    class CheckedLifoAllocator
    {
        JOINT_DEVKIT_LOGGER("Joint.DevKit.CheckedLifoAllocator");

    private:
        Wrapped_    _wrapped;

    public:
        bool Contains(void* ptr)
        { return ptr != nullptr && _wrapped.Contains(ptr); }

        void* Allocate(size_t size, size_t alignment)
        {
            void* result = _wrapped.Allocate(size, alignment);
            if (!result)
                JOINT_DEVKIT_THROW(std::bad_alloc());
            return result;
        }

        void* Shrink(void* ptr, size_t oldSize, size_t newSize, size_t alignment)
        {
            void* result = _wrapped.Shrink(ptr, oldSize, newSize, alignment);
            if (!result)
                JOINT_DEVKIT_THROW(std::bad_alloc());
            return result;
        }

        void Deallocate(void* ptr, size_t size)
        {
            if (ptr != nullptr)
                _wrapped.Deallocate(ptr, size);
        }
    };


    using DefaultLifoAllocator = CheckedLifoAllocator<
        CombinedLifoAllocator<
            InplaceLifoAllocator<256>,
            ThreadLocalLifoAllocator<64 * 1024>,
            LargeBlockFallbackLifoAllocator
        >
    >;

}}

#endif
