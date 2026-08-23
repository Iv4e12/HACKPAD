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

#define BUTTON_BACK   0
#define BUTTON_ENTER  1
#define BUTTON_FINISH 2
#define BUTTON_DELETE 3

enum Screen
{
  SCREEN_START,
  SCREEN_MAIN,
  SCREEN_CIPHER,
  SCREEN_TEXT,
  SCREEN_RESULT
};

Screen currentScreen = SCREEN_START;

enum Mode
{
  MODE_ENCODE,
  MODE_DECODE
};

Mode selectedMode = MODE_ENCODE;

enum Cipher
{
  CIPHER_THERAPRISM,
  CIPHER_FORDESE,
  CIPHER_BILL
};

Cipher selectedCipher = CIPHER_THERAPRISM;

int menuSelection = 0;

const char LETTERS[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

const int LETTER_COUNT =
sizeof(LETTERS) - 1;

int selectedLetter = 0;

String inputText = "";
String outputText = "";

const int MAX_TEXT_LENGTH = 64;

int lastEncoderState = 0;

unsigned long lastEncoderMove = 0;

const unsigned long ENCODER_DELAY = 2;

unsigned long lastButtonPress = 0;

const unsigned long BUTTON_DELAY = 150;

void clearDisplay()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);
}

void drawStartScreen()
{
  clearDisplay();

  display.setCursor(25, 5);
  display.println("HACKPAD");

  display.setCursor(22, 18);
  display.println("GRAVITY FALLS");

  display.display();
}

void drawMainMenu()
{
  clearDisplay();

  display.println("MODE");

  display.setCursor(0, 12);

  if (menuSelection == 0)
    display.print("> ");
  else
    display.print("  ");

  display.println("ENCODE");

  display.setCursor(0, 22);

  if (menuSelection == 1)
    display.print("> ");
  else
    display.print("  ");

  display.println("DECODE");

  display.display();
}

void drawCipherMenu()
{
  clearDisplay();

  display.println("SELECT CIPHER");

  display.setCursor(0, 11);

  if (menuSelection == 0)
  {
    display.print("> THERAPRISM");
  }
  else if (menuSelection == 1)
  {
    display.print("> FORDESE");
  }
  else
  {
    display.print("> BILL");
  }

  display.setCursor(0, 21);

  display.print("Turn wheel");

  display.display();
}

void drawTextScreen()
{
  clearDisplay();

  display.println("TEXT:");

  display.setCursor(0, 11);

  String shownText = inputText;

  if (shownText.length() > 20)
  {
    shownText =
    shownText.substring(
      shownText.length() - 20
    );
  }

  display.print(shownText);

  display.setCursor(0, 22);

  display.print("> ");

  display.print(LETTERS[selectedLetter]);

  display.print("  ENTER");

  display.display();
}

void drawResultScreen()
{
  clearDisplay();

  display.println("RESULT:");

  display.setCursor(0, 11);

  String shownResult = outputText;

  if (shownResult.length() > 20)
  {
    shownResult =
    shownResult.substring(
      shownResult.length() - 20
    );
  }

  display.print(shownResult);

  display.setCursor(0, 22);
  display.print("SW1 = BACK");

  display.display();
}

void drawCurrentScreen()
{
  switch (currentScreen)
  {
    case SCREEN_START:
      drawStartScreen();
      break;

    case SCREEN_MAIN:
      drawMainMenu();
      break;

    case SCREEN_CIPHER:
      drawCipherMenu();
      break;

    case SCREEN_TEXT:
      drawTextScreen();
      break;

    case SCREEN_RESULT:
      drawResultScreen();
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

bool encoderButtonPressed()
{
  static bool previousState = HIGH;

  bool currentState =
  digitalRead(ENCODER_BUTTON);

  bool pressed =
  previousState == HIGH &&
  currentState == LOW;

  previousState =
  currentState;

  return pressed;
}

int readPhysicalButton()
{
  if (
    millis() - lastButtonPress
    < BUTTON_DELAY
  )
  {
    return -1;
  }

  int pins[] =
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
      lastButtonPress =
      millis();

      return i;
    }
  }

  return -1;
}

void moveMenu(int amount, int itemCount)
{
  menuSelection += amount;

  if (menuSelection < 0)
  {
    menuSelection =
    itemCount - 1;
  }

  if (menuSelection >= itemCount)
  {
    menuSelection = 0;
  }
}

void moveLetter(int amount)
{
  selectedLetter += amount;

  if (selectedLetter < 0)
  {
    selectedLetter =
    LETTER_COUNT - 1;
  }

  if (selectedLetter >= LETTER_COUNT)
  {
    selectedLetter = 0;
  }
}

void addSelectedLetter()
{
  if (
    inputText.length()
    >= MAX_TEXT_LENGTH
  )
  {
    return;
  }

  char selected =
  LETTERS[selectedLetter];

  inputText += selected;
}

void deleteLastCharacter()
{
  if (inputText.length() == 0)
  {
    return;
  }

  inputText.remove(
    inputText.length() - 1
  );
}

