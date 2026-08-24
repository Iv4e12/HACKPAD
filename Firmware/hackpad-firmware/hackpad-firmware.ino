#include <Arduino.h>
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

enum Screen
{
    START_SCREEN,
    MAIN_MENU,
    CIPHER_MENU,
    TEXT_ENTRY,
    RESULT_SCREEN
};

Screen screen = START_SCREEN;

enum Mode
{
    ENCODE_MODE,
    DECODE_MODE
};

Mode mode = ENCODE_MODE;

enum Cipher
{
    THERAPRISM,
    FORDESE,
    BILL
};

Cipher cipher = THERAPRISM;

int menuIndex = 0;

const char LETTERS[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

const int LETTER_COUNT =
    sizeof(LETTERS) - 1;

int selectedLetter = 0;

String inputText = "";
String outputText = "";

const int MAX_TEXT_LENGTH = 64;

int lastEncoderState = 0;

unsigned long lastButtonTime = 0;

const unsigned long BUTTON_DELAY = 150;

void clearDisplay()
{
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
}

void refreshDisplay()
{
    display.display();
}

void showStartScreen()
{
    clearDisplay();

    display.setCursor(25, 4);
    display.println("HACKPAD");

    display.setCursor(16, 18);
    display.println("GRAVITY FALLS");

    refreshDisplay();
}

void showMainMenu()
{
    clearDisplay();

    display.println("SELECT MODE");

    display.setCursor(0, 12);

    if (menuIndex == 0)
        display.print("> ");
    else
        display.print("  ");

    display.println("ENCODE");

    display.setCursor(0, 22);

    if (menuIndex == 1)
        display.print("> ");
    else
        display.print("  ");

    display.println("DECODE");

    refreshDisplay();
}

void showCipherMenu()
{
    clearDisplay();

    display.println("SELECT CIPHER");

    display.setCursor(0, 12);

    if (menuIndex == 0)
        display.print("> THERAPRISM");
    else if (menuIndex == 1)
        display.print("> FORDESE");
    else
        display.print("> BILL");

    refreshDisplay();
}

void showTextEntry()
{
    clearDisplay();

    display.println("ENTER TEXT:");

    display.setCursor(0, 10);

    String visibleText = inputText;

    if (visibleText.length() > 20)
    {
        visibleText =
            visibleText.substring(
                visibleText.length() - 20
            );
    }

    display.println(visibleText);

    display.setCursor(0, 22);

    display.print("> ");
    display.print(LETTERS[selectedLetter]);

    display.print("  SELECT");

    refreshDisplay();
}

void showResult()
{
    clearDisplay();

    display.println("RESULT:");

    display.setCursor(0, 10);

    String visibleResult = outputText;

    if (visibleResult.length() > 20)
    {
        visibleResult =
            visibleResult.substring(
                visibleResult.length() - 20
            );
    }

    display.println(visibleResult);

    display.setCursor(0, 22);
    display.print("SW1: BACK");

    refreshDisplay();
}

void drawScreen()
{
    switch (screen)
    {
        case START_SCREEN:
            showStartScreen();
            break;

        case MAIN_MENU:
            showMainMenu();
            break;

        case CIPHER_MENU:
            showCipherMenu();
            break;

        case TEXT_ENTRY:
            showTextEntry();
            break;

        case RESULT_SCREEN:
            showResult();
            break;
    }
}

int readEncoder()
{
    int a = digitalRead(ENCODER_A);
    int b = digitalRead(ENCODER_B);

    int currentState =
        (a << 1) | b;

    int movement = 0;

    if (currentState != lastEncoderState)
    {
        if (
            (lastEncoderState == 0 && currentState == 1) ||
            (lastEncoderState == 1 && currentState == 3) ||
            (lastEncoderState == 3 && currentState == 2) ||
            (lastEncoderState == 2 && currentState == 0)
        )
        {
            movement = 1;
        }

        else if (
            (lastEncoderState == 0 && currentState == 2) ||
            (lastEncoderState == 2 && currentState == 3) ||
            (lastEncoderState == 3 && currentState == 1) ||
            (lastEncoderState == 1 && currentState == 0)
        )
        {
            movement = -1;
        }

        lastEncoderState =
            currentState;
    }

    return movement;
}

bool encoderPressed()
{
    static bool previous = HIGH;

    bool current =
        digitalRead(ENCODER_BUTTON);

    bool pressed =
        previous == HIGH &&
        current == LOW;

    previous = current;

    return pressed;
}

int readButton()
{
    if (
        millis() - lastButtonTime
        < BUTTON_DELAY
    )
    {
        return -1;
    }

    int pins[4] =
    {
        SW1_PIN,
        SW2_PIN,
        SW3_PIN,
        SW4_PIN
    };

    for (int i = 0; i < 4; i++)
    {
        if (digitalRead(pins[i]) == LOW)
        {
            lastButtonTime =
                millis();

            return i;
        }
    }

    return -1;
}

void moveMenu(int amount, int count)
{
    menuIndex += amount;

    if (menuIndex < 0)
        menuIndex = count - 1;

    if (menuIndex >= count)
        menuIndex = 0;
}

void moveLetter(int amount)
{
    selectedLetter += amount;

    if (selectedLetter < 0)
        selectedLetter = LETTER_COUNT - 1;

    if (selectedLetter >= LETTER_COUNT)
        selectedLetter = 0;
}

void addLetter()
{
    if (
        inputText.length()
        >= MAX_TEXT_LENGTH
    )
    {
        return;
    }

    inputText +=
        LETTERS[selectedLetter];
}

void deleteLetter()
{
    if (inputText.length() > 0)
    {
        inputText.remove(
            inputText.length() - 1
        );
    }
}

void runCipher()
{
    if (cipher == THERAPRISM)
    {
        if (mode == ENCODE_MODE)
            outputText =
                theraprismEncode(inputText);
        else
            outputText =
                theraprismDecode(inputText);
    }

    else if (cipher == FORDESE)
    {
        if (mode == ENCODE_MODE)
            outputText =
                fordeseEncode(inputText);
        else
            outputText =
                fordeseDecode(inputText);
    }

    else if (cipher == BILL)
    {
        if (mode == ENCODE_MODE)
            outputText =
                billEncode(inputText);
        else
            outputText =
                billDecode(inputText);
    }
}

void selectItem()
{
    if (screen == MAIN_MENU)
    {
        if (menuIndex == 0)
            mode = ENCODE_MODE;
        else
            mode = DECODE_MODE;

        menuIndex = 0;

        screen = CIPHER_MENU;

        drawScreen();
    }

    else if (screen == CIPHER_MENU)
    {
        cipher =
            (Cipher)menuIndex;

        inputText = "";
        outputText = "";

        selectedLetter = 0;

        screen = TEXT_ENTRY;

        drawScreen();
    }

    else if (screen == TEXT_ENTRY)
    {
        addLetter();

        drawScreen();
    }

    else if (screen == RESULT_SCREEN)
    {
        screen = MAIN_MENU;

        menuIndex = 0;

        drawScreen();
    }
}

void goBack()
{
    if (screen == MAIN_MENU)
    {
        return;
    }

    if (screen == CIPHER_MENU)
    {
        screen = MAIN_MENU;
        menuIndex = 0;
    }

    else if (screen == TEXT_ENTRY)
    {
        screen = CIPHER_MENU;
        menuIndex = cipher;

        inputText = "";
    }

    else if (screen == RESULT_SCREEN)
    {
        screen = TEXT_ENTRY;
    }

    drawScreen();
}

void setup()
{
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
    pinMode(SW3_PIN, INPUT_PULLUP);
    pinMode(SW4_PIN, INPUT_PULLUP);

    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_BUTTON, INPUT_PULLUP);

    Wire.setSDA(OLED_SDA);
    Wire.setSCL(OLED_SCL);

    Wire.begin();

    if (
        !display.begin(
            SSD1306_SWITCHCAPVCC,
            0x3C
        )
    )
    {
        while (true)
        {
            delay(100);
        }
    }

    lastEncoderState =
        (digitalRead(ENCODER_A) << 1) |
        digitalRead(ENCODER_B);

    showStartScreen();

    delay(1500);

    screen = MAIN_MENU;
    menuIndex = 0;

    drawScreen();
}

void loop()
{
    int movement =
        readEncoder();

    if (movement != 0)
    {
        if (screen == MAIN_MENU)
        {
            moveMenu(
                movement,
                2
            );

            drawScreen();
        }

        else if (screen == CIPHER_MENU)
        {
            moveMenu(
                movement,
                3
            );

            drawScreen();
        }

        else if (screen == TEXT_ENTRY)
        {
            moveLetter(movement);

            drawScreen();
        }
    }

    if (encoderPressed())
    {
        selectItem();
    }

    int button =
        readButton();

    if (button == 0)
    {
        goBack();
    }

    else if (button == 1)
    {
        selectItem();
    }

    else if (button == 2)
    {
        if (screen == TEXT_ENTRY)
        {
            runCipher();

            screen =
                RESULT_SCREEN;

            drawScreen();
        }
    }

    else if (button == 3)
    {
        if (screen == TEXT_ENTRY)
        {
            deleteLetter();

            drawScreen();
        }
    }
}
