#ifndef __TDT_INFANTRY__
#define __TDT_INFANTRY__

#define Run_Mode 1		
#define Abandon !Run_Mode		/**�����õĲ��ֲ�����**/


#define FB_MAXSPEED		16384.0f/660.0f	  /**< ����ǰ�������ٶ�*/
#define LR_MAXSPEED		7500.0f/660.0f	  /**< �������ҵ�����ٶ�*/
#define ROT_MAXASPEED	4500.0f/660.0f*5	  	/**< ������ת�������ٶȣ���ÿ��*/

#define FB_MINSPEED		7000.0f/6600.0f	  /**< ����ǰ�����С�ٶ�*/
#define LR_MINSPEED		7000.0f/6600.0f	  /**< �������ҵ���С�ٶ�*/
#define ROT_MINASPEED	360.0f/3300.0f*10	  	/**< ������ת����С���ٶȣ���ÿ��*/

/***************���̳��ؼ�λ������************/
/***************I2C GPIO����******************/
#define RCC_I2C1	     RCC_AHB1Periph_GPIOC
#define I2C1_PORT      GPIOC
#define I2C1_Pin_SCL   GPIO_Pin_0
#define I2C1_Pin_SDA   GPIO_Pin_1

#define RCC_I2C2	     RCC_AHB1Periph_GPIOB
#define I2C2_PORT      GPIOB
#define I2C2_Pin_SCL   GPIO_Pin_0
#define I2C2_Pin_SDA   GPIO_Pin_1

//#define RCC_I2C2_SCL	     RCC_AHB1Periph_GPIOA
//#define RCC_I2C2_SDA	     RCC_AHB1Periph_GPIOA
//#define I2C2_PORT_SCL      GPIOA
//#define I2C2_Pin_SCL   GPIO_Pin_1
//#define I2C2_PORT_SDA      GPIOA
//#define I2C2_Pin_SDA   GPIO_Pin_2

//#define RCC_I2C2_SCL	     RCC_AHB1Periph_GPIOC
//#define RCC_I2C2_SDA	     RCC_AHB1Periph_GPIOD
//#define I2C2_PORT_SCL      GPIOC
//#define I2C2_Pin_SCL   GPIO_Pin_12
//#define I2C2_PORT_SDA      GPIOD
//#define I2C2_Pin_SDA   GPIO_Pin_2

#define WHEELNUM	  4		  /**< ����������Ŀ*/
#define MAXSET3510  16384  /**< 3510�����������ֵ*/
#define MAXSET6623  5000  /**< 6623�����������ֵ*/
#define MAXSETDM50  29000  /**< DM50�����������ֵ*/
#define Belt_Speed  2500  /**< ͬ����2006�������ֵ*/

#define KEY_B			0x8000
#define KEY_V			0x4000
#define KEY_C			0x2000
#define KEY_X			0x1000
#define KEY_Z			0x0800
#define KEY_G			0x0400
#define KEY_F			0x0200
#define KEY_R			0x0100
#define KEY_E			0x0080
#define KEY_Q			0x0040
#define KEY_CTRL	0x0020
#define KEY_SHIFT	0x0010
#define KEY_D			0x0008
#define KEY_A			0x0004
#define KEY_S			0x0002
#define KEY_W			0x0001


/**
* @struct  _RC
* @brief   ����ң�����ṹ��
*/
typedef struct _RC{
					int16_t CH[11];
	        int16_t CH_LAST[11];
					int16_t FB_Set;	/***����ǰ��***/
					int16_t LR_Set;	/***��������***/
					int16_t RT_Set;	/***������ת(��������)***/
					struct
					{
						int16_t value;
						uint8_t     B;			
						uint8_t     V;			
						uint8_t     C;			
						uint8_t     X;		
						uint8_t     Z;			
						uint8_t     G;		
						uint8_t     F;			
						uint8_t     R;			
						uint8_t     E;			
						uint8_t     Q;		
						uint8_t     CTRL;	
						uint8_t     SHIFT;	
						uint8_t     D;			
						uint8_t     A;			
						uint8_t     S;			
						uint8_t     W;			
					} key;
					struct
					{
						int16_t value;
						uint8_t     B;			
						uint8_t     V;			
						uint8_t     C;			
						uint8_t     X;		
						uint8_t     Z;			
						uint8_t     G;		
						uint8_t     F;			
						uint8_t     R;			
						uint8_t     E;			
						uint8_t     Q;		
						uint8_t     CTRL;	
						uint8_t     SHIFT;	
						uint8_t     D;			
						uint8_t     A;			
						uint8_t     S;			
						uint8_t     W;			
					} last_key;
					struct
					{
						uint8_t     left_jump;			
						uint8_t     Right_jump;
					} mouse;
					struct
					{
						uint8_t     left_jump;			
						uint8_t     Right_jump;
					} mouse_last;
					struct
					{
						int value;
						int     B;			
						int     V;			
						int     C;			
						int     X;		
						int     Z;			
						int     G;		
						int     F;			
						int     R;			
						int     E;			
						int     Q;		
						int     CTRL;	
						int     SHIFT;	
						int     D;			
						int     A;			
						int     S;			
						int     W;	
						int     left_jump;
						int     Right_jump;	
						
					} flag;
	     }Remote_Control;

/**
* @enum  wheel_index
* @brief ����������ӵı��
*/
enum wheel_index
{
	wheel1 = 0,	  	/**< ��ǰ�� */
	wheel2 = 1,		  /**< ��ǰ�� */
	wheel3 = 2,	  	/**< �Һ��� */
	wheel4 = 3	  	/**< ����� */
};

/**
* @struct  _vec2f
* @brief ��άfloat�����ṹ��
*/
typedef struct _vec2f
{
	float data[2];
}vec2f;

/**
* @struct  _vec3f
* @brief ��άfloat�����ṹ��
*/
typedef struct _vec3f
{
	float data[3];
}vec3f;

/**
* @struct  _vec3int16
* @brief ��άint16�����ṹ��
*/
typedef struct _vec3int16
{
	 short data[3];
}vec3int16;

/**
* @struct  _vec4f
* @brief ��άfloat�����ṹ��
*/
typedef struct _vec4f
{
	float data[4];
}vec4f;


/**
* @struct  _pid
* @brief pid�ṹ��
*/
typedef struct _pid
{
	float setValue;
	float feedbackValue;
	float error;
	float lastError;
	float deltaError;
	float integralError;
	float integralErrorMax;
	float kp;
	float ki;
	float kd;
	float pOut;
	float iOut;
	float dOut;
	float out;
}pid;
/**
* @struct  _GY_53
* @brief ������ṹ��
*/

typedef struct _GY_53
{
	int Down;
	int Right;
	int Forward;
	int Back;
}GY53;


#endif
