#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <cstddef>
#include <cassert>

template<typename T>
class grid2d {
private:
    int w_ = 0, h_ = 0;
    std::vector<T> data_;

public:
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using pointer = typename std::vector<T>::pointer;
    using const_pointer = typename std::vector<T>::const_pointer;

    grid2d() = default;

    grid2d(int w, int h, const T &init = T())
        : w_(w), h_(h), data_(std::size_t(w) * h, init) {
    }

    int width() const { return w_; }
    int height() const { return h_; }
    bool empty() const { return data_.empty(); }

    reference operator()(int x, int y) { return data_[std::size_t(y) * w_ + x]; }
    const_reference operator()(int x, int y) const { return data_[std::size_t(y) * w_ + x]; }

    reference at(int x, int y) {
        assert(in_bounds(x,y));
        return (*this)(x, y);
    }

    const_reference at(int x, int y) const {
        assert(in_bounds(x,y));
        return (*this)(x, y);
    }

    pointer data() { return data_.data(); }
    const_pointer data() const { return data_.data(); }

    void fill(const T &v) { std::fill(data_.begin(), data_.end(), v); }

private:
    bool in_bounds(int x, int y) const {
        return (unsigned) x < (unsigned) w_ && (unsigned) y < (unsigned) h_;
    }
};
