#include <ch32v00x.h>
#include <debug.h>

#define BLINKY_GPIO_PORT GPIOD
#define BUZZER GPIO_Pin_5
#define RED_LED GPIO_Pin_4
#define GREEN_LED GPIO_Pin_3
#define SERVO_PIN GPIO_Pin_2
#define BLINKY_CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

int main(void) {
    int locked = 0;
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

	uint8_t greenLedState = 0, redLedState = 0;

	while (1) {  
        GPIO_WriteBit(BLINKY_GPIO_PORT, SERVO_PIN, 1);
		GPIO_WriteBit(BLINKY_GPIO_PORT, GREEN_LED, greenLedState);
        GPIO_WriteBit(BLINKY_GPIO_PORT, RED_LED, redLedState);

        if (locked) {
		    redLedState = 1;
        }
        else {
            greenLedState = 1;
        }
	}
}

void NMI_Handler(void) {}

void HardFault_Handler(void) {
	while (1) {}
}
