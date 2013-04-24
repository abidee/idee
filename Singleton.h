/**
* @file Singleton.h
* @author X_Q
* @brief simple singleton write in c++
* todo: how to create a singleton object which ctor has multi-parameters?
*/
#ifndef IDEE_SINGLETON_H
#define IDEE_SINGLETON_H

namespace idee
{

template< class T>
class  Singleton
{
private:
	static T* instance_ptr_;	
public:
	void release()
	{
		if(instance_ptr_)
			delete instance_ptr_;
	};

	static T* instance_ptr()
	{
		if(NULL == instance_ptr_)
		{
			instance_ptr_ = new T();
			//assert(instance_ptr_ != NULL);
		}
		return instance_ptr_;
	}

	///create a instance with 1 parameter in the ctor
	template <class C> static T* instance_ptr( C val ) 
	{
		if ( instance_ptr_ == NULL ) 
		{
		instance_ptr_ = new T(val);
		}
		return instance_ptr_;
    }

	///create a instance with 2 parameter in the ctor
    template <class C, class M> static T* instance_ptr( C val1, M val2 ) 
	{
		if ( instance_ptr_ == NULL ) 
		{
		instance_ptr_ = new T(val1, val2);
		}
		return instance_ptr_;
    }

	////////////////////////////////////////////////////////////////////////
	static T& instance()
	{
		T* ptr = instance_ptr();
		return *ptr;
	}

	template <class C> static T& instance(C val) 
	{
		T* ptr = instance_ptr(val);
		return *ptr;
	}

	template <class C, class M> static T& instance(C val1, M val2) 
	{
		T* ptr = instance_ptr(val1, val2);
		return *ptr;
	}
};

template< class T>
T* Singleton<T>::instance_ptr_ = NULL;

}
#endif