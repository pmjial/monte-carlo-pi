#include <chrono>
#include <print>
#include <random>

#include "multi_thread.hpp"
#include "single_thread.hpp"

namespace chrono = std::chrono;

int main(int argc, char** argv) {
    if (argc < 4) {
        std::println(stderr, "Usage: .exe <type:single_thread|multi_thread> <n:int> <r:int> [processors:int]");
        return 1;
    }

    std::string cmd_type = argv[1];
    double n = std::atof(argv[2]);
    double r = std::atof(argv[3]);
    uint processors = std::thread::hardware_concurrency();
    if (argc == 5) {
        processors = std::atof(argv[4]);
    } else {
        std::println(stderr, "No \"processors\" set, defaulting to {}", processors);
    }

    std::mt19937 engine{};
    double pi;
    auto start = chrono::high_resolution_clock::now();
    if (cmd_type == "single_thread") {
        pi = single_thread::approximate_pi(engine, n, r);
    } else if (cmd_type == "multi_thread") {
        pi = multi_thread::approximate_pi(engine, n, r, processors);
    } else {
        std::println(stderr, "Invalid \"cmd_type\": {}. Choose one from: single_thread|multi_thread", cmd_type);
        return 1;
    }
    auto elapsed = chrono::high_resolution_clock::now() - start;

    std::println("pi: {}, elapsed: {}", pi, chrono::duration_cast<chrono::milliseconds>(elapsed));
}
