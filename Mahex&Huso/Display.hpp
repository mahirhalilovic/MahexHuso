#include <Windows.h>

class Display {
    HWND m_hwnd;
    bool m_isFullscreen = false;
    RECT m_windowedRect = {0};
    int m_width = 800;
    int m_height = 600;

    public:
        Display(HWND);
        bool SetResolution(int, int);
        bool ToggleFullscreen();
};