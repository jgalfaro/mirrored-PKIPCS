/*
Télécom SudParis, Télécom Saint-Étienne
2017, April
Pierre-Olivier Rocher
*/

#pragma once

#include <stonepine/architecture/architecture.h>

#ifdef __WINDOWS__
#ifdef WORKER_EXPORTS
#define WORKER_API __declspec(dllexport)
#else
#define WORKER_API __declspec(dllimport)
#endif
#else
#define WORKER_API
#endif

#include <thread>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>

struct userData
{
	std::string msg;
	int year;
};

struct threadMessage;

class worker final
{

private:

	std::unique_ptr<std::thread> m_thread;

	std::queue<std::shared_ptr<threadMessage>> m_queue;

	std::mutex m_mutex;
	std::condition_variable m_conditionVariable;
	std::atomic<bool> m_timerExit;

	std::string m_threadName;

private:

	worker(const worker&);
	worker& operator=(const worker&);

	/// Entry point for the worker thread
	void process(void);

	/// Entry point for timer thread
	void timerThread(void);

protected:

	WORKER_API worker(void) noexcept;

public:

	WORKER_API worker(const std::string& fm_threadName) noexcept;
	~worker(void);

	/// Called once to create the worker thread
	/// @return TRUE if thread is created. FALSE otherwise. 
	WORKER_API const bool createThread(void);

	/// Called once a program exit to exit the worker thread
	WORKER_API void exitThread(void);

	/// Get the ID of this thread instance
	/// @return The worker thread ID
	WORKER_API const std::thread::id getThreadId(void) const noexcept (false);

	/// Get the ID of the currently executing thread
	/// @return The current thread ID
	WORKER_API static const std::thread::id getCurrentThreadId(void);

	/// Add a message to thread queue. 
	/// @param[in] data - thread specific information created on the heap using operator new.
	WORKER_API const void postMessage(const userData* data) noexcept (false);

};
