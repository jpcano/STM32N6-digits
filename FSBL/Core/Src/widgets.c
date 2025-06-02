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
static uint32_t touchscreen_color_idx = 0;
uint32_t  colors[24] = {UTIL_LCD_COLOR_BLUE , UTIL_LCD_COLOR_GREEN, UTIL_LCD_COLOR_RED, UTIL_LCD_COLOR_CYAN,
                        UTIL_LCD_COLOR_MAGENTA, UTIL_LCD_COLOR_YELLOW,UTIL_LCD_COLOR_LIGHTBLUE, UTIL_LCD_COLOR_LIGHTGREEN,
                        UTIL_LCD_COLOR_LIGHTRED, UTIL_LCD_COLOR_WHITE, UTIL_LCD_COLOR_LIGHTMAGENTA, UTIL_LCD_COLOR_LIGHTYELLOW,
                        UTIL_LCD_COLOR_DARKGREEN, UTIL_LCD_COLOR_DARKRED, UTIL_LCD_COLOR_DARKCYAN,UTIL_LCD_COLOR_DARKMAGENTA,
                        UTIL_LCD_COLOR_LIGHTGRAY, UTIL_LCD_COLOR_GRAY, UTIL_LCD_COLOR_DARKGRAY, UTIL_LCD_COLOR_DARKYELLOW,
                        UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_BROWN, UTIL_LCD_COLOR_ORANGE, UTIL_LCD_COLOR_DARKBLUE};

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
  for(k = 0; k < hTS.Width/40; k++)
  {
    for(l = 0; l < hTS.Height/40; l++)
    {
      UTIL_LCD_DrawRect(40*k, 40*l,40,40, UTIL_LCD_COLOR_BLACK);
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

    for(i = 0; i < hTS.Width/40; i++)
    {
      for(j = 0; j < hTS.Height/40; j++)
      {
        if(((x_new_pos > 40*i) && (x_new_pos < 40*(i+1))) && ((y_new_pos > 40*j) && (y_new_pos < 40*(j+1))))
        {
          UTIL_LCD_FillRect(x_previous_pos, y_previous_pos,40,40, UTIL_LCD_COLOR_ST_BLUE_DARK);
          UTIL_LCD_DrawRect(x_previous_pos, y_previous_pos,40,40, UTIL_LCD_COLOR_BLACK);

          UTIL_LCD_FillRect(40*i, 40*j,40,40, colors[(touchscreen_color_idx++ % 24)]);

          x_previous_pos = 40*i;
          y_previous_pos = 40*j;
          break;
        }
      }
    }
    x_new_pos = TS_State.TouchX;
    y_new_pos = TS_State.TouchY;
  }
}

