#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>
#include <EEPROM.h>

#define BT_STOP 3
#define BT_SELECT 4
#define BT_MODE 5
#define LDR 6
#define BUZZER 7
#define DEBOUNCE 250
#define CLOCK 1
#define ALARM 2
#define COUNT 3
#define BAR 6

Adafruit_SSD1306 display(128, 32, &Wire);

int Mode[3] = {CLOCK, ALARM, COUNT};
int Mode_n = 0;
int addr = 0;

int Display_status = 0;
int Display_BG[2] = {1, 0};
int Display_Text[2] = {0, 1};
int Display_cursor[2] = {0, 1};
int Display_Text_cursor[2] = {1, 0};
int Display_frame[2] = {0, 1};

bool cursor_state = 0;
int pattern = 0;
unsigned long int last_state_cursor = 0;

int Hour_time_now = 0;
int Minute_time_now = 0;
int Sec_time_now = 0;

int Hour_time_alarm = 0;
int Minute_time_alarm = 0;

int Hour_time_count = 0;
int Minute_time_count = 0;
int Sec_time_count = 0;

int BT[3] = {BT_MODE, BT_SELECT, BT_STOP};
unsigned long int last_time_click[3] = {0, 0, 0};
int Readswitch[3];
int Read_BAR;

int Clock_mode = 0, Alarm_mode = 0, Count_mode = 0;
int state_alarm = 0, state_count = 0;

