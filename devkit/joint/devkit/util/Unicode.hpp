#ifndef JOINT_DEVKIT_UTIL_UNICODE_HPP
#define JOINT_DEVKIT_UTIL_UNICODE_HPP


#include <joint/devkit/Config.hpp>
#include <joint/public/JointConfig.h>

#include <string>


namespace joint {
namespace devkit
{

    template < typename OutputIt_ >
    OutputIt_ WriteCharToUtf8Iterator(char32_t c, OutputIt_ it)
    {
        JOINT_ASSERT_EX("Joint.DevKit.WriteCharToUtf8Iterator", c < 0x10FFFF);

        if (JOINT_CORE_LIKELY(c < 0x80))
        {
            *it++ = c;
        }
        else if (c < 0x800)
        {
            *it++ = 0xC0 | ((c >> 6) & 0x1F);
            *it++ = 0x80 | ((c >> 0) & 0x3F);
        }
        else if (c < 0x10000)
        {
            *it++ = 0xE0 | ((c >> 12) & 0x0F);
            *it++ = 0x80 | ((c >>  6) & 0x3F);
            *it++ = 0x80 | ((c >>  0) & 0x3F);
        }
        else
        {
            *it++ = 0xF0 | ((c >> 18) & 0x07);
            *it++ = 0x80 | ((c >> 12) & 0x3F);
            *it++ = 0x80 | ((c >>  6) & 0x3F);
            *it++ = 0x80 | ((c >>  0) & 0x3F);
        }

        return it;
    }


    template < typename InputIt_ >
    InputIt_ ReadCharFromUtf16Range(InputIt_ begin, InputIt_ end, char32_t& outChar)
    {
        JOINT_ASSERT_EX("Joint.DevKit.ReadCharFromUtf16Range", begin != end);

        uint16_t c1 = *begin++;
        if (JOINT_CORE_LIKELY(c1 < 0xD800 || c1 > 0xDFFF))
            outChar = c1;
        else if (begin != end)
        {
            bool isLittleEndian = c1 >= 0xDC00;
            uint16_t c2 = *begin++;

            if (isLittleEndian)
                outChar = (c1 - 0xDC00) + ((c2 - 0xD800) << 10) + 0x10000;
            else
                outChar = (c2 - 0xDC00) + ((c1 - 0xD800) << 10) + 0x10000;
        }
        else
            outChar = 0;

        return begin;
    }


    template < typename InputIt_, typename OutputIt_ >
    OutputIt_ CopyUtf16RangeToUtf8Iterator(InputIt_ ibegin, InputIt_ iend, OutputIt_ obegin)
    {
        char32_t c;
        while (ibegin != iend) {
            ibegin = ReadCharFromUtf16Range(ibegin, iend, c);
            obegin = WriteCharToUtf8Iterator(c, obegin);
        }
        return obegin;
    }

}}

#endif
