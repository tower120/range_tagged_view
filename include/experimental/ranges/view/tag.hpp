#pragma once

#include <range/v3/all.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/iterator_range.hpp>


namespace experimental::ranges::view
{
    namespace details{
        struct default_tag{};
    }

    struct tag_view_access{
        struct tag_view_mark_t{};

        template<typename T, typename = void>
        struct have_tag_view_mark : std::false_type {};

        template<class Iter>
        struct have_tag_view_mark< Iter, std::void_t<decltype( std::declval<Iter>().tag_view_mark )> > : std::true_type {};

        template<class Iter>
        static constexpr const bool is_tag_view_iterator =
            []() -> bool {
                using I = std::decay_t<Iter>;
                if constexpr (have_tag_view_mark<I>::value){
                    if constexpr (std::is_same_v<std::decay_t<decltype(I::tag_view_mark)>, tag_view_mark_t>){
                        return true;
                    }
                }
                return false;
            }();
    };

    template<class Rng, class Tag = details::default_tag>
    class tag_view
        : public ::ranges::view_adaptor<tag_view<Rng, Tag>, Rng>
    {
        friend ::ranges::range_access;

        struct adaptor : public ::ranges::adaptor_base{
            // XXX: mixin interface is temporary. #https://github.com/ericniebler/range-v3/pull/1086
            template<class base_mixin>
            struct mixin : base_mixin
            {
                mixin() = default;
                using base_mixin::base_mixin;

                friend tag_view_access;
                using tag = Tag;
            private:
                static const constexpr tag_view_access::tag_view_mark_t tag_view_mark{};
            };
        };

        adaptor begin_adaptor() const { return {}; }
        adaptor end_adaptor()   const { return {}; }
    public:
        using tag = Tag;

        using tag_view::view_adaptor::view_adaptor;
    };

    template<class Tag>
    struct tag_fn
    {
        template<typename Rng>
        decltype(auto) operator()(Rng&& rng) const
        {
            using namespace ::ranges::view;

            return tag_view<all_t<Rng>, Tag>(
                all(static_cast<Rng&&>(rng))
            );
        }
    };

    template<class Tag = details::default_tag>
	inline constexpr const ::ranges::view::view<tag_fn<Tag>> tag{};
}

namespace experimental::ranges
{
    template<typename>
    struct is_tag_view : std::false_type {};
    template<class Rng, class Tag>
    struct is_tag_view<view::tag_view<Rng, Tag>> : std::true_type {};

    template<class T>
    constexpr const bool is_tag_view_v = is_tag_view<T>::value;

    template<class T>
    constexpr const bool is_tag_view_iterator_v = view::tag_view_access::is_tag_view_iterator<T>;
}