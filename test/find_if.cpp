#include <doctest.h>

#include <iostream>
#include <vector>

#include <experimental/ranges/algorithm/find_if.hpp>
#include <range/v3/view/transform.hpp>


TEST_CASE( "testing view::tag traits") {
    using namespace experimental::ranges;

    using Vec = std::vector<int>;
    Vec vec = {1,2,3};
    using I = Vec::iterator;

    {
        I found = algorithm::find_if(
            view::proj(vec)
            | ::ranges::view::transform([](int i) { return -i; }),
            [](int i) -> bool
            {
                return i < -2;
            }
        );
        REQUIRE(*found == 3);
    }

    {
        // test fallback to default find_if
        auto rng =
            vec
            | ::ranges::view::transform([](int i) { return -i; });

        auto found = algorithm::find_if(
            rng,
            [](int i) -> bool
            {
                return i < -2;
            }
        );
        REQUIRE(*found == -3);
    }

}
