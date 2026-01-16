#include "DxLib.h"
#include "GameSettings.h"
#include "FpsController.h"
#include "InputManager.h"
#include "FieldCharacter.h" // Žb’è‘Î‰ž
#include "FieldManager.h"   // Žb’è‘Î‰ž

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
    // Expand the window size by 1 times.
    SetWindowSizeExtendRate(1.0);
    // Set vertical sync
    SetWaitVSyncFlag(FALSE);
    // Always run the application
    SetAlwaysRunFlag(TRUE);
    // Change to window mode
    ChangeWindowMode(gameSettings.isWindowMode());

    // Initialize the DX Library
    if (DxLib_Init() == -1) return -1;

    // Set the drawing target to the back screen
    SetDrawScreen(DX_SCREEN_BACK);

    // FPS object to manage the frame rate
    FpsController fpsController(gameSettings.getTargetFps());

    // Žb’è‘Î‰ž
    FieldCharacter fieldCharacter("00", 128, 128);
    FieldManager fieldManager;

    // Main loop
    while (true) {
        // Break the loop when a quit message (WM_QUIT) is received
        if (ProcessMessage() != 0) {
            break;
        }
        // Clear the screen
        ClearDrawScreen();

        // Update frame timing and calculate FPS
        fpsController.update();

        // Method to update the key input status
        InputManager::instance().update();

        // Žb’è‘Î‰ž
        fieldManager.setCharacter(&fieldCharacter);
        fieldManager.update();
        fieldManager.draw();

        // Method to adjust the frame interval
        fpsController.wait();

        // Update the screen
        ScreenFlip();
    }

    // Finalize the DX Library
    DxLib_End();

    return 0;
}
