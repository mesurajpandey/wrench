/**
 * Copyright (c) 2017. The WRENCH Team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */


#ifndef WRENCH_SERVICEPROPERTY_H
#define WRENCH_SERVICEPROPERTY_H

#include <string>

#define DECLARE_PROPERTY_NAME(name) static const std::string name

#define SET_PROPERTY_NAME(classname, name) const std::string classname::name=#name

namespace wrench {


    /**
     * @brief Configurable properties for a Service
     */
    class ServiceProperty {

    public:

    };

};


#endif //WRENCH_SERVICEPROPERTY_H
