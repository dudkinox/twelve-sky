/****************************************************************************

   Ras library FreeBSD Version

   Copyright(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: pointer.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __POINTER_H__
#define __POINTER_H__

namespace SDSocket 
{
	typedef union _u_any_pointer
	{
		void *obj_ptr;
		const void *const_obj_ptr;
		void (*func_ptr)();
		char data[1];
	} any_pointer_t;

	inline any_pointer_t make_any_pointer(void *_obj_ptr)
	{
		any_pointer_t p;
		p.obj_ptr = _obj_ptr;
		return p;
	}

	inline any_pointer_t make_any_pointer(const void *_obj_ptr)
	{
		any_pointer_t p;
		p.const_obj_ptr = _obj_ptr;
		return p;
	}

	inline any_pointer_t make_any_pointer(void (*_func_ptr)())
	{
		any_pointer_t p;
		p.func_ptr = _func_ptr;
		return p;
	}

	struct _s_unresolvable 
	{
		_s_unresolvable() {}
		template<typename T> _s_unresolvable(const T&) {}
	};

	template<typename T> struct function_return_type { typedef T type; };
	template<> struct function_return_type<void>  { typedef _s_unresolvable type; };

	/* example
	void foo()
	{
		    any_auto_ptr<MyClass> p(new MyClass);
			p->DoSomething();
	}
	*/

	template<class T> class any_auto_ptr
	{
	public:
		explicit any_auto_ptr(T* p = NULL): m_ptr(p) {}
		~any_auto_ptr() { 
			if(m_ptr != NULL) {
				delete m_ptr;
			}
		}
		T* release() { 
			T* __tmp = m_ptr; 
			m_ptr = NULL; 
			return __tmp;
		}
		void reset(T* p = NULL) {
			if((p != m_ptr) && (p != NULL)) {
				delete m_ptr;
			}
			m_ptr = p;
		}
		T* get() { return m_ptr; }
		T& operator*() { return *m_ptr; }
		T* operator->() { return m_ptr; }
		T& operator=(const T& _ptr) { m_ptr = _ptr; return m_ptr;}
		/*
		T& operator=(any_auto_ptr<T>& _ptr) {
			this->reset(_ptr.release());
			return this;
		}
		*/
	private:
		T* m_ptr;
	};
}
#endif
