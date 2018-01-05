#ifndef JOINT_DEVKIT_STACKSTORAGE_HPP
#define JOINT_DEVKIT_STACKSTORAGE_HPP


#include <joint/devkit/util/Holder.hpp>
#include <joint/devkit/util/ScopeExit.hpp>

#include <array>
#include <vector>


namespace joint {
namespace devkit
{

    template < typename T_, size_t BytesSize_ >
    class StackStorage
    {
        using ElementStorage = typename std::aligned_storage<sizeof(T_), alignof(T_)>::type;
        using BufArray = std::array<ElementStorage, BytesSize_ / sizeof(T_)>;

        struct BlockNode
        {
            static const size_t HeaderSize = sizeof(BlockNode*) * 2 + sizeof(size_t);

            BlockNode*       Prev;
            BlockNode*       Next;
            size_t           Count;
            ElementStorage   StorageBegin;
        };

    private:
        BufArray              _buf;
        size_t                _freeOfs;
        BlockNode             _fallbackBlocksHead;

    public:
        StackStorage()
            : _freeOfs()
        { _fallbackBlocksHead.Next = _fallbackBlocksHead.Prev = &_fallbackBlocksHead; }

        StackStorage(const StackStorage&) = delete;
        StackStorage* operator = (const StackStorage&) = delete;

        ~StackStorage()
        {
            while (_freeOfs--)
                reinterpret_cast<T_&>(_buf[_freeOfs]).~T_();

            BlockNode* next;
            for (BlockNode* n = _fallbackBlocksHead.Next; n != &_fallbackBlocksHead; n = next)
            {
                next = n->Next;
                auto count = n->Count;
                auto ptr = &n->StorageBegin;
                for (size_t i = 0; i < count; ++i)
                    reinterpret_cast<T_&>(ptr[i]).~T_();
                free(n);
            }
        }

        T_* Make(size_t count)
        {
            if (count == 0)
                return nullptr;

            return AllocateAndConstruct(count, [&](T_* p) {
                size_t i = 0;
                auto sg(ScopeExit([&]{ while (i--) p[i].~T_(); }));
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
                auto b = reinterpret_cast<BlockNode*>(malloc(BlockNode::HeaderSize + sizeof(ElementStorage) * count));

                b->Count = count;
                auto ptr = &b->StorageBegin;

                auto sg(ScopeExit([&]{ free(b); delete[] ptr; }));
                result = reinterpret_cast<T_*>(ptr);
                constructFunc(result, std::forward<Args_>(args)...);
                sg.Cancel();

                b->Next = _fallbackBlocksHead.Next;
                b->Prev = &_fallbackBlocksHead;
                _fallbackBlocksHead.Next->Prev = b;
                _fallbackBlocksHead.Next = b;
            }

            return result;
        }
    };

}}

#endif
