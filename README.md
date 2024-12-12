# LCD Car Game - Project 3

A simple and fun car game featuring an interactive LCD display! Control your car to collect coins, all while enjoying sound effects. This game was developed for the MSP430, focusing on interrupts, assembly, and peripheral interfacing.

---

## How to Play
The rules are simple:
- **SW1**: Start/stop the car's horizontal movement.
- **SW2**: Cycle the car's color (Red → Blue → Green).
- **SW3**: Move the car up.
- **SW4**: Move the car down.

### Objective
- Collect as many coins as possible!

### Coin Collection Highlights:
- Your **score increases** with each coin collected.
- A **new coin** appears in a random location.
- A **special sound effect** plays for every coin.
- The **background engine sound** continues running throughout.

---

## Features
- **Car Movement**: Up, down, start, and stop controls.
- **Coin Collection System**: Score tracking and random coin placement.
- **Sound Effects**: Background engine sound and coin collection effects using the buzzer.
- **Enhanced Score Display**: Larger font (8x12) for better visibility.
- **Assembly Integration**: Special scoring features implemented in assembly.
- **Smooth Game Play**: Interrupt-driven inputs and real-time updates.

---

## How to Run the Game
1. Navigate to the `lcdLib` directory.
2. Clean old compiled files:
   ```bash
   $ make clean
3. Compile and load the game:
   ```bash
   $ make load-car
## What I Learned
Working on this project helped me understand:
* How to use the LCD display effectively
* Interrupt handling for buttons
* Sound generation using the buzzer
* Basic game mechanics and collision detection
* Assembly language integration with C
* State management in embedded systems

## Known Issues
* Sometimes the coin might spawn close to the screen edges
* Background sound can get a bit repetitive (but it works!)

