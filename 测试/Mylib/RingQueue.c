/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         ���ζ��нṹ
*
* File : RingQueue.c
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2018/02/23
* version: V1.2 
* NOTE(s): 
*
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.
*           2018/02/23   1.add the marco(RQ_ARGUMENT_CHECK_EN) to controll argument check so user can save 
*                          more code.
*                        2.add the ADDRESSING MODE so the buffer can be defined in banked addressing area.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     INCLUDES
*********************************************************************************************************
*/
#include "RingQueue.h"

/*
*********************************************************************************************************
*                                LOCAL FUNCTION DECLARATION
*********************************************************************************************************
*/

#if(RQ_ARGUMENT_CHECK_EN == TRUE)
  #define argCheck(cond,err,rVal)  if(cond) { *perr = (err); return (rVal); }
#else
  #define argCheck(cond,err,rVal)
#endif // of (SPI_ARGUMENT_CHECK_EN == TRUE)



/*
*********************************************************************************************************
*                                LOCAL FUNCTION DECLARE
*********************************************************************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
// �ڲ�ʹ�ã�����������ָ���ڻ��λ���������ǰ�ƶ�һ��(��β�˻��ƻ�ͷ)
static void _forwardPointer(RING_QUEUE *pQueue,pRQTYPE* pPointer);
#pragma CODE_SEG DEFAULT
/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : To initialize the ring queue.    ��ʼ�����ζ���
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               pbuf     pointer to the buffer(an array);             ָ���Զ���Ļ�����(ʵ�ʾ��Ǹ�����)
*               bufSize  the Size of the buffer;                      �������Ĵ�С;
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                           RQ_ERR_NONE                                       
*                           RQ_ERR_SIZE_ZERO
*                           RQ_ERR_POINTER_NULL
*
* Return      : the pointer to the ring queue control block;        ����ָ���ζ��п��ƿ��ָ��
*               0x00 if any error;                                  ����������򷵻�NULL
*
*Note(s):
*********************************************************************************************************
*/

RING_QUEUE* RingQueueInit(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short bufSize,unsigned char *perr)
{
  argCheck(pQueue == 0x00 || pbuf == 0x00,RQ_ERR_POINTER_NULL,0x00);
  argCheck(bufSize == 0,RQ_ERR_SIZE_ZERO,0x00);
  pQueue->RingBufCtr = 0;
  pQueue->RingBuf = pbuf;
  pQueue->RingBufInPtr = pbuf;
  pQueue->RingBufOutPtr = pbuf;
  pQueue->RingBufSize = bufSize;
  pQueue->RingBufEnd = pbuf + bufSize;
  *perr = RQ_ERR_NONE;     
  return pQueue;
}