void runCipher()
{
  if (
    selectedCipher ==
    CIPHER_THERAPRISM
  )
  {
    if (
      selectedMode ==
      MODE_ENCODE
    )
    {
      outputText =
      theraprismEncode(
        inputText
      );
    }
    else
    {
      outputText =
      theraprismDecode(
        inputText
      );
    }
  }

  else if (
    selectedCipher ==
    CIPHER_FORDESE
  )
  {
    if (
      selectedMode ==
      MODE_ENCODE
    )
    {
      outputText =
      fordeseEncode(
        inputText
      );
    }
    else
    {
      outputText =
      fordeseDecode(
        inputText
      );
    }
  }

  else if (
    selectedCipher ==
    CIPHER_BILL
  )
  {
    if (
      selectedMode ==
      MODE_ENCODE
    )
    {
      outputText =
      billEncode(
        inputText
      );
    }
    else
    {
      outputText =
      billDecode(
        inputText
      );
    }
  }
}

void selectCurrentItem()
{
  if (
    currentScreen ==
    SCREEN_MAIN
  )
  {
    if (menuSelection == 0)
    {
      selectedMode =
      MODE_ENCODE;
    }
    else
    {
      selectedMode =
      MODE_DECODE;
    }

    menuSelection = 0;

    currentScreen =
    SCREEN_CIPHER;

    drawCurrentScreen();

    return;
  }

  if (
    currentScreen ==
    SCREEN_CIPHER
  )
  {
    selectedCipher =
    (Cipher)menuSelection;

    inputText = "";

    outputText = "";

    selectedLetter = 0;

    currentScreen =
    SCREEN_TEXT;

    drawCurrentScreen();

    return;
  }

  if (
    currentScreen ==
    SCREEN_TEXT
  )
  {
    addSelectedLetter();

    drawCurrentScreen();

    return;
  }

  if (
    currentScreen ==
    SCREEN_RESULT
  )
  {
    currentScreen =
    SCREEN_MAIN;

    menuSelection = 0;

    drawCurrentScreen();

    return;
  }
}

void goBack()
{
  switch (currentScreen)
  {
    case SCREEN_MAIN:

      break;

    case SCREEN_CIPHER:

      currentScreen =
      SCREEN_MAIN;

      menuSelection = 0;

      break;

    case SCREEN_TEXT:

      currentScreen =
      SCREEN_CIPHER;

      menuSelection =
      (int)selectedCipher;

      inputText = "";

      break;

    case SCREEN_RESULT:

      currentScreen =
      SCREEN_TEXT;

      break;

    default:

      currentScreen =
      SCREEN_MAIN;

      break;
  }

  drawCurrentScreen();
}

void setup()
{
  pinMode(
    SW1_PIN,
    INPUT_PULLUP
  );

  pinMode(
    SW2_PIN,
    INPUT_PULLUP
  );

  pinMode(
    SW3_PIN,
    INPUT_PULLUP
  );

  pinMode(
    SW4_PIN,
    INPUT_PULLUP
  );

  pinMode(
    ENCODER_A,
    INPUT_PULLUP
  );

  pinMode(
    ENCODER_B,
    INPUT_PULLUP
  );

  pinMode(
    ENCODER_BUTTON,
    INPUT_PULLUP
  );

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

  drawStartScreen();

  delay(1500);

  currentScreen =
  SCREEN_MAIN;

  menuSelection = 0;

  drawCurrentScreen();
}

void loop()
{
  int movement =
  readEncoder();

  if (
    movement != 0 &&
    millis() - lastEncoderMove
    >= ENCODER_DELAY
  )
  {
    lastEncoderMove =
    millis();

    if (
      currentScreen ==
      SCREEN_MAIN
    )
    {
      moveMenu(
        movement,
        2
      );

      drawCurrentScreen();
    }

    else if (
      currentScreen ==
      SCREEN_CIPHER
    )
    {
      moveMenu(
        movement,
        3
      );

      drawCurrentScreen();
    }

    else if (
      currentScreen ==
      SCREEN_TEXT
    )
    {
      moveLetter(
        movement
      );

      drawCurrentScreen();
    }
  }

  if (
    encoderButtonPressed()
  )
  {
    selectCurrentItem();
  }

  int button =
  readPhysicalButton();

  if (button != -1)
  {
    if (
      button ==
      BUTTON_BACK
    )
    {
      goBack();
    }

    else if (
      button ==
      BUTTON_ENTER
    )
    {
      selectCurrentItem();
    }

    else if (
      button ==
      BUTTON_FINISH
    )
    {
      if (
        currentScreen ==
        SCREEN_TEXT
      )
      {
        runCipher();

        currentScreen =
        SCREEN_RESULT;

        drawCurrentScreen();
      }
    }

    else if (
      button ==
      BUTTON_DELETE
    )
    {
      if (
        currentScreen ==
        SCREEN_TEXT
      )
      {
        deleteLastCharacter();

        drawCurrentScreen();
      }
    }
  }
}
