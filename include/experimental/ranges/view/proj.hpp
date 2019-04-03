#pragma once

#include "tag.hpp"
#include "tag_base.hpp"

namespace experimental::ranges::view
{
    namespace details{
        struct proj_tag{};
    }

    inline constexpr const auto proj = tag<details::proj_tag>;
}

namespace experimental::ranges
{
    template<typename Rng>
    constexpr const bool is_proj_range = !std::is_same_v<void,
        decltype(find_view_tag_iterator<view::details::proj_tag>(std::declval<Rng>().begin()))
    >;

    template<class Rng>
    constexpr const bool proj_range_may_dangle = std::decay_t<decltype(find_view_tag_iterator<view::details::proj_tag>(std::declval<Rng>().begin()))>::may_dangle;

    template<class Rng>
    decltype(auto) project(Rng&& rng){
        return tag_base<view::details::proj_tag>(std::forward<Rng>(rng));
    }

    template<class Base, class Iterator>
    decltype(auto) project_to(Iterator&& iterator){
        using I = std::decay_t<Iterator>;
        static_assert(::ranges::Iterator<I>::value, "Iterator must be Iterator / not founded.");

        constexpr const bool found = std::is_same_v<I, Base>;

        if constexpr (found){
            return iterator;
        } else {
            return project_to<Base>(iterator.base());
        }
    }

    template<class Iterator, class Rng>
    decltype(auto) project_to(Iterator&& iterator, const Rng&){
        using I = decltype(std::declval<Rng>().begin());
        return project_to<I>(iterator);
    }

}