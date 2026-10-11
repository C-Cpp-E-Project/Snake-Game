#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, 2, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);

uint32_t t, t1 = 500;
int x = 2, y = 6, z;
int z1[64][2], z2, z3;
int x1, y1;
bool A = true, B = false, C = false, D = false, E = true;
int c1, c2;

void display() {
  if (c1 <= 254 && c2 == 0)
    matrix.drawPixel(x, y, matrix.Color(256 - (c1 += 2), c1, 0));
  else if (c2 <= 254 && c1 >= 256)
    matrix.drawPixel(x, y, matrix.Color(0, 256 - (c2 += 2), c2));
  else
    matrix.drawPixel(x, y, matrix.Color(255 - (c2 -= 2), 0, c1 -= 2));
  matrix.show();
}

void over() {
  digitalWrite(12, 1);
  digitalWrite(11, 0);
  lcd.setCursor(0, 0);
  lcd.print("---Game Over!---");
  lcd.setCursor(0, 1);
  lcd.print("Score = " + String(z));
  for (int i = 0; i < 20; i++) {
    if (i == 9) {
      Serial.println("---------GAME OVER...---------");
      Serial.println("Score = " + String(z));
      continue;
    }
    Serial.println(" ");
  }
  for (int i = 0; i < 64; i++)
    matrix.setPixelColor(i, 0);
  while (1)
    if (Serial.available() && Serial.read() == 32) {
      matrix.show();
      digitalWrite(13, 1);
    }
}

void start() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score = 0");
  t = millis() + 250;
  digitalWrite(11, 1);
  digitalWrite(12, 0);
  randomSeed(micros());
  x1 = random(1, 7), y1 = random(0, 5);
  matrix.drawPixel(x1, y1, matrix.Color(255, 255, 0));
  matrix.show();
}

void check() {
  while (matrix.getPixelColor(x1 + y1 * 8) > 0)
    x1 = random(0, 7), y1 = random(0, 7);
  matrix.drawPixel(x1, y1, matrix.Color(255, 255, 0));
  lcd.setCursor(8, 0);
  lcd.print(++z);
  t1 -= 7;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Press the Enter key to play.");
  matrix.begin();
  matrix.setBrightness(255);
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("---Snake Game!---");
  for (int i = 11; i < 14; i++)
    pinMode(i, OUTPUT);
}

void loop() {
  if (Serial.available() && Serial.read() == 13) {
    start();
    while (1) {
      if (millis() >= t) {
        t += t1;
        if (A == true) ++x;
        else if (B == true) ++y;
        else if (C == true) --x;
        else if (D == true) --y;

        if (z2 == 63) z2 = 0;
        else if (z3 == 64) z3 = 1;
        z1[++z2][0] = x, z1[z2][1] = y;

        if ((x == x1) && (y == y1)) check();
        else if (matrix.getPixelColor(x + y * 8) > 0 && matrix.getPixelColor(x + y * 8) != 16776960) {
          over();
        } else if ((x == 8) || (x == -1) || (y == -1) || (y == 8)) {
          over();
        } else matrix.drawPixel(z1[z3++][0], z1[z3][1], 0);

        for (int i = 0; i < 10; i++)
          Serial.read();

        display();
        E = true;
      }
      if (Serial.available() && E == 1) {
        char ch = Serial.read();
        if ((ch == 'd' || ch == 'D') && C == 0)
          A = true, B = false, D = false, Serial.println("keyboard = d");
        else if ((ch == 's' || ch == 'S') && D == 0)
          B = true, A = false, C = false, Serial.println("keyboard = s");
        else if ((ch == 'a' || ch == 'A') && A == 0)
          C = true, B = false, D = false, Serial.println("keyboard = a");
        else if ((ch == 'w' || ch == 'W') && B == 0)
          D = true, A = false, C = false, Serial.println("keyboard = w");
        else Serial.println("keyboard = no data...?");
        E = false;
      }
    }
  }
}

// struct
// enum
// switch-case
// sizeof
// constrain
// bitwise
// pointer
// circular buffer
// class