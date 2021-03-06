#include "Test.h"

#include "imgui/imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& currentTestPointer) : currentTest(currentTestPointer) { }
	
	TestMenu::~TestMenu() {}

	void TestMenu::OnImGuiRender() {
		for(auto& test : tests) {
			if(ImGui::Button(test.first.c_str())) {
				currentTest = test.second();
			}
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
