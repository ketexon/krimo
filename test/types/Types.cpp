#define KRIMO_READONLY_PROPERTY_SILENTLY_FAIL

#include <iostream>
#include <krimo/types/Delegate.hpp>
#include <krimo/types/TypeID.hpp>
#include <krimo/types/Property.hpp>
#include <krimo/types/HashString.hpp>
#include <krimo/EventEmitter.hpp>

#include "../Test.hpp"

int TestFn(int x){
	return x;
}

int staticNoArgsNoReturnGlobal = 0;
void StaticNoArgsNoReturn(){
	staticNoArgsNoReturnGlobal = 10;
}

int testFn2Global;
void TestFn2(int x){
	testFn2Global = x;
}



struct Class {
	int x = 0;

	int member(int x) {
		return x;
	};

	void voidMember(int x){
		this->x = x;
	}
};

void FunctionsTest(){
	using namespace krimo;

	KRIMO_TEST_CASE("Functions");

	// calling a null function should throw
	{
		Function<int(int)> test;
		KRIMO_ASSERT_THROWS(test(0));
	}

	// static function wo args and return
	{
		Function<void()> test{StaticNoArgsNoReturn};
		test();
		KRIMO_ASSERT(staticNoArgsNoReturnGlobal == 10);
	}

	// static functions wo return
	{
		Function<void(int)> test{&TestFn2};
		test(12);
		KRIMO_ASSERT(testFn2Global == 12);
	}

	// static functions w return
	{
		Function<int(int)> test{&TestFn};
		KRIMO_ASSERT(test(5) == 5);
	}

	// member functions
	{
		Class instance;
		Function<int(int)> test{instance, &Class::member};
		KRIMO_ASSERT(test(5) == 5);
	}

	// member functions wo return
	{
		Class instance;
		Function<void(int)> test{instance, &Class::voidMember};
		test(13);
		KRIMO_ASSERT(instance.x == 13);
	}

	// lambda functions wo captures
	{
		Function<int(int)> test([](int x) { return x; });
		KRIMO_ASSERT(test(10) == 10);
	}

	// lambda functions w value captures
	{
		int x = 10;
		Function<int(int)> test([x](int y) { return x + y; });
		KRIMO_ASSERT(test(10) == 20);
	}

	// lambda functions w reference captures
	{
		int x = 10;
		Function<int(int)> test([&x](int y) { x = 20; return y; });
		KRIMO_ASSERT(test(10) == 10);
		KRIMO_ASSERT(x == 20);
	}

	KRIMO_TEST_CASE("Events");

	// empty event
	{
		Event<int> event;
		KRIMO_ASSERT_NOTHROW(event(10));
	}

	// event with no args
	{
		int x = 0;
		Action<> action = [&x]() { x = 10; };
		Event<> event;

		event += action;

		event();
		KRIMO_ASSERT(x == 10);
	}

	// two events
	{
		int x = 0;
		Action<int> action = [&x](int y) { x += y; };
		Event<int> event;

		event.Add(action);
		event += action;

		event(10);
		KRIMO_ASSERT(x == 20);
	}

	// order of events
	{
		int x = 0;
		Action<int> action1 = [&x](int y) {
			if(x == 0) x = 1;
		};
		Action<int> action2 = [&x](int y) {
			if(x == 0) x = 2;
		};
		Event<int> event;

		event += action1;
		event += action2;

		event(0);
		KRIMO_ASSERT(x == 1);
	}

	// removing events
	{
		int x = 0;
		Action<> action = [&x]() { x++; };

		Event<> event;

		event += action;
		event += action;
		event();
		KRIMO_ASSERT(x == 2);

		event -= action;
		event();
		KRIMO_ASSERT(x == 3);

		event -= action;
		event();
		KRIMO_ASSERT(x == 3);

		KRIMO_ASSERT_NOTHROW(event -= action);
		event();
		KRIMO_ASSERT(x == 3);
	}
}

