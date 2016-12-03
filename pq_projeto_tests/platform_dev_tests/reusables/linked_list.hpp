/*
* Copyright (C) 2013 to 2016 by Felipe Lavratti
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in the
* Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LINKED_LIST_HPP_
#define LINKED_LIST_HPP_

#ifdef __cplusplus
#include <typeinfo>
#include <type_traits>

#define __ll_auto_type(___zcxzzarg) auto
#define __ll_typeof(___zarg) std::remove_reference<decltype(___zarg)>::type

template<class P, class M>
size_t __ll_offsetof(const M P::*member)
{
    return (size_t) &( reinterpret_cast<P*>(0)->*member);
}

template<class P, class M>
P* __ll_container_of_impl(M* ptr, const M P::*member)
{
    return (P*)( (char*)ptr - __ll_offsetof(member));
}

#define __ll_container_of(ptr, type, member) \
     __ll_container_of_impl (ptr, &type::member)
#endif

#include <reusables/linked_list.h>

#endif /* LINKED_LIST_HPP_ */
