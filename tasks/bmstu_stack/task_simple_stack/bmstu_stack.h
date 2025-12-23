#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <cassert>
namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() =default;

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack() { clear(); }

	template <typename... Args>
	void emplace(Args&&... args)
	{
		T* n_data = gtn();
		new (&n_data[size_]) T(std::forward<Args>(args)...);
		operator delete(data_);
		data_ = n_data;
		size_++;

	}

	void push(T&& value) {emplace(std::move(value));}

	void clear() noexcept {
		for(size_t i = size_; i > 0; --i)
			data_[i-1].~T();

		operator delete(data_);
		data_ = nullptr;
		size_ = 0;
	}

	void push(const T& value) { emplace(value);}

	void pop() {
		if(empty()){
			throw std::underflow_error("Stack is empty");
		}
		data_[size_-1].~T();
		--size_;
	}

	T& top() { 
		if(empty()){
			throw std::underflow_error("Stack is empty");
		}

		return data_[size_-1]; }
		
	const T& top() const { 
		if(empty()){
			throw std::underflow_error("Stack is empty");
		}
		
		return data_[size_-1]; }

   private:
	T* data_ = nullptr;
	size_t size_ = 0;

	T* gtn(){
		T* n_data =(T*)(operator new(sizeof(T) * (size_ + 1)));
		for (size_t i = 0; i<size_; i++){
			new(&n_data[i]) T(std::move(data_[i]));
			data_[i].~T();
		}
		return n_data;
	}
};
}  // namespace bmstu
