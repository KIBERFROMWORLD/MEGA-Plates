#include "plates.h"

// Global objects — accessible in both Main and EventLoop
Window*  wnd;
Button   myButton;
Text*     myText;
Slider*   mySlider;
CheckBox myCheckBox;
InputBox myInputBox;

Main {
    // Create a 1000x1000 window
    wnd = createWindow("Plates", {0, 0, 1000, 1000},
                       WINDOW_SHOWN,
                       RENDERER_ACCELERATED | RENDERER_TARGETTEXTURE,
                       "win"
                    );

    // Dark background
    setBackground({20, 20, 30, 255}, wnd);

    // Header text
    myText = &createText(wnd, "NotoSans", "Welcome to Plates!",
                        {50, 30, 600, 50}, {255, 255, 255, 255}, 32, "title"
                    );

    // Button with rounded corners
    myButton = createButton(wnd, {50, 120, 200, 60}, {70, 130, 255, 255}, "btn1");
    addRoundCorners("btn1", 12);

    // Slider (progress 0.0 – 1.0)
    mySlider = &createSlider(wnd,
                            {50, 220, 300, 16},
                            {60, 60, 60, 255},    // track color (off)
                            {70, 130, 255, 255},  // track color (on)
                            0.4f,
                            "slider1"
                        );

    // Checkbox
    createCheckBox(wnd, {50, 280, 30}, {70, 130, 255, 255}, false, "check1");

    // Input box
    createInputBox(wnd, "Type something...",
                   {50, 340, 400, 40},
                   "",   // initial text
                   18,
                   {220, 220, 220, 255},
                   "input1"
                );

    // Image (file must be next to the executable)
    // createImage(wnd, "logo.png", {800, 30, 150, 150}, "logo");

    // Sound
    // Sound bgm = createSound("music.ogg", "bgm", SOUND_FLAG_DECODE);
    // setSoundLooping(bgm);
    // playSound(bgm);
}

EventLoop {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    // --- Button click ---
    bool hovered = mx >= myButton.rect.x && mx <= myButton.rect.x + myButton.rect.w
                && my >= myButton.rect.y && my <= myButton.rect.y + myButton.rect.h;

    if (hovered && (mainSpace.Event & EVENT_MOUSE_BUTTON_DOWN)) {
        // Button was clicked — change its color
        changeObjectProperties("Color", "255 80 80 255", "btn1");
    }

    // --- Read slider value ---
    if (mySlider->name == "slider1") {
        // mySlider.progress is a float from 0.0 to 1.0
        (void)mySlider->progress;
    }

    // --- Read input box text ---
    if (myInputBox.name == "input1" && myInputBox.changed) {
        // myInputBox.buf is a std::string with the current content
    }

    // --- Read checkbox state ---
    if (myCheckBox.name == "check1") {
        // myCheckBox.isOn is a bool
    }

    // --- Quit on Escape ---
    if (mainSpace.Event & EVENT_KEY_DOWN) {
        const Uint8* keys = GetKeyboardState(nullptr);
        if (keys[SCANCODE_ESCAPE]) {
            mainSpace.running = false;
        }
    }
}
