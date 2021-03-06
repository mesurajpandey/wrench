/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <memory>
#include <iostream>

#include <xbt/ex.hpp>
#include "wrench/logging/TerminalOutput.h"
#include "wrench/simgrid_S4U_util/S4U_PendingCommunication.h"
#include "wrench/simulation/SimulationMessage.h"
#include "wrench/workflow/execution_events/FailureCause.h"

XBT_LOG_NEW_DEFAULT_CATEGORY(pending_communication, "Log category for Pending Communication");

namespace wrench {

    /**
     * @brief Wait for the pending communication to complete
     *
     * @return A (unique pointer to a) simulation message
     *
     * @throw std::shared_ptr<NetworkError>
     */
    std::unique_ptr<SimulationMessage> S4U_PendingCommunication::wait() {

      try {
        if (this->comm_ptr->get_state() != simgrid::s4u::Activity::State::FINISHED) {
          this->comm_ptr->wait();
        }
      } catch (xbt_ex &e) {
        if (e.category == network_error) {
          throw std::shared_ptr<NetworkError>(
                  new NetworkError(NetworkError::RECEIVING, NetworkError::FAILURE, this->mailbox_name));
        } else if (e.category == timeout_error) {
          throw std::shared_ptr<NetworkError>(
                  new NetworkError(NetworkError::RECEIVING, NetworkError::TIMEOUT, this->mailbox_name));
        } else {
          throw std::runtime_error(
                  "S4U_PendingCommunication::wait(): Unexpected xbt_ex exception (" + std::to_string(e.category) + ")");
        }
      }
      return std::unique_ptr<SimulationMessage>(this->simulation_message);
    }

    /**
     * @brief Wait for any pending communication completion
     * @param pending_comms: a list of pending communications
     * @param timeout: timeout value in seconds (-1 means no timeout)
     *
     * @return the index of the comm to which something happened (success or failure)
     *
     * @throw std::invalid_argument
     */
    unsigned long S4U_PendingCommunication::waitForSomethingToHappen(
            std::vector<std::unique_ptr<S4U_PendingCommunication>> pending_comms, double timeout) {
      std::vector<S4U_PendingCommunication *> raw_pointer_comms;
      for (auto it = pending_comms.begin(); it != pending_comms.end(); it++) {
        raw_pointer_comms.push_back((*it).get());
      }
      return S4U_PendingCommunication::waitForSomethingToHappen(raw_pointer_comms, timeout);
    }


    /**
     * @brief Wait for any pending communication completion
     * @param pending_comms: a list of pending communications
     * @param timeout: timeout value in seconds (-1 means no timeout)
     *
     * @return the index of the comm to which something happened (success or failure)
     *
     * @throw std::invalid_argument
     */
    unsigned long S4U_PendingCommunication::waitForSomethingToHappen(
            std::vector<S4U_PendingCommunication *> pending_comms, double timeout) {

      std::set<S4U_PendingCommunication *> completed_comms;

      if (pending_comms.empty()) {
        throw std::invalid_argument("S4U_PendingCommunication::waitForSomethingToHappen(): invalid argument");
      }

      std::vector<simgrid::s4u::CommPtr> pending_s4u_comms;
      for (auto it = pending_comms.begin(); it < pending_comms.end(); it++) {
        pending_s4u_comms.push_back((*it)->comm_ptr);
      }

      unsigned long index;
      bool one_comm_failed = false;
      try {
        index = (unsigned long) simgrid::s4u::Comm::wait_any(&pending_s4u_comms);
      } catch (xbt_ex &e) {
        if (e.category != network_error) {
          throw std::runtime_error(
                  "S4U_PendingCommunication::waitForSomethingToHappen(): Unexpected xbt_ex exception (" +
                  std::to_string(e.category) + ")");
        }
        one_comm_failed = true;
      } catch (std::exception &e) {
        throw std::runtime_error("S4U_PendingCommunication::waitForSomethingToHappen(): Unexpected std::exception  (" +
                                 std::string(e.what()) + ")");
      }

      if (one_comm_failed) {
        for (index = 0; index < pending_s4u_comms.size(); index++) {
          try {
            pending_s4u_comms[index]->test();
          } catch (xbt_ex &e) {
            if (e.category == network_error) {
              break;
            } else {
              throw std::runtime_error(
                      "S4U_PendingCommunication::waitForSomethingToHappen(): Unexpected xbt_ex exception (" +
                      std::to_string(e.category) + ")");
            }
          } catch (std::exception &e) {
            throw std::runtime_error(
                    "S4U_PendingCommunication::waitForSomethingToHappen(): Unexpected std::exception  (" +
                    std::string(e.what()) + ")");
          }
        }
      }

      return index;
    }

    /**
     * @brief Constructor
     *
     * @param mailbox: a mailbox name
     */
    S4U_PendingCommunication::S4U_PendingCommunication(std::string mailbox) : mailbox_name(mailbox) {
    }


};