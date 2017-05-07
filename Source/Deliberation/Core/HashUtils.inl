namespace deliberation
{
template<typename Head, typename Neck, typename... Tail>
std::size_t
HashCombine(const Head & head, const Neck & neck, const Tail &... tail)
{
    std::hash<Head> hashA;
    auto            a = hashA(head);
    auto            b = HashCombine(neck, tail...);

    return a ^ b + 0x9e3779b9 + (a << 6) + (a >> 2);
}

template<typename Head>
std::size_t HashCombine(const Head & head)
{
    std::hash<Head> hash;
    return hash(head);
}
}
