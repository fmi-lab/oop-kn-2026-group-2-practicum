#pragma once

#include <functional>
#include <vector>

template <class U, class V>
class LazyMap
{
   public:
	LazyMap(const std::vector<U> &source, std::function<V(U)> f)
		: source(source), f(f)
	{
	}

	class iterator
	{
		typename std::vector<U>::const_iterator it;
		const std::function<V(U)>			   &f;

	   public:
		iterator(typename std::vector<U>::const_iterator it,
				 const std::function<V(U)>				&f)
			: it(it), f(f)
		{
		}

		V operator*() const { return f(*it); }
		// V *operator->() const {return &f(*it); }
		bool operator!=(const iterator &other) const { return it != other.it; }
		bool operator==(const iterator &other) const { return it == other.it; }

		iterator &operator++()
		{
			++it;
			return *this;
		}
		iterator operator++(int)
		{
			iterator old = *this;
			operator++();
			return old;
		}
	};

	iterator begin() const { return iterator(source.begin(), f); }

	iterator end() const { return iterator(source.end(), f); }

   private:
	const std::vector<U> &source;
	std::function<V(U)>	  f;
};
