#include <ch32v00x.h>
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"

#define PWM_MODE1 0
#define PWM_MODE2 1
#define PWM_MODE PWM_MODE2
#define GPIO_PORT GPIOD
#define BUZZER GPIO_Pin_5
#define RED_LED GPIO_Pin_4
#define GREEN_LED GPIO_Pin_3
#define SERVO_PIN GPIO_Pin_2
#define LDR_PIN GPIO_Pin_1
#define CLOCK_ENABLE RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE)
#define MAX_DURATION "000000000000000000000000"
#define DELAY 30

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void Delay_Init(void);
void Delay_Ms(uint32_t n);

void TIM1_PWMOut_Init(u16 arr, u16 psc, u16 ccp) {
    GPIO_InitTypeDef GPIO_InitStructure={0};
    TIM_OCInitTypeDef TIM_OCInitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE );
    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

	#if (PWM_MODE == PWM_MODE1)
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;

	#elif (PWM_MODE == PWM_MODE2)
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

	#endif

    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ccp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init( TIM1, &TIM_OCInitStructure );

    TIM_CtrlPWMOutputs(TIM1, ENABLE );
    TIM_OC1PreloadConfig( TIM1, TIM_OCPreload_Disable );
    TIM_ARRPreloadConfig( TIM1, ENABLE );
    TIM_Cmd( TIM1, ENABLE );
}

void open_door() {
	TIM1_PWMOut_Init( 100, 2000-1, 50 );
	GPIO_WriteBit(GPIO_PORT, GREEN_LED, 1);
	GPIO_WriteBit(GPIO_PORT, RED_LED, 0);
	Delay_Ms(7000);
}

void lock_door() {
	GPIO_WriteBit(GPIO_PORT, GREEN_LED, 0);
	GPIO_WriteBit(GPIO_PORT, RED_LED, 1);
	TIM1_PWMOut_Init( 100, 2000-1, 75 );
	Delay_Ms(2000);
}

int main(void) {
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();

	GPIO_InitTypeDef GPIO_InitStructure = {0};

	CLOCK_ENABLE;
	GPIO_InitStructure.GPIO_Pin = BUZZER;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RED_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GREEN_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LDR_PIN;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

	int locked = 0;
	char duration[50] = "";
	char expected_duration[50] = "";
	open_door();

	while (1) {
		GPIO_WriteBit(GPIO_PORT, GREEN_LED, ~locked);
    	GPIO_WriteBit(GPIO_PORT, RED_LED, locked);
		int val = GPIO_ReadInputDataBit(GPIOD, LDR_PIN); 
		
		while (val == 0) {  
			GPIO_WriteBit(GPIO_PORT, GREEN_LED, ~locked);
			GPIO_WriteBit(GPIO_PORT, RED_LED, locked);

			// Reading from the LDR sensor
			int val2 = GPIO_ReadInputDataBit(GPIOD, LDR_PIN);
			char val2Str[5];
			sprintf(val2Str, "%d", val2);
			strcat(duration, val2Str);
		
			// Set/release lock
			if (strcmp(duration, "0001") == 0) {
				duration[0] = '\0';

				// Release lock
				if (locked) {
					while (GPIO_ReadInputDataBit(GPIOD, LDR_PIN)) {}

					// Reading duration again
					while (1) {
						int val3 = GPIO_ReadInputDataBit(GPIOD, LDR_PIN);
						char val3Str[5];
						sprintf(val3Str, "%d", val3);
						strcat(duration, val3Str);

						// Correct duration
						if (strcmp(duration, expected_duration) == 0) {
							duration[0] = '\0';
							expected_duration[0] = '\0';
							locked = 0;
							open_door();
							break;
						}

						// Too long
						else if (strcmp(duration, MAX_DURATION) == 0) {
							GPIO_WriteBit(GPIO_PORT, BUZZER, 1);
							Delay_Ms(200);
							GPIO_WriteBit(GPIO_PORT, BUZZER, 0);
							duration[0] = '\0';
							break;
						}

						// Too short
						if (val3 == 1) {
							GPIO_WriteBit(GPIO_PORT, BUZZER, 1);
							Delay_Ms(200);
							GPIO_WriteBit(GPIO_PORT, BUZZER, 0);
							duration[0] = '\0';
							break; 
						}

						Delay_Ms(DELAY);
					}

					break;
				}

				// Set lock duration
				else {
					expected_duration[0] = '\0';
					locked = 1;
					while (GPIO_ReadInputDataBit(GPIOD, LDR_PIN)) {}

					// Reading duration again
					while (1) {
						int val3 = GPIO_ReadInputDataBit(GPIOD, LDR_PIN);
						char val3Str[5];
						sprintf(val3Str, "%d", val3);
						strcat(expected_duration, val3Str);

						// Trying to set a duration that is too long
						if (strcmp(expected_duration, MAX_DURATION) == 0) {
							GPIO_WriteBit(GPIO_PORT, BUZZER, 1);
							Delay_Ms(200);
							GPIO_WriteBit(GPIO_PORT, BUZZER, 0);
							expected_duration[0] = '\0';
							break;
						}

						if (val3 == 1) {
							lock_door();
							break;
						}

						Delay_Ms(DELAY);
					}

					break;
				}
			}

			// Unlock door with correct duration
			else if (locked && strcmp(duration, expected_duration) == 0) {
				duration[0] = '\0';
				locked = 1;
				open_door(); 
				lock_door(); // Close door after 7 second delay
				break;
			}

			// Too long
			else if (strcmp(duration, MAX_DURATION) == 0) {
				GPIO_WriteBit(GPIO_PORT, BUZZER, 1);
				Delay_Ms(200);
				GPIO_WriteBit(GPIO_PORT, BUZZER, 0);
				duration[0] = '\0';
				break;
			}
		
			// Too short
			else if (val2 == 1) {
				GPIO_WriteBit(GPIO_PORT, BUZZER, 1);
				Delay_Ms(200);
				GPIO_WriteBit(GPIO_PORT, BUZZER, 0);
				duration[0] = '\0';
				break;
			}
		
			Delay_Ms(DELAY);
		}
	}
}

void NMI_Handler(void) {}

void HardFault_Handler(void) {
	while (1) {}
}
