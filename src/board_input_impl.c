#include "board_input_impl.h"

#ifndef _WIN32
#pragma message("Error: Board Input invalid implementation (windows only)")
#error
#endif

#include <assert.h>
#include <stdbool.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#include <processthreadsapi.h>

#define BOARD_KEY_UP      0b0000
#define BOARD_KEY_DOWN    0b1000
#define BOARD_KEY_RELEASE 0b0001
#define BOARD_KEY_PRESS   0b0010
#define BOARD_KEY_REPEAT  0b0100

static bool running = false;
static bool quit    = false;

static LRESULT CALLBACK LowLevelKeyboardProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);

static bool board_input_initialized = false;
static HANDLE hThread = NULL;
static DWORD dwThreadId = (DWORD)0;
static CRITICAL_SECTION CriticalSection = { 0 };

static uint8_t board_input_key_states[BOARD_KEY_COUNT] = { 0 };

static void board_input_set_key(uint8_t key_code, uint8_t state)
{
    //EnterCriticalSection(&CriticalSection);
    {
        board_input_key_states[key_code] = state;
    }
    //LeaveCriticalSection(&CriticalSection);
}

static DWORD WINAPI board_input_loop(LPVOID lpParam)
{
    // lpParam not used
    UNREFERENCED_PARAMETER(lpParam);


    while (!quit)
    {
        while (running)
        {
            Sleep(100);
        }
    }
    
    return (DWORD)EXIT_SUCCESS;
}

void board_input_initialize(void)
{
    if (board_input_initialized)
    {
        fprintf(stderr, "Erorr: Cannot initialize board input, already initialized\n");
        return;
    }

    quit = false;
    
    hThread = CreateThread(
            NULL,                                      /* default security attributes   */
            0,                                         /* use default stack size        */
            (LPTHREAD_START_ROUTINE)board_input_loop,  /* thread function name          */
            NULL,                                      /* argument to thread function   */
            0,                                         /* use default creation flags    */
            &dwThreadId);                              /* returns the thread identifier */

    //fprintf(stderr, "Created Thread: %d\n", (int)dwThreadId);

    if (hThread == NULL)
    {
        fprintf(stderr, "CreateThread error: %d\n", GetLastError());
        ExitProcess(3);
    }
    
    BOOL initializedCriticalSection = InitializeCriticalSectionAndSpinCount(
                                          &CriticalSection, 
                                          0x00000000); 
    
    if (!initializedCriticalSection)
    {
        fprintf(stderr, "InitializeCriticalSectionAndSpinCount error: %d\n", GetLastError());
        ExitProcess(3);
    }

    //HOOKPROC hkprcSysMsg = SetWindowsHookEx(
    //                           WH_KEYBOARD_LL,        /* keystroke message hook */
    //                           LowLevelKeyboardProc,  /* hook procedure */
    //                           NULL,                  /* handle to hook procedure thread (null since thread is in within this code) */
    //                           0);                    /* id of hook procedure thread */

    //if (hkprcSysMsg == NULL)
    //{
    //    fprintf(stderr, "Failed to SetWindowsHookEx: %d\n", GetLastError());
    //}
   
    board_input_initialized = true;
}

void board_input_start(void)
{
    running = true;
}
void board_input_stop(void)
{
    running = false;
}

void board_input_destroy(void)
{
    quit = true;

    board_input_stop();

    BOOL bAttach = AttachThreadInput(
        GetCurrentThreadId(),
        dwThreadId,
        TRUE);

    WaitForSingleObject(hThread, (DWORD)INFINITE);

    CloseHandle(hThread);

    // Release resources used by the critical section object.
    DeleteCriticalSection(&CriticalSection);

    board_input_initialized = false;
}

LRESULT CALLBACK LowLevelKeyboardProc(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    KBDLLHOOKSTRUCT* kbHook = (KBDLLHOOKSTRUCT*)lParam;

    DWORD vkCode = kbHook->vkCode;
    
    if (0 <= vkCode && vkCode < BOARD_KEY_COUNT)
    {

        switch (wParam)
        {
            case WM_KEYDOWN:
                //board_input_set_key(vkCode, true);
                break;

            case WM_KEYUP:
                //board_input_set_key(vkCode, false);
                break;
        }
    }

    return CallNextHookEx((HHOOK)NULL, nCode, wParam, lParam);
}

void board_input_poll(void)
{
    //static MSG msg = { 0 };

    //while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) != 0)
    {
        //TranslateMessage(&msg);
        //DispatchMessage(&msg);
    }

    SHORT key_pressed = 0;
    uint8_t key_state = 0;

    bool up = 0;
    bool down = 0;
    bool release = 0;
    bool pressed = 0;
    bool repeat = 0;

    for (int i = 0; i < BOARD_KEY_COUNT; ++i)
    {
        key_pressed = GetAsyncKeyState(i);
        key_state = board_input_key_states[i];

        
        //if (key_pressed != 0)
        //{
        //    fprintf(stderr, "%d, %c: %d\n", i, (char)i, key_pressed);
        //}

        up = key_pressed == 0;
        down = key_pressed != 0;
        release = ((key_state & BOARD_KEY_DOWN) != 0) && up;
        pressed = (key_state == BOARD_KEY_UP) && down;
        repeat = ((key_state & BOARD_KEY_DOWN) != 0) && down;

        board_input_key_states[i] = (up * BOARD_KEY_UP) | (down * BOARD_KEY_DOWN) | (release * BOARD_KEY_RELEASE) | (pressed * BOARD_KEY_PRESS) | (repeat * BOARD_KEY_REPEAT);
    }
}

void board_input_reset(void)
{
    memset(board_input_key_states, (uint8_t)0, (size_t)BOARD_KEY_COUNT);
}

bool board_input_key_just_pressed(uint8_t key_code)
{
    if (key_code >= BOARD_KEY_COUNT)
    {
        return false;
    }

    uint8_t key_state = board_input_key_states[key_code];

    bool key_pressed = (key_state & BOARD_KEY_PRESS) != 0;

    return key_pressed;
}
