#include "stm32f10x_lib.h"
#include "stm32f10x_spi.h"
#include "MAX1270.h"


//����һ��ȫ�ֱ�����������ʱ
unsigned char max1270_Delay_t=4;

/*��ʱ(max1270_Delay_t*2+2)����������*/
#define max1270_ShortDelay() {unsigned char t=max1270_Delay_t;while(--t!=0);}

/*��ʱ(max1270_Delay_t*12+8)����������*/
#define max1270_LongDelay(){unsigned char t=max1270_Delay_t*6;while(--t!=0);}

void max1270_init(void)
{


  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
  /*����max1270��ʱ���źź����������ź�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*����max1270����������ź�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*����SHDN�ź�*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
  /*����Ƭѡ�ź�CS���͵�ƽ��Ч*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

   GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init (GPIOB, &GPIO_InitStructure);

  /* SPI1 Config -------------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1 */
  SPI_Cmd(SPI1, ENABLE);
}

/*������max1270_SPI_Write()
���ܣ���SPI ����д��1 ���ֽڵ�����
������dat��Ҫд�������*/
void max1270_SPI_Write(char data)
{	
  SPI_SendData(SPI1, data);
  while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE)==RESET);
}

/*������max1270_SPI_Read()
���ܣ���SPI ���߶�ȡ1 ���ֽڵ�����
���أ���ȡ��������*/
//unsigned int max1270_SPI_Read(void)
u16 max1270_SPI_Read(void)
{
  return(SPI_ReceiveData(SPI1));
}
/*������max1270_wr()
���ܣ�ִ��max1270 ��ָ�����ͨ��ѡ����
������cmd��������*/
void max1270_wr(unsigned char CH)
{   unsigned char cmd;
    cmd=0x81;//������0-5v;�ⲿʱ��
    cmd+=CH*16;
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);//ѡͨƬѡ�ź�
    max1270_LongDelay();
    max1270_SPI_Write(cmd);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    max1270_LongDelay();
}
/*������max1270_re()
����:��ȡADת������*/
//unsigned int max1270_re(void)
u16 max1270_re(void)
{
    //unsigned int AD;
    u16 AD;
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
    while(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE)==RESET);
    max1270_LongDelay();
    AD = max1270_SPI_Read()>>4;
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    max1270_LongDelay();
    return AD;
}//Դ�ļ����˽���



