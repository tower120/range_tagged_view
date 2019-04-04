#include <doctest.h>

#include <iostream>
#include <vector>

#include <range/v3/view/transform.hpp>

#include <experimental/ranges/view/tag.hpp>
#include <experimental/ranges/view/tag_base.hpp>


TEST_CASE( "testing view::tag traits") {
    using namespace experimental::ranges;

    using Vec = std::vector<int>;
    Vec vec = {1,2,3};

    auto list = view::tag(vec);

    REQUIRE(is_tag_view_v<decltype(list)>);
    REQUIRE(is_tag_view_iterator_v<decltype(list.begin())>);
}
