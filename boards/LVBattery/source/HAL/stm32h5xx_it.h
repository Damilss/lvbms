#ifndef __STM32H5xx_IT_H
#define __STM32H5xx_IT_H

#include "../../../../shared/common/peripherals/i2c.h"

#ifdef __cplusplus
 extern "C" {
#endif

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void I2C2_EV_IRQHandler(void);
void I2C3_EV_IRQHandler(void);
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c);
void FDCAN1_IT0_IRQHandler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_IT_H */
