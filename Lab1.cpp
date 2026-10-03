#include <windows.h>
#include <string>
#include "module1/module1.h"
#include "module2/module2.h"

namespace
{
    constexpr int ID_WORK1 = 1001;
    constexpr int ID_WORK2 = 1002;
    const wchar_t WINDOW_CLASS[] = L"Lab1Variant16Window";

    std::wstring savedText;
    int savedNumber = 1;
    bool hasText = false;
    bool hasNumber = false;

    LRESULT CALLBACK WindowProc(HWND window, UINT message,
        WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case ID_WORK1:
                if (ShowTextDialog(window, savedText))
                {
                    hasText = true;
                    InvalidateRect(window, nullptr, TRUE);
                }
                return 0;

            case ID_WORK2:
                if (ShowNumberDialog(window, savedNumber))
                {
                    hasNumber = true;
                    InvalidateRect(window, nullptr, TRUE);
                }
                return 0;
            }
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT paint{};
            HDC dc = BeginPaint(window, &paint);
            HGDIOBJ oldFont = SelectObject(dc,
                GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(dc, TRANSPARENT);

            RECT area{};
            GetClientRect(window, &area);
            InflateRect(&area, -24, -20);

            std::wstring output =
                L"Лабораторна робота 1. Варіант 16\n\n"
                L"Оберіть у меню Робота1 або Робота2.\n\n"
                L"Введений текст: ";
            output += hasText ? savedText : L"(ще не введено)";
            output += L"\n\nВибране число: ";
            output += hasNumber ? std::to_wstring(savedNumber)
                                : L"(ще не вибрано)";

            DrawTextW(dc, output.c_str(), -1, &area,
                DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
            SelectObject(dc, oldFont);
            EndPaint(window, &paint);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProcW(window, message, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE,
    PWSTR, int showCommand)
{
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground =
        reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    windowClass.lpszClassName = WINDOW_CLASS;

    if (!RegisterClassExW(&windowClass))
    {
        MessageBoxW(nullptr, L"Не вдалося зареєструвати клас вікна.",
            L"Lab1", MB_OK | MB_ICONERROR);
        return 1;
    }

    HMENU menu = CreateMenu();
    if (!menu ||
        !AppendMenuW(menu, MF_STRING, ID_WORK1, L"Робота1") ||
        !AppendMenuW(menu, MF_STRING, ID_WORK2, L"Робота2"))
    {
        if (menu) DestroyMenu(menu);
        MessageBoxW(nullptr, L"Не вдалося створити меню.",
            L"Lab1", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND window = CreateWindowExW(0, WINDOW_CLASS,
        L"Lab1 - Варіант 16", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 760, 420,
        nullptr, menu, instance, nullptr);
    if (!window)
    {
        DestroyMenu(menu);
        MessageBoxW(nullptr, L"Не вдалося створити головне вікно.",
            L"Lab1", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(window, showCommand);
    UpdateWindow(window);


    MSG message{};
    int result;
    while ((result = GetMessageW(&message, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return result == -1 ? 1 : static_cast<int>(message.wParam);
}
