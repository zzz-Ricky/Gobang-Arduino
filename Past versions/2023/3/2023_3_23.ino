#include <TFT_eSPI.h>
#include <SPI.h>

// create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
float cursorX = 240.0;
float cursorY = 160.0;
int resetCount = 0;

// setup for button
const int buttonPin1 = 3;
const int buttonPin2 = 4;
const int buzzerPin = 13;

int buttonState1 = 0;
int buttonState2 = 0;
int userstate = 0;

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A7
#define JOYSTICK_Y_PIN A6
#define JOYSTICK_BUTTON_PIN 34

// Define colours in 4-digit hex
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define BROWN 0xFD6B

///////////////
void setup()
{

    Serial.begin(9600);
    // Initalize the screen and set the orientation correctly, then make sure it's clear.
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(0xFD6B); // fills screen with a brown background
    pinMode(buzzerPin, OUTPUT);

    // Set the cursor to the top left of the screen and print some text.
    tft.setCursor(0, 0);
    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.println("Tries to");
    tft.println("let five");
    tft.println("stones");
    tft.println("in a row");

    // Tell the arduino that the joystick button is on pin 9 and write it high so it gets set to 0 when pushed.
    // If you'd like to learn more about this check out the Elegoo documentation on the Joystick.
    pinMode(JOYSTICK_BUTTON_PIN, INPUT);
    digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);

    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
}

// Set the cursor movement
int x = 0;
int y = 0;
int resetCount_Button = 0;

