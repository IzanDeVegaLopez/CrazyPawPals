// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

template<typename T>
class ObjectPool {
public:

	// The purpose of this iterator is to be able to traverse with for-each loop and still
	// get a pointer. If we simply return the pointer as an iterator it does not work.
	//
	struct const_ptr_iterator {
		const T *_ptr;

		const_ptr_iterator(const T *ptr) :
				_ptr(ptr) {
		}

		const T* operator*() const {
			return _ptr;
		}

		bool operator==(const const_ptr_iterator &o) const {
			return _ptr == o._ptr;
		}

		bool operator!=(const const_ptr_iterator &o) const {
			return _ptr != o._ptr;
		}
		const_ptr_iterator& operator++() {
			_ptr++;
			return *this;
		}
		const_ptr_iterator operator++(int) {
			auto tmp = *this;
			++(*this);
			return tmp;
		}
	};

	struct iterator {
		T *_ptr;

		iterator(T *ptr) :
				_ptr(ptr) {
		}

		T* operator*() {
			return _ptr;
		}

		bool operator==(const iterator &o) const {
			return _ptr == o._ptr;
		}

		bool operator!=(const iterator &o) const {
			return _ptr != o._ptr;
		}
		iterator& operator++() {
			_ptr++;
			return *this;
		}
		iterator operator++(int) {
			auto tmp = *this;
			++(*this);
			return tmp;
		}
	};

	ObjectPool(std::size_t size) noexcept :
			_size(size), //
			_pool(new T[size]), //
			_used(size), //
			_lastUsed(size - 1) {
	}

	virtual ~ObjectPool() {
		delete[] _pool;
	}

	T* alloc() {
		auto i = (_lastUsed + 1) % _size;
		while (_used[i] && i != _lastUsed)
			i = (i + 1) % _size;
		if (!_used[i]) {
			_used[i] = true;
			return _pool + i;
		} else {
			return nullptr;
		}
	}

	void free(T *p) {
		auto idx = p - _pool;
		assert(_used[idx]);
		_used[idx] = false;
	}

	T& operator[](std::size_t idx) {
		return _pool[idx];
	}

	std::size_t size() const {
		return _size;
	}

	inline iterator begin() {
		return iterator { _pool };
	}

	inline iterator end() {
		return iterator { _pool + _size };
	}

	inline const_ptr_iterator begin() const {
		return const_ptr_iterator { _pool };
	}

	inline const_ptr_iterator end() const {
		return const_ptr_iterator { _pool + _size };
	}

	inline bool is_used(std::size_t idx) const {
		assert(idx >= 0 && idx < _size);
		return _used[idx];
	}

	inline bool is_used(const T *p) const {
		return is_used(p - _pool);
	}

private:
	std::size_t _size;
	T *_pool;
	std::vector<bool> _used;
	std::size_t _lastUsed;
};

