#include "module1.h"
#include "resource.h"

namespace
{
    constexpr int MAX_TEXT_LENGTH = 255;

    
    INT_PTR CALLBACK TextDialogProc(HWND dialog, UINT message,
        WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_INITDIALOG:
        {
           
            SetWindowLongPtrW(dialog, DWLP_USER, lParam);
            auto* text = reinterpret_cast<std::wstring*>(lParam);
            SendDlgItemMessageW(dialog, IDC_TEXT_INPUT,
                EM_SETLIMITTEXT, MAX_TEXT_LENGTH, 0);
            SetDlgItemTextW(dialog, IDC_TEXT_INPUT, text->c_str());
            return TRUE;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDOK:
            {
                auto* text = reinterpret_cast<std::wstring*>(
                    GetWindowLongPtrW(dialog, DWLP_USER));
                wchar_t buffer[MAX_TEXT_LENGTH + 1]{};
                GetDlgItemTextW(dialog, IDC_TEXT_INPUT, buffer,
                    MAX_TEXT_LENGTH + 1);
                *text = buffer;
                EndDialog(dialog, IDOK);
                return TRUE;
            }
            case IDCANCEL:
                EndDialog(dialog, IDCANCEL);
                return TRUE;
            }
            break;

        case WM_CLOSE:
            EndDialog(dialog, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
}

bool ShowTextDialog(HWND owner, std::wstring& text)
{
    std::wstring draft = text;
    INT_PTR result = DialogBoxParamW(GetModuleHandleW(nullptr),
        MAKEINTRESOURCEW(IDD_TEXT_DIALOG), owner, TextDialogProc,
        reinterpret_cast<LPARAM>(&draft));

    if (result == -1)
        MessageBoxW(owner, L"Не вдалося відкрити діалог введення тексту.",
            L"Lab1", MB_OK | MB_ICONERROR);

    if (result != IDOK) return false;
    text = draft;
    return true;
}