bool BT_STOP_state = 1;
void pull_EEPROM()
{
    addr = 0;
    EEPROM.get(addr, Hour_time_now);
    addr += sizeof(Hour_time_now);
    EEPROM.get(addr, Minute_time_now);
    addr += sizeof(Minute_time_now);
    EEPROM.get(addr, Hour_time_alarm);
    addr += sizeof(Hour_time_alarm);
    EEPROM.get(addr, Minute_time_alarm);
    addr += sizeof(Minute_time_alarm);
    EEPROM.get(addr, state_alarm);
}
void setup()
{
    pull_EEPROM();
    pinMode(BT_MODE, INPUT_PULLUP);
    pinMode(BT_SELECT, INPUT_PULLUP);
    pinMode(BT_STOP, INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(increasing_sec);
    Serial.begin(9600);
}
void reset_time_count()
{
    Hour_time_count = 0;
    Minute_time_count = 0;
    Sec_time_count = 0;
}
void set_time()
{
    addr = 0;
    EEPROM.put(addr, Hour_time_now);
    addr += sizeof(Hour_time_now);
    EEPROM.put(addr, Minute_time_now);
}
void set_alarm_time()
{
    addr = sizeof(Hour_time_now) + sizeof(Minute_time_now);
    EEPROM.put(addr, Hour_time_alarm);
    addr += sizeof(Hour_time_alarm);
    EEPROM.put(addr, Minute_time_alarm);
    addr += sizeof(Minute_time_now);
    EEPROM.put(addr, state_alarm);
}
void loop()
{
    Display_status = digitalRead(LDR);
    Read_BAR = analogRead(BAR);
    for (int i = 0; i < 3; i++)
    {
        Readswitch[i] = digitalRead(BT[i]);
    }
    if (Readswitch[2] == 0 && millis() - last_time_click[2] > DEBOUNCE && Mode[Mode_n] == 3)
    {
        state_count = !state_count;
        last_time_click[2] = millis();
    }
    else if (Readswitch[2] == 0 && millis() - last_time_click[2] > DEBOUNCE && Hour_time_alarm == Hour_time_now && Minute_time_alarm == Minute_time_now)
    {
        BT_STOP_state = 1;
        last_time_click[2] = millis();
    }

    if (Readswitch[0] == 0 && millis() - last_time_click[0] > DEBOUNCE)
    {
        Mode_n++;
        Alarm_mode = 0;
        Clock_mode = 0;
        Count_mode = 0;

        if (Mode_n >= 3)
        {
            Mode_n = 0;
        }
        last_time_click[0] = millis();
    }
    switch (Mode[Mode_n])
    {
    case 1:
        Clock();
        break;
    case 2:
        Alarm();
        break;
    case 3:
        count_time();
        break;
    }
}
void Clock()
{

    if (Readswitch[1] == 0 && millis() - last_time_click[1] > DEBOUNCE)
    {
        if (++Clock_mode >= 4)
            Clock_mode = 0;
        last_time_click[1] = millis();
    }
    switch (Clock_mode)
    {
    case 0:
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Clock");
        display.setTextSize(3);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(14, 8);
        if (Hour_time_now < 10)
            display.print("0");
        display.print(Hour_time_now);
        display.print(":");
        if (Minute_time_now < 10)
            display.print("0");
        display.print(Minute_time_now);
        display.setCursor(101, 22);
        display.setTextSize(1);
        display.print(".");
        if (Sec_time_now < 10)
            display.print("0");
        display.print(Sec_time_now);
        display.display();
        break;
    case 1 : 
    int set_time_q;
    int Readswitch_clock;
    while (Readswitch[1]!=0)
    {
     if (analogRead(BAR) >= 800)
            set_time_q = 0;
        else if (analogRead(BAR) <= 240)
            set_time_q = 1;
    display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Clock");
        if (set_time_q == 1)
        {
            display.fillRect(103, 4, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("YES");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("NO");
            Clock_mode = 1;
        }
        else
        {
            display.fillRect(103, 16, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("YES");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("NO");
            Clock_mode = -1;
        }
        display.display(); 
        Readswitch[1]=digitalRead(BT[1]);

    }
        break;
    case 2:
        Hour_time_now = map(analogRead(BAR), 0, 1023, 0, 24);
        if (Hour_time_now == 24)
            Hour_time_now = 23;
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Clock");
        if (cursor_state)
            display.fillRect(18, 8, 37, 22, Display_cursor[Display_status]);
        if (millis() - last_state_cursor > 500)
        {
            cursor_state = !cursor_state;
            last_state_cursor = millis();
        }

        display.setTextSize(3);
        if (cursor_state)
            display.setTextColor(Display_Text_cursor[Display_status]);
        else
            display.setTextColor(Display_Text[Display_status]);
        display.setCursor(20, 9);
        if (Hour_time_now < 10)
            display.print("0");
        display.print(Hour_time_now);
        display.setTextColor(Display_Text[Display_status]);
        display.print(":");
        if (Minute_time_now < 10)
            display.print("0");
        display.print(Minute_time_now);
        display.display();
        break;
    case 3:
        Minute_time_now = map(analogRead(BAR), 0, 1023, 0, 60);
        if (Hour_time_now == 60)
            Hour_time_now = 59;
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Clock");
        if (cursor_state)
            display.fillRect(72, 8, 37, 22, Display_cursor[Display_status]);
        if (millis() - last_state_cursor > 500)
        {
            cursor_state = !cursor_state;
            last_state_cursor = millis();
        }
        display.setTextSize(3);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(20, 9);
        if (Hour_time_now < 10)
            display.print("0");
        display.print(Hour_time_now);
        display.print(":");
        display.setTextColor(Display_Text_cursor[Display_status]);
        if (Minute_time_now < 10)
            display.print("0");
        display.print(Minute_time_now);
        display.display();
        break;
    }
}
void Alarm()
{
    if (Readswitch[1] == 0 && millis() - last_time_click[1] > DEBOUNCE)
    {
        if (++Alarm_mode >= 4)
            Alarm_mode = 0;
        last_time_click[1] = millis();
    }
    switch (Alarm_mode)
    {
    case 0:
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Alarm");
        display.setTextSize(3);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(10, 8);
        if (Hour_time_alarm < 10)
            display.print("0");
        display.print(Hour_time_alarm);
        display.print(":");
        if (Minute_time_alarm < 10)
            display.print("0");
        display.print(Minute_time_alarm);
        display.setCursor(101, 20);
        display.setTextSize(1);
        if (state_alarm == 1)
        {
            display.fillRect(103, 4, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("OFF");
        }
        else
        {
            display.fillRect(103, 16, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("OFF");
        }
        display.display();
        break;
    case 1:
        Hour_time_alarm = map(analogRead(BAR), 0, 1023, 0, 24);
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Alarm");
        if (cursor_state)
            display.fillRect(8, 8, 37, 22, Display_cursor[Display_status]);
        if (millis() - last_state_cursor > 500)
        {
            cursor_state = !cursor_state;
            last_state_cursor = millis();
        }

        display.setTextSize(3);
        if (cursor_state)
            display.setTextColor(Display_Text_cursor[Display_status]);
        else
            display.setTextColor(Display_Text[Display_status]);
        display.setCursor(10, 9);
        if (Hour_time_alarm < 10)
            display.print("0");
        display.print(Hour_time_alarm);
        display.setTextColor(Display_Text[Display_status]);
        display.print(":");
        if (Minute_time_alarm < 10)
            display.print("0");
        display.print(Minute_time_alarm);
        display.setCursor(101, 20);
        display.setTextSize(1);
        if (state_alarm == 1)
        {
            display.fillRect(103, 4, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("OFF");
        }
        else
        {
            display.fillRect(103, 16, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("OFF");
        }
        display.display();
        break;
    case 2:
        Minute_time_alarm = map(analogRead(BAR), 0, 1023, 0, 60);
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Alarm");
        if (cursor_state)
            display.fillRect(63, 8, 37, 22, Display_cursor[Display_status]);
        if (millis() - last_state_cursor > 500)
        {
            cursor_state = !cursor_state;
            last_state_cursor = millis();
        }
        display.setTextSize(3);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(10, 9);
        if (Hour_time_alarm < 10)
            display.print("0");
        display.print(Hour_time_alarm);
        display.print(":");
        display.setTextColor(Display_Text_cursor[Display_status]);
        if (Minute_time_alarm < 10)
            display.print("0");
        display.print(Minute_time_alarm);
        display.setCursor(101, 20);
        display.setTextSize(1);
        if (state_alarm == 1)
        {
            display.fillRect(103, 4, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("OFF");
        }
        else
        {
            display.fillRect(103, 16, 22, 11, Display_cursor[Display_status]);
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("OFF");
        }
        display.display();
        break;
    case 3:
        if (analogRead(BAR) >= 800)
            state_alarm = 0;
        else if (analogRead(BAR) <= 240)
            state_alarm = 1;
        display.clearDisplay();
        display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
        display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
        display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
        display.setTextSize(1);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(3, 0);
        display.print("Alarm");
        display.setTextSize(3);
        display.setTextColor(Display_Text[Display_status]);
        display.setCursor(10, 9);
        if (Hour_time_alarm < 10)
            display.print("0");
        display.print(Hour_time_alarm);
        display.print(":");
        if (Minute_time_alarm < 10)
            display.print("0");
        display.print(Minute_time_alarm);
        display.setCursor(101, 20);
        display.setTextSize(1);
        if (state_alarm == 1)
        {
            if (cursor_state)
                display.fillRect(103, 4, 22, 11, Display_cursor[Display_status]);
            if (millis() - last_state_cursor > 500)
            {
                cursor_state = !cursor_state;
                last_state_cursor = millis();
            }
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("OFF");
            BT_STOP_state = 0;
        }
        else
        {
            if (cursor_state)
                display.fillRect(103,16, 22, 11, Display_cursor[Display_status]);
            if (millis() - last_state_cursor > 500)
            {
                cursor_state = !cursor_state;
                last_state_cursor = millis();
            }
            display.setCursor(105, 6);
            display.setTextSize(1);
            display.setTextColor(Display_Text[Display_status]);
            display.print("ON");
            display.setCursor(105, 18);
            display.setTextSize(1);
            display.setTextColor(Display_Text_cursor[Display_status]);
            display.print("OFF");
        }
        display.display();

        break;
    }
}

void count_time()
{
    if (Readswitch[1] == 0 && millis() - last_time_click[1] > DEBOUNCE && state_count == 0 && Mode[Mode_n] == 3)
    {
        reset_time_count();
    }
    display.clearDisplay();
    display.fillRect(0, 0, 128, 32, Display_BG[Display_status]);
    display.drawRect(1, 2, 125, 29, Display_frame[Display_status]);
    display.fillRect(2, 0, 30, 8, Display_BG[Display_status]);
    display.setTextSize(1);
    display.setTextColor(Display_Text[Display_status]);
    display.setCursor(3, 0);
    display.print("Count");
    display.setTextSize(3);
    display.setTextColor(Display_Text[Display_status]);
    display.setCursor(3, 10);
    if (Hour_time_count < 10)
        display.print("0");
    display.print(Hour_time_count);
    display.print(":");
    if (Minute_time_count < 10)
        display.print("0");
    display.print(Minute_time_count);
    display.setCursor(90, 13);
    display.setTextSize(2);
    display.print(".");
    if (Sec_time_count < 10)
        display.print("0");
    display.print(Sec_time_count);
    display.display();
}

void alarm_clock_sound()
{
    tone(BUZZER, 250, 500);
}

void increasing_sec()
{
    if (++Sec_time_now >= 60)
    {
        Sec_time_now = 0;
        Minute_time_now++;
        set_time();
        set_alarm_time();
    };
    if (Minute_time_now >= 60)
    {
        Minute_time_now = 0;
        Hour_time_now++;
    };

    if (Hour_time_now >= 24)
        Hour_time_now = 0;

    //====================================
    if (state_count == 1)
    {
        if (++Sec_time_count >= 60)
        {
            Sec_time_count = 0;
            Minute_time_count++;
        };
        if (Minute_time_count >= 60)
        {
            Minute_time_count = 0;
            Hour_time_count++;
        };
        if (Hour_time_count >= 24)
            Hour_time_count = 0;
    }
    //=====================================
    if (state_alarm == 1 && BT_STOP_state == 0 && Alarm_mode == 0)
    {
        if (Hour_time_alarm == Hour_time_now && Minute_time_alarm == Minute_time_now)
        {
            alarm_clock_sound();
            state_alarm == 0;
        }
    }
}
