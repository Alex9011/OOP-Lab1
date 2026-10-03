#include "module2.h"
#include "resource.h"

namespace
{
    constexpr int MIN_NUMBER = 1;
    constexpr int MAX_NUMBER = 100;
    constexpr int PAGE_STEP = 10;

    INT_PTR CALLBACK NumberDialogProc(HWND dialog, UINT message,
        WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_INITDIALOG:
        {
            SetWindowLongPtrW(dialog, DWLP_USER, lParam);
            auto* number = reinterpret_cast<int*>(lParam);

            SCROLLINFO info{};
            info.cbSize = sizeof(info);
            info.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
            info.nMin = MIN_NUMBER;
            info.nMax = MAX_NUMBER;
            info.nPage = 1; 
            info.nPos = *number;
            SetScrollInfo(GetDlgItem(dialog, IDC_NUMBER_SCROLL),
                SB_CTL, &info, TRUE);
            SetDlgItemInt(dialog, IDC_NUMBER_VALUE, *number, FALSE);
            return TRUE;
        }

        case WM_HSCROLL:
        {
            HWND scroll = GetDlgItem(dialog, IDC_NUMBER_SCROLL);
            if (reinterpret_cast<HWND>(lParam) != scroll) return FALSE;
            auto* number = reinterpret_cast<int*>(
                GetWindowLongPtrW(dialog, DWLP_USER));
            int position = *number;

            switch (LOWORD(wParam))
            {
            case SB_LINELEFT:   --position; break;
            case SB_LINERIGHT:  ++position; break;
            case SB_PAGELEFT:   position -= PAGE_STEP; break;
            case SB_PAGERIGHT:  position += PAGE_STEP; break;
            case SB_LEFT:       position = MIN_NUMBER; break;
            case SB_RIGHT:      position = MAX_NUMBER; break;
            case SB_THUMBTRACK:
            case SB_THUMBPOSITION:

                position = HIWORD(wParam);
                break;
            default:
                return TRUE;
            }

            if (position < MIN_NUMBER) position = MIN_NUMBER;
            if (position > MAX_NUMBER) position = MAX_NUMBER;
            *number = position;

            SCROLLINFO info{};
            info.cbSize = sizeof(info);
            info.fMask = SIF_POS;
            info.nPos = position;
            SetScrollInfo(scroll, SB_CTL, &info, TRUE);
            SetDlgItemInt(dialog, IDC_NUMBER_VALUE, position, FALSE);
            return TRUE;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IDOK:
            case IDCANCEL:
                EndDialog(dialog, LOWORD(wParam));
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

bool ShowNumberDialog(HWND owner, int& number)
{
    int draft = number;
    if (draft < MIN_NUMBER) draft = MIN_NUMBER;
    if (draft > MAX_NUMBER) draft = MAX_NUMBER;

    INT_PTR result = DialogBoxParamW(GetModuleHandleW(nullptr),
        MAKEINTRESOURCEW(IDD_NUMBER_DIALOG), owner, NumberDialogProc,
        reinterpret_cast<LPARAM>(&draft));

    if (result == -1)
        MessageBoxW(owner, L"Не вдалося відкрити діалог вибору числа.",
            L"Lab1", MB_OK | MB_ICONERROR);

    if (result != IDOK) return false;
    number = draft;
    return true;
}
