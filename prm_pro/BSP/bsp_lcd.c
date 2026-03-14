#include "bsp_lcd.h"
#include "gd32f10x.h"

#define LCD_RST_PORT 	GPIOB
#define LCD_RST_PIN 	GPIO_PIN_5

#define LCD_A0_PORT 	GPIOA
#define LCD_AO_PIN 		GPIO_PIN_6

#define SPI_CS_PORT 	GPIOA
#define SPI_CS_PIN 		GPIO_PIN_4

#define SPI_SCK_PORT 	GPIOA
#define SPI_SCK_PIN 	GPIO_PIN_5

#define SPI_MOSI_PORT 	GPIOA
#define SPI_MOSI_PIN 	GPIO_PIN_7

#define LCD_RST_ON 		GPIO_BC(LCD_RST_PORT) = (uint32_t)LCD_RST_PIN
#define LCD_RST_OFF 	GPIO_BOP(LCD_RST_PORT) = (uint32_t)LCD_RST_PIN

#define SPI_CS_ON 		GPIO_BC(SPI_CS_PORT) = (uint32_t)SPI_CS_PIN
#define SPI_CS_OFF 		GPIO_BOP(SPI_CS_PORT) = (uint32_t)SPI_CS_PIN

#define LCD_A0_DATA 	GPIO_BOP(LCD_A0_PORT) = (uint32_t)LCD_AO_PIN
#define LCD_A0_CMD 		GPIO_BC(LCD_A0_PORT) = (uint32_t)LCD_AO_PIN

#define LCD_WIDTH 		240
#define LCD_HEIGHT 		160

#define LCD_CMD 0
#define LCD_DATA 1

void lcd_gpio_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA); // ʹ�ܶ˿�ʱ��
	rcu_periph_clock_enable(RCU_GPIOB); // ʹ�ܶ˿�ʱ��
	rcu_periph_clock_enable(RCU_AF);
	
	gpio_init(LCD_RST_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LCD_RST_PIN);
	gpio_init(LCD_A0_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LCD_AO_PIN);
	gpio_init(SPI_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SPI_CS_PIN);
	gpio_init(SPI_SCK_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, SPI_SCK_PIN);
	gpio_init(SPI_MOSI_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, SPI_MOSI_PIN);
}

void lcd_spi_init(void)
{
	spi_parameter_struct spi_init_struct;

	rcu_periph_clock_enable(RCU_SPI0);

	spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
	spi_init_struct.device_mode = SPI_MASTER;
	spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
	spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
	spi_init_struct.nss = SPI_NSS_SOFT;
	spi_init_struct.prescale = SPI_PSC_128;
	spi_init_struct.endian = SPI_ENDIAN_MSB;
	spi_init(SPI0, &spi_init_struct);

	/* enable SPI0 */
	spi_enable(SPI0);
}

uint8_t spi_send_byte(uint8_t byte)
{

	/* loop while data register in not emplty */
	while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE))
		;

	/* send byte through the SPI0 peripheral */
	spi_i2s_data_transmit(SPI0, byte);

	/* wait to receive a byte */
	while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE))
		;

	/* return the byte read from the SPI bus */
	return (spi_i2s_data_receive(SPI0));
}

void LCD_delay(uint32_t ms)
{
	uint32_t i = ms * 1000;
	while (i--)
	{
		__NOP();
	}
}
void LCD_write(uint8_t type, uint8_t data)
{
	if (type == LCD_CMD)
	{
		LCD_A0_CMD;
	}
	else
	{
		LCD_A0_DATA;
	}

	SPI_CS_ON;
	spi_send_byte(data);
	SPI_CS_OFF;
}

void LCD_write_data(uint8_t type, uint8_t *data, uint16_t len)
{
	uint16_t i = 0;

	if (type == LCD_CMD)
	{
		LCD_A0_CMD;
	}
	else
	{
		LCD_A0_DATA;
	}

	SPI_CS_ON;

	for (i = 0; i < len; i++)
	{
		spi_send_byte(data[i]);
	}
	SPI_CS_OFF;
}

void LCD_reset(void)
{
	LCD_RST_ON;
	LCD_delay(20);
	LCD_RST_ON;
	LCD_delay(20);
	LCD_RST_OFF;
	LCD_delay(200);
}

void display_ascll4824(uint8_t x,uint8_t y,uint8_t mode,uint8_t dat);

