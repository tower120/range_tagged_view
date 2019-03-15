#pragma once

#include "tag.hpp"

namespace experimental::ranges::view{

    namespace details{
        struct push_base_tag{};
    }

    template<class Rng>
    using push_base_view = tag_view<Rng, details::push_base_tag>;


    struct push_base_fn
    {
        template<typename Rng>
        decltype(auto) operator()(Rng&& rng) const
        {
            using namespace ::ranges::view;

            return push_base_view<all_t<Rng>>(
                all(static_cast<Rng&&>(rng))
            );
        }
    };

    inline constexpr const ::ranges::view::view<push_base_fn> push_base;
}