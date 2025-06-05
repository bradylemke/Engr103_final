#include <Adafruit_CircuitPlayground.h>

// Function prototypes
void lockInBall();
void flashAll(int r, int g, int b);
void highlightBet();
void detectShake();
void checkButtons();
void showTable();
void spinEffect();
void showScoreBar();

float X, Y, Z, totalAccel;
int bet = 0; // 0 = no bet, 1 = red, 2 = green
int ballPosition = -1;
bool spinning = false;
int score = 0;
bool gameOver = false;
int multiplier = 1;
int lastSwitchState = -1; // For detecting multiplier toggle

// 1 = red, 2 = green
int slotColor[10] = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2};

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);

  showTable();
  showScoreBar();
  delay(500);
}

void loop() {
  if (gameOver) return;

  // Detect multiplier switch change
  int switchState = CircuitPlayground.slideSwitch();
  if (switchState != lastSwitchState) {
    lastSwitchState = switchState;
    multiplier = (switchState == 1) ? 3 : 1;
    Serial.print("Multiplier changed to: x");
    Serial.println(multiplier);
  }

  checkButtons();
  detectShake();
  delay(50);
}

// Show red and green layout
void showTable() {
  for (int i = 0; i < 10; i++) {
    if (slotColor[i] == 1) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0); // Red
    } else {
      CircuitPlayground.setPixelColor(i, 0, 255, 0); // Green
    }
  }
}

// Handle betting inputs
void checkButtons() {
  if (CircuitPlayground.leftButton()) {
    bet = 1;
    Serial.println("Bet placed on RED");
    CircuitPlayground.playTone(440, 100);
    showTable();
    highlightBet();
  }

  if (CircuitPlayground.rightButton()) {
    bet = 2;
    Serial.println("Bet placed on GREEN");
    CircuitPlayground.playTone(660, 100);
    showTable();
    highlightBet();
  }
}

// Dim non-bet color while keeping bet color full
void highlightBet() {
  for (int i = 0; i < 10; i++) {
    if (bet == 1) {
      if (slotColor[i] == 1) {
        CircuitPlayground.setPixelColor(i, 255, 0, 0); // Full red
      } else {
        CircuitPlayground.setPixelColor(i, 0, 15, 0);  // Dim green
      }
    } else if (bet == 2) {
      if (slotColor[i] == 2) {
        CircuitPlayground.setPixelColor(i, 0, 255, 0); // Full green
      } else {
        CircuitPlayground.setPixelColor(i, 15, 0, 0);  // Dim red
      }
    }
  }
}

// Detect shake and trigger spin
void detectShake() {
  X = Y = Z = 0;
  for (int i = 0; i < 10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  totalAccel = sqrt(X * X + Y * Y + Z * Z);

  if (totalAccel > 20.0) {
    spinning = true;
    spinEffect();
  }

  if (spinning && totalAccel < 15.0) {
    spinning = false;
    lockInBall();
  }
}

// Animate spinning white LED ball
void spinEffect() {
  int steps = random(10, 30); // Total spin steps
  int delayTime = 30;         // Start speed

  for (int i = 0; i < steps; i++) {
    ballPosition = i % 10; // Move around the ring
    showTable();
    highlightBet();
    CircuitPlayground.setPixelColor(ballPosition, 255, 255, 255); // Ball
    delay(delayTime*i/steps);
    delayTime += 5; // Slow down gradually
  }
}

// Finalize result and update score
void lockInBall() {
  showTable();
  highlightBet();
  CircuitPlayground.setPixelColor(ballPosition, 255, 255, 255); // Ball

  int landedColor = slotColor[ballPosition];

  if (bet != 0) {
    if (bet == landedColor) {
      Serial.println("You WON!");
      CircuitPlayground.playTone(880, 300);
      flashAll(0, 255, 0);
      score += 1 * multiplier;
    } else {
      Serial.println("You LOST.");
      CircuitPlayground.playTone(220, 300);
      flashAll(255, 0, 0);
      score -= 1 * multiplier;
    }
  } else {
    Serial.println("No bet placed.");
    CircuitPlayground.playTone(330, 300);
    flashAll(0, 0, 255);
  }

  Serial.print("Current Score: ");
  Serial.println(score);

  showScoreBar();

  if (score >= 5) {
    Serial.println("GAME OVER: You WIN!");
    flashAll(0, 255, 255);
    gameOver = true;
  } else if (score <= -5) {
    Serial.println("GAME OVER: You LOSE.");
    flashAll(255, 0, 255);
    gameOver = true;
  }

  bet = 0;
}

// Flash all LEDs for feedback
void flashAll(int r, int g, int b) {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 10; i++) {
      CircuitPlayground.setPixelColor(i, r, g, b);
    }
    delay(150);
    CircuitPlayground.clearPixels();
    delay(100);
  }

  showTable();
  highlightBet();
  if (ballPosition >= 0) {
    CircuitPlayground.setPixelColor(ballPosition, 255, 255, 255);
  }
}

// Show score bar from -5 (left/red) to +5 (right/green)
void showScoreBar() {
  CircuitPlayground.clearPixels();
  int center = 5;

  if (score < 0) {
    for (int i = 1; i <= abs(score); i++) {
      CircuitPlayground.setPixelColor(center - i, 255, 0, 0); // Red
    }
  }

  if (score > 0) {
    for (int i = 1; i <= score; i++) {
      CircuitPlayground.setPixelColor(center + i - 1, 0, 255, 0); // Green
    }
  }
}