/*
*********************************************************************************************************
*                                        RingQueueIn()
*
* Description : Enqueue an element.    ���һ��Ԫ��
*
* Arguments   : pQueue   pointer to the ring queue control block;    ָ���ζ��п��ƿ��ָ��
*               data     the data to enqueue;                        Ҫ��ӵ�����
*               option   option when queue is full ,you can choose:  ����������ʱ���ѡ��,�����ѡ��:
*                            RQ_OPTION_WHEN_FULL_DISCARD_FIRST          ������ͷ��Ԫ�������ȥ�µ�Ԫ��
*                            RQ_OPTION_WHEN_FULL_DONT_IN                ������¸���Ԫ��
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                             RQ_ERR_NONE                            if no err happen
*                             RQ_ERR_POINTER_NULL                    if pointer is 0x00
*                             RQ_ERR_BUFFER_FULL                     if buffer is full
*
* Return       : the Elements Count after enqueue the element
*                    ���ú���������е�Ԫ�ظ���
*Note(s)       :
*********************************************************************************************************
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
unsigned short RingQueueIn(RING_QUEUE *pQueue,RQTYPE data,unsigned char option,unsigned char *perr)
{
  argCheck(pQueue == 0x00,RQ_ERR_POINTER_NULL,0x00);
  if(pQueue->RingBufCtr >= pQueue->RingBufSize){
    *perr = RQ_ERR_BUFFER_FULL;     
    if(option == RQ_OPTION_WHEN_FULL_DISCARD_FIRST){
      _forwardPointer(pQueue,&pQueue->RingBufOutPtr); /* Wrap OUT pointer                          */  
    }else{                                            // option == RQ_OPTION_WHEN_FULL_DONT_IN
      return pQueue->RingBufCtr;
    }
  }else{
    pQueue->RingBufCtr++;                             /* No, increment character count            */      
    *perr = RQ_ERR_NONE;
  }
  *pQueue->RingBufInPtr = data;                       /* Put character into buffer                */  
  _forwardPointer(pQueue,&pQueue->RingBufInPtr);      /* Wrap IN pointer                          */  
  return pQueue->RingBufCtr;
}
/*
*********************************************************************************************************
*                                        RingQueueOut()
*
* Description : Dequeue an element.       ����һ��Ԫ��
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                              RQ_ERR_NONE                            if no err happen
*                              RQ_ERR_POINTER_NULL                    if pointer is 0x00
*                              RQ_ERR_BUFFER_EMPTY                    if buffer is empty
*
* Return      : 0                 if any error or the data is 0;
*               others            the data 
*               
*Note(s):
*********************************************************************************************************
*/
RQTYPE RingQueueOut(RING_QUEUE *pQueue,unsigned char *perr)
{
  RQTYPE data;
  argCheck(pQueue == 0x00,RQ_ERR_POINTER_NULL,0x00);
  if(pQueue->RingBufCtr == 0){
    *perr = RQ_ERR_BUFFER_EMPTY;        
    return 0;
  }
  pQueue->RingBufCtr--;                                      /*  decrement character count           */  
  data = *pQueue->RingBufOutPtr;                      /* Get character from buffer                */  
  _forwardPointer(pQueue,&pQueue->RingBufOutPtr);        /* Wrap OUT pointer                          */  
  *perr = RQ_ERR_NONE;
  return data;
}
#pragma CODE_SEG DEFAULT
/*
*********************************************************************************************************
*                                        RingQueueMatch()
*
* Description : Match the given buffer in RingQueue          �ڻ��ζ�����ƥ�����������
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               pbuf     pointer to the chars need to match;
*               len      the length of the chars
* Return      :  -1       Don't match            -1    ��û��ƥ�䵽
*                >= 0     match                  >= 0  ��ƥ�䵽��
*
*Note(s):
*********************************************************************************************************
*/

short RingQueueMatch(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short len)
{
  pRQTYPE pPosQ,pCurQ,pCurB,pEndB;
  unsigned short rLen,Cnt;
  if(len > pQueue->RingBufCtr)
    return -1;
  pPosQ = pQueue->RingBufOutPtr;
  pEndB = pbuf + len;
  Cnt = 0;
  rLen = pQueue ->RingBufCtr;
  while(rLen-- >= len){          // if remian length of queue bigger than buffer. continue
    pCurQ = pPosQ;
    pCurB = pbuf;
    while(pCurB != pEndB && *pCurQ == *pCurB) {    // compare one by one,until match all(pCurB==pEndB) or some one don't match
      _forwardPointer(pQueue,&pCurQ);
      pCurB++;
    }
    if(pCurB == pEndB)                                                 // if match all
       return Cnt;
     Cnt++;
     _forwardPointer(pQueue,&pPosQ);
  }
  return -1;
}

/*
*********************************************************************************************************
*                                        RingQueueClear()
*
* Description:  Clear the RingQueue.        ��ջ��ζ���
*
* Arguments  :  pQueue    pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*
* Return:             
*
* Note(s):
*********************************************************************************************************
*/

void RingQueueClear(RING_QUEUE *pQueue)
{
#if(RQ_ARGUMENT_CHECK_EN == TRUE)
  if(pQueue == 0x00)
    return;
#endif
  pQueue->RingBufCtr = 0;
  pQueue->RingBufInPtr = pQueue -> RingBufOutPtr;
}

/*
*********************************************************************************************************
*                                       LOCAL FUNCTION 
*********************************************************************************************************
*/

#pragma CODE_SEG __NEAR_SEG NON_BANKED
static void _forwardPointer(RING_QUEUE *pQueue,pRQTYPE* pPointer)
{
  if (++*pPointer == pQueue->RingBufEnd)   
    *pPointer = pQueue->RingBuf;        /* Wrap OUT pointer                          */  
}
#pragma CODE_SEG DEFAULT
