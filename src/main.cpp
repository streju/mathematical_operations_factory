#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <thread>
#include <vector>

#include "Config.hpp"
#include "Customer.hpp"
#include "MachinesService.hpp"
#include "Manager.hpp"
#include "Operation.hpp"
#include "Shop.hpp"
#include "Tools/ProgramStopController.hpp"
#include "Tools/Randoms.hpp"
#include "Tools/ThreadsPool.hpp"
#include "Tools/ThreadWrapper.hpp"
#include "Transport.hpp"
#include "Warehouse.hpp"
#include "WarehouseEntryPoint.hpp"
#include "Worker.hpp"
#include "WorkersPool.hpp"

using namespace std;

bool wasStarted{false};

enum class Input
{
    start,
    stop,
    invalid
};

Input evaluateInput()
{
    string cmd;
    cin >> cmd;

    if (cmd == "START" && !wasStarted) return Input::start;
    if (cmd == "STOP" && wasStarted) return Input::stop;
    Logger() << "Invalid command please type START to run or STOP to end program" << endl;
    return evaluateInput();
}

void start()
{
    wasStarted = true;

    const auto& config = Config::getConfig();

    const auto stopController = make_shared<tools::ProgramStopControllerHelper>();

    const auto machinesService = make_shared<MachinesService>(stopController, config.nrOfMachines);
    const auto warehouseEntry = make_shared<WarehouseEntryPoint>(
        std::make_shared<Warehouse>(config.maxSizeOfDepoInWarehouse, stopController));
    const auto worker = make_shared<Worker>(machinesService, warehouseEntry, stopController);
    Manager manager{
        stopController,
        make_unique<WorkersPool>(stopController, config.nrOfWorkers),
        machinesService,
        warehouseEntry,
        worker,
        config.minTimeBetweenOperations,
        config.maxTimeBetweenOperations};
    tools::ThreadWrapper manager_thread{thread(&Manager::start, &manager), true};

    const auto shop = make_shared<Shop>();
    vector<unique_ptr<tools::ThreadWrapper>> transporters(config.nrOfTransporters);
    const auto transport = make_shared<Transport>(stopController, warehouseEntry, shop);
    for (unsigned i = 1; i <= config.nrOfTransporters; i++)
    {
        transporters.emplace_back(make_unique<tools::ThreadWrapper>(
            thread(&Transport::start, transport, i), true));
    }

    vector<unique_ptr<tools::ThreadWrapper>> customers(config.nrOfCustomers);
    const auto customer = make_shared<Customer>(stopController, shop);
    for (unsigned customerNr = 1; customerNr <= config.nrOfCustomers; customerNr++)
    {
        customers.emplace_back(make_unique<tools::ThreadWrapper>(
            thread(&Customer::start, customer, customerNr), true));
    }
    const auto stopAction = async([stopController]
        {
            if (evaluateInput() == Input::stop)
            {
                Logger() << "Begin of stop program procedure." << endl;
                stopController->stop();
            }
        });
}

int main()
{
    cout << "Type START to run simulation. To end program type STOP in any moment." << endl;

    if (evaluateInput() == Input::start)
    {
        start();
    }
    return 0;
}
