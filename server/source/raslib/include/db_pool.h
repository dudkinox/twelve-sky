/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: db_pool.h,v 1.9 2007/09/15 11:51:47 wglee Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __DB_CONN_POOL_H__
#define __DB_CONN_POOL_H__

#pragma interface
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>

#include "log.h"
#include "raslib.h"
#include "thread.h"
#include "atomic.h"
#include "pointer.h"
#include "address.h"
#include "semaphore.h"
#include "MersenneTwister.h"
#include <unistd.h>

using namespace std;
using namespace SDLog;
using namespace SDSocket;

#define MAX_DB_CONN_RETRY_CNT      3
#define DEFAULT_DB_MT_TIME         30
#define MAX_SIZE_OF_DB_ACCT        64
#define MAX_SIZE_OF_DB_PASSWD      64
#define MAX_SIZE_OF_DB_NAME        64
#define MAX_SIZE_OF_DB_HOST_ADDR   32

namespace SDSocket {
template<class T> class CDBConnPool
{
#define INCREASE_RUN_CNT(num_ptr) {\
	atomic_inc_uint32(num_ptr);\
}

#define DECREASE_RUN_CNT(num_ptr) {\
	atomic_dec_uint32(num_ptr);\
}
	public:
		enum DB_STATUS {
			DB_CONN_READY           = 0x0001,
			DB_CONN_IN_QUERY        = 0x0002,
			DB_CONN_CONN_CLOSED     = 0x0003,
			DB_CONN_IN_ERROR_COND   = 0x0004
		};

		enum DB_ASSIGNMENT {
			DB_CONN_FOR_SERVICE     = 0x0020,
			DB_CONN_FOR_SPARE       = 0x0030
		};

		typedef unsigned long long ident_t;
		typedef struct _s_db_maintenance {
			ident_t ele_ident;
			DB_STATUS status;
		} db_maintenance_t;

		typedef struct _s_db_info {
			char m_user_id_str[MAX_SIZE_OF_DB_ACCT];
			char m_user_passwd_str[MAX_SIZE_OF_DB_PASSWD];
			char m_db_name_str[MAX_SIZE_OF_DB_NAME];
			char m_db_host_addr_str[MAX_SIZE_OF_DB_HOST_ADDR];
			Address::Port m_db_host_port;
			unsigned int m_d_start_up_conn;
			unsigned int m_d_min_spare_conn;
			unsigned int m_d_max_spare_conn;
			unsigned int m_d_max_allowed_conn;
		} db_info_t;

		typedef struct _s_db_pool_t {
			ident_t ele_ident;
			T* db_conn_ptr;
			volatile unsigned int run_cnt;
			unsigned int status;
			unsigned int assignment;
		} db_pool_t;

		typedef vector<db_maintenance_t> db_conn_maintenance_vec_t;
		typedef map<ident_t, db_pool_t>  db_conn_pool_map_t;

		explicit CDBConnPool(void);
		~CDBConnPool(void) throw();

		int init_pool(void);
		void set_db_info(db_info_t _db_info);
		T* get_db_conn(db_pool_t &op_ele_ref);
		bool set_db_conn_status(ident_t _conn_ident, DB_STATUS _status);
		unsigned int get_num_of_conn(void) throw();
		bool pool_maintenance(void) throw();

		class CDBConnPoolManager : public Thread {
			public:
				CDBConnPoolManager(CDBConnPool<T>* db_conn_pool_ptr) throw(Exception);
				CDBConnPoolManager(CDBConnPool<T>* db_conn_pool_ptr, int maintenance_time) throw(Exception);
				CDBConnPoolManager(void) throw();
				void set_mt_time(int maintenance_time);
				void set_db_conn_pool(CDBConnPool<T>* db_conn_pool_ptr, int maintenance_time);
				virtual void run(void);
			protected:

			private:
				static Semaphore sem_ocm;
				CDBConnPool<T>* m_db_conn_pool_ptr;
				int m_maintenance_time;
		};

	protected:
		bool _connect_to_db(DB_ASSIGNMENT _assignment);
		bool _remove_connection(ident_t _conn_ident);

		unsigned int m_n_db_conn;
		unsigned int m_n_db_spare_conn;

	private:
		db_info_t m_db_info;
		db_conn_pool_map_t m_db_conn_map;
		bool m_flag_db_pool_initialized;
		static Semaphore sem_pool;
		sem_t m_sem_conn;
};


template <class T> Semaphore CDBConnPool<T>::sem_pool;
template <class T> Semaphore CDBConnPool<T>::CDBConnPoolManager::sem_ocm;

template <class T> CDBConnPool<T>::CDBConnPool(void) 
{
	m_flag_db_pool_initialized = false;
	m_n_db_conn = 0;
	m_n_db_spare_conn = 0;

	::sem_init(&m_sem_conn, 0, 1);
}

