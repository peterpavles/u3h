#pragma once

#include <iostream>

template <class T>
class Singleton
{

public:
	template <typename Args>
	static T* Init_instance(Args args)
	{
		if (!instance_)
		{
			instance_ = new T(std::forward<Args>(args));
		}
		return instance_;
	}

	static T* Init_instance()
	{
		if (!instance_)
		{
			instance_ = new T();
		}
		return instance_;
	}

	static T* Get_instance()
	{
		return instance_;
	}

	static void destroy_instance()
	{
		delete instance_;
		instance_ = nullptr;
	}

private:
	static T* instance_;
};

template <class T> T*  Singleton<T>::instance_ = nullptr;
