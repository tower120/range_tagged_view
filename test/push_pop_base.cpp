#include <doctest.h>

#include <iostream>
#include <vector>
#include <string>

#include <range/v3/view/transform.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/unique.hpp>
#include <range/v3/view/remove_if.hpp>

#include <experimental/ranges/view/tag_base.hpp>
#include <experimental/ranges/view/push_base.hpp>
#include <experimental/ranges/view/pop_base.hpp>


namespace ranges{
    using experimental::ranges::tag_base;
}

namespace ranges::view{
    using experimental::ranges::view::tag;
    using experimental::ranges::view::tag_base;
    using experimental::ranges::view::push_base;
    using experimental::ranges::view::pop_base;
}

struct forward_iterator_fn
{
    template<typename Rng>
    decltype(auto) operator()(Rng&& rng) const
    {
        return ranges::view::iota(rng.begin(), rng.end());
    }
};

inline constexpr const ranges::view::view<forward_iterator_fn> forward_iterator;


TEST_CASE("testing push_base / pop_base") {
    using namespace ranges;

    std::vector<int> i1 = {10, 10,  20,  21,  30};
    std::vector<int> i2 = {100,101, 200, 200, 300};

    auto unique_pairs =
        ranges::view::zip(i1, i2)
        | view::push_base
            | view::keys
            | view::unique
        | view::pop_base
        | view::push_base
            | view::values
            | view::unique
        | view::pop_base;

    REQUIRE(ranges::equal(unique_pairs, {
        std::pair(10, 100), std::pair(20, 200), std::pair(30, 300)
    }));
}


TEST_CASE("testing nested push_base / pop_base") {
    using namespace ranges;

    std::vector<int> is = {10, 20, 30};

    auto list =
        is
        | view::push_base
            | view::transform([](int i){ return -i;})
            | view::push_base
                | view::transform([](int i){ return i*10;})
            | view::pop_base
        | view::pop_base;

    REQUIRE(ranges::equal(list, {10, 20, 30}));
}


TEST_CASE("complex push/pop/tag base") {
    using namespace ranges;

    std::vector<int> i1 = {10, 10,  20,  21,  30};
    std::vector<int> i2 = {100,101, 200, 200, 300};

    struct k1{};  struct k2{};

    auto list =
        ranges::view::zip(i1, i2)
        | view::push_base
            | view::keys
            | view::unique
            | view::transform([](int i){ return -i;})
            | view::tag<k1>
        | view::pop_base
        | view::push_base
            | view::values
            | view::unique
            | view::transform([](int i){ return std::to_string(i) + "n";})
            | view::tag<k2>
        | view::pop_base
        | forward_iterator | view::transform([](auto&& iter){
            return std::make_pair(*tag_base<k1>(iter), *tag_base<k2>(iter));
          });

    /*for(auto[key, value] : list){
        std::cout << key << " : " << value << ", ";
    }*/

    using pair = std::pair<int, std::string>;
    REQUIRE(ranges::equal(list, {
        pair(-10, "100n"), pair(-20, "200n"), pair(-30, "300n")
    }));
}