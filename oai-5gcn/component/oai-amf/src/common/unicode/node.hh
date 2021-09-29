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

#ifndef UNF_TRIE_NODE_HH
#define UNF_TRIE_NODE_HH

namespace UNF {
  namespace Trie {
    class Node {
    public:
      unsigned jump(unsigned char ch) const { return base() + ch; }
      unsigned value() const { return base(); }
      unsigned check_char() const { return data>>24; }
      unsigned to_uint() const { return data; }

      static const Node* from_uint_array(const unsigned* node_uints)
      { return reinterpret_cast<const Node*>(node_uints); }

    private:
      unsigned base() const { return data & 0xFFFFFF; }

    private:
      unsigned data;
    };
  }
}

#endif
