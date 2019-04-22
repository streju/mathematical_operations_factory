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

#include "src/WorkersPool.hpp"
#include "src/ThreadWrapper.hpp"

using namespace std;

int main()
{
    srand(time(NULL));

    int nrOfWorkers{3};
    int maxTimeBetweenOperations{10};
    int minTimeBetweenOperations{2};

    //to remove
    vector<std::unique_ptr<ThreadWrapper>> workersThreads(nrOfWorkers);
//    workersThreads.reserve(nrOfWorkers);
    vector<shared_ptr<Worker>> workers(nrOfWorkers);

    WorkersPool workersPool{nrOfWorkers};

    auto safeQueue = make_shared<ThreadSafeQueue>();
    auto machinesService = make_shared<MachinesService>();

    for (int i=0; i<nrOfWorkers; ++i)
    {
        shared_ptr<Worker> worker = make_shared<Worker>(i+1, safeQueue, machinesService);
        workersThreads.emplace_back(std::make_unique<ThreadWrapper>(std::thread(&Worker::tryAct, worker), true));
        workers.push_back(worker);
    }

    Manager manager{minTimeBetweenOperations, maxTimeBetweenOperations, safeQueue};
    ThreadWrapper manager_thread{std::thread(&Manager::start, &manager), true};

//    for (auto& thread : workersThreads)
//    {
//        thread.join();
//    }
    return 0;
}
