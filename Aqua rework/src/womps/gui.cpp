#include "main.h"

DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse;

void gui::begin(const char* name) {
	ImGui::Begin(name, NULL, window_flags);
	ImGui::SetWindowSize({ WIDTH, HEIGHT });

    mw();
}

void gui::pf(ImFont * font) {
    ImGui::PushFont(font);
}

void gui::mw() {
    GetWindowRect(hwnd, &rc);

    if (ImGui::GetWindowPos().x != 0 || ImGui::GetWindowPos().y != 0)
    {
        MoveWindow(hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, WIDTH, HEIGHT, TRUE);
        ImGui::SetWindowPos(ImVec2(0.f, 0.f));
    }
}

void gui::setpos(float posX, float posY) {
    ImGui::SetCursorPos({ posX, posY });
}

void gui::child(const char* name, float posX, float posY, float sizeX, float sizeY) {
    ImGui::SetCursorPos({ posX, posY });
    ImGui::BeginChild(name, { sizeX, sizeY });
}

void gui::cend() {
    ImGui::EndChild();
}

void gui::Blur(HWND hwnd) {
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 };

            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}