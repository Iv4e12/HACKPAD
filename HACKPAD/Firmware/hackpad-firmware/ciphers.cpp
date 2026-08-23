#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ciphers.h"

#define SW1_PIN 0
#define SW2_PIN 1
#define SW3_PIN 2
#define SW4_PIN 3

#define OLED_SDA 4
#define OLED_SCL 5

#define ENCODER_A 6
#define ENCODER_B 7
#define ENCODER_BUTTON 8

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

const int buttonPins[] = {
    SW1_PIN,
    SW2_PIN,
    SW3_PIN,
    SW4_PIN
};

const int BUTTON_COUNT = 4;

enum Screen {
    MAIN_MENU,
    CIPHER_MENU,
    MODE_MENU,
    TEXT_SCREEN,
    RESULT_SCREEN
};

Screen currentScreen = MAIN_MENU;

enum Cipher {
    CIPHER_THERAPRISM,
    CIPHER_FORDESE,
    CIPHER_BILL
};

Cipher selectedCipher = CIPHER_THERAPRISM;

enum Mode {
    MODE_ENCODE,
    MODE_DECODE
};

Mode selectedMode = MODE_ENCODE;

int menuSelection = 0;

String inputText = "";
String outputText = "";

unsigned long lastButtonTime = 0;
unsigned long lastEncoderTime = 0;

int lastEncoderState = 0;

void clearDisplay() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
}

void showTitle(const char *title) {
    clearDisplay();

    display.setTextSize(1);
    display.println(title);

    display.drawLine(
        0,
        9,
        SCREEN_WIDTH - 1,
        9,
        SSD1306_WHITE
    );
}

void showCentered(const char *text) {
    clearDisplay();

    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;

    display.getTextBounds(
        text,
        0,
        0,
        &x1,
        &y1,
        &w,
        &h
    );

    int x = (SCREEN_WIDTH - w) / 2;
    int y = (SCREEN_HEIGHT - h) / 2;

    display.setCursor(x, y);
    display.print(text);

    display.display();
}

void drawMainMenu() {
    showTitle("GRAVITY FALLS");

    const char *items[] = {
        "ENCODE",
        "DECODE",
        "SETTINGS"
    };

    for (int i = 0; i < 3; i++) {
        display.setCursor(0, 12 + i * 6);

        if (i == menuSelection) {
            display.print("> ");
        }
        else {
            display.print("  ");
        }

        display.print(items[i]);
    }

    display.display();
}

void drawCipherMenu() {
    showTitle("CIPHER");

    const char *items[] = {
        "THERAPRISM",
        "FORDESE",
        "BILL"
    };

    for (int i = 0; i < 3; i++) {
        display.setCursor(0, 12 + i * 6);

        if (i == menuSelection) {
            display.print("> ");
        }
        else {
            display.print("  ");
        }

        display.print(items[i]);
    }

    display.display();
}

void drawModeMenu() {
    showTitle("MODE");

    display.setCursor(0, 14);

    if (selectedMode == MODE_ENCODE) {
        display.print("> ENCODE");
    }
    else {
        display.print("> DECODE");
    }

    display.display();
}

int readEncoder() {
    int a = digitalRead(ENCODER_A);
    int b = digitalRead(ENCODER_B);

    int currentState = (a << 1) | b;

    int movement = 0;

    if (currentState != lastEncoderState) {

        if (
            (lastEncoderState == 0 && currentState == 1) ||
            (lastEncoderState == 1 && currentState == 3) ||
            (lastEncoderState == 3 && currentState == 2) ||
            (lastEncoderState == 2 && currentState == 0)
        ) {
            movement = 1;
        }

        if (
            (lastEncoderState == 0 && currentState == 2) ||
            (lastEncoderState == 2 && currentState == 3) ||
            (lastEncoderState == 3 && currentState == 1) ||
            (lastEncoderState == 1 && currentState == 0)
        ) {
            movement = -1;
        }

        lastEncoderState = currentState;
    }

    return movement;
}

bool encoderPressed() {
    static bool previousState = HIGH;

    bool currentState = digitalRead(ENCODER_BUTTON);

    bool pressed =
    previousState == HIGH &&
    currentState == LOW;

    previousState = currentState;

    return pressed;
}

