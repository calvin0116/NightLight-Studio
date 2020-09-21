#pragma once

template <class T>
class Singleton
{
private:
	/* Here will be the instance stored */
	static T* instance;

protected:
	/* Creation only can be done by the class itself*/
	Singleton() {};
	~Singleton() {};
public:
	
	/* Static access method. */
	static T* GetInstance();

	static void DestroyInstance();
};

template <class T>
T* Singleton<T>::instance = nullptr;

template <class T>
T* Singleton<T>::GetInstance()
{
	if (instance == nullptr)
		instance = new T();

	return instance;
}

template <class T>
void Singleton<T>::DestroyInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}