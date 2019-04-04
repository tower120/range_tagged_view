#include <doctest.h>

#include <iostream>
#include <vector>

#include <experimental/ranges/view/proj.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/transform.hpp>


TEST_CASE( "testing view::tag traits") {
    using namespace experimental::ranges;

    using Vec = std::vector<int>;
    Vec vec = {1,2,3};
    using I = Vec::iterator;

    {
        auto rng =
            vec
            | ::ranges::view::transform([](int i) { return -i; });

        REQUIRE(!is_proj_range<decltype(rng)>);
    }

    {
        auto rng =
            view::proj(vec)
            | ::ranges::view::transform([](int i) { return -i; });

        REQUIRE(is_proj_range<decltype(rng)>);
        REQUIRE(project(rng.begin()) == vec.begin());
    }

    {
        auto rng =
            vec
            | ::ranges::view::transform([](int i) { return -i; });

        auto found = ::ranges::find_if(rng,
            [](int i) -> bool
            {
                return i < -2;
            }
        );

        I i = project_to<I>(found);
        REQUIRE(*i == 3);

        I i2 = project_to(found, vec);
        REQUIRE(i == i2);
    }
}
