#ifndef LAZYCODE_MAPRANGE_H_
#define LAZYCODE_MAPRANGE_H_
#include "rangeBase.h"
namespace LazyCode {

template <typename Range, typename Func>
class MapRange : public RangeBase {
    Range range;
    Func func;

   public:
    MapRange(RmRef<Range>&& range, Func func)
        : range(std::move(range)), func(func) {}

    MapRange(RmRef<Range>& range, Func func) : range(range), func(func) {}

    inline bool hasValue() { return range.hasValue(); }
    inline void moveNext() { range.moveNext(); }
    inline decltype(auto) getValue() { return func(range.getValue()); }

    inline auto begin() { return RangeIterator<MapRange<Range, Func>>(*this); }
    inline auto end() { return RangeIterator<MapRange<Range, Func>>(*this); }
};

template <typename Func, typename Range, EnableIfRange<Range> = 0>
inline auto map(Func&& func, Range&& range) {
    return MapRange<Range, RmRef<Func>>(std::forward<Range>(range),
                                        std::forward<Func>(func));
}

template <typename Func, typename Container, EnableIfNotRange<Container> = 0>
inline auto map(Func&& func, Container&& container) {
    return map(std::forward<Func>(func),
               toRange(std::forward<Container>(container)));
}
template <typename Func>
class MapBuilder {
    Func func;

   public:
    MapBuilder(Func func) : func(func) {}
    template <typename T>
    friend inline auto operator|(T&& iterable,
                                 const MapBuilder<Func>& builder) {
        return map(builder.func, std::forward<T>(iterable));
    }
};

template <typename Func>
inline MapBuilder<Func> map(Func&& func) {
    return MapBuilder<Func>(std::forward<Func>(func));
}

}  // namespace LazyCode
#endif /* LAZYCODE_MAPRANGE_H_*/