// Start Game
void loop()
{
    float JoyStickX = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -1000, 1000) / 2000.0;
    float JoyStickY = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -1000, 1000) / 2000.0;

    if (JoyStickX > 0.02)
    {
        cursorX = cursorX + 36;
        tone(buzzerPin, 200, 90);
    }

    if (JoyStickX < -0.02)
    {
        cursorX = cursorX - 36;
        tone(buzzerPin, 200, 90);
    }

    if (JoyStickY > 0.02)
    {
        cursorY = cursorY + 36;
        tone(buzzerPin, 200, 90);
    }

    if (JoyStickY < -0.02)
    {
        cursorY = cursorY - 36;
        tone(buzzerPin, 200, 90);
    }
    Serial.println(JoyStickX);
    Serial.println(JoyStickY);

    ///////////////////////////================================If possible, try to put these codes in setup part and add extra one in reset button.
    //////=========================However, if do so, the display of the cursor will cover the line of the background board.
    //////=========================Need one plan to deal with that the board lines are above white stones.(Optional).
    tft.drawRect(96, 16, 288, 288, BLACK); // draw the square of the board
    // Then draw lines inside
    // Set up the board lines
    int i = 0;
    int j = 0;
    for (i = 132; i <= 348; i = i + 36)
    {
        tft.drawLine(i, 16, i, 304, BLACK);
    }
    for (j = 52; j <= 268; j = j + 36)
    {
        tft.drawLine(96, j, 384, j, BLACK);
    }
    // Limit the cursor to the bounds of the screen
    cursorX = constrain(cursorX, 96, 384);
    cursorY = constrain(cursorY, 16, 304);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Read the pin for the button in the joystick, and if pressed add one to resetCount. Otherwise, reset it to 0.
    if (digitalRead(JOYSTICK_BUTTON_PIN) == 0)
        resetCount += 1;
    else
        resetCount = 0;

    // If the button has been held for around a second, reset the canvas.
    if (resetCount > 1000)
    {

        Serial.print("Joystick Held down"); // placeholder function

        cursorX = 240.0;
        cursorY = 160.0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Set the correction for repetition
    buttonState1 = digitalRead(buttonPin1);
    buttonState2 = digitalRead(buttonPin2);
    int ColorState = 0x0000;
    for (; buttonState1 == HIGH;)
    {
        ColorState = tft.readPixel(int(cursorX) + 2, int(cursorY) + 2);
        if (ColorState == BLACK)
        {
            tone(buzzerPin, 300, 100);
            Serial.print("Spot is already black");
            delay(100);
            break;
        }
        if (ColorState == WHITE)
        {
            tone(buzzerPin, 300, 100);
            Serial.print("Spot is already white");
            delay(100);
            break;
        }
        else
        {
            if (userstate == 0)
            {
                tone(buzzerPin, 1200, 100);
                Serial.print("BLACK button pressed");
                tft.fillSmoothCircle(int(cursorX), int(cursorY), 15, BLACK, BLACK);
                userstate = 1;
                checkwinnerBLACK();
                delay(10);
                break;
            }
            else
            {
                tone(buzzerPin, 600, 100);
                Serial.print("WHITE button pressed");
                tft.fillSmoothCircle(int(cursorX), int(cursorY), 15, WHITE, WHITE);
                userstate = 0;
                ////========================================One unrun code;====================/////
                checkwinnerWHITE();
                delay(10);
                break;
            }
        }
        delay(25);
    }

    while (buttonState2 == HIGH)
    {
        resetCount_Button += 5;
        tone(buzzerPin, 800, 100);
        delay(1);
        if (resetCount_Button > 5000)
        {
            Serial.print("RESET PLACEHOLDER");
            tft.fillScreen(TFT_DARKGREY); // fills screen with a grey background
            tft.drawString("GAME DRAW! PLAYER RESET BOARD", 60, 148, 2);
            delay(3500);
            tft.fillScreen(0xFD6B);       // fills screen with a brown background
            Serial.print("cursor reset"); // placeholder function
            cursorX = 240.0;
            cursorY = 160.0;
            userstate = 0;
            delay(200);
            resetCount_Button = 0;

            break;
        }
        ///////===============================Correction without real run attempts=================================/////////////////
        if (buttonState2 != HIGH)
        {
            resetCount_Button = 0;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Finally, draw a pixel where the cursor currently is
    for (; userstate == 0;)
    {
        tft.drawSmoothCircle(int(cursorX), int(cursorY), 20, BLACK, BLACK);
        delay(100);
        tft.drawSmoothCircle(int(cursorX), int(cursorY), 20, BROWN, BROWN);
        delay(10);
        break;
    }

    // Finally, draw a pixel where the cursor currently is
    for (; userstate == 1;)
    {
        tft.drawSmoothCircle(int(cursorX), int(cursorY), 20, WHITE, WHITE);
        delay(100);
        tft.drawSmoothCircle(int(cursorX), int(cursorY), 20, BROWN, BROWN);
        delay(10);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkwinnerBLACK()
{
    checkwinnerBLACKHor();
    checkwinnerBLACKVer();
    checkwinnerBLACKDia();
}

void checkwinnerBLACKHor()
{
    //
    // check the winner
    int x = 0;
    int j = 0;
    int winner_countBlack = 0;
    int readcolor = 0;
    ///////
    for (x = 0; x <= 5; x = x + 1)
    {

        readcolor = tft.readPixel(int(cursorX) + 2 + 36 * x, int(cursorY) + 2);
        if (readcolor == BLACK)
        {
            winner_countBlack = winner_countBlack + 1;
        }
        else
        {
            break;
        }
    }
    x = 0;
    for (x = 1; x <= 5; x = x + 1)
    {
        readcolor = tft.readPixel(int(cursorX) + 2 - 36 * x, int(cursorY) + 2);
        if (readcolor == BLACK)
        {
            winner_countBlack = winner_countBlack + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countBlack >= 5)
    {
        Blackwin();
    }
    winner_countBlack = 0;
}

void checkwinnerBLACKVer()
{
    int j = 0;
    int winner_countBlackV = 0;
    int readcolor = 0;
    int colorplaceX = 0;
    int colorplaceY = 0;
    ///////
    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2;
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == BLACK)
        {
            winner_countBlackV = winner_countBlackV + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackV = winner_countBlackV + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2;
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == BLACK)
        {
            winner_countBlackV = winner_countBlackV + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackV = winner_countBlackV + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countBlackV >= 5)
    {
        Blackwin();
    }
    winner_countBlackV = 0;
}

/////////////////////
/////////////////////
void checkwinnerBLACKDia()
{
    int j = 0;
    int winner_countBlackD1 = 0;
    int winner_countBlackD2 = 0;
    int readcolor = 0;
    int colorplaceX = 0;
    int colorplaceY = 0;
    ////////
    ///////
    // from top right corner to left bottom.
    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 + (36 * j);
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == BLACK)
        {
            winner_countBlackD1 = winner_countBlackD1 + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackD1 = winner_countBlackD1 + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 - (36 * j);
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == BLACK)
        {
            winner_countBlackD1 = winner_countBlackD1 + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackD1 = winner_countBlackD1 + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countBlackD1 >= 5)
    {
        Blackwin();
    }

    winner_countBlackD1 = 0;

    ////////
    ///////
    // from top left corner to right bottom.

    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 + (36 * j);
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == BLACK)
        {
            winner_countBlackD2 = winner_countBlackD2 + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackD2 = winner_countBlackD2 + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 - (36 * j);
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == BLACK)
        {
            winner_countBlackD2 = winner_countBlackD2 + 1;
            break;
        }
        if (readcolor == BLACK)
        {
            winner_countBlackD2 = winner_countBlackD2 + 1;
        }
        else
        {
            break;
        }
    }

    if (winner_countBlackD2 >= 5)
    {
        Blackwin();
    }

    winner_countBlackD2 = 0;
}

void checkwinnerWHITE()
{
    checkwinnerWHITEHor();
    checkwinnerWHITEVer();
    checkwinnerWHITEDia();
}

void checkwinnerWHITEHor()
{
    //
    // check the winner
    int x = 0;
    int j = 0;
    int winner_countWhite = 0;
    int readcolor = 0;
    // horziontal
    for (x = 0; x <= 5; x = x + 1)
    {

        readcolor = tft.readPixel(int(cursorX) + 2 + 36 * x, int(cursorY) + 2);
        if (readcolor == WHITE)
        {
            winner_countWhite = winner_countWhite + 1;
        }
        else
        {
            break;
        }
    }
    x = 0;
    for (x = 1; x <= 5; x = x + 1)
    {
        readcolor = tft.readPixel(int(cursorX) + 4 - 36 * x, int(cursorY) + 4);
        if (readcolor == WHITE)
        {
            winner_countWhite = winner_countWhite + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countWhite >= 5)
    {
        Whitewin();
    }
    winner_countWhite = 0;
}

void checkwinnerWHITEVer()
{
    int j = 0;
    int winner_countWhiteV = 0;
    int readcolor = 0;
    int colorplaceX = 0;
    int colorplaceY = 0;

    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2;
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == WHITE)
        {
            winner_countWhiteV = winner_countWhiteV + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteV = winner_countWhiteV + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2;
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == WHITE)
        {
            winner_countWhiteV = winner_countWhiteV + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteV = winner_countWhiteV + 1;
        }
        else
        {
            break;
        }
    }

    if (winner_countWhiteV >= 5)
    {
        Whitewin();
    }
    winner_countWhiteV = 0;
}

