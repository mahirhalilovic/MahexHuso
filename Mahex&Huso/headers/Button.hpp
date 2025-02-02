#include <windows.h>
#include <string>

class Button {
    public:
    int x, y, width, height;
    std::wstring text;
    bool hovered = false;
    HFONT font = nullptr;
    COLORREF textColor = RGB(0, 0, 0);
    COLORREF bgColor = RGB(150, 150, 150);
    COLORREF hoverBgColor = RGB(200, 200, 200);

    Button() = default;

    Button(int x, int y, int width, int height, const std::wstring& text)
        : x(x), y(y), width(width), height(height), text(text) {}

    ~Button() {
        if(font) DeleteObject(font);
    }

    void SetFont(const std::wstring& fontName, int fontSize, bool bold = false) {
        if(font) DeleteObject(font);

        font = CreateFont(
            fontSize, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL,
            FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, fontName.c_str()
        );
    }

    bool IsMouseOver(int mouseX, int mouseY) {
        return mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height;
    }

    void Render(HDC hdc) {
        int savedDC = SaveDC(hdc);

        HBRUSH bgBrush = CreateSolidBrush(hovered ? hoverBgColor : bgColor);
        RECT rect = {x, y, x + width, y + height};
        FillRect(hdc, &rect, bgBrush);
        DeleteObject(bgBrush);

        if(font) {
            SelectObject(hdc, font);
        }

        SetTextColor(hdc, textColor);
        SetBkMode(hdc, TRANSPARENT);

        DrawText(hdc, text.c_str(), text.length(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        RestoreDC(hdc, savedDC);
    }

    void ResetHoverState() {
        hovered = false;
    }
};