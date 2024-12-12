#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6
#define SW1 1    
#define SW2 2    
#define SW3 4    
#define SW4 8    
#define SWITCHES 15

// Buzzer on P2.6
#define BUZZER BIT6

// Car and coin properties (keeping previous definitions)
static int carX = 10;
static int carY = 80;
static int carMoving = 0;
static unsigned int carColor = COLOR_RED;
static int moveUp = 0;
static int moveDown = 0;
static int coinX = 100;
static int coinY = 60;
static int score = 0;
static const int COIN_SIZE = 6;

// Buzzer state
static int buzzing = 0;
static int buzzerCycles = 0;
static int currentNote = 0;

void buzzer_init() {
    timerAUpmode();    // Used to drive speaker
    P2SEL2 &= ~(BUZZER);
    P2SEL &= ~(BUZZER);
    P2SEL |= BUZZER;   // Select P2.6 as Timer output
    P2DIR = BUZZER;    // Set P2.6 as output
}

void buzzer_set_period(short cycles) {
    CCR0 = cycles;
    CCR1 = cycles >> 1;
}

void play_sound(int note) {
    buzzing = 1;
    currentNote = note;
    buzzerCycles = 0;
    
    switch(note) {
    case 1:  // SW1 sound - low beep
        buzzer_set_period(4000);
        break;
    case 2:  // SW2 sound - medium beep
        buzzer_set_period(3000);
        break;
    case 3:  // SW3 sound - high beep
        buzzer_set_period(2000);
        break;
    case 4:  // SW4 sound - higher beep
        buzzer_set_period(1500);
        break;
    case 5:  // Coin collection sound - victory beep
        buzzer_set_period(1000);
        break;
    }
}

void stop_sound() {
    buzzing = 0;
    buzzer_set_period(0);
}

// Previous helper functions remain the same
char checkCollision() {
    return (carX < (coinX + COIN_SIZE) &&
            (carX + 20) > coinX &&
            carY < (coinY + COIN_SIZE) &&
            (carY + 14) > coinY);
}

void spawnNewCoin() {
    coinX = (coinX + 37 + score * 13) % (screenWidth - COIN_SIZE);
    coinY = (coinY + 23 + score * 7) % (screenHeight - COIN_SIZE);
}

void drawScore() {
    char scoreStr[15];
    scoreStr[0] = 'S';
    scoreStr[1] = 'c';
    scoreStr[2] = 'o';
    scoreStr[3] = 'r';
    scoreStr[4] = 'e';
    scoreStr[5] = ':';
    scoreStr[6] = ' ';
    scoreStr[7] = '0' + score;
    scoreStr[8] = '\0';
    drawString5x7(5, 5, scoreStr, COLOR_WHITE, COLOR_BLACK);
}

void switch_init() {
    P2REN |= SWITCHES;
    P2IE |= SWITCHES;
    P2OUT |= SWITCHES;
    P2DIR &= ~SWITCHES;
}

void wdt_c_handler() {
    static int secCount = 0;

    secCount++;
    
    // Handle buzzer timing
    if (buzzing) {
        buzzerCycles++;
        if (buzzerCycles > 10) {  // Sound duration
            stop_sound();
        }
    }
    
    if (secCount >= 25) {
        secCount = 0;
        
        // Erase old positions
        fillRectangle(carX, carY, 20, 14, COLOR_BLACK);
        fillRectangle(coinX, coinY, COIN_SIZE, COIN_SIZE, COLOR_BLACK);
        
        if (carMoving) {
            carX += 2;
            if (carX > screenWidth) {
                carX = -20;
            }
        }
        
        if (moveUp) {
            carY -= 2;
            if (carY < 15) carY = 15;
        }
        if (moveDown) {
            carY += 2;
            if (carY > screenHeight - 14) carY = screenHeight - 14;
        }
        
        if (checkCollision()) {
            score++;
            spawnNewCoin();
            drawScore();
            play_sound(5);  // Play coin collection sound
        }
        
        // Draw game elements
        fillRectangle(coinX, coinY, COIN_SIZE, COIN_SIZE, COLOR_YELLOW);
        fillRectangle(carX, carY, 20, 10, carColor);
        fillRectangle(carX+3, carY+10, 4, 4, COLOR_WHITE);
        fillRectangle(carX+13, carY+10, 4, 4, COLOR_WHITE);
    }
}

void main() {
    P1DIR |= LED;
    P1OUT |= LED;
    
    configureClocks();
    lcd_init();
    switch_init();
    buzzer_init();  // Initialize buzzer
    
    enableWDTInterrupts();      
    or_sr(0x8);
    
    clearScreen(COLOR_BLACK);
    drawScore();
    
    // Initial drawings
    fillRectangle(coinX, coinY, COIN_SIZE, COIN_SIZE, COLOR_YELLOW);
    fillRectangle(carX, carY, 20, 10, carColor);
    fillRectangle(carX+3, carY+10, 4, 4, COLOR_WHITE);
    fillRectangle(carX+13, carY+10, 4, 4, COLOR_WHITE);

    while (1) {
        P1OUT &= ~LED;
        or_sr(0x10);
        P1OUT |= LED;
    }
}

void __interrupt_vec(PORT2_VECTOR) Port_2(void) {
    if (P2IFG & SWITCHES) {
        P2IFG &= ~SWITCHES;
        
        int button_state = ~P2IN & SWITCHES;
        
        if (!(P2IN & SW1)) {
            carMoving = !carMoving;
            play_sound(1);  // SW1 sound
        }
        
        if (!(P2IN & SW2)) {
            if (carColor == COLOR_RED) carColor = COLOR_BLUE;
            else if (carColor == COLOR_BLUE) carColor = COLOR_GREEN;
            else carColor = COLOR_RED;
            play_sound(2);  // SW2 sound
        }
        
        if (button_state & SW3 && !moveUp) {
            moveUp = 1;
            play_sound(3);  // SW3 sound
        } else if (!(button_state & SW3)) {
            moveUp = 0;
        }
        
        if (button_state & SW4 && !moveDown) {
            moveDown = 1;
            play_sound(4);  // SW4 sound
        } else if (!(button_state & SW4)) {
            moveDown = 0;
        }
    }
}

void __interrupt_vec(WDT_VECTOR) WDT(void) {
    wdt_c_handler();
}