/////////////////////
/////////////////////
void checkwinnerWHITEDia()
{
    int j = 0;
    int winner_countWhiteD1 = 0;
    int winner_countWhiteD2 = 0;
    int readcolor = 0;
    int colorplaceX = 0;
    int colorplaceY = 0;
    ////////
    ///////
    // from top right corner to left bottom.
    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 + (36 * j);
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == WHITE)
        {
            winner_countWhiteD1 = winner_countWhiteD1 + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteD1 = winner_countWhiteD1 + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 - (36 * j);
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == WHITE)
        {
            winner_countWhiteD1 = winner_countWhiteD1 + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteD1 = winner_countWhiteD1 + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countWhiteD1 >= 5)
    {
        Whitewin();
    }

    winner_countWhiteD1 = 0;

    ////////
    ///////
    // from top left corner to right bottom.

    for (j = 0; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 + (36 * j);
        colorplaceY = int(cursorY) + 2 - (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);

        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 18 & readcolor == WHITE)
        {
            winner_countWhiteD2 = winner_countWhiteD2 + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteD2 = winner_countWhiteD2 + 1;
        }
        else
        {
            break;
        }
    }
    j = 0;
    for (j = 1; j <= 5; j = j + 1)
    {
        colorplaceX = int(cursorX) + 2 - (36 * j);
        colorplaceY = int(cursorY) + 2 + (36 * j);
        colorplaceY = constrain(colorplaceY, 18, 306);
        colorplaceX = constrain(colorplaceX, 98, 386);
        readcolor = tft.readPixel(colorplaceX, colorplaceY);
        if (colorplaceY == 306 & readcolor == WHITE)
        {
            winner_countWhiteD2 = winner_countWhiteD2 + 1;
            break;
        }
        if (readcolor == WHITE)
        {
            winner_countWhiteD2 = winner_countWhiteD2 + 1;
        }
        else
        {
            break;
        }
    }
    if (winner_countWhiteD2 >= 5)
    {
        Whitewin();
    }

    winner_countWhiteD2 = 0;
}

