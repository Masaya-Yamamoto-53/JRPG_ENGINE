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

    return 0;
}
