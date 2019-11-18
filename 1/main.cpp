#include <iostream>

class CallableBase
{
public:
	CallableBase() = default;

	virtual void  operator()()
	{
		
	}
};

template <typename T>
class Callable : public CallableBase
{
public:
	Callable() = default;

	template <typename F>
	Callable(F f) : functor(f)
	{
	}

	void operator()() override
	{
		functor();
	}

private:
	T functor;
};

class Function
{
public:
	template <typename T>
	Function(T f)
	{
		m_callable = new Callable<T>(f);
	}

	void operator()()
	{
		m_callable->operator()();
	}

private:
	CallableBase* m_callable;
};

//	Given code, write Function with 98 std
void func(Function f)
{
	f();
}

int main()
{
	auto f = [](){ std::cout << "Called" << std::endl; };
	func(f);
}
