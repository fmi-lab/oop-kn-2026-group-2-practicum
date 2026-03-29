#pragma once

#include <utility>
#include "../week_05/maybe.hpp"

template <class U, class V>
class Function {
	struct Pair {
		U first;
		V second;
	};

	Pair	   *data;
	std::size_t size;
	std::size_t capacity;

	std::size_t find(const U &key) const {
		for (std::size_t i = 0; i < size; ++i) {
			if (data[i].first == key) { return i; }
		}
		return -1u;
	}

	/// Тук е важно, да отбележим, че изискваме U, V да имат конструктори по подразбиране, за да 
	/// е валиден записа new Pair[new_capacity].
	/// Също, искаме копиращ оператор '='. 
	/// След две седмици ще видим как може да изискваме само конструктор по подразбиране и move-оператор'='.
	void resize() {
		std::size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
		Pair	   *new_data	 = new Pair[new_capacity];
		for (std::size_t i = 0; i < size; ++i) {
			new_data[i] = data[i];
		}
		delete[] data;
		data	 = new_data;
		capacity = new_capacity;
	}

	void append(const U &key, const V &value) {
		if (size == capacity) { resize(); }
		data[size++] = {key, value};
	}

   public:
	Function() : data(nullptr), size(0), capacity(0) {}
	~Function() { delete[] data; }
	Function(const Function &other) : data(new Pair[other.capacity]), size(other.size), capacity(other.capacity) {
		for (std::size_t i = 0; i < size; ++i) {
			data[i] = other.data[i];
		}
	}

	// тук има edge case, когато data = nullptr, но това е ок, защото тогава capacity = 0, и няма да се опитаме да копираме нищо.
	Function<U, V> &operator=(const Function &other) {
		if (this == &other) return *this;
		if (other.capacity > capacity) {
			Pair *new_data = new Pair[other.capacity];
			delete[] data;
			data	 = new_data;
			capacity = other.capacity;
		}
		size = other.size;
		for (std::size_t i = 0; i < size; ++i) {
			data[i] = other.data[i];
		}
		return *this;
	}

	void addPair(const U &key, const V &value) {
		std::size_t index = find(key);
		if (index != -1u) {
			data[index].second = value;
		} else {
			append(key, value);
		}
	}

	void addPair(const U &key, const Maybe<V> &value) {
		if (value.hasValue()) { addPair(key, *value); }
	}

	Maybe<V> getValue(const U &key) const {
		std::size_t index = find(key);
		if (index != -1u) {
			return Maybe<V>(data[index].second);
		} else {
			return Maybe<V>(); // nothing
		}
	}

	Maybe<V> getValue(const Maybe<U> &key) const {
		if (key.hasValue()) {
			return getValue(*key);
		} else {
			return Maybe<V>(); // nothing
		}
	}

	bool hasKey(const U &key) const { return find(key) != -1u; }

	// Тук не може да ползваме getValue, защото той връща Maybe<V>, съдържащо КОПИЕ на стойността, а ние искаме РЕФЕРЕНЦИЯ към нея, за да може да се променя.
	// Ако на някой му се чете повече, може да види какво е std::reference_wrapper.
	V &operator[](const U &key) const {
		std::size_t index = find(key);
		if (index != -1u) {
			return data[index].second;
		} else {
			exit(1);
		}
	}

	Maybe<V> operator()(const U &key) const { return getValue(key); }

	Function<U, V> &operator+=(const Function<U, V> &other) {
		for (std::size_t i = 0; i < other.size; ++i) {
			addPair(other.data[i].first, other.data[i].second);
		}
		return *this;
	}

	Function<U, V> operator+(const Function<U, V> &other) const {
		Function<U, V> result = *this;
		result += other;
		return result;
	}

	template <class A, class B, class C>
	friend Function<A, C> operator*(const Function<A, B> &f, const Function<B, C> &g) {
		Function<A, C> result;
		for (std::size_t i = 0; i < f.size; ++i) {
			const A &key_f = f.data[i].first;
			const B &val_f = f.data[i].second;
			// тук за да ми е удобно и да няма два вложени цикъла,
			// може addPair да приема Maybe<V>, и да добавя само ако има стойност, а ако няма, просто да не прави нищо.
			result.addPair(key_f, g.getValue(val_f));
		}
		return result;
	}
};
