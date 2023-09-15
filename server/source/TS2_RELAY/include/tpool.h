#ifndef __TPOOL_H__
#define __TPOOL_H__

#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

#include "log.h"
#include "mutex.h"
#include "thread.h"
#include "exception.h"

using namespace std;
using namespace SDSocket;

#define DEFAULT_TPOOL_JOB_WAIT_TO  100
//#define DEFAULT_TPOOL_MTIME        60
#define DEFAULT_TPOOL_MTIME        1 

enum eThreadStat {
	WAITING = 0,
	RUNNING = 1,
	STOPPED = 2,
};

enum eThreadType {
    STARTUP   = 1,
    SPARE     = 2,
    TEMPORARY = 3,
};

typedef struct tpool_job {
	void (*routine)(void *arg_ptr);
	void *arg_ptr;
} tpool_job_t;

typedef struct _tpool_t {
	pthread_t thread_id;
    eThreadType thread_type;
	tpool_job_t thread_job;
	eThreadStat thread_stat;
	pthread_cond_t wait_for_job;
	pthread_mutex_t wait_for_job_mutex;
	sem_t thd_is_ready;
	time_t last_ack_at;
    time_t last_run_at;
    time_t created_at;
} tpool_t;

typedef map<pthread_t, tpool_t *> tpool_map_t;
typedef tpool_map_t::iterator tpool_map_iterator;
typedef tpool_map_t::value_type tpool_map_type;
typedef vector<pthread_t> thread_list_t;

class CThreadPool
{
public:
	CThreadPool(unsigned int num_threads);
	~CThreadPool(void);
	int add_job(void (*routine)(void *arg_ptr), void *arg_ptr);
	int mtpool_thread(void);

	class CThreadPoolMThread : public Thread {
	public:
		CThreadPoolMThread(CThreadPool *tp_obj_ptr);
		void run(void);
	private:
		CThreadPool *m_tp_obj_ptr;
	};
    
		
protected:
	int run(void);
	int init_thread_pool(void);
	void destroy(void);
	void set_num_of_threads(unsigned int num_threads) { m_num_threads = num_threads; }

private:
	static void *init_single_thread(void *arg_ptr);
    int add_single_thread(eThreadType thread_type, pthread_t *thread_id_ptr, bool to_be_inserted = true);

	tpool_map_t m_tpool_map;
	unsigned int m_num_threads;
    unsigned int m_num_spare_threads;
    unsigned int m_num_temp_threads;
	sem_t m_sem_tpool_map_in_use;
	static Semaphore thread_lock;
};
#endif
