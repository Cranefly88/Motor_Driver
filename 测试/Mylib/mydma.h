#ifndef __MYDMA_H__
#define	__MYDMA_H__

#include "board.h"

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);//����DMAx_CHx
void MYDMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//ʹ��һ��DMA����		   
void DMA1_Stream3_IRQHandler(void);

#endif
