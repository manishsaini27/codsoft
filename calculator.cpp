#include <windows.h>
#include <string>
#include <sstream>

HWND hNum1, hNum2, hResult;
HWND hAdd, hSub, hMul, hDiv;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddControls(HWND);

std::string GetText(HWND hwnd) {
    char buffer[256];
    GetWindowText(hwnd, buffer, 256);
    return std::string(buffer);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"CalculatorWindow";
    wc.lpfnWndProc = WindowProcedure;

    RegisterClassW(&wc);

    CreateWindowW(L"CalculatorWindow", L"C++ GUI Calculator",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 350, 300, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void AddControls(HWND hwnd) {
    CreateWindowW(L"Static", L"Number 1:", WS_VISIBLE | WS_CHILD,
                  50, 30, 70, 20, hwnd, NULL, NULL, NULL);
    hNum1 = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                          130, 30, 150, 20, hwnd, NULL, NULL, NULL);

    CreateWindowW(L"Static", L"Number 2:", WS_VISIBLE | WS_CHILD,
                  50, 60, 70, 20, hwnd, NULL, NULL, NULL);
    hNum2 = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                          130, 60, 150, 20, hwnd, NULL, NULL, NULL);

    hAdd = CreateWindowW(L"Button", L"+", WS_VISIBLE | WS_CHILD,
                         50, 100, 50, 30, hwnd, (HMENU)1, NULL, NULL);
    hSub = CreateWindowW(L"Button", L"-", WS_VISIBLE | WS_CHILD,
                         110, 100, 50, 30, hwnd, (HMENU)2, NULL, NULL);
    hMul = CreateWindowW(L"Button", L"*", WS_VISIBLE | WS_CHILD,
                         170, 100, 50, 30, hwnd, (HMENU)3, NULL, NULL);
    hDiv = CreateWindowW(L"Button", L"/", WS_VISIBLE | WS_CHILD,
                         230, 100, 50, 30, hwnd, (HMENU)4, NULL, NULL);

    CreateWindowW(L"Static", L"Result:", WS_VISIBLE | WS_CHILD,
                  50, 150, 70, 20, hwnd, NULL, NULL, NULL);
    hResult = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
                            130, 150, 150, 20, hwnd, NULL, NULL, NULL);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            AddControls(hwnd);
            break;

        case WM_COMMAND: {
            double num1 = std::stod(GetText(hNum1));
            double num2 = std::stod(GetText(hNum2));
            double res = 0;
            bool valid = true;

            switch (wp) {
                case 1: res = num1 + num2; break;
                case 2: res = num1 - num2; break;
                case 3: res = num1 * num2; break;
                case 4:
                    if (num2 != 0)
                        res = num1 / num2;
                    else {
                        SetWindowText(hResult, "Error: Divide by zero");
                        valid = false;
                    }
                    break;
            }

            if (valid) {
                std::ostringstream oss;
                oss << res;
                SetWindowTextA(hResult, oss.str().c_str());
            }
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}
