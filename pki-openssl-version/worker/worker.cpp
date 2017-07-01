/*
Télécom SudParis, Télécom Saint-Étienne
2017, April
Pierre-Olivier Rocher
*/

#include <iostream>

#include "worker.h"

#define MSG_EXIT_THREAD			1
#define MSG_POST_USER_DATA		2
#define MSG_TIMER				3

struct threadMessage
{
	threadMessage(int i, const void* m) { id = i; msg = m; }
	int id;
	const void* msg;
};

worker::worker(const std::string& fm_threadName) noexcept : worker()
{
	m_threadName = fm_threadName;
}

worker::~worker(void)
{
	exitThread();
}

const bool worker::createThread(void)
{
	if (m_thread == nullptr)
	{
		m_thread = std::make_unique<std::thread>(&worker::process, this);
	}

	return true;
}

const std::thread::id worker::getThreadId(void) const noexcept (false)
{
	if (m_thread == nullptr)
	{
		throw std::runtime_error("no thread is running");
	}

	return m_thread->get_id();
}

const std::thread::id worker::getCurrentThreadId(void)
{
	return std::this_thread::get_id();
}

void worker::exitThread(void)
{
	if (m_thread == nullptr)
	{
		return;
	}

	// Put exit thread message into the queue
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		m_queue.emplace(std::make_unique<threadMessage>(threadMessage(MSG_EXIT_THREAD, 0)));

		m_conditionVariable.notify_one();
	}

	m_thread->join();

	m_thread->~thread();

	//delete m_thread;
	//m_thread = 0;
}

const void worker::postMessage(const userData* data) noexcept (false)
{
	if (m_thread == nullptr)
	{
		throw std::runtime_error("no thread is running");
	}

	// Add user data msg to queue and notify worker thread
	std::unique_lock<std::mutex> lk(m_mutex);
	//std::shared_lock<std::mutex> lk(m_mutex);

	m_queue.emplace(std::make_unique<threadMessage>(threadMessage(MSG_POST_USER_DATA, data)));

	m_conditionVariable.notify_one();
}

void worker::timerThread(void)
{
	//while (!m_timerExit)
	while (m_timerExit == false)
	{
		// Sleep for 250ms then put a MSG_TIMER message into queue
		std::this_thread::sleep_for(std::chrono::duration<long double, std::milli>(250));

		// Add timer msg to queue and notify worker thread
		std::unique_lock<std::mutex> uL(m_mutex);
		//std::shared_lock<std::mutex> lk(m_mutex);

		m_queue.emplace(std::make_unique<threadMessage>(threadMessage(MSG_TIMER, 0)));
		m_conditionVariable.notify_one();
	}
}

worker::worker(void) noexcept
{
	m_thread = { nullptr };

	m_timerExit = { false };
}

void worker::process(void)
{
	m_timerExit = false;

	auto tT = std::thread(&worker::timerThread, this);

	while (true)
	{
		//threadMessage* msg = 0;
		std::shared_ptr<threadMessage> msg = { nullptr };

		{
			// Wait for a message to be added to the queue
			std::unique_lock<std::mutex> lk(m_mutex);
			//std::shared_lock<std::mutex> lk(m_mutex);

			while (m_queue.empty() == true)
			{
				m_conditionVariable.wait(lk);
			}
				
			if (m_queue.empty() == true)
			{
				continue;
			}

			msg = m_queue.front();

			m_queue.pop();
		}

		switch (msg->id)
		{
			case MSG_POST_USER_DATA:
			{
				//ASSERT_TRUE(msg->msg != NULL);

				// Convert the ThreadMsg void* data back to a UserData* 
				const userData* uD = static_cast<const userData*>(msg->msg);

				//std::cout << userData->msg.c_str() << " " << userData->year << " on " << m_threadName << std::endl;

				// Delete dynamic data passed through message queue
				delete uD;

				break;
			}

			case MSG_TIMER:
				//cout << "Timer expired on " << m_threadName << endl;

				break;

			case MSG_EXIT_THREAD:
			{
				m_timerExit = true;
				tT.join();



				std::unique_lock<std::mutex> lk(m_mutex);
				//std::shared_lock<std::mutex> lk(m_mutex);

				while (!m_queue.empty())
				{
					msg = m_queue.front();
					m_queue.pop();

				}

				//cout << "Exit thread on " << m_threadName << endl;
				return;
			}

			//default:
				//ASSERT();
		}
	}
}
