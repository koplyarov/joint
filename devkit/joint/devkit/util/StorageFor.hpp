#ifndef JOINT_DEVKIT_STORAGE_FOR_HPP
#define JOINT_DEVKIT_STORAGE_FOR_HPP

// Copyright (c) 2016, Dmitry Koplyarov <koplyarov.da@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted,
// provided that the above copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.


#include <type_traits>
#include <utility>


#if defined(_MSC_VER) && _MSC_VER < 1900
#   define DETAIL_JOINT_DEVKIT_ALIGNOF __alignof
#else
#   define DETAIL_JOINT_DEVKIT_ALIGNOF alignof
#endif


namespace joint {
namespace devkit
{

    template < typename T_ >
    class StorageFor
    {
    public:
        struct NoConstructTag { };

    private:
        using Storage = typename std::aligned_storage<sizeof(T_), DETAIL_JOINT_DEVKIT_ALIGNOF(T_)>::type;

    private:
        Storage     obj;

    public:
        StorageFor() { }
        ~StorageFor() { }

        template < typename... Args_ >
        void Construct(Args_&&... args)
        { new(&obj) T_(std::forward<Args_>(args)...); }

        void Destruct()
        { Ref().~T_(); }

        T_& Ref() { return *reinterpret_cast<T_*>(&obj); }
        const T_& Ref() const { return *reinterpret_cast<const T_*>(&obj); }

        T_& operator * () { return Ref(); }
        const T_& operator * () const { return Ref(); }

        T_* operator -> () { return &Ref(); }
        const T_* operator -> () const { return &Ref(); }
    };

}}

#endif
