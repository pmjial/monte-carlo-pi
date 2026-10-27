#pragma once

#include <atomic>
#include <random>
#include <thread>
#include <vector>

#include "src/monte_carlo.hpp"

namespace multi_thread {
auto approximate_pi(std::uniform_random_bit_generator auto rng, size_t n, double r, uint p) -> double {
    std::atomic<double> n_within_r = 0;
    auto functor = [r, &n_within_r](auto thread_rng, size_t thread_n) {
        n_within_r += monte_carlo(thread_rng, thread_n, r);
    };

    std::vector<std::jthread> jobs;
    jobs.reserve(p);
    size_t portion = n / p;
    size_t remainder = n % p;
    for (uint i = 0; i < p; i++) {
        // Each thread gets its own engine seeded from the parent one, so threads draw different samples
        decltype(rng) thread_rng{rng()};
        size_t thread_n = portion + (i < remainder ? 1 : 0);
        auto job = std::jthread(functor, thread_rng, thread_n);
        jobs.push_back(std::move(job));
    }

    for (auto& job : jobs) {
        job.join();
    }

    return (4.0 * n_within_r / n);
}
}  // namespace multi_thread
