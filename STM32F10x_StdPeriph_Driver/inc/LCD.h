#ifndef __LCD_H
#define __LCD_H

void LCD_INPUT(unsigned char data);
void LCD_Init(void);
void LCD_DISP_CHAR(unsigned char ch);
void LCD_CMD(unsigned char ch);
void LCD_SetCursor(unsigned char addr);
void LCD_Clr(void);
void LCD_DISP_STRING(unsigned char * ch,unsigned pos);

#endif

