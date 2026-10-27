#pragma once

#include <random>
#include <thread>

#include "src/monte_carlo.hpp"

namespace multi_thread {
auto approximate_pi(std::uniform_random_bit_generator auto rng, size_t n, double r, uint p) -> double {
    std::atomic<double> n_within_r;
    auto functor = [&rng, r, &n_within_r](size_t thread_n) { n_within_r += monte_carlo(rng, thread_n, r); };

    std::vector<std::jthread> jobs;
    jobs.reserve(p);
    double portion = 1.0 / p;
    for (uint i = 1; i <= p; i++) {
        double thread_n = portion * n;
        auto job = std::jthread(functor, thread_n);
        jobs.push_back(std::move(job));
    }

    for (auto& job : jobs) {
        job.join();
    }

    return (4.0 * n_within_r / n);
}
}  // namespace multi_thread
