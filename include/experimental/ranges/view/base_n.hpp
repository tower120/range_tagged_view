#pragma once

#include <range/v3/all.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/iterator_range.hpp>


namespace experimental::ranges::view{

    template<int n, class Rng>
    class base_n_view
        : public ::ranges::view_adaptor<base_n_view<n, Rng>, Rng>
    {
        friend ::ranges::range_access;

        struct adaptor : public ::ranges::adaptor_base
        {
            template<int i = n, class Iter>
            decltype(auto) get_base_iter(Iter&& iter) const {
                if constexpr(i == 1){
                    return iter.base();
                } else {
                    return get_base_iter<i-1>(iter.base());
                }
            }

            template<class Iterator>
            decltype(auto) read(Iterator it) const
            {
                return *get_base_iter(std::move(it));
            }
        };

        adaptor begin_adaptor() const { return {}; }
        adaptor end_adaptor()   const { return {}; }

    public:
        using base_n_view::view_adaptor::view_adaptor;
    };


    template<int n>
    struct base_n_fn
    {
        template<typename Rng>
        decltype(auto) operator()(Rng&& rng) const
        {
            using namespace ::ranges::view;

            return base_n_view<n, all_t<Rng>>(
                all(static_cast<Rng&&>(rng))
            );
        }
    };

    template<int n>
	inline constexpr const ::ranges::view::view<base_n_fn<n>> base_n{};
}