#include "enigma_gui.h"
#include "enigma_global.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int WIDTH = 960;
int HEIGHT = 480;

Texture::Texture(const char* path) 
{
	id = 0;
	width = 0, height = 0, channels = 0;
	void* ptr = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
	if (ptr)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr);

		stbi_image_free(ptr);
	}
	else
	{
		std::cerr << "ERROR::STB::Failed to load texture: " << path << std::endl;
	}
}

GLFWwindow* init()
{
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit())
	{
		std::cerr << "ERROR::GLFW:Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Enigma", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "ERROR::GLFW::Failed to create window" << std::endl;
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();


	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	return window;
}

void glfwResizeWindowCallback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glfwSetWindowSize(window, WIDTH, HEIGHT);
}

void glfwErrorCallback(int error, const char* descrition)
{
	fprintf(stderr, "ERROR::GLFW::%d:%s", error, descrition);
}

void inputTextWindow(char* inputText, size_t& textBufferSize)
{
	ImGui::Begin("Input", nullptr,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDecoration);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Plaintext");

	ImGui::InputTextMultiline(
		"Plaintext", inputText, textBufferSize,
		ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24),
		ImGuiInputTextFlags_AllowTabInput);

	ImGui::End();
}

void outputTextWindow(char* outputText, size_t& textBufferSize)
{
	ImGui::Begin("Output", nullptr,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDecoration);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Ciphertext");

	ImGui::InputTextMultiline(
		"Ciphertext", outputText, textBufferSize + (textBufferSize / 5 + 1),
		ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 24),
		ImGuiInputTextFlags_ReadOnly);

	ImGui::End();
}

void configurationWindow(
	char* plugboard,
	char* inputText,
	char* outputText,
	std::string& reflectorKey,
	std::vector<std::string>& rotorKeys,
	std::vector<int>& offsets,
	int& numberOfRotors,
	Texture& logo
)
{
	ImGui::Begin("Enigma Machine", nullptr,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoDecoration);

	ImGui::SameLine(ImGui::GetWindowWidth() / 2.0f - logo.width / 4.0f);
	ImGui::Image((void*)(intptr_t)logo.id, ImVec2(logo.width * .5f, logo.height * .5f));

	ImGui::SeparatorText("Configuration");

	ImGui::SetNextItemWidth(75.0f);
	ImGui::InputInt("Number of rotors", &numberOfRotors);
	Enigma_machine::getInstance().setRotorCount((uint8_t)numberOfRotors);
	rotorKeys.resize(Enigma_machine::getInstance().getRotorCount(), ROTOR_LIST.begin()->first);
	offsets.resize(Enigma_machine::getInstance().getRotorCount());

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// setting reflector key
	ImGui::Text(("Reflector: " + reflectorKey).c_str());
	if (ImGui::Button("Select reflector"))
		ImGui::OpenPopup("Reflector list");
	if (ImGui::BeginPopup("Reflector list"))
	{
		ImGui::SeparatorText("Reflector List");
		for (auto [name, hash] : REFLECTOR_LIST)
			if (ImGui::Selectable(name.c_str()))
				reflectorKey = name;
		ImGui::EndPopup();
	}

	ImGui::Spacing();
	ImGui::Separator();

	// setting rotor keys
	for (size_t i = 0; i < Enigma_machine::getInstance().getRotorCount(); i++)
	{
		ImGui::Spacing();
		ImGui::Text(("Rotor %d: " + rotorKeys[i]).c_str(), i + 1);
		if (ImGui::Button(("Select rotor " + std::to_string(i + 1)).c_str()))
			ImGui::OpenPopup(("Rotor List " + std::to_string(i + 1)).c_str());
		if (ImGui::BeginPopup(("Rotor List " + std::to_string(i + 1)).c_str()))
		{
			ImGui::SeparatorText("Rotor List");
			for (auto [name, hash] : ROTOR_LIST)
				if (ImGui::Selectable((name).c_str()))
					rotorKeys[i] = name;
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(75.0f);
		ImGui::InputInt(("Offset " + std::to_string(i + 1)).c_str(), &offsets[i]);

		ImGui::Spacing();
		ImGui::Separator();
	}

	ImGui::Spacing();

	ImGui::InputText("Plugboard", plugboard, 13 * 3);

	ImGui::Spacing();
	ImGui::Separator();

	if (ImGui::Button("Encode"))
	{
		// applying enigma settings

		Enigma_machine::getInstance().setRotorCount(numberOfRotors);

		std::vector<std::pair<std::string, std::string>> rotors(numberOfRotors);
		for (size_t i = 0; i < Enigma_machine::getInstance().getRotorCount(); i++)
			rotors[i] = { ROTOR_LIST[rotorKeys[i]].substr(0, 26), ROTOR_LIST[rotorKeys[i]].substr(27, 2) };
		Enigma_machine::getInstance().setRotors(rotors);

		Enigma_machine::getInstance().setOffsets(offsets);

		Enigma_machine::getInstance().setReflector(REFLECTOR_LIST[reflectorKey]);

		Enigma_machine::getInstance().setPlugboard();

		for (size_t i = 0; i < strlen(plugboard); i++)
		{
			if (std::isalpha(plugboard[i]) && std::isalpha(plugboard[i + 1]))
			{
				std::string plugboardPair;
				plugboardPair.push_back(plugboard[i]);
				plugboardPair.push_back(plugboard[i + 1]);
				Enigma_machine::getInstance().addPair(plugboardPair);
				i += 2;
				continue;
			}
		}

		//Enigma_machine::getInstance().print(); // used for debugging

		// encoding and formating resulting string

		std::string out = Enigma_machine::getInstance().encode(inputText);
		int maxChars = std::floor(ImGui::GetWindowContentRegionMax().x / 7.4);
		for (size_t i = 5; i < out.size(); i += 5)
		{
			if (((i + 6) % maxChars) < 6)
				out.insert(out.begin() + i, '\n');
			else
				out.insert(out.begin() + i, ' ');
			i++;
		}
		memcpy(outputText, &out[0], sizeof(char) * (out.size() + 1));
	}

	ImGui::End();
}