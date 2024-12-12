#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "score.h"

void drawString8x12(u_char col, u_char row, char *string, u_int fgColorBGR, u_int bgColorBGR);
void drawChar8x12(u_char col, u_char row, char c, u_int fgColorBGR, u_int bgColorBGR);

#define LED BIT6
#define SLEEP_LED BIT0
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

// sound state
static int backgroundSound = 1;
static int coinSoundPriority = 0;  
static int currentPeriod = 0;
static int playingSound = 0;
static int soundCount = 0;

// car position and properties
static int carX = 10;
static int carY = 80;
static int carMoving = 0;
static unsigned int carColor = COLOR_RED;
static int moveUp = 0;
static int moveDown = 0;

// coin properties
static int coinX = 100;
static int coinY = 60;
static int score = 0;
static const int COIN_SIZE = 6;

void switch_sound(int type) {
    switch(type) {
        case 0:  // background engine sound
            if (!coinSoundPriority) {
                currentPeriod = (currentPeriod == 4000) ? 4500 : 4000;
                buzzer_set_period(currentPeriod);
            }
            break;
        case 1:  // coin collection sound
            coinSoundPriority = 1;
            buzzer_set_period(950);
            break;
    }
}

void play_coin_sound() {
    buzzer_set_period(950);    // Higher pitch for coin collection
    playingSound = 1;
    soundCount = 0;
}

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

void drawChar8x12(u_char rcol, u_char rrow, char c, u_int fgColorBGR, u_int bgColorBGR) {
    u_char col = 0;
    u_char row = 0;
    u_char bit = 0x80; 
    u_char oc = c - 0x20;

    lcd_setArea(rcol, rrow, rcol + 7, rrow + 11);
    while (row < 12) {
        bit = 0x80;    // reset bit for each new row
        while (col < 8) {
            u_int colorBGR = (font_8x12[oc][row] & bit) ? fgColorBGR : bgColorBGR;
            lcd_writeColor(colorBGR);
            col++;
            bit >>= 1;  // right shift
        }
        col = 0;
        row++;
    }
}

void drawString8x12(u_char col, u_char row, char *string, u_int fgColorBGR, u_int bgColorBGR) {
    u_char cols = col;
    while (*string) {
        drawChar8x12(cols, row, *string++, fgColorBGR, bgColorBGR);
        cols += 9;
    }
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
    
    fillRectangle(0, 0, screenWidth, 20, COLOR_BLACK);
    drawString8x12(5, 5, scoreStr, COLOR_WHITE, COLOR_BLACK);
}

void switch_init() {
    P2REN |= SWITCHES;
    P2IE |= SWITCHES;
    P2OUT |= SWITCHES;
    P2DIR &= ~SWITCHES;
}

void wdt_c_handler() {

    static int secCount = 0;
    static int soundTick = 0;

    // Handle sounds
    if (playingSound) {
        soundCount++;
        if (soundCount >= 20) {  // coin sound duration
            buzzer_off();
            playingSound = 0;
            coinSoundPriority = 0;  // background sound again
        }
    } else if (backgroundSound) {
        soundTick++;
        if (soundTick >= 25) {  // change engine sound every 25 ticks
            soundTick = 0;
            switch_sound(0);
        }
    }

    secCount++;
    if (secCount >= 25) {
        secCount = 0;
        
        //erase old car and coin
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
            // score++;
            score = specialScoreInc(score);
            spawnNewCoin();
            drawScore();
            play_coin_sound();  //coin collection sound
        }
        
        // draw coin
        fillRectangle(coinX, coinY, COIN_SIZE, COIN_SIZE, COLOR_YELLOW);
        
        // draw car
        fillRectangle(carX, carY, 20, 10, carColor);
        fillRectangle(carX+3, carY+10, 4, 4, COLOR_WHITE);
        fillRectangle(carX+13, carY+10, 4, 4, COLOR_WHITE);
    }
}

void main() {
    // P1DIR |= LED;
    // P1OUT |= LED;
    P1DIR |= SLEEP_LED;  // sets sleep/wake LED pin as output
    P1OUT &= ~SLEEP_LED;
    
    configureClocks();
    lcd_init();
    switch_init();
    buzzer_init();    // initialize buzzer
    switch_sound(0);

    enableWDTInterrupts();      
    or_sr(0x8);
    
    clearScreen(COLOR_BLACK);
    drawScore();
    
    //buzzer off
    buzzer_off();
    
    //draws initial coin
    fillRectangle(coinX, coinY, COIN_SIZE, COIN_SIZE, COLOR_YELLOW);
    
    //sraws initial car
    fillRectangle(carX, carY, 20, 10, carColor);
    fillRectangle(carX+3, carY+10, 4, 4, COLOR_WHITE);
    fillRectangle(carX+13, carY+10, 4, 4, COLOR_WHITE);

    while (1) {
        P1OUT &= ~LED;
        or_sr(0x10);
        P1OUT |= LED;
    }
}

void enterSleep() {
    P1OUT &= ~LED;        // turn off LED
    P1OUT |= SLEEP_LED;   // turn on sleep/wake LED
    
    __bis_SR_register(LPM3_bits + GIE);
}

void exitSleep() {
    P1OUT &= ~SLEEP_LED;  // Turn off sleep/wake LED
    P1OUT |= LED;         // Turn on main LED
    
}

void __interrupt_vec(PORT2_VECTOR) Port_2(void) {
    if (P2IFG & SWITCHES) {
        P2IFG &= ~SWITCHES;
        
        int button_state = ~P2IN & SWITCHES;
        
        if (!(P2IN & SW1)) {
            carMoving = !carMoving;
        }
        
        if (!(P2IN & SW2)) {
            if (carColor == COLOR_RED) carColor = COLOR_BLUE;
            else if (carColor == COLOR_BLUE) carColor = COLOR_GREEN;
            else carColor = COLOR_RED;
        }
        
        moveUp = (button_state & SW3) ? 1 : 0;
        moveDown = (button_state & SW4) ? 1 : 0;

        if (!(P2IN & SW4)) {  // assuming SW4 is used for sleep/wake
            if (P1OUT & SLEEP_LED) {
                exitSleep();
                __bic_SR_register_on_exit(LPM3_bits); 
            } else {
                enterSleep();
            }
        }
    }
}

void __interrupt_vec(WDT_VECTOR) WDT(void) {
    wdt_c_handler();
}