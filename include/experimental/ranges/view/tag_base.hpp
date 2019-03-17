#pragma once

#include <range/v3/view/all.hpp>
#include <range/v3/iterator_range.hpp>
#include <range/v3/range_concepts.hpp>

#include "tag.hpp"

namespace experimental::ranges{

    // TODO: remove
    // does this needed?
    template<class Tag = view::details::default_tag, class Rng>
    decltype(auto) tag_base_rng(Rng&& rng){
        using R = std::decay_t<Rng>;
        static_assert(::ranges::is_view<R>::value, "Range must be view / Tag not founded.");

        constexpr const auto found = []() -> bool {
            if constexpr (is_tag_view_v<R>)
            {
                if constexpr (std::is_same_v<typename R::tag, Tag>){
                    return true;
                }
            }
            return false;
        };

        if constexpr (found()){
            return rng.base();
        } else {
            return tag_base_rng<Tag>(rng.base());
        }
    }


    template<class Tag = view::details::default_tag, class Iterator>
    decltype(auto) tag_base_iterator(Iterator&& iterator){
        using I = std::decay_t<Iterator>;
        static_assert(::ranges::Iterator<I>::value, "Iterator must be Iterator / Tag not founded.");

        constexpr const auto found = []() -> bool{
            if constexpr (is_tag_view_iterator_v<I>)
            {
                if constexpr (std::is_same_v<typename I::tag, Tag>){
                    return true;
                }
            }
            return false;
        };

        if constexpr (found()){
            return iterator.base();
        } else {
            return tag_base_iterator<Tag>(iterator.base());
        }
    }


    template<class Tag = view::details::default_tag, class Iterator>
    decltype(auto) tag_base(Iterator&& iterator){
        return tag_base_iterator<Tag>(std::forward<Iterator>(iterator));
    }


}

namespace experimental::ranges::view
{

    template<class Rng, class Tag = details::default_tag>
    class tag_base_view
        : public ::ranges::view_adaptor<tag_base_view<Rng, Tag>, Rng>
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
                    return tag_base_iterator<Tag>(this->base());
                }
            };

            template<class Iterator>
            decltype(auto) read(Iterator it) const
            {
                return *tag_base_iterator<Tag>(it);
            }
        };

        adaptor begin_adaptor() const { return {}; }
        adaptor end_adaptor()   const { return {}; }

    public:
        using tag_base_view::view_adaptor::view_adaptor;
    };


    template<class Tag>
    struct tag_base_fn
    {
        template<typename Rng>
        decltype(auto) operator()(Rng&& rng) const
        {
            using namespace ::ranges::view;

            return tag_base_view<all_t<Rng>, Tag>(
                all(static_cast<Rng&&>(rng))
            );
        }
    };

    template<class Tag = details::default_tag>
	inline constexpr const ::ranges::view::view<tag_base_fn<Tag>> tag_base{};

}