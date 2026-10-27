#pragma once

#include <cmath>
#include <random>

auto monte_carlo(std::uniform_random_bit_generator auto rng, size_t n, double r) -> double {
    double n_within_r = 0;
    std::uniform_real_distribution<> dist(0, r);
    for (size_t i = 0; i < n; i++) {
        double px = dist(rng);
        double py = dist(rng);
        double plen = std::sqrt(px * px + py * py);

        n_within_r += plen <= r;
    }
    return n_within_r;
}
