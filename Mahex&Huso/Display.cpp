#include "Display.hpp"

Display::Display(HWND hwnd) : m_hwnd(hwnd) {
    GetWindowRect(hwnd, &m_windowedRect);
}

bool Display::SetResolution(int width, int height) {
    m_width = width;
    m_height = height;

    RECT rect = {0, 0, width, height};
    DWORD style = m_isFullscreen ? (WS_POPUP | WS_VISIBLE) : (WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    AdjustWindowRect(&rect, style, FALSE);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenW - windowWidth) / 2;
    int y = (screenH - windowHeight) / 2;

    if(m_isFullscreen) {
        SetWindowPos(m_hwnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_FRAMECHANGED);
    } else {
        SetWindowPos(m_hwnd, HWND_TOP, x, y, windowWidth, windowHeight, SWP_FRAMECHANGED);
    }

    return true;
}

bool Display::ToggleFullscreen() {
    m_isFullscreen = !m_isFullscreen;

    if(m_isFullscreen) {
        GetWindowRect(m_hwnd, &m_windowedRect);

        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

        MONITORINFO mi = {sizeof(mi)};
        GetMonitorInfo(MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
        SetWindowPos(m_hwnd, HWND_TOP,
            mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_FRAMECHANGED);
    } else {
        SetWindowLongPtr(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);

        SetWindowPos(m_hwnd, HWND_TOP,
            m_windowedRect.left, m_windowedRect.top,
            m_windowedRect.right - m_windowedRect.left,
            m_windowedRect.bottom - m_windowedRect.top,
            SWP_FRAMECHANGED);
    }

    SetResolution(m_width, m_height);
    return true;
}