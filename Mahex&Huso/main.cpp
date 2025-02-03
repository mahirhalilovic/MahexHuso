// Game is inspired by the game named Fireboy and Watergirl. In this game there are two players whose goal
// is to complete levels through working together. Characters are controlled via WASD and arrow keys. 
// Players are met with multiple obstacles and they need to work together to finish a level. It is planned
// for the game to have multiple levels as well as achievements.


// ==== TASKS ====

// [+] Implement basic game loop
// [+] Create movement mechanics for characters
// [+] Implement gravity for jumping
// [+] Implement stationary tiles
// [+] Implement collisions
// [+] Implement sprites
// [+] Draw character sprites and tiles
// [+] Create testing level
// [+] Prevent movement outside of borders
// [+] Design basic game level to test out collisions and movement
// [-] Add cooperative mechanics(other player)
// [-] Implement end goals
// [-] Add obstacles
// [+] Level restarting
// [-] Implement score mechanics
// [+] Add main menu
// [-] Add sound effects and music
// [-] Implement custom level creator
// [-] Implement custom level saving, making own files
// [-] Implement achievements
// [-] Unlocking new skins
// [-] Skin selection in main menu
// [-] Add everything to a class
// [-] Spatial partitioning, quadtrees, multithreading?
// [-] Non-rectangular shapes, SAT, bounding boxes, penetration depth?

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include "headers/Game.hpp"

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = _T("Mahex&Huso");
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if(!RegisterClassEx(&wincl)) return 0;

    hwnd = CreateWindowEx(
        0,
        _T("Mahex&Huso"),                                           // Classname
        _T("Mahex&Huso"),                                           // Title Text
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,   // Non-resizable window
        152,                                                        // Center the window assuming
        50,                                                         // screen size is 1200x720
        WINDOW_WIDTH+16,                                            // Window width
        WINDOW_HEIGHT+39,                                           // and height considering border and ribbon
        HWND_DESKTOP,                                               // The window is a child-window to desktop
        NULL,                                                       // No menu
        hThisInstance,                                              // Program Instance handler
        NULL                                                        // No Window Creation data
    );

    ShowWindow(hwnd, nCmdShow);

    Game game(hwnd);

    DWORD previousTime = GetTickCount();
    DWORD currentTime;
    DWORD deltaTime;

    while(true) {
        if(PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) {
            if(messages.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }

        currentTime = GetTickCount();
        deltaTime = currentTime - previousTime;

        if(deltaTime >= 1000 / FPS_MAX) {
            game.Update();
            game.Render();

            previousTime = currentTime;
        } else {
            Sleep(1);
        }
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}