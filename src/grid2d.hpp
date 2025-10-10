#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <cstddef>
#include <cassert>
#include <typeinfo>
#include <cxxabi.h>
#include <string>
#include <iostream>

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

    grid2d(int w, int h, const T &init = T(), std::string debug_name = "")
        : w_(w), h_(h), data_(std::size_t(w) * h, init) {
        int status = 0;
        const char *mangled = typeid(T).name();
        char *demangled = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
        std::string name = status == 0 ? demangled : mangled;
        std::free(demangled);
        std::cout << "grid2d = " << name << " " << debug_name << std::endl;
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
