#include "basic/BSTest.hpp"
#include "basic/BCTest.hpp"
#include "packet/PSTest.hpp"
#include "packet/PCTest.hpp"

#include "util/thread/Thread.h"
#include "network/core/CGFileParser.h"
#include <sstream>

int main(void)
{
	PSTest();

	//thread sleep
	std::chrono::seconds dura(3);
	std::chrono::system_clock::time_point StartTime = std::chrono::system_clock::now();
	std::this_thread::sleep_for(dura);

	PCTest();

	getchar();
}