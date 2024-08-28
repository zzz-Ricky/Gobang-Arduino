void menU()
{
    draw background
        draw text
}

void setup()
{
    menu();
}

void loop()[if joystick is pressed
                menu = 1

            if menu = 0 break or play music

                      if menu = 1 draw board]

    -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --void Menu()
{
    tft.fillScreen(WHITE); // fills screen with a grey background
    tft.setTextColor(BLACK);
    tft.drawString("GOBANG", 120, 172, 2);
    tft.drawString("HOLD DOWN JOYSTICK TO BEGIN", 156, 136, 1);
    tft.drawString("CONNECT 5 TO WIN", 192, 136, 1);
}

voil loop()
{
    if (digitalRead(JOYSTICK_BUTTON_PIN) == 0)
        Menuhold += 1;

    if (Menuhold > 1000)
    {

        // show controls

        tft.fillScreen(WHITE); // fills screen with a grey background
        tft.setTextColor(BLACK);
        tft.drawString("<LOADING>", 120, 172, 2);
        tft.drawString("Press Right button to place stone", 120, 172, 1);
        tft.drawString("Press Left button to reset board", 156, 136, 1)

            delay(5000) :

                          display board
    }
}
