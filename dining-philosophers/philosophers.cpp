#include <iostream>
#include <time.h>
#include <vector>
#include <thread>
#include <mutex>
#include <unistd.h>

using namespace std;

class Fork
{
public:
    int name;
    mutable std::mutex mtx;

    Fork(int v = 0) : name(v) {}

    Fork(Fork &&other)
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        name = std::move(other.name);
        other.name = 0;
    }
};

class Philosopher
{
public:
    int name;
    int count = 0;

    Philosopher(int v = 0) : name(v) {}

    void eat(Fork &left_fork, Fork &right_fork)
    {
        if (left_fork.mtx.try_lock())
        {
            for (int i = 0; i <= rand() % 3 + 1; i++)
            {
                if (right_fork.mtx.try_lock())
                {
                    std::cout << "Philosopher " << name << " eats" << endl;
                    sleep(rand() % (21 - 10) + 10);
                    count++;
                    right_fork.mtx.unlock();
                    left_fork.mtx.unlock();
                    return;
                }
                sleep(1);
            }
            left_fork.mtx.unlock();
        }
    }

    void think(int time)
    {
        std::cout << "Philosopher " << name << " thinks" << endl;
        sleep(time);
    }
};

void simulation(Philosopher &philosopher, Fork &left_fork, Fork &right_fork)
{
    for (int i = 0; i <= 13; i++)
    {
        philosopher.think(rand() % 7 + 13);
        philosopher.eat(left_fork, right_fork);
    }
}

int main()
{
    int sum = 0;
    int count_philosopher;
    vector<thread> threads;
    vector<Fork> forks;
    vector<Philosopher> philosophers;

    std::cout << "Enter count of philosophers> ";
    std::cin >> count_philosopher;

    for (int i = 0; i < count_philosopher; i++)
    {
        philosophers.push_back(Philosopher(i));
        forks.push_back(Fork(i));
    }

    for (int i = 0; i < count_philosopher; i++)
        threads.push_back(thread(simulation, ref(philosophers[i]), ref(forks[i]), ref(forks[(i + 1) % count_philosopher])));

    for (thread &t : threads)
        t.join();

    std::cout << endl
              << "Result:" << endl;
    for (int i = 0; i < count_philosopher; i++)
        std::cout << "Philosopher " << philosophers[i].name << " eating count " << philosophers[i].count << endl;
    std::cout << sum << endl;
}