template <class T> CDBConnPool<T>::~CDBConnPool(void) throw() 
{
	typename CDBConnPool<T>::db_conn_pool_map_t::iterator iter;

	::sem_destroy(&m_sem_conn);
	for(iter = m_db_conn_map.begin(); iter != m_db_conn_map.end(); ++iter) {
		if(iter->second.db_conn_ptr) {
			delete iter->second.db_conn_ptr;
			iter->second.db_conn_ptr = NULL;
		}
	}
}

template <class T> unsigned int CDBConnPool<T>::get_num_of_conn(void) throw() 
{
	unsigned int n_conn_count = m_db_conn_map.size();

	if(m_n_db_conn != n_conn_count) {
		--sem_pool;
		m_n_db_conn = n_conn_count;
		++sem_pool;
	}
	return m_n_db_conn;
}

template <class T> void CDBConnPool<T>::set_db_info(db_info_t _db_info) 
{
	::memcpy((db_info_t *)&m_db_info, (db_info_t *)&_db_info, sizeof(db_info_t));
}

template <class T> int CDBConnPool<T>::init_pool(void)
{
	unsigned int i = 0;
	DB_ASSIGNMENT new_conn_assignment = DB_CONN_FOR_SERVICE;

	if((m_db_info.m_d_start_up_conn <= 0) ||
			((m_db_info.m_d_start_up_conn + m_db_info.m_d_max_spare_conn) > m_db_info.m_d_max_allowed_conn)) {
		return -1;
	}

	for(i = 1; i <= (m_db_info.m_d_start_up_conn + m_db_info.m_d_min_spare_conn); i++) {
		if(i <= m_db_info.m_d_start_up_conn) {
			new_conn_assignment = DB_CONN_FOR_SERVICE;
		} else if((i > m_db_info.m_d_start_up_conn) && 
				(i <= (m_db_info.m_d_start_up_conn + m_db_info.m_d_min_spare_conn))) {
			new_conn_assignment = DB_CONN_FOR_SPARE;
		}

		if(this->_connect_to_db(new_conn_assignment)) {
#if 0 /* comate */
			if(new_conn_assignment == DB_CONN_FOR_SERVICE) {
				m_n_db_conn++;
			} else if(new_conn_assignment == DB_CONN_FOR_SPARE) {
				m_n_db_spare_conn++;
			}
#endif
		} else {
			return -2;
		}
	}
	return 1;
}

template <class T> bool CDBConnPool<T>::_connect_to_db(DB_ASSIGNMENT _assignment) 
{
	if(m_db_conn_map.size() > m_db_info.m_d_max_allowed_conn) {
		return false;
	} else {
		MTRand mtrand;
		T* new_db_conn_ptr = new T;
		if(new_db_conn_ptr->connect(m_db_info.m_db_host_addr_str, m_db_info.m_db_host_port, m_db_info.m_db_name_str, 
		                            m_db_info.m_user_id_str, m_db_info.m_user_passwd_str) != NULL) {
			db_pool_t new_db_ele;
			new_db_ele.ele_ident = (ident_t)mtrand.randInt();
			new_db_ele.db_conn_ptr = new_db_conn_ptr;
			new_db_ele.run_cnt = 0;
			new_db_ele.status = DB_CONN_READY;
			new_db_ele.assignment = _assignment;
			m_db_conn_map[new_db_ele.ele_ident] = new_db_ele;

			if(_assignment == DB_CONN_FOR_SERVICE) {
				--sem_pool;
				m_n_db_conn++;
				++sem_pool;
			} else if(_assignment == DB_CONN_FOR_SPARE) {
				--sem_pool;
				m_n_db_spare_conn++;
				++sem_pool;
			}
		} else {
			return false;
		}
	}
	return true;
}

template <class T> bool CDBConnPool<T>::_remove_connection(ident_t _conn_ident) 
{
	bool fFound = false;
	typename CDBConnPool<T>::db_conn_pool_map_t::iterator iter;

	for(iter = m_db_conn_map.begin(); iter != m_db_conn_map.end(); ++iter) {
		if(iter->first == _conn_ident) {
			iter->second.db_conn_ptr->disconnect();
			if(iter->second.db_conn_ptr != NULL) {
				delete iter->second.db_conn_ptr;
				iter->second.db_conn_ptr = NULL;
			}
			fFound = true;
			break;
		}
	} 

	if(fFound) {
		m_db_conn_map.erase(iter);
	}
	return fFound;
}

template <class T> T* CDBConnPool<T>::get_db_conn(db_pool_t &op_ele_ref) 
{
	bool fFound = false;
	T* ret_val_ptr = NULL;
	typename CDBConnPool<T>::db_conn_pool_map_t::const_iterator iter;

	::sem_wait(&m_sem_conn);
	for(iter = m_db_conn_map.begin(); iter != m_db_conn_map.end(); ++iter) {
		if(iter->second.status == DB_CONN_READY) {
			set_db_conn_status(iter->first, DB_CONN_IN_QUERY);
			op_ele_ref = iter->second;
			fFound = true;
			break;
		} 
	}

	if(!fFound) {
		if(m_n_db_spare_conn < m_db_info.m_d_max_spare_conn) {
			if(this->_connect_to_db(DB_CONN_FOR_SPARE)) {
    			for(iter = m_db_conn_map.begin(); iter != m_db_conn_map.end(); ++iter) {
   			     	if(iter->second.status == DB_CONN_READY) {
						set_db_conn_status(iter->first, DB_CONN_IN_QUERY);
           				op_ele_ref = iter->second;
			            fFound = true;
           			 	break;
        			}
    			}
#if 0
				--sem_pool;
				m_n_db_spare_conn++;
				++sem_pool;
#endif
			} 
		} 
	}
	::sem_post(&m_sem_conn);

	if(fFound) { 
		ret_val_ptr = iter->second.db_conn_ptr;
	} else {
		ret_val_ptr = NULL;
	}
	return ret_val_ptr;
}

