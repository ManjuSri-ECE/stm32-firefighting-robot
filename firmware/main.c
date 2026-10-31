/*
 * STM32F407VGT6 firefighting robot - reference firmware
 * STM32 HAL / CubeIDE. Newly developed example, not original project code.
 * Proposed pin map: flame DO PA0; L293D IN1..IN4 PB0..PB3;
 * motor PWM TIM3_CH1 PA6 and TIM3_CH2 PA7; servos TIM4_CH1 PB6,
 * TIM4_CH2 PB7; pump relay PC0.
 * Assumptions: flame module active-low; relay active-high; HSE=8 MHz.
 * Verify board, module polarity, wiring, timer clocks and voltage levels.
 * Test first with wheels raised and pump disconnected.
 */
#include "main.h"
#include <stdint.h>

#define FLAME_PORT GPIOA
#define FLAME_PIN GPIO_PIN_0
#define FLAME_ACTIVE GPIO_PIN_RESET
#define RELAY_PORT GPIOC
#define RELAY_PIN GPIO_PIN_0

TIM_HandleTypeDef htim3, htim4;

void Motor_Init(void);
void Motor_SetSpeed(uint8_t left, uint8_t right);
void Motor_Forward(void);
void Motor_Stop(void);
uint8_t Flame_IsDetected(void);
void Pump_Init(void);
void Pump_On(void);
void Pump_Off(void);

static void Clock_Init(void);
static void GPIO_Init_All(void);
static void TIM3_PWM_Init(void);
static void TIM4_PWM_Init(void);
static void Servo_SetAngle(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t angle);
void Error_Handler(void);

int main(void)
{
    HAL_Init();
    Clock_Init();
    GPIO_Init_All();
    TIM3_PWM_Init();
    TIM4_PWM_Init();

    Motor_Init();
    Pump_Init();

    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2) != HAL_OK) Error_Handler();

    Servo_SetAngle(&htim4, TIM_CHANNEL_1, 90U);
    Servo_SetAngle(&htim4, TIM_CHANNEL_2, 90U);
    Pump_Off();

    while (1)
    {
        if (HAL_GPIO_ReadPin(FLAME_PORT, FLAME_PIN) == FLAME_ACTIVE)
        {
            uint8_t angle;
            Motor_Stop();
            Pump_Off();

            /* Sweep the nozzle while the flame input remains active. */
            for (angle = 55U; angle <= 125U; angle = (uint8_t)(angle + 10U))
            {
                Servo_SetAngle(&htim4, TIM_CHANNEL_1, angle);
                HAL_Delay(200U);
                if (HAL_GPIO_ReadPin(FLAME_PORT, FLAME_PIN) == FLAME_ACTIVE)
                {
                    Pump_On();
                    HAL_Delay(500U);
                    Pump_Off();
                }
            }
            Servo_SetAngle(&htim4, TIM_CHANNEL_1, 90U);
            HAL_Delay(300U);
        }
        else
        {
            Pump_Off();
            Motor_SetSpeed(35U, 35U);
            Motor_Forward();
        }
        HAL_Delay(50U);
    }
}

static void GPIO_Init_All(void)
{
    GPIO_InitTypeDef g = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, GPIO_PIN_RESET);

    g.Pin = FLAME_PIN; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(FLAME_PORT, &g);

    g.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
    g.Mode = GPIO_MODE_OUTPUT_PP; g.Pull = GPIO_NOPULL; g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &g);

    g.Pin = RELAY_PIN; g.Mode = GPIO_MODE_OUTPUT_PP; g.Pull = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW; HAL_GPIO_Init(RELAY_PORT, &g);

    g.Pin = GPIO_PIN_6|GPIO_PIN_7; g.Mode = GPIO_MODE_AF_PP;
    g.Pull = GPIO_NOPULL; g.Speed = GPIO_SPEED_FREQ_LOW; g.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &g);

    g.Pin = GPIO_PIN_6|GPIO_PIN_7; g.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &g);
}

/* Assumes APB1 timer clock = 84 MHz: 1 us tick, 20 ms PWM period. */
static void TIM3_PWM_Init(void)
{
    TIM_OC_InitTypeDef oc = {0};
    __HAL_RCC_TIM3_CLK_ENABLE();
    htim3.Instance = TIM3; htim3.Init.Prescaler = 83U;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP; htim3.Init.Period = 19999U;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) Error_Handler();
    oc.OCMode = TIM_OCMODE_PWM1; oc.Pulse = 0U;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH; oc.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &oc, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &oc, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
}

static void TIM4_PWM_Init(void)
{
    TIM_OC_InitTypeDef oc = {0};
    __HAL_RCC_TIM4_CLK_ENABLE();
    htim4.Instance = TIM4; htim4.Init.Prescaler = 83U;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP; htim4.Init.Period = 19999U;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK) Error_Handler();
    oc.OCMode = TIM_OCMODE_PWM1; oc.Pulse = 1500U;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH; oc.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &oc, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &oc, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
}

static void Servo_SetAngle(TIM_HandleTypeDef *timer, uint32_t channel, uint8_t angle)
{
    uint32_t pulse;
    if (angle > 180U) angle = 180U;
    pulse = 500U + ((uint32_t)angle * 2000U) / 180U;
    __HAL_TIM_SET_COMPARE(timer, channel, pulse);
}

/* 8 MHz HSE -> 168 MHz SYSCLK. Replace if the board clock differs. */
static void Clock_Init(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE; osc.HSEState = RCC_HSE_ON;
    osc.PLL.PLLState = RCC_PLL_ON; osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM = 8U; osc.PLL.PLLN = 336U; osc.PLL.PLLP = RCC_PLLP_DIV2; osc.PLL.PLLQ = 7U;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) Error_Handler();
    clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV4; clk.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_5) != HAL_OK) Error_Handler();
}

void Error_Handler(void)
{
    Motor_Stop(); Pump_Off(); __disable_irq();
    while (1) { }
}
