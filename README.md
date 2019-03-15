This range-v3 views allow to put some "anchor"/"label"/"tag" in your view compsition sequence. And provide facilities to back to that point.

Implemented as chain of `view`'s base() calls.

# Motivation examples


## view::tag

Useful to work with algorithms:

```cpp
using namespace ranges;

using List = std::vector<Data>;
List list = {{1}, {2}};    

auto l  = list 
        | view::tag<>
        | view::transform(&Data::i) 
        | view::remove_if([](int i){ return i%2; });    

List::iterator i = tag_base(find(l, 5));
```

range-v3 comes with optional projection argument for all algorithms. But projection is not always the right tool. For example, it is impossible to use projection to prefilter range.

Direct comparison with projection:
```cpp
List::iterator found = tag_base(find(
     list
     | view::tag 
     | view::transform(&Data::i), 2
));
```
```cpp
List::iterator found = find(list, 2, &Data::i);
```

## view::tag_base

```cpp
using namespace ranges;

using List = std::vector<Data>;
List list = {{1}, {1}, {2}, {3}};

struct t{};

auto l = list
        | view::tag<t>
        | view::transform(&Data::i)
        | view::unique;
        | view::tag_base<t>;

for(Data& data: l){
    std::cout << data.i << " ";
}
```
Output:
```
1 2 3
```

`tag_base<Tag>` will return base of first occurrence of `view::tag<Tag>`.
Can be used without `Tag` parameter.

## view::push_base + view::pop_base

Same as `view::tag` + `view::tag_base` but designed for nested use (hence - safer for general use).
```cpp
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
```
Result:
```
{10,100}, {20,200}, {30,300}
```

## All together

```cpp
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
```
Result
```
{-10, "100n"}, {-20, "200n"}, {-30, "300n"}
```

## Additional iterator member

`tag_base` and `pop_base` iterators has `origin()` member - which returns the original base iterator:
```cpp
auto list = vec
            | view::push_base
            | view::remove_if([](auto&&) { return false; });
            | view::pop_base

REQUIRE(list.begin().origin() == vec.begin());
```

### Perfoemance overhead

Not measured yet. Chain of n `base()` calls (`base().base().base()`), should fold to O(1) in release mode, because all view types are statically known, and all iterator/view offsets/memory locations are known.