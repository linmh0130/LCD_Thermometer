
#include "HT1621.h"


unsigned int n1, n2, n3, n4;

unsigned char Ht1621Tab[]={0x00,0x00,0x00,0x00};
unsigned char DispTab[]={0x7B,0x12,0x67,0x57,0x1E,0x5D,0x7D,0x13,0x7F,0x5F,0x69,0x00,0x04};
              //          0    1    2    3    4    5    6    7    8    9    C   void

//-----------------------------------------------------------------------------------------
//函数名称：Delay()
//功    能：延时子程序
//-----------------------------------------------------------------------------------------
void Delay(unsigned char us)  //5,7,9
{
  while(--us);
}

//-----------------------------------------------------------------------------------------
//函数名称：Delayms()
//-----------------------------------------------------------------------------------------
void Delayms(unsigned int ims)
{
  unsigned int i,j;
    for(i=0;i<ims;i++)
      for(j=0;j<65;j++) { Delay(1); }
}

//-----------------------------------------------------------------------------------------
//Ht1621Wr_Data()
//-----------------------------------------------------------------------------------------
void Ht1621Wr_Data(unsigned char Data,unsigned char cnt)
{
  unsigned char i;
  for (i=0;i<cnt;i++)
   {
     Ht1621_WR_0();
     if((Data & 0x80)==0x80) {Ht1621_DO_1();}
     else {Ht1621_DO_0();}
     Ht1621_WR_1();
     Data<<=1;
   }
}
//-----------------------------------------------------------------------------------------
//void Ht1621WrCmd(uchar Cmd)
//-----------------------------------------------------------------------------------------
void Ht1621WrCmd(unsigned char Cmd)
{
   Ht1621_CS_0();
   Ht1621Wr_Data(0x80,4);          //写入命令标志100
   Ht1621Wr_Data(Cmd,8);           //写入命令数据
   Ht1621_CS_1();
}
//-----------------------------------------------------------------------------------------
//void Ht1621WrOneData(uchar Addr,uchar Data)
//-----------------------------------------------------------------------------------------
void Ht1621WrOneData(unsigned char Addr,unsigned char Data)
{
  Ht1621_CS_0();
  Ht1621Wr_Data(0xa0,3);  //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  Ht1621Wr_Data(Data,4); //写入数据的前四位 7  6  5  4
  Ht1621_CS_1();
}
//-----------------------------------------------------------------------------------------
//void Ht1621WrAllData()
//-----------------------------------------------------------------------------------------
void Ht1621WrAllData(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
  unsigned char i;
  Ht1621_CS_0();
  Ht1621Wr_Data(0xa0,3); //写入数据标志101
  Ht1621Wr_Data(Addr<<2,6); //写入地址数据
  for (i=0;i<cnt;i++)
   {
    Ht1621Wr_Data(*p,8); //写入数据
    p++;
   }
  Ht1621_CS_1();
}

//-----------------------------------------------------------------------------------------
//void Ht1621_Init(void)
//-----------------------------------------------------------------------------------------
void Ht1621_Init(void)
{
   Ht1621WrCmd(BIAS);
   Ht1621WrCmd(RC256);             //使用内部振荡器
   //Ht1621WrCmd(XTAL);             //使用外部振荡器
   Ht1621WrCmd(SYSDIS);
   Ht1621WrCmd(WDTDIS1);
   Ht1621WrCmd(SYSEN);
   Ht1621WrCmd(LCDON);
}
//-----------------------------------------------------------------------------------------
//void ClearLCD(void)
//-----------------------------------------------------------------------------------------
void ClearLCD(void)
{
	Ht1621WrAllData(0,Ht1621Tab,16);// Clear LCD display
}
//-----------------------------------------------------------------------------------------
//DISPLAY
//-----------------------------------------------------------------------------------------
void Display(void)
{
   unsigned char com;
   com = 0;
   if((DispTab[n1]&0x01) == 0x01){ com = com + 0x10;}
   if((DispTab[n2]&0x01) == 0x01){ com = com + 0x20;}
   if((DispTab[n3]&0x01) == 0x01){ com = com + 0x40;}
   if((DispTab[n4]&0x01) == 0x01){ com = com + 0x80;}
   Ht1621WrOneData(0 , com);
   com = 0;
   if((DispTab[n1]&0x02) == 0x02){ com = com + 0x10;}
   if((DispTab[n2]&0x02) == 0x02){ com = com + 0x20;}
   if((DispTab[n3]&0x02) == 0x02){ com = com + 0x40;}
   if((DispTab[n4]&0x02) == 0x02){ com = com + 0x80;}
   Ht1621WrOneData(1 , com);
   com = 0;
   if((DispTab[n1]&0x10) == 0x10){ com = com + 0x10;}
   if((DispTab[n2]&0x10) == 0x10){ com = com + 0x20;}
   if((DispTab[n3]&0x10) == 0x10){ com = com + 0x40;}
   if((DispTab[n4]&0x10) == 0x10){ com = com + 0x80;}
   Ht1621WrOneData(2 , com);
   com = 0;
   if((DispTab[n1]&0x40) == 0x40){ com = com + 0x10;}
   if((DispTab[n2]&0x40) == 0x40){ com = com + 0x20;}
   if((DispTab[n3]&0x40) == 0x40){ com = com + 0x40;}
   if((DispTab[n4]&0x40) == 0x40){ com = com + 0x80;}
   Ht1621WrOneData(3 , com);
   com = 0;
   if((DispTab[n1]&0x20) == 0x20){ com = com + 0x10;}
   if((DispTab[n2]&0x20) == 0x20){ com = com + 0x20;}
   if((DispTab[n3]&0x20) == 0x20){ com = com + 0x40;}
   if((DispTab[n4]&0x20) == 0x20){ com = com + 0x80;}
   Ht1621WrOneData(4 , com);
   com = 0;
   if((DispTab[n1]&0x08) == 0x08){ com = com + 0x10;}
   if((DispTab[n2]&0x08) == 0x08){ com = com + 0x20;}
   if((DispTab[n3]&0x08) == 0x08){ com = com + 0x40;}
   if((DispTab[n4]&0x08) == 0x08){ com = com + 0x80;}
   Ht1621WrOneData(5 , com);
   com = 0;
   if((DispTab[n1]&0x04) == 0x04){ com = com + 0x10;}
   if((DispTab[n2]&0x04) == 0x04){ com = com + 0x20;}
   if((DispTab[n3]&0x04) == 0x04){ com = com + 0x40;}
   if((DispTab[n4]&0x04) == 0x04){ com = com + 0x80;}
   Ht1621WrOneData(6 , com);
}