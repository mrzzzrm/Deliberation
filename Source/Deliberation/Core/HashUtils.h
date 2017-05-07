#pragma once

#include <functional>

namespace deliberation
{
template<typename Head, typename Neck, typename... Tail>
std::size_t
HashCombine(const Head & head, const Neck & neck, const Tail &... tail);

template<typename Head>
std::size_t HashCombine(const Head & head);
};

#include <Deliberation/Core/HashUtils.inl>
