/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_DARK_VIOLE);

  //int col, row;
  //int centerCol = width/2, centerRow = height/2;
  //for (row = 0; row < 15; row++) {
  //for (col = -row; col < row / 2; col ++){
  //  drawPixel(centerCol + col, centerRow + row, COLOR_PINK);
  //}
  //}

  //for (row = 5; row < 25; row++){
    //for (col = -row / 3; col <= row /4; col++){
      //if (row % 2 == 0){
	//drawPixel(centerCol + col, centerRow + row + 5, COLOR_PINK);
	//}
      //}
    //}
  drawString5x7(10, 10, "Print Text", COLOR_GREEN, COLOR_RED);
  drawString5x7(20, 20, "Welcome", COLOR_GREEN, COLOR_RED);
  //drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);
  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  
}
