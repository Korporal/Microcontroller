#include <stdio.h>
#include <string.h>
#include <stm32f7xx_hal.h>
#include <stm32_hal_legacy.h>
#include <ESP8266.h>
#define CR 0x0D
#define LF 0x0A

//#ifdef __cplusplus
//extern "C"
//#endif

extern "C" void SysTick_Handler(void);

void InitPin(uint32_t PinNumber);
void InitUART(UART_HandleTypeDef * UART_Handle);
void InitClocks();
void Zeroize(void * Buffer, size_t Length);
HAL_StatusTypeDef SendCommand(UART_HandleTypeDef * UART, const char * Text);
int Receive(UART_HandleTypeDef * UART, uint32_t Timeout);
bool ContainsOK(uint8_t * Buffer, int Len);

// A modified version of: https://visualgdb.com/tutorials/arm/stm32/uart/hal/

int main(void)
{
	HAL_StatusTypeDef status;
	int len;
	int I, J;
	uint32_t delay = 100;
	
	HAL_Init();
 
	ESP8266 device; // Note, this does call empty arg constructor

	for (I=0; I < 1000; I++)
	{
		status = device.SendCommand("AT");
		
		if (status != HAL_OK)
			break;
		
		len = device.ReceiveResponse(500);
		
		if (len != 11)
			break;
		
		HAL_Delay(delay);
		
		status = device.SendCommand("AT+GMR");
		
		if (status != HAL_OK)
			break;
		
		len = device.ReceiveResponse(500);
		
		if (len != 115)
			break;
		
		HAL_Delay(delay);

	}

	return 0;
}


bool ContainsOK(uint8_t * Buffer, int len)
{
	//int len = strlen((const char *)Buffer);
	
	if (len < 4)
		return false;
	
	for (int X = 0; X < len; X++)
	{
		if (X + 4 > len)
			return false;
		
		if (Buffer[X] == 'O' && Buffer[X + 1] == 'K' && Buffer[X + 2] == CR && Buffer[X + 3] == LF)
			return true;
		
	}
	
	return false;
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}