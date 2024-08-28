if (JoyStickX > 0.05)
{
    cursorX = cursorX + 36;
    tone(buzzerPin, 200, 90);
}

if (JoyStickX < -0.05)
{
    cursorX = cursorX - 36;
    tone(buzzerPin, 200, 90);
}

if (JoyStickY > 0.05)
{
    cursorY = cursorY + 36;
    tone(buzzerPin, 200, 90);
}

if (JoyStickY < -0.05)
{
    cursorY = cursorY - 36;
    tone(buzzerPin, 200, 90);
}
