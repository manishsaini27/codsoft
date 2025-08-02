#include <windows.h>
#include <vector>
#include <string>

HWND hInput, hListBox, hAddBtn, hMarkBtn, hDeleteBtn;

std::vector<std::wstring> tasks;
std::vector<bool> status; // false = pending, true = completed

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// Function to update ListBox content
void RefreshTaskList() {
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    for (size_t i = 0; i < tasks.size(); ++i) {
        std::wstring display = status[i] ? L"[✔] " : L"[ ] ";
        display += tasks[i];
        SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)display.c_str());
    }
}

// UI setup
void AddControls(HWND hwnd) {
    CreateWindowW(L"Static", L"Enter Task:", WS_VISIBLE | WS_CHILD,
                  20, 20, 80, 25, hwnd, NULL, NULL, NULL);

    hInput = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                           100, 20, 300, 25, hwnd, NULL, NULL, NULL);

    hAddBtn = CreateWindowW(L"Button", L"Add", WS_VISIBLE | WS_CHILD,
                            420, 20, 70, 25, hwnd, (HMENU)1, NULL, NULL);

    hListBox = CreateWindowW(L"ListBox", NULL,
                             WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                             20, 60, 470, 200, hwnd, (HMENU)2, NULL, NULL);

    hMarkBtn = CreateWindowW(L"Button", L"Mark as Completed", WS_VISIBLE | WS_CHILD,
                             20, 270, 200, 30, hwnd, (HMENU)3, NULL, NULL);

    hDeleteBtn = CreateWindowW(L"Button", L"Delete Task", WS_VISIBLE | WS_CHILD,
                               250, 270, 200, 30, hwnd, (HMENU)4, NULL, NULL);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"ToDoGUIApp";
    wc.lpfnWndProc = WindowProc;

    RegisterClassW(&wc);

    CreateWindowW(L"ToDoGUIApp", L"To-Do List Manager (C++ GUI)",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  100, 100, 520, 360, NULL, NULL, NULL, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            AddControls(hwnd);
            break;

        case WM_COMMAND:
            switch (wp) {
                case 1: { // Add Task
                    wchar_t inputText[256];
                    GetWindowTextW(hInput, inputText, 256);
                    if (wcslen(inputText) > 0) {
                        tasks.push_back(inputText);
                        status.push_back(false); // default = pending
                        SetWindowTextW(hInput, L"");
                        RefreshTaskList();
                    }
                } break;

                case 3: { // Mark Completed
                    int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index >= 0 && index < (int)status.size()) {
                        status[index] = true;
                        RefreshTaskList();
                    }
                } break;

                case 4: { // Delete Task
                    int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index >= 0 && index < (int)tasks.size()) {
                        tasks.erase(tasks.begin() + index);
                        status.erase(status.begin() + index);
                        RefreshTaskList();
                    }
                } break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}
