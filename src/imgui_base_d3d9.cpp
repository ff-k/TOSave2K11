#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <tchar.h>

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != 0 && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

typedef struct {
    HWND windowHandle;
    bool quitRequested;
} frame_params;

// NOTE(furkan): Note that this won't work if multiple 
//               compilation units are being used. Change 
//               "static" to "extern" in that case.
static void FrameUpdate(frame_params *frameParams);

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CmdLine, int ShowCmd) {
    WNDCLASSEXA windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.lpszClassName = "TOSave2K11";
    windowClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    
    int windowWidth  = 960;
    int windowHeight = 480;
    
    int monitorW = GetSystemMetrics(SM_CXSCREEN);
    int monitorH = GetSystemMetrics(SM_CYSCREEN);
        
    if (RegisterClassExA(&windowClass)){
        HWND windowHandle = CreateWindowEx(0, 
                                           windowClass.lpszClassName,
                                           "TOSave2K11", 
                                           WS_POPUP | WS_VISIBLE, 
                                           (monitorW-windowWidth)/2, (monitorH-windowHeight)/2, 
                                           windowWidth, windowHeight, 
                                           0, 0, windowClass.hInstance, 0);
        if(windowHandle){
            if (CreateDeviceD3D(windowHandle)){
                ShowWindow(windowHandle, SW_SHOW);
                SetWindowPos(windowHandle, HWND_TOP, 0, 0, 0, 0, 
                             SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
                UpdateWindow(windowHandle);
                
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                
                ImGui::StyleColorsDark();
                
                ImGui_ImplWin32_Init(windowHandle);
                ImGui_ImplDX9_Init(g_pd3dDevice);
                
                frame_params frameParams;
                frameParams.windowHandle = windowHandle;
                frameParams.quitRequested = false;
                
                MSG message;
                ZeroMemory(&message, sizeof(message));
                while (!frameParams.quitRequested){
                    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE) > 0){
                        TranslateMessage(&message);
                        DispatchMessage(&message);
                        
                        if(message.message == WM_QUIT){
                            frameParams.quitRequested = true;
                        }
                    }
            
                    ImGui_ImplDX9_NewFrame();
                    ImGui_ImplWin32_NewFrame();
                    ImGui::NewFrame();
                    
                    FrameUpdate(&frameParams);
                    
                    ImGui::EndFrame();
                    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
                    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
                    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0, 0, 0, 255);
                    g_pd3dDevice->Clear(0, 0, 
                                        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
                                        clear_col_dx, 1.0f, 0);
                    if (g_pd3dDevice->BeginScene() >= 0) {
                        ImGui::Render();
                        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
                        g_pd3dDevice->EndScene();
                    }
                    
                    SetWindowRgn(windowHandle, 
                                 CreateRoundRectRgn(0, 0, windowWidth, windowHeight, 5, 5), 
                                 TRUE);
                    
                    HRESULT result = g_pd3dDevice->Present(0, 0, 0, 0);
                    if (result == D3DERR_DEVICELOST && 
                        g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET){
                        ResetDevice();
                    }
                }
            
                ImGui_ImplDX9_Shutdown();
                ImGui_ImplWin32_Shutdown();
                ImGui::DestroyContext();
                
                CleanupDeviceD3D();
            }
            
            DestroyWindow(windowHandle);
            
        }
        
        UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
    }

    return 0;
}

int main(int argCount, char **args) {
    return WinMain(GetModuleHandle(0), 0, "", 0);
}
