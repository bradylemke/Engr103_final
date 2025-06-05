// Author: Brady Lemke

// Made for the Adafruit Circuit Playground Express

// ROULETTE: Bet at your own risk!

A handheld roulette game built for the Adafruit Circuit Playground Express using motion sensing, LED feedback, and sound.

// Gameplay Overview

Test your luck by betting on red or green, give the board a shake to spin the wheel, a white LED marks where the "ball" lands, and your score adjusts based on the outcome. Get to +3 and you win, but if you get to -3 you lose!

// How It Works

The Circuit Playground Express uses:
  - LED ring to show the roulette table (alternating red/green)
  - Accelerometer to detect shakes and trigger the spin
  - Buttons for placing bets
  - Sound output for win/loss indication
  - LED score bar to show current score from -3 to +3

// Rules

  - The LED ring has 10 slots, alternates between red and green, and the ball lands randomly when shaken
  - Bet: Left button = Bet on Red, Right button = Bet on Green
  - Spin: Shake the Circuit Playground Express to start the spin, and the ball (white LED) lands on a random slot
  - Scoring: +1 if your bet matches the landing color, -1 if it doesn't. Score is displayed with a visual LED bar, and you win at +5, lose at -5
  - Game Over: LEDs flash in cyan if you win, purple if you lose, inputs are locked once the game ends
  - Score Display: Center LED = neutral, LEDs to the right = green, positive score, LEDs to the left = red, negative score

// Inputs

[Action - Input]
  - Bet on Red - Left Button
  - Bet on Green - Right Button
  - Spin the wheel - Shake the board (> 20)

// Outputs

[Feedback - Description]
  - White LED - Ball position after spin
  - Red/Green LEDs - Roulette layout
  - Dimmed LEDs - Non-bet color is dimmed for clarity
  - Sound - Tone indicates win, loss, or no bet
  - Score Bar - LEDs show live score (-5 to +5)
  - Flash Effect - LEDs flash color on win/loss/no bet
