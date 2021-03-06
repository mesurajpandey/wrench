/**
 * Copyright (c) 2017-2018. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef WRENCH_VIRTUALIZEDCLUSTERSERVICE_H
#define WRENCH_VIRTUALIZEDCLUSTERSERVICE_H

#include <map>
#include <simgrid/s4u/VirtualMachine.hpp>

#include "VirtualizedClusterServiceProperty.h"
#include "VirtualizedClusterServiceMessagePayload.h"
#include "wrench/services/Service.h"
#include "wrench/services/compute/ComputeService.h"
#include "wrench/simgrid_S4U_util/S4U_VirtualMachine.h"
#include "wrench/simulation/Simulation.h"
#include "wrench/workflow/job/PilotJob.h"


namespace wrench {

    class Simulation;

    class ComputeService;

    /**
     * @brief A  virtualized cluster-based compute service
     */
    class VirtualizedClusterService : public ComputeService {

    private:
        std::map<std::string, std::string> default_property_values = {
                {VirtualizedClusterServiceProperty::SUPPORTS_PILOT_JOBS,    "true"},
                {VirtualizedClusterServiceProperty::SUPPORTS_STANDARD_JOBS, "true"},
                {VirtualizedClusterServiceProperty::VM_BOOT_OVERHEAD_IN_SECONDS, "0.0"}
        };

        std::map<std::string, std::string> default_messagepayload_values = {
                {VirtualizedClusterServiceMessagePayload::STOP_DAEMON_MESSAGE_PAYLOAD,                  "1024"},
                {VirtualizedClusterServiceMessagePayload::DAEMON_STOPPED_MESSAGE_PAYLOAD,               "1024"},
                {VirtualizedClusterServiceMessagePayload::RESOURCE_DESCRIPTION_REQUEST_MESSAGE_PAYLOAD, "1024"},
                {VirtualizedClusterServiceMessagePayload::RESOURCE_DESCRIPTION_ANSWER_MESSAGE_PAYLOAD,  "1024"},
                {VirtualizedClusterServiceMessagePayload::GET_EXECUTION_HOSTS_REQUEST_MESSAGE_PAYLOAD,  "1024"},
                {VirtualizedClusterServiceMessagePayload::GET_EXECUTION_HOSTS_ANSWER_MESSAGE_PAYLOAD,   "1024"},
                {VirtualizedClusterServiceMessagePayload::CREATE_VM_REQUEST_MESSAGE_PAYLOAD,            "1024"},
                {VirtualizedClusterServiceMessagePayload::CREATE_VM_ANSWER_MESSAGE_PAYLOAD,             "1024"},
                {VirtualizedClusterServiceMessagePayload::MIGRATE_VM_REQUEST_MESSAGE_PAYLOAD,           "1024"},
                {VirtualizedClusterServiceMessagePayload::MIGRATE_VM_ANSWER_MESSAGE_PAYLOAD,            "1024"},
                {VirtualizedClusterServiceMessagePayload::SUBMIT_STANDARD_JOB_REQUEST_MESSAGE_PAYLOAD,  "1024"},
                {VirtualizedClusterServiceMessagePayload::SUBMIT_STANDARD_JOB_ANSWER_MESSAGE_PAYLOAD,   "1024"},
                {VirtualizedClusterServiceMessagePayload::SUBMIT_PILOT_JOB_REQUEST_MESSAGE_PAYLOAD,     "1024"},
                {VirtualizedClusterServiceMessagePayload::SUBMIT_PILOT_JOB_ANSWER_MESSAGE_PAYLOAD,      "1024"}
        };

    public:
        VirtualizedClusterService(const std::string &hostname,
                                  std::vector<std::string> &execution_hosts,
                                  double scratch_space_size,
                                  std::map<std::string, std::string> property_list = {},
                                  std::map<std::string, std::string> messagepayload_list = {});

        /***********************/
        /** \cond DEVELOPER    */
        /***********************/

        virtual std::string createVM(const std::string &pm_hostname,
                                     unsigned long num_cores = ComputeService::ALL_CORES,
                                     double ram_memory = ComputeService::ALL_RAM,
                                     std::map<std::string, std::string> property_list = {},
                                     std::map<std::string, std::string> messagepayload_list = {});

        virtual bool migrateVM(const std::string &vm_hostname, const std::string &dest_pm_hostname);

        std::vector<std::string> getExecutionHosts();


        /***********************/
        /** \endcond          **/
        /***********************/

        /***********************/
        /** \cond INTERNAL    */
        /***********************/
        void submitStandardJob(StandardJob *job, std::map<std::string, std::string> &service_specific_args) override;

        void submitPilotJob(PilotJob *job, std::map<std::string, std::string> &service_specific_args) override;

        void terminateStandardJob(StandardJob *job) override;

        void terminatePilotJob(PilotJob *job) override;

        ~VirtualizedClusterService();

        /***********************/
        /** \endcond          **/
        /***********************/


    protected:

        /***********************/
        /** \cond INTERNAL    */
        /***********************/

        friend class Simulation;

        int main() override;

        virtual bool processNextMessage();

        virtual void processGetResourceInformation(const std::string &answer_mailbox);

        virtual void processGetExecutionHosts(const std::string &answer_mailbox);

        virtual void processCreateVM(const std::string &answer_mailbox,
                                     const std::string &pm_hostname,
                                     const std::string &vm_hostname,
                                     unsigned long num_cores,
                                     double ram_memory,
                                     std::map<std::string, std::string> &property_list,
                                     std::map<std::string, std::string> &messagepayload_list);

        virtual void processMigrateVM(const std::string &answer_mailbox,
                                      const std::string &vm_hostname,
                                      const std::string &dest_pm_hostname);

        virtual void processSubmitStandardJob(const std::string &answer_mailbox, StandardJob *job,
                                              std::map<std::string, std::string> &service_specific_args);

        virtual void processSubmitPilotJob(const std::string &answer_mailbox, PilotJob *job);

        void stopAllVMs();

        /** @brief List of execution host names */
        std::vector<std::string> execution_hosts;

        /** @brief Map of available RAM at hosts */
        std::map<std::string, double> cs_available_ram;

        /** @brief A map of VMs described by the VM actor, the actual compute service, and the total number of cores */
        std::map<std::string, std::tuple<std::shared_ptr<S4U_VirtualMachine>, std::shared_ptr<ComputeService>, unsigned long>> vm_list;

        /***********************/
        /** \endcond           */
        /***********************/
    };

}

#endif //WRENCH_VIRTUALIZEDCLUSTERSERVICE_H
