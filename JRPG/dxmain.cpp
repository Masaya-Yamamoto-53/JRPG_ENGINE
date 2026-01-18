#include "DxLib.h"
#include "GameSettings.h"
#include "FpsController.h"
#include "InputManager.h"
#include "DebugManager.h"
#include "FieldCharacter.h" // 暫定対応
#include "FieldManager.h"   // 暫定対応

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

    // コンソールを作成
    AllocConsole();
    FILE* out = nullptr;
    freopen_s(&out, "CON", "w", stdout);

    // Load game settings
    auto & settings = GameSettings::instance();
    settings.load("config.ini");

    // DX Library Settings
    // Disable outputting log files
    SetOutApplicationLogValidFlag(FALSE);
    // Set screen mode (resolution and color bit depth)
    SetGraphMode(
          settings.getWindowWidth()
        , settings.getWindowHeight()
        , settings.getColorBitDepth()
    );
    // Expand the window size by 1 times.
    SetWindowSizeExtendRate(1.0);
    // Set vertical sync
    SetWaitVSyncFlag(FALSE);
    // Always run the application
    SetAlwaysRunFlag(TRUE);
    // Change to window mode
    ChangeWindowMode(settings.isWindowMode());

    // Initialize the DX Library
    if (DxLib_Init() == -1) return -1;

    // Set the drawing target to the back screen
    SetDrawScreen(DX_SCREEN_BACK);

    // FPS object to manage the frame rate
    FpsController fpsController(settings.getTargetFps());

    // 暫定対応
    FieldCharacter fieldCharacter("00");
    FieldManager fieldManager;
    fieldManager.load();

    // Main loop
    while (true) {
        // DxLibが修了要求を返したらループを抜ける
        if (ProcessMessage() != 0) {
            break;
        }
        // Clear the screen
        ClearDrawScreen();

        // Update frame timing and calculate FPS
        fpsController.update();

        // Method to update the key input status
        InputManager::instance().update();

        // デバッグモード切替
        if (InputManager::instance().isKeyTriggered(KEY_INPUT_ESCAPE)) {
            DebugManager::instance().toggle();
        }

        // 暫定対応
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
