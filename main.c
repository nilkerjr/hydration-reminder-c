#define UNICODE
#define _UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>  // for progress bar

#pragma comment(lib, "comctl32.lib")

#define ID_BUTTON_100   1
#define ID_BUTTON_200   2
#define ID_BUTTON_300   3
#define ID_BUTTON_400   4
#define ID_BUTTON_RESET 5
#define ID_TIMER        6

// Globals
int dailyGoal = 2000;
int waterIntake = 0;
HWND hProgressBar, hLabel, hGoalInput, hSetGoalButton;

// Forward declarations
void UpdateProgress(HWND hwnd);

// --- Update Progress Bar and Label ---
void UpdateProgress(HWND hwnd) 
{
    wchar_t text[100];
    swprintf_s(text, 100, L"Water Intake: %d / %d ml", waterIntake, dailyGoal);
    SendMessage(hLabel, WM_SETTEXT, 0, (LPARAM)text);

    int progress = (int)(((double)waterIntake / dailyGoal) * 100);
    SendMessage(hProgressBar, PBM_SETPOS, progress, 0);
}

// --- Window Procedure ---
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {
        case WM_CREATE: 
        {
            InitCommonControls();

            // Title label
            CreateWindowW(L"STATIC", L"💧 Hydration Reminder 💧",
                          WS_VISIBLE | WS_CHILD | SS_CENTER,
                          50, 10, 300, 30, hwnd, NULL, NULL, NULL);

            // Goal input label
            CreateWindowW(L"STATIC", L"Enter daily goal (ml):",
                          WS_VISIBLE | WS_CHILD,
                          50, 50, 150, 20, hwnd, NULL, NULL, NULL);

            // Goal input box
            hGoalInput = CreateWindowW(L"EDIT", L"2000",
                                       WS_VISIBLE | WS_CHILD | WS_BORDER,
                                       200, 50, 80, 20, hwnd, NULL, NULL, NULL);

            // Set goal button
            hSetGoalButton = CreateWindowW(L"BUTTON", L"Set Goal",
                                           WS_VISIBLE | WS_CHILD,
                                           290, 50, 80, 22, hwnd, (HMENU)ID_BUTTON_RESET, NULL, NULL);

            // Progress label
            hLabel = CreateWindowW(L"STATIC", L"Water Intake: 0 / 2000 ml",
                                   WS_VISIBLE | WS_CHILD | SS_CENTER,
                                   50, 80, 320, 25, hwnd, NULL, NULL, NULL);

            // Progress bar
            hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL,
                                          WS_CHILD | WS_VISIBLE,
                                          50, 110, 320, 25, hwnd, NULL, NULL, NULL);
        
            SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

            SendMessage(hProgressBar, PBM_SETSTEP, 1, 0);

            // Buttons
            CreateWindowW(L"BUTTON", L"+100 ml", WS_VISIBLE | WS_CHILD,
                          50, 160, 80, 30, hwnd, (HMENU)ID_BUTTON_100, NULL, NULL);
                          
            CreateWindowW(L"BUTTON", L"+200 ml", WS_VISIBLE | WS_CHILD,
                          140, 160, 80, 30, hwnd, (HMENU)ID_BUTTON_200, NULL, NULL);

            CreateWindowW(L"BUTTON", L"+300 ml", WS_VISIBLE | WS_CHILD,
                          230, 160, 80, 30, hwnd, (HMENU)ID_BUTTON_300, NULL, NULL);

            CreateWindowW(L"BUTTON", L"+400 ml", WS_VISIBLE | WS_CHILD,
                          320, 160, 80, 30, hwnd, (HMENU)ID_BUTTON_400, NULL, NULL);

            CreateWindowW(L"BUTTON", L"Reset", WS_VISIBLE | WS_CHILD,
                          180, 210, 80, 30, hwnd, (HMENU)ID_BUTTON_RESET, NULL, NULL);

            // Reminder every 30 minutes
            SetTimer(hwnd, ID_TIMER, 1 * 10 * 1000, NULL);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wParam)) 
            {
                case ID_BUTTON_100:
                {
                    waterIntake += 100;
                    break;
                }

                case ID_BUTTON_200:
                {
                    waterIntake += 200;
                    break;
                }

                case ID_BUTTON_300:
                {
                    waterIntake += 300;
                    break;
                }

                case ID_BUTTON_400:
                {
                    waterIntake += 400;
                    break;
                }

                case ID_BUTTON_RESET:
                {
                    waterIntake = 0;
                    break;
                }
            }

            if ((HWND)lParam == hSetGoalButton) 
            {
                wchar_t buf[16];

                GetWindowTextW(hGoalInput, buf, 16);
            
                int newGoal = _wtoi(buf);
            
                if (newGoal > 0) 
                {
                    dailyGoal = newGoal;
                    waterIntake = 0;
                    UpdateProgress(hwnd);
                }
            }

            if (waterIntake > dailyGoal)
            {
                waterIntake = dailyGoal;
            }

            UpdateProgress(hwnd);

            if (waterIntake >= dailyGoal) 
            {
                MessageBox(hwnd, L"You reached your daily goal! 🎉", L"Hydration Reminder", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }

        case WM_TIMER:
        {
            MessageBox(hwnd, L"Time to drink some water! 💧", L"Hydration Reminder", MB_OK | MB_ICONINFORMATION);
            break;
        }
        
        case WM_DESTROY:
        {
            KillTimer(hwnd, ID_TIMER);
            PostQuitMessage(0);
        }

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// --- WinMain Entry ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
    SetConsoleOutputCP(CP_UTF8); // Enable emoji display (for text elements)
    const wchar_t CLASS_NAME[] = L"HydrationReminderClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx
    (
        0,
        CLASS_NAME,
        L"Hydration Reminder (GUI)",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, // disable resizing
        CW_USEDEFAULT, CW_USEDEFAULT, 460, 320,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) 
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    
    UpdateWindow(hwnd);

    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}