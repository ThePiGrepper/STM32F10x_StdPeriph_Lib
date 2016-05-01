#include "stm32f10x_lib.h"
#include "stm32f10x_spi.h"
#include "MAX1270.h"


//定义一个全局变量，用于延时
unsigned char max1270_Delay_t=4;

/*延时(max1270_Delay_t*2+2)个机器周期*/
#define max1270_ShortDelay() {unsigned char t=max1270_Delay_t;while(--t!=0);}

/*延时(max1270_Delay_t*12+8)个机器周期*/
#define max1270_LongDelay(){unsigned char t=max1270_Delay_t*6;while(--t!=0);}

void max1270_init(void)
{


  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;
  /*配置max1270的时钟信号和数据输入信号*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*配置max1270的数据输出信号*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /*配置SHDN信号*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOC, GPIO_Pin_15, Bit_SET);
  /*配置片选信号CS，低电平有效*/
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

/*函数：max1270_SPI_Write()
功能：向SPI 总线写入1 个字节的数据
参数：dat：要写入的数据*/
void max1270_SPI_Write(char data)
{	
  SPI_SendData(SPI1, data);
  while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE)==RESET);
}

/*函数：max1270_SPI_Read()
功能：从SPI 总线读取1 个字节的数据
返回：读取到的数据*/
//unsigned int max1270_SPI_Read(void)
u16 max1270_SPI_Read(void)
{
  return(SPI_ReceiveData(SPI1));
}
/*函数：max1270_wr()
功能：执行max1270 纯指令，仅有通道选择功能
参数：cmd：命令字*/
void max1270_wr(unsigned char CH)
{   unsigned char cmd;
    cmd=0x81;//单极：0-5v;外部时钟
    cmd+=CH*16;
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);//选通片选信号
    max1270_LongDelay();
    max1270_SPI_Write(cmd);
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
    max1270_LongDelay();
}
/*函数：max1270_re()
功能:读取AD转换数据*/
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
}//源文件到此结束



