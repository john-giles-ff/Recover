#ifndef THREADSAFE_HPP
#define THREADSAFE_HPP

#ifdef SIMULATOR
//In simulator mode, the handle is never used so pretend an int is the handle
typedef int SemaphoreHandle_t;
#endif 



template <class T>
class ThreadSafe
{
private:
	T _value;
	SemaphoreHandle_t _xSemaphore;				


protected:
	bool Lock()
	{		
#ifndef SIMULATOR
		if (_xSemaphore == NULL)
			return true;

		if (xSemaphoreTake(_xSemaphore, 300) != pdTRUE)
			return false;
#endif
		return true;
	}
	bool Unlock()
	{		
#ifndef SIMULATOR
		xSemaphoreGive(_xSemaphore);
#endif
		return true;
	}



public:	
	ThreadSafe() 
	{ 
#ifdef SIMULATOR
		//_value = { 0 };
		_xSemaphore = { 0 };
#endif
	}

	ThreadSafe(T value)
	{
		_xSemaphore = { 0 };
		_value = value;
	}	

	void SetSemaphore(SemaphoreHandle_t semaphore)
	{
		_xSemaphore = semaphore;
	}		
	
	T operator= (ThreadSafe other)
	{
		T val;		
		if (!Lock())
		{			
			return T();
		}

		val = _value;
		Unlock();
		return val;
	}

	ThreadSafe operator= (const T other)
	{
		if (!Lock())
			return *this;


		_value = other;
		Unlock();

		return *this;
	}
	
	operator T() {
		T val;

		if (!Lock())
			return T();

		val = _value;

		Unlock();

		return val;
	}



};


#endif /* GUI_UTILS_HPP */


