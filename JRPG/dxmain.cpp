#include "DxLib.h"
#include "GameSettings.h"
#include "FpsController.h"
#include "InputManager.h"

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

    // Load game settings
    GameSettings gameSettings;
    gameSettings.load("config.ini");

    // DX Library Settings
    // Disable outputting log files
    SetOutApplicationLogValidFlag(FALSE);
    // Set vertical sync
    SetWaitVSyncFlag(FALSE);
    // Always run the application
    SetAlwaysRunFlag(TRUE);
    // Change to window mode
    ChangeWindowMode(gameSettings.isWindowMode());

    // Initialize the DX Library
    if (DxLib_Init() == -1) return -1;

    // FPS object to manage the frame rate
    FpsController fpsController(gameSettings.getTargetFps());

    // Main loop
    while (true) {
        // Break the loop when a quit message (WM_QUIT) is received
        if (ProcessMessage() != 0) {
            break;
        }

        // Update frame timing and calculate FPS
        fpsController.update();

        // Method to update the key input status
        InputManager::instance().update();

        // Method to adjust the frame interval
        fpsController.wait();
    }

    // Finalize the DX Library
    DxLib_End();

    return 0;
}
