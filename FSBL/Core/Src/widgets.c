/*
 * widgets.c
 *
 *  Created on: Jun 2, 2025
 *      Author: suso
 */
#include "main.h"

void TS_Update(void);

TS_Init_t hTS;
TS_State_t  TS_State;
TS_Init_t hTS;
uint16_t x_new_pos = 0, x_previous_pos = 0;
uint16_t y_new_pos = 0, y_previous_pos = 0;

int PIXEL_SIZE = 14;
int BOARD_X = 2;
int BOARD_Y = 5;
int BOARD_SIZE = 28;

void Board(void)
{
  uint16_t k, l;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_NONE;
  hTS.Accuracy = 0;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);

  BSP_TS_EnableIT(0);

  //UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  for(k = BOARD_X; k < BOARD_SIZE + BOARD_X; k++)
  {
    for(l = BOARD_Y; l < BOARD_SIZE + BOARD_Y; l++)
    {
      UTIL_LCD_DrawRect(PIXEL_SIZE*k, PIXEL_SIZE*l,PIXEL_SIZE,PIXEL_SIZE, UTIL_LCD_COLOR_BLACK);
    }
  }
}


void Window(void){
  UTIL_LCD_Clear(UTIL_LCD_COLOR_ST_BLUE_DARK);
  // Font config
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_ST_GRAY_LIGHT);

  uint32_t x_size;
  uint32_t y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  BSP_LCD_FillRect(0, 0, 0, x_size, 45, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"STM32N6 Digits", CENTER_MODE);
}
void BSP_TS_Callback(uint32_t Instance)
{
  TS_Update();
}

void TS_Update(void)
{
  uint16_t i, j;

  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */
    /* Get X and Y position of the first touch post calibrated */
    x_new_pos = TS_State.TouchX;
    y_new_pos = TS_State.TouchY;

    for(i = BOARD_X; i < BOARD_SIZE + BOARD_X; i++)
    {
      for(j = BOARD_Y; j < BOARD_SIZE + BOARD_Y; j++)
      {
        if(((x_new_pos > PIXEL_SIZE*i) && (x_new_pos < PIXEL_SIZE*(i+1))) && ((y_new_pos > PIXEL_SIZE*j) && (y_new_pos < PIXEL_SIZE*(j+1))))
        {
          //UTIL_LCD_FillRect(x_previous_pos, y_previous_pos,PIXEL_SIZE,PIXEL_SIZE, UTIL_LCD_COLOR_ST_BLUE_DARK);
          UTIL_LCD_DrawRect(x_previous_pos, y_previous_pos,PIXEL_SIZE,PIXEL_SIZE, UTIL_LCD_COLOR_BLACK);

          UTIL_LCD_FillRect(PIXEL_SIZE*i, PIXEL_SIZE*j,PIXEL_SIZE,PIXEL_SIZE, UTIL_LCD_COLOR_ST_GRAY_LIGHT);

          x_previous_pos = PIXEL_SIZE*i;
          y_previous_pos = PIXEL_SIZE*j;
          break;
        }
      }
    }
    x_new_pos = TS_State.TouchX;
    y_new_pos = TS_State.TouchY;
  }
}

