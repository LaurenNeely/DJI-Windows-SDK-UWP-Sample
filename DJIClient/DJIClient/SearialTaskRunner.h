#pragma once

#include <ppl.h>
#include <ppltasks.h>
#include <mutex>

class SerialTaskRunner
{
private:
	std::mutex _mutex;
	concurrency::task<void> _lastTask;
public:
	SerialTaskRunner()
		:
		_lastTask([]() {})
	{

	}

	void RunTask(std::function< void()> func)
	{
		_mutex.lock();

		_lastTask = _lastTask.then([func]()
		{
			func();
		});

		_mutex.unlock();
	}
};
