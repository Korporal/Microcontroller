#include <ESP8266.h>

bool ESP8266::clocks_initialized = false;
ESP8266::ESP8266()
{
	InitClocks();
	InitPin(GPIO_PIN_12);
	InitPin(GPIO_PIN_2);
	InitUART(&UART_Handle);
}

void ESP8266::InitClocks()
{
	if (clocks_initialized)
		return;
	
	__UART5_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();

	clocks_initialized = true;
}
void ESP8266::InitUART(UART_HandleTypeDef * UART_Handle)
{
	UART_Handle->Instance        = UART5;
	UART_Handle->Init.BaudRate   = 115200;
	UART_Handle->Init.WordLength = UART_WORDLENGTH_8B;     // These settings match the ESP8266 default settings and the terminal application
	UART_Handle->Init.StopBits   = UART_STOPBITS_1;
	UART_Handle->Init.Parity     = UART_PARITY_NONE;
	UART_Handle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UART_Handle->Init.Mode       = UART_MODE_TX_RX;
	
	if (HAL_UART_Init(UART_Handle) != HAL_OK)
		asm("bkpt 255");
}

ESP8266::~ESP8266()
{
	;
}
void ESP8266::InitPin(uint32_t PinNumber)
{
	GPIO_InitTypeDef GPIO_Init = {};
	
	switch (PinNumber)
	{
	case GPIO_PIN_12:
		{
			GPIO_Init.Pin = GPIO_PIN_12;
			GPIO_Init.Mode = GPIO_MODE_AF_PP;
			GPIO_Init.Alternate = GPIO_AF8_UART5;
			GPIO_Init.Speed = GPIO_SPEED_HIGH;
			GPIO_Init.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOC, &GPIO_Init);
			return;
		}
	case GPIO_PIN_2:
		{
			GPIO_Init.Pin = GPIO_PIN_2;
			GPIO_Init.Mode = GPIO_MODE_AF_OD;
			GPIO_Init.Alternate = GPIO_AF8_UART5;
			GPIO_Init.Speed = GPIO_SPEED_HIGH;
			GPIO_Init.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOD, &GPIO_Init);
			return;
		}
	}
}

HAL_StatusTypeDef ESP8266::SendCommand(const char * Text)
{
	uint8_t buffer[512];

	int len = strlen((const char *)Text);
	
	strcpy((char*)buffer, Text);
	
	buffer[len] = CR;
	buffer[len + 1] = LF;
	
	return HAL_UART_Transmit(&UART_Handle, buffer, len + 2, HAL_MAX_DELAY);
}

// A crude way to read and get an idea of how many bytes were sent/available.
// I'm not clear on the underlying behavior here so this is as I say, crude.
int ESP8266::ReceiveResponse(uint32_t Timeout)
{
	HAL_StatusTypeDef status;
	int count = 0;
	uint8_t buffer[512] = {};

	status = HAL_UART_Receive(&UART_Handle, &(buffer[count]), 1, Timeout);       // 50 mS just to get some response.
	
	while(status == HAL_OK)
	{
		count++;
		
		//		if (buffer[count - 1] == 'K' && count > 1 && buffer[count - 2] == 'O') 
		//			break;
		
				status = HAL_UART_Receive(&UART_Handle, &(buffer[count]), 1, Timeout);      // 50 mS just to get some response.
	}
	
	return count;
}

