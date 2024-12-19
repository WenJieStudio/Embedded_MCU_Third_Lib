/**
  ******************************************************************************
  * @file       ppl_gpio_private.h
  * @author     WenJie_Studio
  * @version    V 0.2.0
  * @date       2024-12-15
  * @brief      PPL GPIO私有头文件定义
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#ifndef __PPL_GPIO_PRIVATE_H
#define __PPL_GPIO_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------- 包含头文件区 -------------------------------*/
#include "ppl_gpio.h"
#include "ppl_gpio_stm32f1.h"
#include <stddef.h>  // 为NULL定义

/*------------------------------- 私有函数声明区 -------------------------------*/
// STM32F1 GPIO内部操作函数
PPL_GPIO_Status GPIO_Init_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Write_Internal(GPIO_Base* self, uint8_t state);
uint8_t GPIO_Read_Internal(GPIO_Base* self);
PPL_GPIO_Status GPIO_Toggle_Internal(GPIO_Base* self);

// 内部辅助函数
static bool Is_GPIO_Initialized(STM32F1_GPIO* gpio);
static bool Is_GPIO_Locked(STM32F1_GPIO* gpio);
static void Enable_GPIO_Clock(GPIO_TypeDef* port);

#ifdef __cplusplus
}
#endif

#endif /* __PPL_GPIO_PRIVATE_H */
