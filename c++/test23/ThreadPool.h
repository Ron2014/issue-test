#pragma once
#include <atomic>
#include <thread>
#include <mutex>
#include <array>
#include <list>
#include <functional>
#include <condition_variable>

template <uint32_t ThreadCount = 10>		// 池子的大小, 在初始化时通过泛型参数指定

class ThreadPool {
	// ======================================================================== 消费消息
	void Task() {
		// 休息状态就真的永远休眠了
		// 没有恢复线程的接口
		while (!this->m_bBreak) {
			this->NextJob()();				// 取出函数并执行
			this->m_varWait.notify_one();
		}
	}

	std::function<void(void)> NextJob() {
		std::function<void(void)> res;
		std::unique_lock<std::mutex> job_lock(m_mutQueue);

		// 条件触发: 有任务排队, 或者处于休息状态
		// 触发结果: (从消息队列中取出消息) 开始工作
		this->m_varJobAvailable.wait(job_lock, [this]() ->bool { 
			return this->m_nJobCount || this->m_bBreak; 
		});

		if (!this->m_bBreak) {
			res = this->m_Queue.front();
			this->m_Queue.pop_front();
			--this->m_nJobCount;
		}
		else { 
			// 处于休息状态时, 剩下的工作都不管了, 尽快退出消费接口
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

	// delete -> ~ThreadPool() -> JoinAll() -> WaitAll -> join子线程
	~ThreadPool() {
		this->JoinAll();
	}

	inline uint32_t Size() const {
		return ThreadCount;						// 池子的容量(最多有多少个线程同时在跑)
	}

	inline uint32_t JobsRemaining() {
		std::lock_guard<std::mutex> guard(this->m_mutQueue);
		return this->m_Queue.size();			// 排队的任务, 其实是个消息队列
	}

	// ======================================================================== 生产消息
	void AddJob(std::function<void(void)> job) {
		std::lock_guard<std::mutex> guard(this->m_mutQueue);
		this->m_Queue.emplace_back(job);
		++this->m_nJobCount;
		this->m_varJobAvailable.notify_one();
	}

	void JoinAll(bool WaitForAll = true) {
		if (!this->m_bFinished) {
			if (WaitForAll) {
				// 主线程阻塞, 等待所有Job完成
				// wait过程中还是会接受新的Jobs的
				WaitAll();
			}

			// 此时队列为空, 所有子线程是空转线程
			// 都阻塞在 m_varJobAvailable 条件上
			this->m_bBreak = true;
			this->m_varJobAvailable.notify_all();

			// 通过串联线程, 回收资源
			for (auto &x : this->m_aryThreads) {
				if (x.joinable()) {
					x.join();
				}
			}

			// 总觉得这一句应该放在if头部
			this->m_bFinished = true;	// m_bFinished 赋值后对象就销毁了, 没有实际意义
		}
	}

	void WaitAll() {
		if (this->m_nJobCount > 0) {
			std::unique_lock<std::mutex> lk(this->m_mutWait);
			// 触发条件: 队列是空的
			// 触发结果: 线程串联起来, 等待他们完全退出
			this->m_varWait.wait(lk, [this] { return this->m_nJobCount == 0; });
			lk.unlock();
		}
	}

private:
	std::array<std::thread, ThreadCount> m_aryThreads;		// 储存线程对象的地方

	// queue
	std::list<std::function<void(void)>> m_Queue;
	std::mutex              m_mutQueue;
	
	std::condition_variable m_varJobAvailable; 	// condition for NextJob, 子线程用

	// wait all
	std::condition_variable m_varWait;			// condition for wait all, 主线程用
	std::mutex              m_mutWait;

	std::atomic_int         m_nJobCount;		// equal to m_Queue.size()
	std::atomic_bool        m_bBreak;			// idle, save the jobs in queue
	std::atomic_bool        m_bFinished;		// destroied
};