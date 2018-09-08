#include "basic/BSTest.hpp"
#include "basic/BCTest.hpp"
#include "packet/PSTest.hpp"
#include "packet/PSTest.hpp"

#include "util/thread/Thread.h"


int main(void)
{
	BSTest();

	//thread sleep
	std::chrono::seconds dura(3);
	std::chrono::system_clock::time_point StartTime = std::chrono::system_clock::now();
	std::this_thread::sleep_for(dura);

	BCTest();
	getchar();
}