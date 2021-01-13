#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <vector>

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test {
	private:
		Test*& currentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> tests;
		
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name) {
			std::cout << "Registering test " << name << std::endl;

			tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};
}
