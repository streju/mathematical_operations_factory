#pragma once

#include <memory>
#include <string>
#include <utility>

#include "IWorker.hpp"
#include "Operation.hpp"
#include "OperationResult.hpp"
#include "IMachinesService.hpp"
#include "IWarehouseEntryPoint.hpp"
#include "Tools/IProgramStopControllerHelper.hpp"

class Worker : public IWorker
{
public:
    Worker(const MachinesServicePtr& machinesService,
        const WarehouseEntryPointPtr& warehouse,
        const tools::ProgramStopControllerPtr& stopController);

    void startProcessingOperation(const unsigned threadNr, const OperationPtr& operation) override;
    void loadTransport(unsigned threadNr) override;
    void movePendingsOperationsToWarehouse(unsigned threadNr) override;
private:
    void transportToMachine(const std::string& prefix, const OperationPtr& operation) const;
    void putOperationResultToMagazine(const std::string& prefix, const OperationPtr& operation,
        const OperationResultPtr& operationResult);

    MachinesServicePtr machinesService_;
    WarehouseEntryPointPtr warehouse_;
    tools::ProgramStopControllerPtr stopController_;
};
