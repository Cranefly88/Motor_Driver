#ifndef __SPI_H
#define __SPI_H
#include "board.h"

 	    													  
void SPI2_Init(void);			 //��ʼ��SPI1��
void SPI2_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
long int SPI2_ReadWriteByte(long int TxData);//SPI1���߶�дһ���ֽ�
void Spi2_Function(int OutputBuff[],int16_t InputBuff[], unsigned int NoOfBytes);
		 
#endif