void LCD_Init(void)
{
	lcd_gpio_init();
	lcd_spi_init();
	LCD_reset();

	LCD_write(LCD_CMD, 0x30);   //Extension Command1
	LCD_write(LCD_CMD, 0x6e);   //Enable Master
	
	LCD_write(LCD_CMD, 0x31);   //Extension Command2
	LCD_write(LCD_CMD, 0xd7);   //Disable Auto Read
	LCD_write(LCD_DATA,0x9f);  //Extension Command
	LCD_write(LCD_CMD, 0xe0);   //Enable OTP Read
	LCD_write(LCD_DATA,0x00);  //Extension Command
	LCD_delay(20);
	LCD_write(LCD_CMD, 0xe3);   //OTP Up-Load
	LCD_delay(20);
	LCD_write(LCD_CMD, 0xe1);   //OTP Control Out
		
	LCD_write(LCD_CMD, 0x30);   //Extension Command1
	LCD_write(LCD_CMD, 0x94);   //Sleep Out
	LCD_write(LCD_CMD, 0xae);   //Display off
	LCD_delay(50);
	
	LCD_write(LCD_CMD, 0x20);   //Power Control
	LCD_write(LCD_DATA,0x0b);  //VB,VR,VF AllON

	LCD_write(LCD_CMD, 0x0C);   	// D0=0 LSB on top

	LCD_write(LCD_CMD, 0x81);   //Set VOP
	LCD_write(LCD_DATA,0x14);  //0x03/0x3f=13.8V 0x2b=13.0 ;0x1d/0x04=15.0V		
	LCD_write(LCD_DATA,0x05);  //	
	
	LCD_write(LCD_CMD, 0x31);   	// Extension Command 2	
	LCD_write(LCD_CMD, 0x32);   //Analog Circuit Set	
	LCD_write(LCD_DATA,0x00);  //
	LCD_write(LCD_DATA,0x01);  //Booster Efficiency-6KHz
	LCD_write(LCD_DATA,0x02);  //0x04=Bias=1/10	0x02=1/12
	
	LCD_write(LCD_CMD, 0x51);   //Booster Level 	
	LCD_write(LCD_DATA,0xfb);  //*10		
	
	LCD_write(LCD_CMD, 0x30);   //Extension Command1	
	
	LCD_write(LCD_CMD, 0xf0);   //Display Mode
	LCD_write(LCD_DATA,0x10);  //Mono Mode
	
	LCD_write(LCD_CMD, 0xca);   //Display Control
	LCD_write(LCD_DATA,0x00);  //CL Dividing Ratio---Not Divide
	LCD_write(LCD_DATA,0x9f);  //Duty Set---- 1/160		
	LCD_write(LCD_DATA,0x00);  //Frame Inversion
	
	LCD_write(LCD_CMD, 0x31);   	// Extension Command 2
	
	LCD_write(LCD_CMD, 0xf2);   //Temperature Range
	LCD_write(LCD_DATA,0x1E);  // TA=-10
	LCD_write(LCD_DATA,0x28);  // TB=0
	LCD_write(LCD_DATA,0x32);  // TC=10
	
	LCD_write(LCD_CMD, 0xf0);   //Frame rate
	LCD_write(LCD_DATA,0x16);  //
	LCD_write(LCD_DATA,0x16);  //
	LCD_write(LCD_DATA,0x16);  // 0X18
	LCD_write(LCD_DATA,0x16);  //102Hz
	
	LCD_write(LCD_CMD, 0x30);   //Extension Command1	
	LCD_write(LCD_CMD, 0xbc);   //Data Scan Direction
	LCD_write(LCD_DATA,0x00);  //Address direction
	
	LCD_write(LCD_CMD, 0xa6);  //Normal display
	
	LCD_write(LCD_CMD, 0x31);   //Extension Command2
	LCD_write(LCD_CMD, 0x40);   //Internal Power Supply

	LCD_clear(0);
	LCD_write(LCD_CMD, 0xaf);  //Display ON
	
	LCD_delay(100);
}

void LCD_set_window(uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end)
{
	LCD_write(LCD_CMD, 0x30);
	LCD_write(LCD_CMD, 0x15);	  /* 列地址设置 */
	LCD_write(LCD_DATA, x_start); /* 起始列低4位 */
	LCD_write(LCD_DATA, x_end);	  /* 结束列低4位 */

	LCD_write(LCD_CMD, 0x75);	  /* 页地址设置 */
	LCD_write(LCD_DATA, y_start); /* 起始页 */
	LCD_write(LCD_DATA, y_end);	  /* 结束页 */
}

void LCD_clear(uint8_t color)
{
	uint16_t i;

	uint8_t fill_data = color ? 0xFF : 0x00;

	/* 设置全屏区域 */
	LCD_set_window(0, LCD_WIDTH-1, 0, LCD_HEIGHT / 8 -1);

	/* 写入数据命令 */
	LCD_write(LCD_CMD, 0x5C); /* 写内存命令 */

	/* 填充数据 */
	for (i = 0; i < (LCD_WIDTH * LCD_HEIGHT /8); i++)
	{
		LCD_write(LCD_DATA, fill_data);
	}
}

void LCD_Fill(uint8_t color,uint8_t x_start, uint8_t x_end, uint8_t y_start, uint8_t y_end)
{
	uint16_t i;


	/* 设置全屏区域 */
	LCD_set_window(x_start, x_end, y_start, y_end);

	/* 写入数据命令 */
	LCD_write(LCD_CMD, 0x5C); /* 写内存命令 */

	/* 填充数据 */
	for (i = 0; i < (LCD_WIDTH * LCD_HEIGHT /8); i++)
	{
		LCD_write(LCD_DATA, color);
	}
}

void LCD_FillData(uint8_t *data)
{
	uint16_t i;

	LCD_set_window(0, LCD_WIDTH-1, 0, LCD_HEIGHT / 8 -1);

	/* 写入数据命令 */
	LCD_write(LCD_CMD, 0x5C); /* 写内存命令 */
	for (i = 0; i < (LCD_WIDTH * LCD_HEIGHT /8); i++)
	{
		LCD_write(LCD_DATA, data[i]);
	}
	
}

