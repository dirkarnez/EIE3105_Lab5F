#include "stm32f10x.h"									// Device header
#include "stdbool.h"

//TI2 PA1 Tim2 Ch2
#define BUTTON_RCC_GPIO RCC_APB2Periph_GPIOA
#define BUTTON_GPIO GPIOA
#define BUTTON_GPIO_PIN GPIO_Pin_1
#define L3_RCC_GPIO RCC_APB2Periph_GPIOB

static bool state = true;

int main(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef timerInitStructure;
		
	// GPIO clock for I/O
	RCC_APB2PeriphClockCmd(BUTTON_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(L3_RCC_GPIO, ENABLE);
		
	// Configure I/O for L3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
		
	// Configure I/O for BUTTON
	GPIO_InitStruct.GPIO_Pin = BUTTON_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
		
	//Timer 2 set up
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	timerInitStructure.TIM_Prescaler = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 5;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_TIxExternalClockConfig(TIM2, TIM_TIxExternalCLK1Source_TI2, TIM_ICPolarity_Rising, 0);

	//Enable update event for Timer2
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	while(1) {}
}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
	if(state == 1) {
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		state = 0;
	} else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		state = 1;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}													
}