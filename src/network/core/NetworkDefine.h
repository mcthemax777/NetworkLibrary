#pragma once

#include "SystemDefine.h"
#include <cstdint>

///ip length
#define MAX_IP_LEN 15

///set buffer count in event-driven input/output 
#define EVENT_BUFFER_SIZE 50

///not use
#define MAX_CONNECT_SIZE 100

///loop time in milliseconds
#define NETWORK_LOOP_DT 1000

///buffer size(TODO. get from cg file)
#define RCV_BUF 5000

///buffer count in buffer pool(TODO. get from cg file)
#define ALLOCATE_BUFFER_SIZE 10

namespace CG
{
	///host id(now file descript)
	typedef int32_t HostId;

	///header data type(connect, disconnect, receive)
	typedef int8_t dataType_t;

	///whole data size
	typedef uint64_t dataSize_t;
}