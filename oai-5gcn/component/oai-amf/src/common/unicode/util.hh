/* Copyright (c) 2010-2017 Takeru Ohta Copyright (c) 2011-2018 Akinori MUSHA
 * Licensed under the MIT license.
 *
 */
/*
*The MIT License
*
*Copyright (c) 2010 Takeru Ohta <phjgt308@gmail.com>
*Copyright (c) 2011-2018 Akinori MUSHA <knu@idaemons.org> (extended Ruby support)
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
 */

#ifndef UNF_UTIL_HH
#define UNF_UTIL_HH

namespace UNF {
  namespace Util {
    inline bool is_utf8_char_start_byte(char byte) {
      if(!(byte&0x80))    return true; // ascii
      else if (byte&0x40) return true; // start of a UTF-8 character byte sequence
      return false;
    }

    inline const char* nearest_utf8_char_start_point(const char* s) {
      for(; is_utf8_char_start_byte(*s)==false; s++);
      return s;
    }

    template <class CharStream>
    inline void eat_until_utf8_char_start_point(CharStream& in) {
      for(; is_utf8_char_start_byte(in.peek())==false; in.read());
    }
  }
}

#endif
