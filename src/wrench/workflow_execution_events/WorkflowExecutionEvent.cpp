/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * @brief TBD
 */


#include <simgrid_S4U_util/S4U_Mailbox.h>
#include <exception/WRENCHException.h>
#include "WorkflowExecutionEvent.h"

namespace wrench {

		WorkflowExecutionEvent::WorkflowExecutionEvent() {
			this->type = WorkflowExecutionEvent::UNDEFINED;
			this->job = nullptr;
			this->compute_service = nullptr;
		}

		std::unique_ptr<WorkflowExecutionEvent> WorkflowExecutionEvent::get_next_execution_event(std::string mailbox) {

			// Get the message on the mailbox
			std::unique_ptr<SimulationMessage> message = S4U_Mailbox::get(mailbox);

			std::unique_ptr<WorkflowExecutionEvent> event =
							std::unique_ptr<WorkflowExecutionEvent>(new WorkflowExecutionEvent());

			switch (message->type) {

				case SimulationMessage::STANDARD_JOB_DONE: {
					std::unique_ptr<JobDoneMessage> m(static_cast<JobDoneMessage *>(message.release()));
					event->type = WorkflowExecutionEvent::STANDARD_JOB_COMPLETION;
					event->job = m->job;
					event->compute_service = m->compute_service;
					return event;
				}

				case SimulationMessage::STANDARD_JOB_FAILED: {
					std::unique_ptr<JobFailedMessage> m(static_cast<JobFailedMessage *>(message.release()));
					event->type = WorkflowExecutionEvent::STANDARD_JOB_FAILURE;
					event->job = m->job;
					event->compute_service = m->compute_service;
					return event;
				}

				default: {
					throw WRENCHException("Non-handled message type when generating execution event");
				}
			}

		}
};