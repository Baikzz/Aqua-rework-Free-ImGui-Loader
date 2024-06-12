#pragma once
#pragma comment(lib, "D3DX11.lib")
#define IMGUI_DEFINE_MATH_OPERATORS

#include <d3d11.h>
#include <dwmapi.h>
#include <tchar.h>
#include <windows.h>
#include <D3DX11tex.h>
#include <tchar.h>
#include <map>
#include <chrono>
#include <cmath>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

extern ImColor bg; // Main window color bg
extern ImColor cbg; // Childs color background
extern ImColor mainColor; // Used on buttons & items
extern ImColor secondColor; // Used on buttons when are clicked etc...
extern ImColor white; // General colors
extern ImColor gray;
extern ImColor lblack;

extern ImColor convToInt(ImColor color);

extern HWND hwnd;
extern RECT rc;

#define WIDTH 425
#define HEIGHT 480

static char username[20] = "";
static char password[20] = "";
static char license[20] = "";

static int TAB;
static int subTAB;

static float progress = 0;
static float cumbola = 500;

namespace gui {
	void pf(ImFont* font);
	void begin(const char* name);
	void setpos(float posX, float posY);
	void child(const char* name, float posX, float posY, float sizeX, float sizeY);
	void Blur(HWND hwnd);
	void cend();
	void mw();
}

namespace custom {
	bool button(const char* name, float posX, float posY, float sizeX, float sizeY);
	bool input(const char* name, const char* hint, char buf[], size_t buf_size, float posX, float posY, float width, ImGuiInputTextFlags womp);
	void text(const char* text, float posX, float posY, ImFont* font, float fontSize, ImColor color);
	bool imageButton(ID3D11ShaderResourceView* ptr, float posX, float posY, bool active);
	void progressbar(float ptr, float posX, float posY, float sizeX, float sizeY);
	bool multicolorButton(const char* label, const char* text, float posX, float posY, float width, float height, ImColor c1, ImColor c2, ImColor c3, ImColor c4);
	void style();
}