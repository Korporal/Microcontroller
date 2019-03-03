#include <stdio.h>
#include <string.h>
#include <stm32f7xx_hal.h>
#include <stm32_hal_legacy.h>
#define CR 0x0D
#define LF 0x0A

class ESP8266
{
private:
	static bool clocks_initialized;
	UART_HandleTypeDef UART_Handle = { };
	DMA_HandleTypeDef DMA_Handle = { };
	void InitDMA(DMA_HandleTypeDef * DMA_Handle);
	void InitUART(UART_HandleTypeDef * UART_Handle);
	void InitPin(uint32_t PinNumber);
	void InitClocks();

public:
	ESP8266();
	HAL_StatusTypeDef SendCommand(const char * Text);
	int ReceiveResponse(uint32_t Timeout);
	~ESP8266();
};

