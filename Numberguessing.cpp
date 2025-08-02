#include <windows.h>
#include <ctime>
#include <string>

HWND hInput, hButton, hOutput;
int secretNumber;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// Function to add GUI controls
void AddControls(HWND hwnd) {
    CreateWindowW(L"Static", L"Guess the number (1 to 100):", WS_VISIBLE | WS_CHILD,
                  50, 40, 200, 20, hwnd, NULL, NULL, NULL);

    hInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                           50, 70, 180, 25, hwnd, NULL, NULL, NULL);

    hButton = CreateWindowW(L"Button", L"Submit", WS_VISIBLE | WS_CHILD,
                            250, 70, 80, 25, hwnd, (HMENU)1, NULL, NULL);

    hOutput = CreateWindowW(L"Static", L"", WS_VISIBLE | WS_CHILD,
                            50, 110, 300, 25, hwnd, NULL, NULL, NULL);
}

// Convert input from edit box to integer
int GetGuess(HWND inputBox) {
    wchar_t text[256];
    GetWindowTextW(inputBox, text, 256);
    return _wtoi(text);  // Convert to int
}

// Entry point
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    srand(static_cast<unsigned int>(time(0)));
    secretNumber = rand() % 100 + 1;

    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"GuessGameWindow";
    wc.lpfnWndProc = WindowProcedure;

    RegisterClassW(&wc);

    CreateWindowW(L"GuessGameWindow", L"Number Guessing Game",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 400, 250, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Windows message handler
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            AddControls(hwnd);
            break;

        case WM_COMMAND:
            if (wp == 1) { // Button click
                int guess = GetGuess(hInput);
                std::wstring message;

                if (guess < 1 || guess > 100) {
                    message = L"⚠️ Enter a number between 1 and 100!";
                } else if (guess < secretNumber) {
                    message = L"📉 Too low! Try again.";
                } else if (guess > secretNumber) {
                    message = L"📈 Too high! Try again.";
                } else {
                    message = L"🎉 Correct! You guessed the number!";
                }

                SetWindowTextW(hOutput, message.c_str());
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}
