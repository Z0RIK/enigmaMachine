#pragma once

#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <unordered_set>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "enigma_machine.h"

struct Texture {
	GLuint id;
	int width, height, channels;

	Texture(const char*);
};

GLFWwindow* init();

void glfwResizeWindowCallback(GLFWwindow* window, int width, int height);
void glfwErrorCallback(int error, const char* descrition);

void inputTextWindow(char* inputText, size_t& textBufferSize);
void outputTextWindow(char* outputText, size_t& textBufferSize);
void configurationWindow(
	char* plugboard,
	char* inputText,
	char* outputText,
	std::string& reflectorKey,
	std::vector<std::string>& rotorKeys,
	std::vector<int>& offsets,
	int& numberOfRotors,
	Texture& logo
);

#endif // !GUI_H
