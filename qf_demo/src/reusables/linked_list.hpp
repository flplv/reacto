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

#include "linked_list.h"

#endif /* LINKED_LIST_HPP_ */
