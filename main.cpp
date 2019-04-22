#include <iostream>

#include <thread>
#include <map>
#include <memory>
#include <vector>

#include <stdlib.h>
#include <time.h>

#include "Manager.hpp"
#include "Worker.hpp"
#include "SafeQueue.hpp"
#include "Operation.hpp"
#include "MachinesService.hpp"

using namespace std;

int main()
{
    srand(time(NULL));

    int nrOfWorkers{3};
    int maxTimeBetweenOperations{10};
    int minTimeBetweenOperations{2};

    vector<thread> workersThreads(nrOfWorkers);
    vector<shared_ptr<Worker>> workers(nrOfWorkers);

    auto safeQueue = make_shared<ThreadSafeQueue>();
    auto machinesService = make_shared<MachinesService>();

    for (int i=0; i<nrOfWorkers; ++i)
    {
        shared_ptr<Worker> worker = make_shared<Worker>(i+1, safeQueue, machinesService);
        workersThreads.emplace_back(&Worker::tryAct, worker);
        workers.push_back(worker);
    }

    Manager manager{minTimeBetweenOperations, maxTimeBetweenOperations, safeQueue};
    thread manager_thread(&Manager::start, &manager);

    manager_thread.join();
    for (auto& thread : workersThreads)
    {
        thread.join();
    }
    return 0;
}