int readButton() {

    if (millis() - lastButtonTime < 150) {
        return -1;
    }

    for (int i = 0; i < BUTTON_COUNT; i++) {

        if (digitalRead(buttonPins[i]) == LOW) {

            lastButtonTime = millis();

            return i;
        }
    }

    return -1;
}

String processCipher(String text) {

    if (selectedCipher == CIPHER_THERAPRISM) {

        if (selectedMode == MODE_ENCODE) {
            return theraprismEncode(text);
        }
        else {
            return theraprismDecode(text);
        }
    }

    if (selectedCipher == CIPHER_FORDESE) {

        if (selectedMode == MODE_ENCODE) {
            return fordeseEncode(text);
        }
        else {
            return fordeseDecode(text);
        }
    }

    if (selectedCipher == CIPHER_BILL) {

        if (selectedMode == MODE_ENCODE) {
            return billEncode(text);
        }
        else {
            return billDecode(text);
        }
    }

    return text;
}

void showResult() {

    clearDisplay();

    display.println("RESULT:");

    display.println();

    display.println(outputText);

    display.display();
}

void setup() {

    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(
            buttonPins[i],
            INPUT_PULLUP
        );
    }

    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_BUTTON, INPUT_PULLUP);

    Wire.setSDA(OLED_SDA);
    Wire.setSCL(OLED_SCL);

    Wire.begin();

    if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
    )) {
        while (true) {
        }
    }

    lastEncoderState =
    (digitalRead(ENCODER_A) << 1) |
    digitalRead(ENCODER_B);

    showCentered("HACKPAD");

    delay(1000);

    drawMainMenu();
}

void loop() {

    int movement = readEncoder();

    if (movement != 0 &&
        millis() - lastEncoderTime > 30) {

        lastEncoderTime = millis();

    if (currentScreen == MAIN_MENU ||
        currentScreen == CIPHER_MENU) {

        menuSelection += movement;

    if (menuSelection < 0) {
        menuSelection = 2;
    }

    if (menuSelection > 2) {
        menuSelection = 0;
    }
        }

        drawCurrentScreen();
        }

        if (encoderPressed()) {

            if (currentScreen == MAIN_MENU) {

                if (menuSelection == 0) {
                    selectedMode = MODE_ENCODE;
                    currentScreen = CIPHER_MENU;
                    menuSelection = 0;
                }

                else if (menuSelection == 1) {
                    selectedMode = MODE_DECODE;
                    currentScreen = CIPHER_MENU;
                    menuSelection = 0;
                }

                drawCurrentScreen();
            }

            else if (currentScreen == CIPHER_MENU) {

                selectedCipher =
                (Cipher)menuSelection;

                inputText = "";

                currentScreen = TEXT_SCREEN;

                drawCurrentScreen();
            }

            else if (currentScreen == RESULT_SCREEN) {

                currentScreen = MAIN_MENU;
                menuSelection = 0;

                drawCurrentScreen();
            }
        }

        int button = readButton();

        if (button != -1) {

            if (button == 0) {

                currentScreen = MAIN_MENU;
                menuSelection = 0;

                drawCurrentScreen();
            }

            else if (button == 1) {

                if (currentScreen == CIPHER_MENU) {

                    selectedCipher =
                    (Cipher)menuSelection;

                    currentScreen = TEXT_SCREEN;

                    drawCurrentScreen();
                }
            }

            else if (button == 2) {

                if (currentScreen == TEXT_SCREEN) {

                    outputText =
                    processCipher(inputText);

                    currentScreen =
                    RESULT_SCREEN;

                    showResult();
                }
            }

            else if (button == 3) {

                if (inputText.length() > 0) {

                    inputText.remove(
                        inputText.length() - 1
                    );
                }

                drawCurrentScreen();
            }
        }
}

void drawCurrentScreen() {

    if (currentScreen == MAIN_MENU) {
        drawMainMenu();
    }

    else if (currentScreen == CIPHER_MENU) {
        drawCipherMenu();
    }

    else if (currentScreen == MODE_MENU) {
        drawModeMenu();
    }

    else if (currentScreen == TEXT_SCREEN) {

        clearDisplay();

        display.println("TEXT:");

        display.println(inputText);

        display.display();
    }

    else if (currentScreen == RESULT_SCREEN) {
        showResult();
    }
}
