#include "enigma_gui.h"

double indexOfCoincedence(const std::string& input)
{
	std::vector<int> table(26, 0);
	int numberOfCharacters{};
	double result{}, appearancePercentage{};

	for (char c : input)
	{
		if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		{
			c = ('a' <= c && c <= 'z') ? c - 'a' : c - 'A';
			table[c]++;
			numberOfCharacters++;
		}
	}

	for (uint8_t i = 0; i < 26; i++)
	{
		appearancePercentage = table[i] * 1.0 / numberOfCharacters;
		result += appearancePercentage * appearancePercentage;
	}

	return result;
}

int main() {

	GLFWwindow* mainWindow = init();
	if (!mainWindow)
	{
		return EXIT_FAILURE;
	}

	Texture logo("Logo.png");

	size_t textBufferSize = 512;
	char* inputText = new char[textBufferSize]{};
	char* outputText = new char[textBufferSize + (textBufferSize / 5 + 1)] {};

	char plugboard[13 * 3] = {};

	std::string reflectorKey = "Swiss K UKW - K";
	std::vector<std::string> rotorKeys(Enigma_machine::getInstance().getRotorCount(), "Commercial Enigma IC");
	std::vector<int> offsets(Enigma_machine::getInstance().getRotorCount(), 0);
	int numberOfRotors = Enigma_machine::getInstance().getRotorCount();

	int frameW, frameH;
	ImVec4 clearColor = ImVec4(.4f, .5f, .5f, 1.0f);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwGetFramebufferSize(mainWindow, &frameW, &frameH);
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// input window
		ImGui::SetNextWindowPos(ImVec2(frameW / 3.0f * 0, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(frameW / 3.0f, frameH));
		inputTextWindow(inputText, textBufferSize);

		// configuration window
		ImGui::SetNextWindowPos(ImVec2(frameW / 3.0f * 1, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(frameW / 3.0f, frameH));
		configurationWindow(plugboard, inputText, outputText, reflectorKey, rotorKeys, offsets, numberOfRotors, logo);
		
		// output window
		ImGui::SetNextWindowPos(ImVec2(frameW / 3.0f * 2, 0.0f));
		ImGui::SetNextWindowSize(ImVec2(frameW / 3.0f, frameH));
		outputTextWindow(outputText, textBufferSize);

		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(mainWindow);
	}

	delete[] inputText;
	delete[] outputText;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return EXIT_SUCCESS;
}


