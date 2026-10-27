#pragma once

#include <random>

#include "src/monte_carlo.hpp"

namespace single_thread {
auto approximate_pi(std::uniform_random_bit_generator auto rng, size_t n, double r) -> double {
    double n_within_r = monte_carlo(rng, n, r);
    return (4.0 * n_within_r / n);
}
}  // namespace single_thread
