#pragma once

#include <windows.h>
#include <string>

class Label {
    public:
    int x, y, width, height;
    std::wstring text;
    HFONT font = nullptr;
    COLORREF textColor = RGB(0, 0, 0);

    Label() = default;

    Label(int x, int y, int width, int height, const std::wstring &text)
        : x(x), y(y), width(width), height(height), text(text) {}

    ~Label() {
        if(font) DeleteObject(font);
    }

    void SetFont(const std::wstring &fontName, int fontSize, bool bold = false) {
        if(font) DeleteObject(font);

        font = CreateFont(
            fontSize, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, fontName.c_str()
        );
    }

    void SetPos(int posX, int posY) {
        x = posX;
        y = posY;
    }

    void SetText(const std::wstring &txt) {
        text.clear();
        text = std::move(txt);
    }

    void Render(HDC hdc) {
        int savedDC = SaveDC(hdc);

        RECT rect = {x, y, x + width, y + height};

        if(font) {
            SelectObject(hdc, font);
        }

        SetTextColor(hdc, textColor);
        SetBkMode(hdc, TRANSPARENT);

        DrawText(hdc, text.c_str(), text.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        RestoreDC(hdc, savedDC);
    }
};