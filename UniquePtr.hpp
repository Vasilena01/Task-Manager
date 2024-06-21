#include <iostream>

template <typename T>
class UniquePtr
{
	T* ptr;
public:
	UniquePtr();
	UniquePtr(T* ptr);

	UniquePtr(const UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr&) = delete;

	UniquePtr(UniquePtr&&) noexcept;
	UniquePtr& operator=(UniquePtr&&) noexcept;

	explicit operator bool() const;

	T& operator*();
	const T& operator*() const;
	
	T* operator->();
	const T* operator->() const;

	T* getPtr() const;
	~UniquePtr();
};

template<typename T>
UniquePtr<T>::UniquePtr() : ptr(nullptr)
{}

template <typename T>
UniquePtr<T>::UniquePtr(T* ptr) : ptr(ptr)
{}

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr<T>&& other) noexcept
{
	ptr = other.ptr;
	other.ptr = nullptr;
}
template <typename T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& other) noexcept
{
	if (this != &other)
	{
		delete ptr;
		ptr = other.ptr;
		other.ptr = nullptr;
	}
	return *this;
}

template<typename T>
UniquePtr<T>::operator bool() const
{
	return ptr != nullptr;
}

template <typename T>
T& UniquePtr<T>::operator*()
{
	return *ptr;
}

template <typename T>
const T& UniquePtr<T>::operator*() const
{
	return *ptr;
}

template <typename T>
T* UniquePtr<T>::operator->()
{
	return ptr;
}

template <typename T>
const T* UniquePtr<T>::operator->() const
{
	return ptr;
}

template<typename T>
T* UniquePtr<T>::getPtr() const
{
	return ptr;
}

template <typename T>
UniquePtr<T>::~UniquePtr()
{
	delete ptr;
}
