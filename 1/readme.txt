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