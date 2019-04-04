#include <doctest.h>

#include <iostream>
#include <vector>

#include <range/v3/view/transform.hpp>

#include <experimental/ranges/view/tag.hpp>
#include <experimental/ranges/view/tag_base.hpp>


TEST_CASE("testing tag_base") {
    using namespace experimental::ranges;

    std::vector<int> vec = {1,2,3};

    auto list = vec
                | view::tag
                | ranges::view::transform([](int i) -> std::pair<int, int>{
                    return {i,i};
                });

    auto tag_base       = tag_base_rng(list);
    auto tag_base_begin = tag_base_iterator(list.begin());
    auto tag_base_end   = tag_base_iterator<>(list.end());
    REQUIRE(tag_base.begin() == tag_base_begin);
    REQUIRE(tag_base.end()   == tag_base_end);
    REQUIRE(tag_base_begin == vec.begin());
    REQUIRE(tag_base_end   == vec.end());
}


TEST_CASE( "testing view::tag_base") {
    using namespace experimental::ranges;

    std::vector<int> vec = {1,2,3};

    struct{} p;

    auto list = vec
                | view::tag(p)
                | ranges::view::transform([](int i) {
                        return i+10;
                   })
                | view::tag_base(p);

    REQUIRE(ranges::equal(list, {1,2,3}));
    REQUIRE(list.begin().origin() == vec.begin());
    REQUIRE(list.end().origin()   == vec.end());
}