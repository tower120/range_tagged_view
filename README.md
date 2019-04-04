Better projections for ranges!

This library allow to use standard views as projection.  
Moreover, you can project inside view chain. In other words - come back to your previous view.   

Implemented as chain of `view` base() calls.

# Motivation examples

range-v3 comes with optional projection argument for all algorithms. But projection is not always the right tool. For example, it is impossible to use projection to prefilter range:
```cpp
List::iterator found = find(
    view::perspective(list) | view::transform(&Data::i) | view::unique, 2
);
```

Direct comparison with range projection:
```cpp
List::iterator found = find(
    view::perspective(list) | view::transform(&Data::i), 2
);
```
```cpp
List::iterator found = find(list, 2, &Data::i);
```

# Documentation

## project_to

Standalone function `project_to`

```cpp
List list;

auto l  = list 
        | view::transform(&Data::i) 
        | view::remove_if([](int i){ return i%2; });    

auto found = find(l, 5);
List::iterator i  = project_to(found, list);        // project to list iterator
List::iterator i2 = project_to<List::iterator>(found);
```

## view::perspective

View, which set point of return. Latter, you can back to that point with `view::project`, or standalone `project`.
May accept tag as last parameter.

```cpp
using namespace ranges;

using List = std::vector<Data>;
List list  = {{1}, {2}};    

auto l  = list 
        | view::perspective
        | view::transform(&Data::i) 
        | view::remove_if([](int i){ return i%2; });    

List::iterator i = project(find(l, 5)); // will project back to list (perspective point)
```

## view::project

Project previously declared perspective. May accept tag as last parameter.

```cpp
using namespace ranges;

struct Data{ int i; };
using List = std::vector<Data>;
List list  = {{1}, {1}, {2}, {3}};

struct{} t;

auto l = 
    view::perspective(list, t)
    | view::transform(&Data::i)
    | view::unique;
    | view::project(t);

for(Data& data: l){
    std::cout << data.i << " ";
}
```
Output:
```
1 2 3
```

`view::project` will return base of first occurrence of `view::perspective`.
Can be used without `Tag` parameter.

## view::push_view + view::pop_view

Same as `view::perspective` + `view::project` but designed for nested use (hence - safer for general use).
```cpp
using namespace ranges;

std::vector<int> i1 = {10, 10,  20,  21,  30};
std::vector<int> i2 = {100,101, 200, 200, 300};

auto unique_pairs =
    ranges::view::zip(i1, i2)
    | view::push_view
        | view::keys
        | view::unique
    | view::pop_view
    | view::push_view
        | view::values
        | view::unique
    | view::pop_view;   
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

struct{} k1;  struct{} k2;

auto list =
    ranges::view::zip(i1, i2)
    | view::push_view
        | view::keys
        | view::unique
        | view::transform([](int i){ return -i;})
        | view::set_base(k1)
    | view::pop_view
    | view::push_view
        | view::values
        | view::unique
        | view::transform([](int i){ return std::to_string(i) + "n";})
        | view::set_base(k2)
    | view::pop_view
    | forward_iterator | view::transform([](auto&& iter){
        return std::make_pair(*get_base(iter, k1), *project(iter, k2));
    });
```
Result
```
{-10, "100n"}, {-20, "200n"}, {-30, "300n"}
```

## Additional iterator member

`project` and `pop_view` iterators has `origin()` member - which returns the original base iterator:
```cpp
auto list = vec
            | view::push_view
            | view::remove_if([](auto&&) { return false; });
            | view::pop_view

REQUIRE(list.begin().origin() == vec.begin());
```

### Perfomance overhead

Not measured yet. Chain of n `base()` calls (`base().base().base()`), should fold to O(1) in release mode, because all view types are statically known, and all iterator/view offsets/memory locations are known.

Zero size overhead on iterators and views.