void Blackwin()
{
    tone(buzzerPin, 900, 100);
    delay(0.25);
    tone(buzzerPin, 1100, 100);
    delay(0.25);
    tone(buzzerPin, 1300, 100);

    Serial.print("BLACK WINS");
    tft.fillScreen(BLACK); // fills screen with a grey background
    tft.setTextColor(WHITE);
    tft.drawString("PLAYER BLACK WINS ", 120, 172, 2);
    tft.drawString("GAME RESET", 156, 136, 2);
    winnermusic();
    delay(3500);
    tft.fillScreen(0xFD6B);       // fills screen with a brown background
    Serial.print("cursor reset"); // placeholder function
    cursorX = 240.0;
    cursorY = 160.0;
    userstate = 0;

    delay(200);
}

void Whitewin()
{
    tone(buzzerPin, 900, 100);
    delay(0.25);
    tone(buzzerPin, 1100, 100);
    delay(0.25);
    tone(buzzerPin, 1300, 100);

    Serial.print("WHITE WINS");
    tft.fillScreen(WHITE); // fills screen with a grey background
    tft.setTextColor(BLACK);
    tft.drawString("PLAYER BLACK WINS ", 120, 172, 2);
    tft.drawString("GAME RESET", 156, 136, 2);
    winnermusic();
    delay(3500);
    tft.fillScreen(0xFD6B);       // fills screen with a brown background
    Serial.print("cursor reset"); // placeholder function
    cursorX = 240.0;
    cursorY = 160.0;
    userstate = 0;

    delay(200);
}

int melody[] = { // Define the melody as an array of notes
    659, 659, 0, 659, 0, 523, 659, 0, 784};
int noteDuration = 200; // Set the duration of each note
int pauseBetweenNotes = 50;
void winnermusic()
{
    for (int i = 0; i < 9; i++)
    {                         // Loop through each note in the melody
        int note = melody[i]; // Get the current note frequency
        if (note != 0)
        {                                            // If the note is not a rest
            tone(buzzerPin, note, noteDuration);     // Play the note
            delay(noteDuration + pauseBetweenNotes); // Wait for the note to finish
        }
        else
        {                                            // If the note is a rest
            delay(noteDuration + pauseBetweenNotes); // Just wait for the duration of the rest
        }
    }
    delay(20); // Wait for 2 seconds before playing the melody again
}
