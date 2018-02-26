#ifndef JOINT_CPP_STRING_HPP
#define JOINT_CPP_STRING_HPP


#include <algorithm>
#include <string.h>

#include <joint.cpp/BytesView.hpp>
#include <joint.cpp/detail/Iterator.hpp>


namespace joint
{

    class Utf32View
    {
        typedef char Byte;
        typedef uint32_t value_type;

    public:
        class iterator : public detail::IteratorOps<iterator, value_type, value_type>
        {
            friend class Utf32View;
            friend class detail::IteratorOps<iterator, value_type, value_type>;

        private:
            const Byte* _begin;
            const Byte* _end;
            const Byte* _ptr;

        public:
            iterator() { }

        protected:
            iterator(const Byte* begin, const Byte* end, const Byte* ptr)
                : _begin(begin), _end(end), _ptr(ptr)
            { }

            bool Equals(iterator other) const
            { return _ptr == other._ptr; }

            value_type Dereference() const
            {
                Byte firstByte = *_ptr;
                value_type valueMask = 0x7F;
                size_t len = 0;
                for (; len < 4; ++len)
                {
                    if ((firstByte & ~valueMask) != ~valueMask)
                        break;
                    valueMask >>= 1;
                }
                len = std::min(len, size_t(_end - _ptr));

                value_type result = firstByte & valueMask;
                for (size_t i = 1; i < len; ++i)
                    result = (result << 6) | (*(_ptr + i) & 0x3F);

                return result;
            }

            void Increment()
            {
                while (_ptr < _end && (*(++_ptr) & 0xC0) == 0x80)
                { }
            }

            void Decrement()
            {
                while (_ptr > _begin && (*(--_ptr) & 0xC0) == 0x80)
                { }
            }
        };

        typedef iterator const_iterator;

    private:
        BytesView _bytes;

    public:
        explicit Utf32View(BytesView bytes)
            : _bytes(bytes)
        { }

        const_iterator begin() const { return const_iterator(_bytes.begin(), _bytes.end(), _bytes.begin()); }
        const_iterator end() const { return const_iterator(_bytes.begin(), _bytes.end(), _bytes.end()); }
    };

    Utf32View::const_iterator begin(const Utf32View& v) { return v.begin(); }
    Utf32View::const_iterator end(const Utf32View& v) { return v.end(); }


    namespace detail
    {
        template < typename String_ >
        class StringImpl
        {
        public:
            BytesView Utf8Bytes() const { return BytesView(Self().Data(), Self().Size()); }
            joint::Utf32View Utf32View() const { return joint::Utf32View(Utf8Bytes()); }

        protected:
            ~StringImpl() { }

        private:
            const String_& Self() const { return static_cast<const String_&>(*this); }
        };
    }


    class StringRef : public detail::StringImpl<StringRef>
    {
        friend class detail::StringImpl<StringRef>;

    private:
        const char*   _data;
        size_t        _size;

    public:
        StringRef(const char* data) // TODO: is this ok?
            : _data(data), _size(strlen(data) + 1)
        { }

        StringRef(const char* data, size_t size)
            : _data(data), _size(size)
        { }

    protected:
        const char* Data() const { return _data; }
        size_t Size() const { return _size; }
    };


    class String : public detail::StringImpl<String>
    {
        friend class detail::StringImpl<String>;

    private:
        std::string   _storage;

    public:
        String()
        { }

        String(const char* value) // TODO: is this ok?
            : _storage(value)
        { }

        explicit String(StringRef stringRef)
            : _storage(stringRef.Utf8Bytes().data(), stringRef.Utf8Bytes().size())
        { }

        explicit String(const std::string& storage)
            : _storage(storage)
        { }

        std::string& Storage() { return _storage; }
        const std::string& Storage() const { return _storage; }

        operator StringRef() const
        { return StringRef(Utf8Bytes().data(), Utf8Bytes().size()); }

    protected:
        const char* Data() const { return _storage.c_str(); }
        size_t Size() const { return _storage.size() + 1; }
    };

    template < typename String_ >
    std::ostream& operator<<(std::ostream& s, const detail::StringImpl<String_>& str)
    { return s << str.Utf8Bytes().data(); }

    template < typename String1_, typename String2_ >
    String operator+(const detail::StringImpl<String1_>& l, const detail::StringImpl<String2_>& r)
    {
        String result(l.Utf8Bytes().data()); // TODO: reimplement
        result.Storage() += r.Utf8Bytes().data();
        return result;
    }

    template < typename String1_, typename String2_ >
    bool operator==(const detail::StringImpl<String1_>& l, const detail::StringImpl<String2_>& r)
    {
        BytesView lb = l.Utf8Bytes();
        BytesView rb = r.Utf8Bytes();
        return strncmp(lb.data(), rb.data(), std::min(lb.size(), rb.size())) == 0;
    }

    template < typename String1_, typename String2_ >
    bool operator!=(const detail::StringImpl<String1_>& l, const detail::StringImpl<String2_>& r)
    { return !(l == r); }

}

#endif
