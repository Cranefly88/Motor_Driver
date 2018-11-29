#ifndef _JUDGEMENT_H
#define _JUDGEMENT_H

#include "board.h"

#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif

#define JudgeBufferLength       200

#define ERROR_BUFFER_EMPTY   0
#define ERROR_CRC16_WRONG    2




//����ϵͳ���ݻ���
__DRIVER_EXT uint8_t JudgeDataBuffer[JudgeBufferLength];
__DRIVER_EXT int full_count;


//��ʽת��������
typedef union
{
    uint8_t U[4];
    float F;
    int I;
}FormatTrans;

/*************************����ϵͳ���ݽṹ��**************************************/

/** 
  * @brief  judgement data command id
  */
typedef enum
{
  GAME_INFO_ID       			= 0x0001,  //10Hz
  REAL_HURT_DATA_ID 			= 0x0002,
  REAL_SHOOT_DATA_ID 			= 0x0003,
  REAL_POWER_HEAT_DATA_ID = 0x0004,  //50hZ
  GAME_RFID_DATA_ID     	= 0x0005,  //10hz
  GAME_RESULT_ID     			= 0x0006,
  GAIN_BUFF_ID       			= 0x0007,
  REAL_POSITION_ID       	= 0x0008,  //50hz
  
  STU_CUSTOM_DATA_ID = 0x0100,
  ROBOT_TO_CLIENT_ID = 0x0101,
  CLIENT_TO_ROBOT_ID = 0x0102,
} judge_data_id_e;


/** 
  * @brief  brief frame header structure definition
  */
typedef __packed struct
{
	uint8_t   sof;
	uint16_t  data_length;
	uint8_t   seq;
	uint8_t   crc8;
}frame_header_t;


__DRIVER_EXT judge_data_id_e judge_cmd_id;
//__DRIVER_EXT frame_header_t judge_frame_header;

/** 
  * @brief  game information structures definition(0x0001)
  *         this package send frequency is 10Hz
  */
typedef __packed struct
{
    uint16_t   stageRemianTime;
    uint8_t    gameProgress;
  /* current race stage
   0 not start
   1 preparation stage
   2 self-check stage
   3 5 seconds count down
   4 fighting stage
   5 result computing stage */
    uint8_t    robotLevel;
    uint16_t   remainHP;
    uint16_t   maxHP;
}extGameRobotState_t;


/** 
  * @brief  real time blood volume change data(0x0002)
  */	
typedef __packed struct
{
    uint8_t   armorType  : 4;
 /* 0-3bits: the attacked armor id:
    0x00: 0 front
    0x01:1 left
    0x02:2 behind
    0x03:3 right
    0x04:3 top(1)
    0x05:3 top(2)
    others reserved*/	
    uint8_t   hurtType    : 4;
 /* 4-7bits: blood volume change type
    0x00: armor attacked
    0x01:module offline*/
}extRobotHurt_t;


/** 
  * @brief  real time shooting data(0x0003)
  */
typedef __packed struct
{
	uint8_t   bulletType;
	uint8_t   bulletFreq;
	float     bulletSpeed;
}extShootData_t;


/** 
  * @brief  ʵʱ������������(0x0004)

	�ֽ�ƫ�� ��С 			˵��
	0 				4 		���������ѹ����λ : ��
	4 				4 		���������������λ : ��
	8 				4 		����������ʣ���λ : ��
	12			  4 		���̹��ʻ��壬��λ : ��
	16 				2 		17mm ǹ������
	18 				2 		42mm ǹ������
  */
typedef __packed struct
{
    float     chassisVolt;
    float     chassisCurrent;
    float     chassisPower;
    float     chassisPowerBuffer;
    uint16_t  shooterHeat0;
    uint16_t  shooterHeat1;
}extPowerHeatData_t;


/** 
  * @brief  ���ؽ�������(0x0005)
  */
typedef __packed struct
{
    uint8_t   cardType;
    uint8_t   cardIdx;
}extRfidDetect_t;


/** 
  * @brief  ����ʤ������(0x0006)
  */
typedef __packed struct
{
    uint8_t  winner;
}extGameResult_t;

/** 
  * @brief  Buff��ȡ����(0x0007)
  */
typedef __packed struct
{
    uint8_t   buffType;
    uint8_t   buffAddition;
} extGetBuff_t;

/** 
  * @brief  ������λ�ó�����Ϣ(0x0008)
	�ֽ�ƫ�� ��С     					˵��
	0         4       λ�� X ����ֵ����λ : ��
	4         4 			λ�� Y ����ֵ����λ : ��
	8         4 			λ�� Z ����ֵ����λ : ��
	12        4 			ǹ�ڳ���Ƕ�ֵ����λ : ��*/
typedef __packed struct
{
    float    xx;
    float    yy;
    float    zz;
    float    yaw;
}extGameRobotPos_t;


typedef struct
{
	frame_header_t          judge_frame_header;
	uint8_t                 judge_rxcmd_id;
  extGameRobotState_t 		game_information;
  extRobotHurt_t  				real_hurt_data;
  extShootData_t       		real_shoot_data;
  extPowerHeatData_t      real_powerheat_data;
  extRfidDetect_t      		rfid_data;
  extGameResult_t      		game_result_data;
  extGetBuff_t         		get_buff_data;
	extGameRobotPos_t       real_robot_pos;
	uint16_t                crc16;
} receive_judge_t;


typedef __packed struct
{
    float  data1;
    float  data2;
    float  data3;
    uint8_t mask;
}extShowData_t;

__DRIVER_EXT extShowData_t JudgeSendData;


/*************************����ϵͳ���ݽṹ��**************************************/

typedef struct _vec4u
{
	uint8_t data[4];
}_vec4u;



void Judge_Uart5_Config(void);
uint8_t get_judge_data(uint8_t *buff,uint16_t sof_arr);
float u8toflaot(uint8_t u4,uint8_t u3,uint8_t u2,uint8_t u1);
u8 judge_data_process(void);
uint8_t get_length(u8 cmdid);
int judge_Ring_queue(void);
void Uart5_SendChar(unsigned char b);


#endif /*_JUDGEMENT_H*/
