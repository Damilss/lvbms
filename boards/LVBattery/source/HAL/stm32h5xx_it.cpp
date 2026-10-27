#include "stm32h5xx_it.h"

#include "../pins.h"
#include "../taskmanager.h"
#include "../../../../shared/common/peripherals/can.h"
#include "../../../../shared/common/peripherals/i2c.h"
#include "../../../../shared/drivers/rtos/FreeRTOS.h"
#include "../../../../shared/drivers/rtos/event_groups.h"
#include "../../../../shared/drivers/rtos/task.h"

extern TaskHandle_t DATA_sendCANHandler;
extern EventGroupHandle_t muxGroup;

void NMI_Handler(void) {while (1) {}}

void HardFault_Handler(void) {while (1) {}}

void MemManage_Handler(void) {while (1) {}}

void BusFault_Handler(void) {while (1) {}}

void UsageFault_Handler(void) {while (1) {}}

void DebugMon_Handler(void){}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  BaseType_t b;
  if (hi2c == i2c::getConfig(I2C2))
    xEventGroupSetBitsFromISR(muxGroup, 0x1, &b);
  if (hi2c == i2c::getConfig(I2C3))
    xEventGroupSetBitsFromISR(muxGroup, 0x2, &b);
  portYIELD_FROM_ISR(b);
}

void I2C2_EV_IRQHandler(void) {
  HAL_I2C_EV_IRQHandler(i2c::getConfig(I2C2));
}

void I2C3_EV_IRQHandler(void) {
  HAL_I2C_EV_IRQHandler(i2c::getConfig(I2C3));
}

void FDCAN1_IT0_IRQHandler(void) {
  HAL_FDCAN_IRQHandler(can::getConfig(DATA_CAN));

  BaseType_t b = xTaskResumeFromISR(DATA_sendCANHandler);
  portYIELD_FROM_ISR(b);
}

void SysTick_Handler(void) {
  uint32_t ulPreviousMask = portSET_INTERRUPT_MASK_FROM_ISR();
  
  //Increment the RTOS tick, pend a context switch
  if( xTaskIncrementTick() != pdFALSE ) portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;  
  //increment systick
  HAL_IncTick();

  portCLEAR_INTERRUPT_MASK_FROM_ISR( ulPreviousMask );
}