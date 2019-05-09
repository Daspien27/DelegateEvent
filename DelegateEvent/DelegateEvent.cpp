// DelegateEvent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

//public class MyClass
//{
//	public delegate bool MyDel();
//	public event MyDel MyEvent;
//
//	public MyClass()
//	{
//		MyEvent += foo1;
//		MyEvent += foo2;
//	}
//
//	public bool InvokeEvent()
//	{
//		return MyEvent ? .Invoke();
//	}
//
//	public bool foo1() { return true; }
//	public bool foo2() { return false; }
//}
//
//int Main()
//{
//	MyClass classObject = new MyClass();
//	printf(classObject.InvokeEvent());
//}

template <typename T> struct FuncExtractor;

template <typename Return, typename... Params>
struct FuncExtractor< Return (*)(Params...)>
{
	using ReturnType = typename Return;
//	using ParamsType = typename Params...; //Can we do this?
};

template <typename DelegateSignature>
struct Event
{
	using ReturnType = typename FuncExtractor<DelegateSignature>::ReturnType;
	std::vector<DelegateSignature> MyList;

	Event& operator += (const DelegateSignature& NewRegister)
	{
		MyList.push_back(NewRegister);

		return *this;
	}

	template <typename... Args>
	ReturnType Invoke(Args&&... args)
	{
		if (MyList.empty())
		{
			throw std::bad_function_call(); //Could use a template parameter to define default behaviours here, i.e. returning a default initialize value instead.
		}

		std::for_each(MyList.begin(), std::next(MyList.begin(), MyList.size() - 1), [&](const auto& Func)
		{
			Func(args...);
		});

		return MyList.back()(args...);
	}
};


struct MyClass
{
	using Delegate = bool(*)();

	Event<Delegate> MyEvent;

	MyClass()
	{
		MyEvent += alwaysTrue;
		MyEvent += alwaysFalse;
	}

	bool InvokeEvent()
	{
		return MyEvent.Invoke();
	}

	static bool alwaysTrue()
	{
		return true;
	}

	static bool alwaysFalse()
	{
		return false;
	}
};

int main()
{
	MyClass classObject;
	std::cout << std::boolalpha << classObject.InvokeEvent();
}