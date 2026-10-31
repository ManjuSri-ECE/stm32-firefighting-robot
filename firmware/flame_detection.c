/* Digital flame sensor interface: proposed DO -> PA0.
 * Assumed active-low output; verify module polarity and change macro if needed. */
#include "main.h"
#include <stdint.h>
#define FLAME_PORT GPIOA
#define FLAME_PIN GPIO_PIN_0
#define FLAME_ACTIVE_LEVEL GPIO_PIN_RESET

void Flame_Init(void)
{
    /* PA0 input with pull-up is configured in main.c. */
}
uint8_t Flame_IsDetected(void)
{
    return (HAL_GPIO_ReadPin(FLAME_PORT, FLAME_PIN) == FLAME_ACTIVE_LEVEL) ? 1U : 0U;
}
