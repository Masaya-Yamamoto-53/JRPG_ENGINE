#include "DxLib.h"

int WINAPI WinMain(
      _In_ HINSTANCE hInstance
    , _In_opt_ HINSTANCE hPrevInstance
    , _In_ LPSTR lpCmdLine
    , _In_ int nCmdShow
) {
    // Suppress unused parameter warnings
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    // DX Library Settings
    // Disable outputting log files
    SetOutApplicationLogValidFlag(FALSE);

    // Initialize the DX Library
    if (DxLib_Init() == -1) return -1;

    // Main loop
    while (true) {
        // Break the loop when a quit message (WM_QUIT) is received
        if (ProcessMessage() != 0) {
            break;
        }
    }

    // Finalize the DX Library
    DxLib_End();

    return 0;
}
