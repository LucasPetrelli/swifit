/**
 * @file debug_adapter.h
 * @author Lucas
 * @date Oct 9, 2017
 *
 * @brief Debug and logging tools implementation
 */

#ifndef CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_
#define CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_

#include "config.h"

#ifdef DEBUG
	#define LOG_DEBUG(args...) {printf("[%s]", __FILE__);printf(args);printf("\n");}
#else
	#define LOG_DEBUG(args...) do {} while(0);
#endif

#endif /* CODE_ADAPTER_DEBUG_ADAPTER_DEBUG_ADAPTER_H_ */
