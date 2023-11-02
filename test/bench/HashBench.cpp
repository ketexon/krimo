#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <ranges>

#include <krimo/types/HashString.hpp>

template<class F, class... Args>
std::chrono::duration<double> Bench(F f, Args... args){
	auto start = std::chrono::high_resolution_clock::now();
	f(args...);
	auto end = std::chrono::high_resolution_clock::now();
	return end - start;
}

template<class F>
auto DoTest(F f, int n, int minCharsInString = 10, int maxCharsInString = 100){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> sizeDistrib(minCharsInString, maxCharsInString);
	std::uniform_int_distribution<> charDistrib('a', 'z');

	auto strings
		= std::ranges::iota_view{0, n}
		| std::ranges::views::transform([&](auto _) {
			auto chars = std::ranges::iota_view{0, sizeDistrib(gen)}
				| std::ranges::views::transform([&](auto _) { return charDistrib(gen); });
				return std::string{chars.begin(), chars.end()};
		});

	return Bench(f, strings);
}

int main(){
	constexpr int minCharsInString = 10;
	constexpr int maxCharsInString = 10000;

	size_t maxHash = 0;

	auto stdHashBench = [&](auto strings){
		for(const auto& str : strings){
			const auto hash = std::hash<std::string>{}(str);
			maxHash = std::max(hash, maxHash);
		}
	};

	auto hashStringBench = [&](auto strings){
		for(const auto& str : strings){
			const auto hash = krimo::HashString(str);
			maxHash = std::max(static_cast<size_t>(hash), maxHash);
		}
	};

	std::vector<int> ns {
		10,
		100,
		1000,
		10000,
		100000,
	};
	auto stdHashRes = std::views::all(ns) | std::views::transform([&](int n) { return DoTest(stdHashBench, n, minCharsInString, maxCharsInString); });
	auto hashStringRes = std::views::all(ns) | std::views::transform([&](int n) { return DoTest(hashStringBench, n, minCharsInString, maxCharsInString); });

	std::cout << "Min chars in string: " << minCharsInString << "\nMax chars in string: " << maxCharsInString << '\n';

	std::cout << std::left << std::setw(16) << "n";
	for(int n : ns){
		std::cout << std::setw(16) << n;
	}
	std::cout << std::endl;

	std::cout << std::left << std::setw(16) << "std::hash";
	for(auto res : stdHashRes){
		std::cout << std::setw(16) << res << std::flush;
	}
	std::cout << std::endl;

	std::cout << std::left << std::setw(16) << "HashString";
	for(auto res : hashStringRes){
		std::cout << std::setw(16) << res << std::flush;
	}
	std::cout << std::endl;

	// auto

	// std::cout
	// 	<< std::left
	// 	<< std::setw(16) << "std::hash" << stdHashBenchResults << '\n'
	// 	<< std::setw(16) << "HashString" << hashStringBenchResults << std::endl;

	return 0;
}