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
int bet = 0;  // 0 = no bet, 1 = red, 2 = green
int ballPosition = -1;
bool spinning = false;
int score = 0;
bool gameOver = false;  // Stops game after win/loss

// 1 = red, 2 = green
int slotColor[10] = { 1, 2, 1, 2, 1, 2, 1, 2, 1, 2 };

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);

  showTable();
  showScoreBar();  // Show score at startup
  delay(500);
}

void loop() {
  if (gameOver) return;

  checkButtons();
  detectShake();
  delay(50);
}

// Board setup
void showTable() {
  for (int i = 0; i < 10; i++) {
    if (slotColor[i] == 1) {
      CircuitPlayground.setPixelColor(i, 255, 0, 0);  // Red
    } else {
      CircuitPlayground.setPixelColor(i, 0, 255, 0);  // Green
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

// Dim the non-bet color significantly while keeping bet color full
void highlightBet() {
  for (int i = 0; i < 10; i++) {
    if (bet == 1) {  // Bet on red
      if (slotColor[i] == 1) {
        CircuitPlayground.setPixelColor(i, 255, 0, 0);  // Full red
      } else {
        CircuitPlayground.setPixelColor(i, 0, 15, 0);  // Very dim green
      }
    } else if (bet == 2) {  // Bet on green
      if (slotColor[i] == 2) {
        CircuitPlayground.setPixelColor(i, 0, 255, 0);  // Full green
      } else {
        CircuitPlayground.setPixelColor(i, 15, 0, 0);  // Very dim red
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

// Flash white LED as ball while spinning
void spinEffect() {
  ballPosition = random(0, 10);
  showTable();
  highlightBet();
  CircuitPlayground.setPixelColor(ballPosition, 255, 255, 255);  // Ball
}

// Finalize result and show outcome
void lockInBall() {
  showTable();
  highlightBet();
  CircuitPlayground.setPixelColor(ballPosition, 255, 255, 255);  // Ball

  int landedColor = slotColor[ballPosition];

  if (bet != 0) {
    if (bet == landedColor) {
      Serial.println("You WON!");
      CircuitPlayground.playTone(880, 300);
      flashAll(0, 255, 0);  // WIN = green flash
      score++;
    } else {
      Serial.println("You LOST.");
      CircuitPlayground.playTone(220, 300);
      flashAll(255, 0, 0);  // LOSS = red flash
      score--;
    }
  } else {
    Serial.println("No bet placed.");
    CircuitPlayground.playTone(330, 300);
    flashAll(0, 0, 255);  // NO BET = blue flash
  }

  Serial.print("Current Score: ");
  Serial.println(score);

  showScoreBar();

  if (score >= 3) {
    Serial.println("GAME OVER: You WIN!");
    flashAll(0, 255, 255);  // Cyan flash
    gameOver = true;
  } 
  else if (score <= -3) {
    Serial.println("GAME OVER: You LOSE.");
    flashAll(255, 0, 255);  // Purple flash
    gameOver = true;
  }

  bet = 0;
}

// Flash LEDs
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

// Show score
void showScoreBar() {
  CircuitPlayground.clearPixels();
  int center = 5;

  if (score < 0) {
    for (int i = 1; i <= abs(score); i++) {
      CircuitPlayground.setPixelColor(center - i, 255, 0, 0);
    }
  }

  if (score > 0) {
    for (int i = 1; i <= score; i++) {
      CircuitPlayground.setPixelColor(center + i - 1, 0, 255, 0);
    }
  }
}