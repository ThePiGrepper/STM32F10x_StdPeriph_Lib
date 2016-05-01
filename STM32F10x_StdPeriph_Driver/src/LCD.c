#include "stm32f10x_lib.h"
#include "LCD.h"

//LCD bits definitions
#define LCD_MAX_Col     32

#define LCD_INPUT_EN()  GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET)
#define LCD_INPUT_DIS() GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET)

#define LCD_READ()      GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET)
#define LCD_WRITE()     GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET)

#define LCD_SET_DRS()   GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET)
#define LCD_SET_IRS()   GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET)


void LCD_INPUT(unsigned char data)
{
    unsigned int i;
    /* Send SPI1 data */ 
    SPI_SendData(SPI1, data); 
    /* Wait for SPI1 Tx buffer empty */ 
    while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE)==RESET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	for(i=10;i>0;i--);
	GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);

  LCD_INPUT_EN();
  for(i=0;i<200;i++);
//  for(i=0;i<100;i++);
  LCD_INPUT_DIS();
  for(i=0;i<200;i++);
//  for(i=0;i<100;i++);

}

void LCD_Init(void)
{
  int i;
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  /* Configure SPI1 pins: SCK and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //Configure Pin21(PB10),LCD-EN; Pin22(PB11),74HC595-G; Pin25(PB12),LCD-RS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  //Configure Pin12(PA2),74HC595-RCK
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    

  
  /* SPI1 Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);  

  /* Enable SPI1 */
  SPI_Cmd(SPI1, ENABLE);

  GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);

  for(i=0;i<100;i++);
  LCD_SET_IRS();
  LCD_WRITE();
  for(i=0;i<100;i++);
  LCD_INPUT(0x38);
  LCD_INPUT(0x0E);
  LCD_INPUT(0x06);
}


void LCD_DISP_CHAR(unsigned char ch)
{
  int i;
  LCD_SET_DRS();
  for(i=0;i<100;i++);
  LCD_INPUT(ch);
}

void LCD_CMD(unsigned char ch)
{
  int i;
  LCD_SET_IRS();
  for(i=0;i<100;i++);
  LCD_INPUT(ch);
}

void LCD_SetCursor(unsigned char addr)
{
  int i;
  LCD_SET_IRS();
  for(i=0;i<100;i++);
  LCD_INPUT(addr|0x80);
}

void LCD_Clr(void)
{
  unsigned char i;
  LCD_SetCursor(0x00);
  for(i=0;i<LCD_MAX_Col;i++)
    LCD_DISP_CHAR(' ');
  LCD_SetCursor(0x40);
  for(i=0;i<LCD_MAX_Col;i++)
    LCD_DISP_CHAR(' ');
}


void LCD_DISP_STRING(unsigned char * ch,unsigned pos)
{
  LCD_SetCursor(pos);
  while(*ch!='\0')
  {
    LCD_DISP_CHAR(*ch); 
    ch++;
  }
}
