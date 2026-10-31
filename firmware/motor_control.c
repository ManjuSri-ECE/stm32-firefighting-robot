/* L293D motor control. Proposed mapping: IN1..IN4 = PB0..PB3;
 * ENA = PA6/TIM3_CH1, ENB = PA7/TIM3_CH2. */
#include "main.h"
#include <stdint.h>
extern TIM_HandleTypeDef htim3;

#define IN1 GPIO_PIN_0
#define IN2 GPIO_PIN_1
#define IN3 GPIO_PIN_2
#define IN4 GPIO_PIN_3

static uint32_t duty(uint8_t percent)
{
    if (percent > 100U) percent = 100U;
    return ((uint32_t)percent * 19999U) / 100U;
}
void Motor_Init(void) { Motor_Stop(); }
void Motor_SetSpeed(uint8_t left, uint8_t right)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty(left));
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty(right));
}
void Motor_Forward(void)
{
    HAL_GPIO_WritePin(GPIOB, IN1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN4, GPIO_PIN_RESET);
}
void Motor_Backward(void)
{
    HAL_GPIO_WritePin(GPIOB, IN1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN4, GPIO_PIN_SET);
}
void Motor_TurnLeft(void)
{
    HAL_GPIO_WritePin(GPIOB, IN1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN4, GPIO_PIN_RESET);
}
void Motor_TurnRight(void)
{
    HAL_GPIO_WritePin(GPIOB, IN1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, IN2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, IN4, GPIO_PIN_SET);
}
void Motor_Stop(void)
{
    HAL_GPIO_WritePin(GPIOB, IN1|IN2|IN3|IN4, GPIO_PIN_RESET);
    if (htim3.Instance == TIM3) Motor_SetSpeed(0U, 0U);
}
