#define D1 18
#define D2 19
#define D3 22
#define D4 23
#define D5 25
#define D6 26

unsigned long chordDelay = 120;

enum Mode { LOWER, CAPITAL, NUMBER, SPECIAL };
Mode currentMode = LOWER;

// ---------- READ PATTERN ----------
uint8_t readPattern() {
  uint8_t p = 0;

  if (digitalRead(D1)) p |= 0b000001;
  if (digitalRead(D2)) p |= 0b000010;
  if (digitalRead(D3)) p |= 0b000100;
  if (digitalRead(D4)) p |= 0b001000;
  if (digitalRead(D5)) p |= 0b010000;
  if (digitalRead(D6)) p |= 0b100000;

  return p;
}

// ---------- WAIT FOR RELEASE ----------
void waitRelease() {
  while (readPattern() != 0);
  delay(50);
}

// ---------- LOWERCASE ----------
void handleLowerCase(uint8_t p) {
  switch(p) {
    case 0b000001: Serial.print("a"); break;
    case 0b000011: Serial.print("b"); break;
    case 0b001001: Serial.print("c"); break;
    case 0b011001: Serial.print("d"); break;
    case 0b010001: Serial.print("e"); break;
    case 0b001011: Serial.print("f"); break;
    case 0b011011: Serial.print("g"); break;
    case 0b010011: Serial.print("h"); break;
    case 0b001010: Serial.print("i"); break;
    case 0b011010: Serial.print("j"); break;

    case 0b000101: Serial.print("k"); break;
    case 0b000111: Serial.print("l"); break;
    case 0b001101: Serial.print("m"); break;
    case 0b011101: Serial.print("n"); break;
    case 0b010101: Serial.print("o"); break;
    case 0b001111: Serial.print("p"); break;
    case 0b011111: Serial.print("q"); break;
    case 0b010111: Serial.print("r"); break;
    case 0b001110: Serial.print("s"); break;
    case 0b011110: Serial.print("t"); break;

    case 0b100101: Serial.print("u"); break;
    case 0b100111: Serial.print("v"); break;
    case 0b111010: Serial.print("w"); break;
    case 0b101101: Serial.print("x"); break;
    case 0b111101: Serial.print("y"); break;
    case 0b110101: Serial.print("z"); break;
  }
}

// ---------- CAPITAL (Persistent) ----------
void handleCapital(uint8_t p) {

  p &= 0b011111;  // Remove Dot6 if present

  switch(p) {
    case 0b000001: Serial.print("A"); break;
    case 0b000011: Serial.print("B"); break;
    case 0b001001: Serial.print("C"); break;
    case 0b011001: Serial.print("D"); break;
    case 0b010001: Serial.print("E"); break;
    case 0b001011: Serial.print("F"); break;
    case 0b011011: Serial.print("G"); break;
    case 0b010011: Serial.print("H"); break;
    case 0b001010: Serial.print("I"); break;
    case 0b011010: Serial.print("J"); break;

    case 0b000101: Serial.print("K"); break;
    case 0b000111: Serial.print("L"); break;
    case 0b001101: Serial.print("M"); break;
    case 0b011101: Serial.print("N"); break;
    case 0b010101: Serial.print("O"); break;
    case 0b001111: Serial.print("P"); break;
    case 0b011111: Serial.print("Q"); break;
    case 0b010111: Serial.print("R"); break;
    case 0b001110: Serial.print("S"); break;
    case 0b011110: Serial.print("T"); break;

    case 0b100101: Serial.print("U"); break;
    case 0b100111: Serial.print("V"); break;
    case 0b111010: Serial.print("W"); break;
    case 0b101101: Serial.print("X"); break;
    case 0b111101: Serial.print("Y"); break;
    case 0b110101: Serial.print("Z"); break;
  }
}

// ---------- NUMBER (Persistent) ----------
void handleNumber(uint8_t p) {
  switch(p) {
    case 0b000001: Serial.print("1"); break;
    case 0b000011: Serial.print("2"); break;
    case 0b001001: Serial.print("3"); break;
    case 0b011001: Serial.print("4"); break;
    case 0b010001: Serial.print("5"); break;
    case 0b001011: Serial.print("6"); break;
    case 0b011011: Serial.print("7"); break;
    case 0b010011: Serial.print("8"); break;
    case 0b001010: Serial.print("9"); break;
    case 0b011010: Serial.print("0"); break;
  }
}

// ---------- SPECIAL (Persistent) ----------
void handleSpecial(uint8_t p) {
  switch(p) {
    case 0b000010: Serial.print(","); break;
    case 0b000110: Serial.print(";"); break;
    case 0b010010: Serial.print(":"); break;
    case 0b010110: Serial.print("?"); break;
    case 0b000100: Serial.print("'"); break;
    case 0b010011: Serial.print("!"); break;
    case 0b010001: Serial.print("."); break;
    case 0b001100: Serial.print("-"); break;
  }
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
}

// ---------- LOOP ----------
void loop() {

  if (readPattern() != 0) {

    delay(chordDelay);
    uint8_t pattern = readPattern();
    waitRelease();

    // ---- SPACE (Dots 2-5-6) ----
    if (pattern == 0b110010) {
      Serial.print(" ");
      return;
    }

    // ---- NEW LINE (All dots) ----
    if (pattern == 0b111111) {
      Serial.println();
      return;
    }

    // ---- CAPITAL MODE (Dot6) ----
    if (pattern == 0b100000) {
      currentMode = CAPITAL;
      return;
    }

    // ---- NUMBER MODE (3-4-5-6) ----
    if (pattern == 0b111100) {
      currentMode = NUMBER;
      return;
    }

    // ---- SPECIAL MODE (4-6) ----
    if (pattern == 0b101000) {
      currentMode = SPECIAL;
      return;
    }

    // ---- RESET TO LOWER (Dot5) ----
    if (pattern == 0b010000) {
      currentMode = LOWER;
      return;
    }

    // ---- MODE EXECUTION ----
    if (currentMode == LOWER) {
      handleLowerCase(pattern);
    }
    else if (currentMode == CAPITAL) {
      handleCapital(pattern);
    }
    else if (currentMode == NUMBER) {
      handleNumber(pattern);
    }
    else if (currentMode == SPECIAL) {
      handleSpecial(pattern);
    }
  }
}