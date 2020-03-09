#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <array>
#include <list>
#include <functional>
#include <condition_variable>

template <uint32_t ThreadCount = 10>
class ThreadPool {
	void Task() {
		while (!this->m_bBreak) {
			this->NextJob()();
			this->m_varWait.notify_one();
		}
	}

	std::function<void(void)> NextJob() {
		std::function<void(void)> res;
		std::unique_lock<std::mutex> job_lock(m_mutQueue);

		this->m_varJobAvailable.wait(job_lock, [this]() ->bool { 
			return this->m_nJobCount || this->m_bBreak; 
		});

		if (!this->m_bBreak) {
			res = this->m_Queue.front();
			this->m_Queue.pop_front();
			--this->m_nJobCount;
		}
		else { 
			res = [] {};
		}
		return res;
	}

public:
	ThreadPool()
		: m_nJobCount(0)
		, m_bBreak(false)
		, m_bFinished(false) {
		for (uint32_t i = 0; i < ThreadCount; ++i) {
			this->m_aryThreads[i] = std::thread([this] {
				this->Task(); 
			});
		}
	}

	~ThreadPool() {
		this->JoinAll();
	}

	inline uint32_t Size() const {
		return ThreadCount;
	}

	inline uint32_t JobsRemaining() {
		std::lock_guard<std::mutex> guard(this->m_mutQueue);
		return this->m_Queue.size();
	}

	void AddJob(std::function<void(void)> job) {
		std::lock_guard<std::mutex> guard(this->m_mutQueue);
		this->m_Queue.emplace_back(job);
		++this->m_nJobCount;
		this->m_varJobAvailable.notify_one();
	}

	void JoinAll(bool WaitForAll = true) {
		if (!this->m_bFinished) {
			if (WaitForAll) {
				WaitAll();
			}

			this->m_bBreak = true;
			this->m_varJobAvailable.notify_all();

			for (auto &x : this->m_aryThreads) {
				if (x.joinable()) {
					x.join();
				}
			}

			this->m_bFinished = true;
		}
	}

	void WaitAll() {
		if (this->m_nJobCount > 0) {
			std::unique_lock<std::mutex> lk(this->m_mutWait);
			this->m_varWait.wait(lk, [this] { return this->m_nJobCount == 0; });
			lk.unlock();
		}
	}

private:
	std::array<std::thread, ThreadCount> m_aryThreads;

	// queue
	std::list<std::function<void(void)>> m_Queue;
	std::mutex              m_mutQueue;
	
	std::condition_variable m_varJobAvailable; 	// condition for NextJob

	// wait all
	std::condition_variable m_varWait;			// condition for wait all
	std::mutex              m_mutWait;

	std::atomic_int         m_nJobCount;		// equal to m_Queue.size()
	std::atomic_bool        m_bBreak;			// idle, save the jobs in queue
	std::atomic_bool        m_bFinished;		// destroied
};