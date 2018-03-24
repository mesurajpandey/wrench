/**
* Copyright (c) 2017. The WRENCH Team.
*
* This program is free software: you can redistribute it and/or modify
        * it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*/

#ifndef WRENCH_SIMULATIONTIMESTAMPTYPES_H
#define WRENCH_SIMULATIONTIMESTAMPTYPES_H


namespace wrench {

    class WorkflowTask;

    /**
    * @brief A "task completion" simulation timestamp
    */
    class SimulationTimestampTaskCompletion {

    public:

        /***********************/
        /** \cond DEVELOPER    */
        /***********************/

        /**
         * @brief Constructor
         * @param task: a workflow task
         */
        SimulationTimestampTaskCompletion(WorkflowTask *task, std::string hostname) {
          this->hostname = hostname;
          this->task = task;
        }

        ~SimulationTimestampTaskCompletion() {
        }

        /***********************/
        /** \endcond           */
        /***********************/

        /**
         * @brief Retrieve the task that has completed
         *
         * @return the task
         */
        WorkflowTask *getTask() {
          return this->task;
        }

        /**
         * @brief Retrieve the host where the task was allocated
         *
         * @return the hostname
         */
        std::string getHostName() {
          return this->hostname;
        }

    private:
        WorkflowTask *task;
        std::string hostname;
    };
};

#endif //WRENCH_SIMULATIONTIMESTAMPTYPES_H
