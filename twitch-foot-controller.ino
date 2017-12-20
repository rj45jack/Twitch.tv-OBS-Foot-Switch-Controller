// Twitch.tv Foot Switch Controller
#include <TrinketKeyboard.h>

int button0 = 0;
int button1 = 1;
int button2 = 2;
int button5 = 5;

void setup()
{
	pinMode(button0, INPUT);
	pinMode(button1, INPUT);
	pinMode(button2, INPUT);
	pinMode(button5, INPUT);

	TrinketKeyboard.begin();
}


void loop()
{
	TrinketKeyboard.poll();
	delay(3);

	if (digitalRead(button0) == HIGH)
	{
		TrinketKeyboard.pressKey(0, KEYCODE_SPACE); // Button0 action or key
		TrinketKeyboard.pressKey(0, 0); // release the key press
	}

	if (digitalRead(button1) == HIGH)
	{
		TrinketKeyboard.pressKey(0, KEYCODE_X); // Button1 action or key
		TrinketKeyboard.pressKey(0, 0); // release the key press
	}

	if (digitalRead(button2) == HIGH)
	{
		TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_CONTROL, KEYCODE_LEFT_CONTROL); // Button2 action or key
		TrinketKeyboard.pressKey(0, 0); // release the key press
	}

	if (digitalRead(button5) == HIGH)
	{
		TrinketKeyboard.pressKey(KEYCODE_MOD_LEFT_SHIFT, KEYCODE_LEFT_SHIFT); // Button5 action or key
		TrinketKeyboard.pressKey(0, 0); // release the key press
	}
	
}