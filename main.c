#include <ch32v00x.h>
#include <debug.h>
#include <debug.h>
#include <stdio.h>  // For sprintf
#include <string.h>

#define BLINKY_GPIO_PORT GPIOD
#define BUZZER GPIO_Pin_5
#define RED_LED GPIO_Pin_4
#define GREEN_LED GPIO_Pin_3
#define SERVO_PIN GPIO_Pin_2
#define LDR_PIN GPIO_Pin_1

#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

void open_door() {
	GPIO_WriteBit(BLINKY_GPIO_PORT, GREEN_LED, 1);
    GPIO_WriteBit(BLINKY_GPIO_PORT, RED_LED, 0);
	Delay_Ms(5000);
	GPIO_WriteBit(BLINKY_GPIO_PORT, GREEN_LED, 0);
    GPIO_WriteBit(BLINKY_GPIO_PORT, RED_LED, 1);
}

int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_InitTypeDef GPIO_InitStructure = {0};

	BLINKY_CLOCK_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BUZZER;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RED_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GREEN_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SERVO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LDR_PIN;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_Init(BLINKY_GPIO_PORT, &GPIO_InitStructure);

	uint8_t greenLedState = 0, redLedState = 1;
	char duration[5] = "";

	GPIO_WriteBit(BLINKY_GPIO_PORT, SERVO_PIN, 1);
	GPIO_WriteBit(BLINKY_GPIO_PORT, GREEN_LED, greenLedState);
    GPIO_WriteBit(BLINKY_GPIO_PORT, RED_LED, redLedState);
	int val = GPIO_ReadInputDataBit(GPIOD, LDR_PIN); 

	while (val == 0) {  
	    int val2 = GPIO_ReadInputDataBit(GPIOD, LDR_PIN);
	    char val2Str[5];
        sprintf(val2Str, "%d", val2);
        strcat(duration, val2Str);
		printf("%d", val2);
	 
	    if (strcmp(duration, "0001") == 0) {
	    	open_door();
			duration[0] = '\0';
			break;
	    }

		else if (strcmp(duration, "0000") == 0) {
	    	GPIO_WriteBit(BLINKY_GPIO_PORT, BUZZER, 1);
			Delay_Ms(200);
			GPIO_WriteBit(BLINKY_GPIO_PORT, BUZZER, 0);
			duration[0] = '\0';
			break;
	    }
	  
	    if (val2 == 1) {
			GPIO_WriteBit(BLINKY_GPIO_PORT, BUZZER, 1);
			Delay_Ms(200);
			GPIO_WriteBit(BLINKY_GPIO_PORT, BUZZER, 0);
	    	duration[0] = '\0';
			break;
	    }
	
	    Delay_Ms(200);
	}
}

void NMI_Handler(void) {}

void HardFault_Handler(void) {
	while (1) {}
