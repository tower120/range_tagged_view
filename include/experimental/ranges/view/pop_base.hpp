#pragma once

#include "push_base.hpp"

namespace experimental::ranges::view{

    template<class Rng>
    class pop_base_view;

    struct pop_view_access
    {
        struct pop_view_mark_t{};

        template<typename T, typename = void>
        struct have_pop_view_mark : std::false_type {};
        template<class Iter>
        struct have_pop_view_mark< Iter, std::void_t<decltype( std::declval<Iter>().pop_view_mark )> > : std::true_type {};

        template<class Iter>
        static constexpr const bool is_pop_view_iterator =
            []() -> bool {
                using I = std::decay_t<Iter>;
                if constexpr (have_pop_view_mark<I>::value){
                    if constexpr (std::is_same_v<std::decay_t<decltype(I::pop_view_mark)>, pop_view_mark_t>){
                        return true;
                    }
                }
                return false;
            }();

        template<int skip = 0, class Iterator>
        static decltype(auto) pop_base_iterator(Iterator&& iterator){
            using I = std::decay_t<Iterator>;
            static_assert(skip >= 0, "Something wrong!");
            static_assert(::ranges::Iterator<I>::value, "push_base not founded.");

            constexpr const auto found = []() -> bool{
                if constexpr (is_tag_view_iterator_v<I>)
                {
                    if constexpr (std::is_same_v<typename I::tag, details::push_base_tag>){
                        return true;
                    }
                }
                return false;
            };

            if constexpr (found()){
                if constexpr (skip == 0){
                    return iterator.base();
                }  else {
                    return pop_base_iterator<skip-1>(iterator.base());
                }
            } else {
                if constexpr (is_pop_view_iterator<I>){
                    return pop_base_iterator<skip+1>(iterator.base());
                } else {
                    return pop_base_iterator<skip>(iterator.base());
                }
            }
        }
    };

    template<class Rng>
    class pop_base_view
        : public ::ranges::view_adaptor<pop_base_view<Rng>, Rng>
    {
        friend ::ranges::range_access;

        struct adaptor : public ::ranges::adaptor_base
        {
            // XXX: mixin interface is temporary. #https://github.com/ericniebler/range-v3/pull/1086
            template<class base_mixin>
            struct mixin : base_mixin
            {
                mixin() = default;
                using base_mixin::base_mixin;

                decltype(auto) origin() const
                {
                    return pop_view_access::pop_base_iterator(this->base());
                }
            private:
                friend pop_view_access;
                static const constexpr pop_view_access::pop_view_mark_t pop_view_mark{};
            };

            template<class Iterator>
            decltype(auto) read(Iterator it) const
            {
                return *pop_view_access::pop_base_iterator(it);
            }
        };

        adaptor begin_adaptor() const { return {}; }
        adaptor end_adaptor()   const { return {}; }

    public:
        using pop_base_view::view_adaptor::view_adaptor;
    };

    struct pop_base_fn
    {
        template<typename Rng>
        decltype(auto) operator()(Rng&& rng) const
        {
            using namespace ::ranges::view;

            return pop_base_view<all_t<Rng>>(
                all(static_cast<Rng&&>(rng))
            );
        }
    };

    inline constexpr const ::ranges::view::view<pop_base_fn> pop_base;
}