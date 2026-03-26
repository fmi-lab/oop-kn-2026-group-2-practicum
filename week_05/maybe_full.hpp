#pragma once

#include <cstdlib>
#include <utility>

template <typename T>
class Maybe {
	bool has_value;
	union {
		T value;
	};

   public:
	Maybe() : has_value(false) {}
	Maybe(const T &value) : has_value(true), value(value) {}
	Maybe(T &&value) : has_value(true), value(std::move(value)) {}
	Maybe(const Maybe &other) : has_value(other.has_value) {
		if (has_value) { value = other.value; }
	}
	Maybe(Maybe &&other) : has_value(other.has_value) {
		if (has_value) { value = std::move(other.value); }
	}

	~Maybe() {
		if (has_value) value.~T();
	}

	Maybe &operator=(const Maybe &other) {
		if (this == &other) return *this;
		if (!other.has_value) {
			if (has_value) value.~T();
			has_value = false;
			return *this;
		} else {
			if (has_value) value = other.value;
			else {
				new (&value) T(other.value);
				has_value = true;
			}
		}

		return *this;
	}

	Maybe &operator=(Maybe &&other) {
		if (this == &other) return *this;
		if (!other.has_value) {
			if (has_value) value.~T();
			has_value = false;
		} else {
			if (has_value) value = std::move(other.value);
			else {
				new (&value) T(std::move(other.value));
				has_value = true;
			}
		}

		return *this;
	}

	bool	 hasValue() const { return has_value; }
	const T &get() const {
		if (!has_value) { std::exit(1); }
		return value;
	}
	T &get() {
		if (!has_value) { std::exit(1); }
		return value;
	}

	void reset() {
		if (has_value) {
			value.~T();
			has_value = false;
		}
	}
			 operator bool() const { return has_value; }
	const T *operator->() const { return &get(); }
	T		*operator->() { return &get(); }
	const T &operator*() const { return get(); }
	T		&operator*() { return get(); }
};
