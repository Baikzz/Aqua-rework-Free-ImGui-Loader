#include "main.h"
#include "fonts.h"
#include "images.h"
#include "particles.h"

#define SNOW_LIMIT 50
#define M_P3       3.14159265358979323846   // pi

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool change_alpha = false;
static float menu_alpha = 1.f;

ID3D11ShaderResourceView* logo = nullptr;
ID3D11ShaderResourceView* logo_blur = nullptr;
ID3D11ShaderResourceView* arrow = nullptr;
ID3D11ShaderResourceView* minimize = nullptr;
ID3D11ShaderResourceView* close = nullptr;

ID3D11ShaderResourceView* twitter = nullptr;
ID3D11ShaderResourceView* youtube = nullptr;
ID3D11ShaderResourceView* web = nullptr;
ID3D11ShaderResourceView* disc = nullptr;

ImFont* InterSemi = nullptr;
ImFont* InterBold = nullptr;

HWND hwnd;
RECT rc;

// Main code
int main(int, char**)
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    hwnd = CreateWindowExW(NULL, wc.lpszClassName, L"Onevade Loader", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (HEIGHT / 2), WIDTH + 2, HEIGHT + 2, 0, 0, 0, 0);

    SetWindowLongA(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);


    POINT mouse;
    rc = { 0 };
    GetWindowRect(hwnd, &rc);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 0.f);

    custom::style();

    InterSemi = io.Fonts->AddFontFromMemoryTTF(Inter_Semibold_m, sizeof(Inter_Semibold_m), 17);
    InterBold = io.Fonts->AddFontFromMemoryTTF(Inter_Bold_m, sizeof(Inter_Bold_m), 25);

    HRESULT LOGO_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo_p, sizeof(logo_p), nullptr, nullptr, &logo, nullptr);
    HRESULT LOGO_BLUR_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo_blur_p, sizeof(logo_blur_p), nullptr, nullptr, &logo_blur, nullptr);
    HRESULT ARROW_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, arrow_p, sizeof(arrow_p), nullptr, nullptr, &arrow, nullptr);
    HRESULT MINIMIZE_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, minimize_p, sizeof(minimize_p), nullptr, nullptr, &minimize, nullptr);
    HRESULT CLOSE_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, close_p, sizeof(close_p), nullptr, nullptr, &close, nullptr);

    HRESULT TWITTER_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, twitter_p, sizeof(twitter_p), nullptr, nullptr, &twitter, nullptr);
    HRESULT YT_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, yt_p, sizeof(yt_p), nullptr, nullptr, &youtube, nullptr);
    HRESULT WEB_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, web_p, sizeof(web_p), nullptr, nullptr, &web, nullptr);
    HRESULT DC_HR = D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, discord_p, sizeof(discord_p), nullptr, nullptr, &disc, nullptr);

    InitializeParticles();

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (change_alpha)
        {

            static DWORD dwTickStart = GetTickCount();
            if (GetTickCount() - dwTickStart > 1500)
            {
                change_alpha = false;
                dwTickStart = GetTickCount();
            }
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        gui::begin("Main");
        {
            float deltaTime = 1.0f;
            UpdateParticles(0.01);
            RenderParticles();

            gui::Blur(hwnd);
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            static float logoPos = -10;

            gui::setpos(385, 15);
            ImGui::Image(close, ImVec2(20, 20));
            if (ImGui::IsItemClicked(0))
                exit(0);

            gui::setpos(360, 15);
            ImGui::Image(minimize, ImVec2(19, 19));
            if (ImGui::IsItemClicked(0))
                ShowWindow(hwnd, SW_MINIMIZE);

            gui::setpos(140, logoPos);
            ImGui::Image(logo, ImVec2(145, 145));
            gui::setpos(105, logoPos + 10);
            ImGui::Image(logo_blur, ImVec2(180, 180));


            gui::pf(InterSemi);

            if (TAB == 0) 
            {
                logoPos = ImLerp(logoPos, 17.f, ImGui::GetIO().DeltaTime * (10 / 2));

                custom::input("###Username", "Username", username, 20, 62, logoPos + 170, 300, 0);
                custom::input("###Password", "Password", password, 20, 62, logoPos + 230, 300, ImGuiInputTextFlags_Password);

                if (custom::multicolorButton("fe", "Login", 62, logoPos + 350, 300, 40, convToInt(mainColor), convToInt(secondColor), convToInt(secondColor), convToInt(mainColor)))
                    TAB = 1;
            }

            if (TAB == 1) 
            {
                progress = 0;
                logoPos = ImLerp(logoPos, -200.f, ImGui::GetIO().DeltaTime * (10 / 2));
                if (logoPos < -170)
                {
                    std::string products[] = { "p1", "p2", "p3", "p4" };
                    ID3D11ShaderResourceView* social[] = { twitter, youtube, web, disc };

                    cumbola = ImLerp(cumbola, 80.f, ImGui::GetIO().DeltaTime * (10 / 2));

                    custom::text("Aqua.xyz", 25, cumbola - 65, InterSemi, 17, white);
                    draw_list->AddRectFilledMultiColor({ 0, cumbola - 30 }, { 425, cumbola - 28 }, convToInt(secondColor), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), convToInt(secondColor));
                    for (int i = 0; i < 4; i++) {
                        gui::child(products[i].c_str(), 22.5, cumbola + 80 * i, 380, 65);
                        {
                            gui::setpos(2.5, 0.5);
                            ImGui::Image(logo, ImVec2(65, 65));

                            if (custom::multicolorButton(products[i].c_str(), "", 327.5, 10, 40, 40, convToInt(mainColor), convToInt(secondColor), convToInt(secondColor), convToInt(mainColor)))
                                TAB = 2;

                            gui::setpos(340, 23);
                            ImGui::Image(arrow, ImVec2(15, 15));
                        }
                        ImGui::EndChild();

                        gui::setpos(135 + 45 * i, cumbola + 350);
                        ImGui::Image(social[i], ImVec2(25, 25));
                    }
                }
            }

            if (TAB == 2)
            {
                cumbola = 500.f;
                logoPos = ImLerp(logoPos, 160.f, ImGui::GetIO().DeltaTime * (10 / 4));
                if (logoPos > 155)
                    progress += ImGui::GetIO().DeltaTime;

                if (progress >= 3.0f) {
                    TAB = 1;
                }
            }
        }
        ImGui::End();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0);
    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}