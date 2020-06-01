#pragma once

#include <iostream>
#include <string>

#include "Tools/Logger.hpp"
#include "Tools/Randoms.hpp"

struct Config
{
    static Config getConfig()
    {
        Logger() << "Do you want to use default random config? If yes, please press 1" << std::endl;
        std::string cmd;
        std::cin >> cmd;
        if (cmd == "1")
        {
            return Config();
        }
        unsigned nrOfWorkers, maxTimeBetweenOperations, minTimeBetweenOperations, maxSizeOfDepoInWarehouse,
            nrOfMachines, nrOfTransporters, nrOfCustomers;

        Logger() << "Please insert number of workers" << std::endl;
        std::cin >> nrOfWorkers;

        Logger() << "Please insert max time berween operations" << std::endl;
        std::cin >> maxTimeBetweenOperations;

        Logger() << "Please insert min time berween operations" << std::endl;
        std::cin >> minTimeBetweenOperations;

        Logger() << "Please insert max size of warehouse" << std::endl;
        std::cin >> maxSizeOfDepoInWarehouse;

        Logger() << "Please insert number of machines to process operations" << std::endl;
        std::cin >> nrOfMachines;

        Logger() << "Please insert number of transporters from warehouse to shop" << std::endl;
        std::cin >> nrOfTransporters;

        Logger() << "Please insert number of customers" << std::endl;
        std::cin >> nrOfCustomers;

        return Config(nrOfWorkers, maxTimeBetweenOperations, minTimeBetweenOperations,
            maxSizeOfDepoInWarehouse, nrOfMachines, nrOfTransporters, nrOfCustomers);
    }

    const unsigned nrOfWorkers;
    const unsigned maxTimeBetweenOperations;
    const unsigned minTimeBetweenOperations;
    const unsigned maxSizeOfDepoInWarehouse;
    const unsigned nrOfMachines;
    const unsigned nrOfTransporters;
    const unsigned nrOfCustomers;
private:
    Config()
        : nrOfWorkers{unsigned(tools::random(2, 4))}
        , maxTimeBetweenOperations{unsigned(tools::random(7, 10))}
        , minTimeBetweenOperations{unsigned(tools::random(2, 4))}
        , maxSizeOfDepoInWarehouse{unsigned(tools::random(5, 11))}
        , nrOfMachines{unsigned(tools::random(2, 3))}
        , nrOfTransporters{unsigned(tools::random(1, 3))}
        , nrOfCustomers{unsigned(tools::random(2, 3))}
    {}
    Config(
        unsigned nrOfWorkers,
        unsigned maxTimeBetweenOperations,
        unsigned minTimeBetweenOperations,
        unsigned maxSizeOfDepoInWarehouse,
        unsigned nrOfMachines,
        unsigned nrOfTransporters,
        unsigned nrOfCustomers)
        : nrOfWorkers{nrOfWorkers}
        , maxTimeBetweenOperations{maxTimeBetweenOperations}
        , minTimeBetweenOperations{minTimeBetweenOperations}
        , maxSizeOfDepoInWarehouse{maxSizeOfDepoInWarehouse}
        , nrOfMachines{nrOfMachines}
        , nrOfTransporters{nrOfTransporters}
        , nrOfCustomers{nrOfCustomers}
    {}
};
