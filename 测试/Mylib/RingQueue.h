/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         ���ζ��нṹ
*
* File : RingQueue.h
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2018/02/23
* version: V1.2
* NOTE(s): ��γ���������һ�������Ļ���������ģ�⻷�ζ��еĹ���
*                   ���������Զ����仺�����ռ�,�û���Ҫ�Լ��������ռ�,����Ҫ��֤��ֱ�ӷ��ʻ�����
*                   // ��ĳ�������ڴ�ռ�
*                   RQTYPE buffer[BUFFER_SIZE];
*                   RING_QUEUE que,*ptr_que;
*                   unsigned char err;
*                   // ��ʼ��
*                   ptr_que = RingQueueInit(&que,buffer,BUFFER_SIZE,&err);
*                   if(err == RQ_ERR_NONE  ){
*                     // ��ʼ���ɹ�,ʹ����������
*                   }
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.
*           2018/02/23   1.add the marco(RQ_ARGUMENT_CHECK_EN) to controll argument check so user can save 
*                          more code.
*                        2.add the ADDRESSING MODE so the buffer can be defined in banked addressing area.
*********************************************************************************************************
*/


#ifndef   RING_QUEUE_H
#define   RING_QUEUE_H

/*
********************************************************************************************
*                                   MISCELLANEOUS
********************************************************************************************
*/

#ifndef  FALSE
#define  FALSE    0
#endif

#ifndef  TRUE
#define  TRUE     1
#endif

/*
*********************************************************************************************************
*                                       ADDRESSING MODE Ѱַģʽ
*********************************************************************************************************
*/

// uncomment the corresponding line to select the addressing mode to the buffer of RingQueue module.
// if you don't understand. Just use the extended addressing mode
// ȡ����Ӧ�е�ע����ѡ���λ�����ģ����ʻ�����ʱʹ�õ�Ѱַ��ʽ
// ����㲻֪������ʲô��˼�Ļ����Ǿ�����չѰַ�����ˣ�����Ĭ�ϵķ�ʽ

// extended addressing mode ��չ��Ѱַ(Ĭ��)
#define RQ_ADDRESSING_MODE
// banked RAM addressing mode   RAM��ҳ��Ѱַ
//#define RQ_ADDRESSING_MODE __rptr
// global addressing mode   ȫ��Ѱַ
//#define RQ_ADDRESSING_MODE __far

/*
*********************************************************************************************************
*                                       CONFIGURATION  ����
*********************************************************************************************************
*/

#define RQ_ARGUMENT_CHECK_EN    TRUE     // TRUE: arguments will be checked, however,this will 
                                         //       cost a little code volume.

/*
*********************************************************************************************************
*                                        CONSTANTS     ����
*********************************************************************************************************
*/
#define   RQ_ERR_NONE                        0u

#define   RQ_ERR_POINTER_NULL                1u
#define   RQ_ERR_SIZE_ZERO                   2u

#define   RQ_ERR_BUFFER_FULL                 3u
#define   RQ_ERR_BUFFER_EMPTY                4u

#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full
#define   RQ_OPTION_WHEN_FULL_DONT_IN        1u       // discard new element when ring buffer is full
/*
*********************************************************************************************************
*                                    DATA TYPE    ��������
*********************************************************************************************************
*/

// define the data type that stores in the RingQueue.       ������ڻ��λ������ڵ����ݵ�����
typedef unsigned char RQTYPE;
typedef RQTYPE *RQ_ADDRESSING_MODE pRQTYPE;
typedef struct {
    unsigned short  RingBufCtr;              /* Number of characters in the ring buffer */
    unsigned short  RingBufSize;             /* Ring buffer Size */    
    pRQTYPE RingBufInPtr;                    /* Pointer to where next character will be inserted        */  
    pRQTYPE RingBufOutPtr;                   /* Pointer from where next character will be extracted     */  
    pRQTYPE RingBuf;                         /* Ring buffer array */  
    pRQTYPE RingBufEnd;                      /* Point to the end of the buffer */
} RING_QUEUE;

/*
*********************************************************************************************************
*                                  FUNCTION PROTOTYPES ����ԭ��
*********************************************************************************************************
*/

RING_QUEUE *RingQueueInit(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short bufSize,unsigned char *perr);
//#pragma CODE_SEG __NEAR_SEG NON_BANKED
unsigned short RingQueueIn(RING_QUEUE *pQueue,RQTYPE data,unsigned char option,unsigned char *perr);
RQTYPE RingQueueOut(RING_QUEUE *pQueue,unsigned char *perr);
//#pragma CODE_SEG DEFAULT
short RingQueueMatch(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short len);
void RingQueueClear(RING_QUEUE *pQueue);

/*
*********************************************************************************************************
*                                        RingQueueIsEmpty()
*
* Description :  whether the RingQueue is empty.   ���ζ����Ƿ�Ϊ��
*
* Arguments   :  pQueue  pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*
* Return      :  TRUE    the RingQueue is empty.
*                FALSE   the RingQueue is not empty.
* Note(s)     :
*********************************************************************************************************
*/

#define RingQueueIsEmpty(pQueue) ((pQueue)->RingBufCtr == 0)

/*
*********************************************************************************************************
*                                        RingQueueIsFull()
*
* Description : whether the RingQueue is full.    ���ζ����Ƿ�Ϊ��
*
* Arguments   : pQueue  pointer to the ring queue control block;   ָ���ζ��п��ƿ��ָ��
*
* Return      : TRUE    the RingQueue is full.
*               FALSE   the RingQueue is not full.
* Note(s)     :
*********************************************************************************************************
*/

#define RingQueueIsFull(pQueue)  ((pQueue)->RingBufCtr >= (pQueue)->RingBufSize)

#endif
