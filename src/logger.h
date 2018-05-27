//
// Created by mariusz on 24.05.18.
//

#ifndef POBR_LOGO_LOGGER_H
#define POBR_LOGO_LOGGER_H

#include <iostream>

#define LEVEL_DEBUG 0
#define LEVEL_INFO 1

#define LOG_LEVEL LEVEL_INFO

#if LOG_LEVEL <= LEVEL_DEBUG
#define DEBUG_ENABLE(x) x
#define DEBUG_DISABLE(x)
#else
#define DEBUG_ENABLE(x)
#define DEBUG_DISABLE(x) x
#endif

#if LOG_LEVEL <= LEVEL_INFO
#define INFO_ENABLE(x) x
#define INFO_DISABLE(X)
#else
#define INFO_ENABLE(x)
#define INFO_DISABLE(X) x
#endif

#endif //POBR_LOGO_LOGGER_H