void TypeIDTest(){
	using namespace krimo;

	KRIMO_TEST_CASE("TypeID");

	// type id should be the same for the same type
	{
		size_t a = TypeID<int>();
		size_t b = TypeID<int>();
		KRIMO_ASSERT(a == b);
	}

	// type id should be different for a different type
	{
		size_t a = TypeID<int>();
		size_t b = TypeID<float>();
		KRIMO_ASSERT(a != b);
	}

	// type id should decay
	{
		size_t v = TypeID<int>();
		size_t lv = TypeID<int&>();
		size_t cv = TypeID<const int>();
		size_t rv = TypeID<int&&>();
		size_t clv = TypeID<const int&>();
		size_t crv = TypeID<const int&&>();
		KRIMO_ASSERT(v == lv);
		KRIMO_ASSERT(v == rv);
		KRIMO_ASSERT(v == cv);
		KRIMO_ASSERT(v == clv);
		KRIMO_ASSERT(v == crv);

		size_t pv = TypeID<int*>();
		size_t av = TypeID<int[2]>();
		KRIMO_ASSERT(pv == av);
	}
}

void PropertyTest(){
	using namespace krimo;

	KRIMO_TEST_CASE("Property");

	// basic properties
	{
		int _prop = 10;
		Property<int> prop{_prop};

		KRIMO_ASSERT(prop == 10);

		_prop = 11;
		KRIMO_ASSERT(prop == 11);

		prop = 12;
		KRIMO_ASSERT(prop == 12);
	}

	// should do compile time failure if KRIMO_READONLY_PROPERTY_SILENTLY_FAIL is not set
	{
		int _prop = 10;
		ReadonlyProperty<int> prop{_prop};

		KRIMO_ASSERT(prop == 10);
		prop = 11;

		KRIMO_ASSERT(prop == 10);
	}
}


consteval uint32_t HashStringAssertConstexpr(){
	using namespace krimo;
	return "hello"_khs;
}

void HashStringTest(){
	using namespace krimo;

	KRIMO_TEST_CASE("HashString");

	// CXString should work like string
	{
		KRIMO_ASSERT(CXString("").Size() == 0);
		KRIMO_ASSERT(CXString("hello").Size() == 5);
	}

	// should work in consteval
	{
		HashStringAssertConstexpr();
	}

	// should work same at runtime
	{
		KRIMO_ASSERT(HashString<"hello">() == HashString("hello"));
	}
}

void EventEmitterTest(){
	using namespace krimo;

	KRIMO_TEST_CASE("EventEmitter");

	struct TestEvent {
		int val;
	};

	// Should work when no listeners
	{
		EventEmitter<TestEvent> test;

		test.Emit<"hello">(TestEvent{ .val = 10 });
		test.Emit("goodbye", TestEvent{ .val = 20 });
	}

	// should call event listeners
	{
		EventEmitter<TestEvent> test;
		int v = 0;
		test.On<"hello">([&](const TestEvent& e) { v += e.val; });

		test.Emit<"hello">(TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 10);

		test.Emit<"hello">(TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 20);
	}

	// should work for template and parameter event names
	{
		EventEmitter<TestEvent> test;

		int v = 0;
		int w = 0;
		int x = 0;
		test.On<"hello">([&](const TestEvent& e) { v += 1; });
		test.On("hello", [&](const TestEvent& e) { w += 10; });
		test.On("hello"_khs, [&](const TestEvent& e) { x += 100; });

		test.Emit<"hello">(TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 1);
		KRIMO_ASSERT(w == 10);
		KRIMO_ASSERT(x == 100);

		test.Emit("hello", TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 2);
		KRIMO_ASSERT(w == 20);
		KRIMO_ASSERT(x == 200);

		test.Emit("hello"_khs, TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 3);
		KRIMO_ASSERT(w == 30);
		KRIMO_ASSERT(x == 300);
	}

	// should call event listeners in order
	{
		EventEmitter<TestEvent> test;
		int v = 0;
		test.On<"hello">([&](const TestEvent& e) { v = 1; });
		test.On<"hello">([&](const TestEvent& e) { v = 2; });

		test.Emit<"hello">(TestEvent{ .val = 10 });

		KRIMO_ASSERT(v == 2);
	}
}

int main(){
	FunctionsTest();
	TypeIDTest();
	PropertyTest();
	HashStringTest();
	EventEmitterTest();
	KRIMO_TESTS_PASSED();
}