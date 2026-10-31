/* Relay-controlled water pump. Proposed relay IN -> PC0, active-high.
 * Use a suitably rated driver/relay and separate pump supply.
 * Never power the pump directly from an STM32 GPIO. */
#include "main.h"
#define RELAY_PORT GPIOC
#define RELAY_PIN GPIO_PIN_0
#define RELAY_ON GPIO_PIN_SET
#define RELAY_OFF GPIO_PIN_RESET

void Pump_Init(void) { HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, RELAY_OFF); }
void Pump_On(void)  { HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, RELAY_ON); }
void Pump_Off(void) { HAL_GPIO_WritePin(RELAY_PORT, RELAY_PIN, RELAY_OFF); }
