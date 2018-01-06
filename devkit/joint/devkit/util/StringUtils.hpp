#ifndef JOINT_DEVKIT_UTIL_STRINGUTILS_HPP
#define JOINT_DEVKIT_UTIL_STRINGUTILS_HPP


#include <string.h>


namespace joint {
namespace devkit
{

    class StringView
    {
    private:
        const char* _data;
        size_t      _size;

    public:
        StringView(const char* data, size_t size)
            : _data(data), _size(size)
        { }

        const char* data() const { return _data; }
        size_t size() const { return _size; }

        const char* c_str() const { return _data; }

        const char* begin() const { return _data; }
        const char* end() const { return _data + _size; }
    };


    template < typename Alloc_ >
    class StringStorage
    {
    private:
        Alloc_&     _alloc;
        char*       _data;
        size_t      _size;

    public:
        StringStorage(Alloc_& alloc, char* data, size_t size)
            : _alloc(alloc), _data(data), _size(size)
        { }

        ~StringStorage()
        { _alloc.Deallocate(_data, _size); }

        StringStorage(StringStorage&& other)
            : _alloc(other._alloc), _data(other._data), _size(other._size)
        {
            other._data = nullptr;
            other._size = 0;
        }

        StringStorage& operator=(StringStorage&&) = delete;
        StringStorage(const StringStorage&) = delete;
        StringStorage& operator=(const StringStorage&) = delete;

        const char* data() const { return _data; }
        size_t size() const { return _size; }

        const char* c_str() const { return _data; }

        const char* begin() const { return _data; }
        const char* end() const { return _data + _size; }
    };


    namespace detail_StringUtils
    {
        inline const char* GetStringData(const char* s) { return s; }
        template < size_t N_ > const char* GetStringData(const char s[N_]) { return s; }
        template < typename String_ > const char* GetStringData(const String_& s) { return s.data(); }


        inline size_t GetStringSize(const char* s) { return strlen(s); }
        template < size_t N_ > size_t GetStringSize(const char s[N_]) { return strlen(s); }
        template < typename String_ > size_t GetStringSize(const String_& s) { return s.size(); }


        inline size_t CalculateJoinedSize(size_t* sizesArray)
        { return 0; }

        template < typename FirstString_, typename... Strings_ >
        inline size_t CalculateJoinedSize(size_t* sizesArray, const FirstString_& s, const Strings_&... strings)
        {
            size_t size = GetStringSize(s);
            *sizesArray = size;
            return size + CalculateJoinedSize(sizesArray + 1, strings...);
        }


        inline char* JoinImpl(size_t* sizesArray, char* data)
        { return data; }

        template < typename FirstString_, typename... Strings_ >
        char* JoinImpl(size_t* sizesArray, char* data, const FirstString_& s, const Strings_&... strings)
        {
            size_t size = *sizesArray;
            memcpy(data, GetStringData(s), size);
            return JoinImpl(sizesArray + 1, data + size, strings...);
        }
    }

    template < typename Alloc_, typename... Strings_ >
    StringStorage<Alloc_> Join(Alloc_& alloc, const Strings_&... strings)
    {
        using namespace detail_StringUtils;

        size_t sizes[sizeof...(Strings_)];
        size_t size = CalculateJoinedSize(sizes, strings...) + 1;

        char* data = (char*)alloc.Allocate(size, 1);
        char* end = JoinImpl(sizes, data, strings...);
        *end = 0;

        return StringStorage<Alloc_>(alloc, data, size);
    }

}}

#endif
