#pragma once

#include <range/v3/algorithm/find_if.hpp>
#include <experimental/ranges/view/proj.hpp>

namespace experimental::ranges::algorithm{

    template<typename Rng, typename F>
    decltype(auto) find_if(Rng&& rng, F pred){
        if constexpr (is_proj_range<Rng>) {
            static_assert(!proj_range_may_dangle<Rng>);
            return project(::ranges::find_if(static_cast<Rng &>(rng), std::move(pred)));
        } else {
            return ::ranges::find_if(std::forward<Rng>(rng), std::move(pred));
        }
    }

}