template<class T> bool CDBConnPool<T>::set_db_conn_status(ident_t _conn_ident, DB_STATUS _status)
{
	bool fFound = false;
	typename CDBConnPool<T>::db_conn_pool_map_t::iterator iter = m_db_conn_map.find(_conn_ident);

	if(iter != m_db_conn_map.end()) {
		--sem_pool;
		fFound = true;
		iter->second.status = _status;
		++sem_pool;
	} 
	return fFound;
}

template<class T> bool CDBConnPool<T>::pool_maintenance(void) throw() 
{
	bool fRet = false;
	unsigned int i = 0;
	DB_ASSIGNMENT assignment;
	db_maintenance_t om_ele;
	typename CDBConnPool<T>::db_conn_pool_map_t::iterator db_pool_iter;
	db_conn_maintenance_vec_t _A_to_be_maintained;

	for(db_pool_iter = m_db_conn_map.begin(); db_pool_iter != m_db_conn_map.end(); ++db_pool_iter) {

		if((db_pool_iter->second.status == DB_CONN_CONN_CLOSED) ||  
		   (db_pool_iter->second.status == DB_CONN_IN_ERROR_COND)) {
			if(db_pool_iter->second.assignment == DB_CONN_FOR_SERVICE) {
				--sem_pool;
				--m_n_db_conn;
				++sem_pool;
			} else if(db_pool_iter->second.assignment == DB_CONN_FOR_SPARE) {
				--sem_pool;
				--m_n_db_spare_conn;
				++sem_pool;
			}
			db_pool_iter->second.db_conn_ptr->disconnect();
			om_ele.ele_ident = db_pool_iter->first;
			om_ele.status = static_cast<DB_STATUS>(db_pool_iter->second.status);
			_A_to_be_maintained.push_back(om_ele);
		}
	} 

	for(i = 0; i < _A_to_be_maintained.size(); i++) {
		this->_remove_connection(_A_to_be_maintained[i].ele_ident);
	}

	while((m_n_db_conn < m_db_info.m_d_start_up_conn) || 
		  (m_n_db_spare_conn < m_db_info.m_d_min_spare_conn)) {
		if(m_n_db_conn < m_db_info.m_d_start_up_conn) {
			assignment = DB_CONN_FOR_SERVICE;
		} else if(m_n_db_spare_conn < m_db_info.m_d_min_spare_conn) {
			assignment = DB_CONN_FOR_SPARE;
		} else { 
			return true;
		}

		if(this->_connect_to_db(assignment)) {
			fRet = true;
		} else {
			fRet = false;
			break; //comate 
		}
	} 

	return fRet;
}

template <class T> CDBConnPool<T>::CDBConnPoolManager::CDBConnPoolManager(CDBConnPool<T>* _db_conn_pool_ptr) throw(Exception)
{
	m_db_conn_pool_ptr = _db_conn_pool_ptr;
	m_maintenance_time = DEFAULT_DB_MT_TIME;
}

template <class T> CDBConnPool<T>::CDBConnPoolManager::CDBConnPoolManager(CDBConnPool<T>* _db_conn_pool_ptr, int maintenance_time) throw(Exception)
{
	m_db_conn_pool_ptr = _db_conn_pool_ptr;
	m_maintenance_time = maintenance_time;
}

template <class T> CDBConnPool<T>::CDBConnPoolManager::CDBConnPoolManager(void) throw()
{
}

template <class T> void CDBConnPool<T>::CDBConnPoolManager::set_mt_time(int maintenance_time) 
{
	--sem_ocm;
	m_maintenance_time = maintenance_time;
	++sem_ocm;
}

template <class T> void CDBConnPool<T>::CDBConnPoolManager::set_db_conn_pool(CDBConnPool<T>* _db_conn_pool_ptr, int maintenance_time)
{
	m_db_conn_pool_ptr = _db_conn_pool_ptr;
	m_maintenance_time = maintenance_time;
}

template <class T> void CDBConnPool<T>::CDBConnPoolManager::run(void)
{
	if(m_maintenance_time <= 0) {
		m_maintenance_time = DEFAULT_DB_MT_TIME;
	}

	while(1) {
		m_db_conn_pool_ptr->pool_maintenance();
		::sleep(m_maintenance_time);
	}
}

} // end of namespace
#endif

