#pragma once

#include <iterator>
#include <random>

namespace tools
{

template<typename Iter>
Iter select_randomly_from_container(Iter start, Iter end) {
    thread_local std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(gen));
    return start;
}

template<typename T>
T random(T min, T max) {
    thread_local std::mt19937 gen{std::random_device{}()};
    return std::uniform_int_distribution<T>{min, max}(gen);
}

template<typename T>
T random_floating(T min, T max) {
    thread_local std::mt19937 gen{std::random_device{}()};
    using dist = std::conditional_t<
        std::is_integral<T>::value,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >;
    return dist{min, max}(gen);
}

} // namespace tools
