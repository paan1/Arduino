#define A 12
#define B 11
int num[] = { ~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F , 0xC2, 0xC7};
unsigned long int lastClickA = 0, lastClickB = 0, ranNO;
int i = 1;


void setup() {
  DDRD = 0xFF;
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  ranNO = random(1, 10);

  PORTD = num[i];
}



void loop() {
  int ReadA = digitalRead(A);
  int ReadB = digitalRead(B);


  if (ReadA == LOW) {
    if ((millis() - lastClickA) > 300)
    {
      lastClickA = millis();
      i++;
    }
    if (i == 10)i = 1;
    PORTD = num[i];
  }


  if (ReadB == LOW) {
    if ((millis() - lastClickB) > 300)
    {
      lastClickB = millis();
      if (i == ranNO) {
        PORTD = num[0];
        randomSeed(analogRead(A0));
        ranNO = random(1, 10);
      }
      else if (i > ranNO) {
        PORTD = num[10];

      }
      else if (i < ranNO) {
        PORTD = num[11];
      }
    }
  }
  if (millis() - lastClickB > 1000)PORTD = num[i];
}
