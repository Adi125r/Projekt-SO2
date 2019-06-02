#include <atomic>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
#include "Fork.h"


class diningPhilosophersProblem {
public:
    std::array<Fork, 5> forks;
    diningPhilosophersProblem();
    std::atomic<bool> ready{false};
};

diningPhilosophersProblem::diningPhilosophersProblem() {}

class philosopher {
public:
    int id;
    diningPhilosophersProblem &table;
    Fork &leftFork;
    Fork &rightFork;
    std::thread t;
    std::mt19937 rng{std::random_device{}()};
    int state = -1;
    int progress = 0;

    bool exit = false;
    philosopher(int _id, diningPhilosophersProblem &table_ref, Fork &l, Fork &r) : id(_id), table(table_ref), leftFork(l), rightFork(r), t(&philosopher::live, this) {}
    void live();
    void eat();
    void think();
};

void philosopher::live() {

    while (!table.ready) {
        std::this_thread::yield();
    }

    while (!exit) {
        think();
        state = 2;
        eat();
    }
    return;
}

void philosopher::think() {
    state = 0;
    int part = std::uniform_int_distribution<int>(15, 25)(rng);
    for (auto i = 1; i <= part; i++) {

        double p = (double)i / (double)part;
        progress = std::round(p * 100);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (exit) {
            return;
        }
    }
}

void philosopher::eat() {
    std::lock(leftFork.mutex, rightFork.mutex);
    std::lock_guard<std::mutex> left_lock(leftFork.mutex, std::adopt_lock);
    std::lock_guard<std::mutex> right_lock(rightFork.mutex, std::adopt_lock);

    state = 1;
    int part = std::uniform_int_distribution<int>(15, 25)(rng);
    
    for (auto i = 1; i <= part; i++) {

        double p = (double)i / (double)part;
        progress = std::round(p * 100);;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (exit) {
            return;
        }
    }
}
