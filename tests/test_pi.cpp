#include <cmath>
#include <cstdint>
#include <numbers>
#include <print>
#include <random>
#include <source_location>

#include "src/multi_thread.hpp"
#include "src/single_thread.hpp"

namespace {
int failures = 0;

void check(bool condition, std::string_view what, std::source_location loc = std::source_location::current()) {
    if (!condition) {
        std::println(stderr, "FAIL {}:{}: {}", loc.file_name(), loc.line(), what);
        failures++;
    }
}

// Always yields min(), so every sampled point is (0, 0) and lands inside the circle.
// With it, approximate_pi returns exactly 4.0 only if all n samples were counted.
struct ZeroRng {
    using result_type = std::uint32_t;
    explicit ZeroRng(result_type = 0) {}
    static constexpr auto min() -> result_type { return 0; }
    static constexpr auto max() -> result_type { return UINT32_MAX; }
    auto operator()() -> result_type { return 0; }
};

constexpr double tolerance = 0.01;

auto near_pi(double value) -> bool {
    return std::abs(value - std::numbers::pi) < tolerance;
}
}  // namespace

int main() {
    std::mt19937 engine{};

    check(near_pi(single_thread::approximate_pi(engine, 1'000'000, 1)), "single_thread approximates pi");
    check(near_pi(single_thread::approximate_pi(engine, 1'000'000, 5)), "single_thread works for r != 1");

    for (uint p : {1u, 2u, 7u, 16u}) {
        check(near_pi(multi_thread::approximate_pi(engine, 1'000'000, 1, p)), "multi_thread approximates pi");
    }

    // If both threads drew the same samples, the result would equal a single run over half of them
    check(multi_thread::approximate_pi(engine, 200'000, 1, 2) != single_thread::approximate_pi(engine, 100'000, 1),
          "multi_thread threads draw different samples");

    // n not divisible by p, including n < p: every sample must still be counted
    check(single_thread::approximate_pi(ZeroRng{}, 10, 1) == 4.0, "single_thread counts all samples");
    check(multi_thread::approximate_pi(ZeroRng{}, 10, 1, 16) == 4.0, "multi_thread counts all samples when n < p");
    check(multi_thread::approximate_pi(ZeroRng{}, 1'000'003, 1, 7) == 4.0,
          "multi_thread counts all samples when n % p != 0");

    if (failures > 0) {
        std::println(stderr, "{} check(s) failed", failures);
        return 1;
    }
    std::println("all checks passed");
}
