/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "wrench/services/compute/batch/BatchServiceProperty.h"

namespace wrench {
    SET_PROPERTY_NAME(BatchServiceProperty, THREAD_STARTUP_OVERHEAD);
    SET_PROPERTY_NAME(BatchServiceProperty, HOST_SELECTION_ALGORITHM);

    SET_PROPERTY_NAME(BatchServiceProperty, BATCH_SCHEDULING_ALGORITHM);
    SET_PROPERTY_NAME(BatchServiceProperty, BATCH_QUEUE_ORDERING_ALGORITHM);
    SET_PROPERTY_NAME(BatchServiceProperty, BATCH_RJMS_DELAY);
    SET_PROPERTY_NAME(BatchServiceProperty, SIMULATED_WORKLOAD_TRACE_FILE);
    SET_PROPERTY_NAME(BatchServiceProperty, OUTPUT_CSV_JOB_LOG);

}