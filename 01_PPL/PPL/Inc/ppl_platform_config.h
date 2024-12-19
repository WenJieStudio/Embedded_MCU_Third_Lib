/**
  ******************************************************************************
  * @file       ppl_platform_config.h
  * @author     WenJie_Studio
  * @version    V 0.2.0
  * @date       2024-12-15
  * @brief      PPL平台配置文件
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */

#ifndef __PPL_PLATFORM_CONFIG_H
#define __PPL_PLATFORM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// 版本控制
#define PPL_VERSION_MAJOR 0
#define PPL_VERSION_MINOR 2
#define PPL_VERSION_PATCH 0

// 平台选择配置
#define PPL_PLATFORM_STM32F1
//#define PPL_PLATFORM_STM32F4
//#define PPL_PLATFORM_GD32

// 确保至少选择了一个平台
#if !defined(PPL_PLATFORM_STM32F1) && !defined(PPL_PLATFORM_STM32F4) && !defined(PPL_PLATFORM_GD32)
#error "At least one platform must be selected in ppl_platform_config.h"
#endif

// 平台特定头文件包含
#ifdef PPL_PLATFORM_STM32F1
    #include "stm32f10x.h"
    #include "ppl_gpio_stm32f1.h"
    #define PPL_GPIO_Instance           STM32F1_GPIO
    #define PPL_GPIO_Config             STM32F1_GPIO_Config
    #define PPL_GPIO_Create             STM32F1_GPIO_Create
    #define PPL_GPIO_Remap              STM32F1_GPIO_Remap
    #define PPL_GPIO_Lock               STM32F1_GPIO_Lock
#elif defined(PPL_PLATFORM_STM32F4)
    #include "stm32f4xx.h"
    #include "ppl_gpio_stm32f4.h"
    #define PPL_GPIO_Instance           STM32F4_GPIO
    #define PPL_GPIO_Config             STM32F4_GPIO_Config
    #define PPL_GPIO_Create             STM32F4_GPIO_Create
    #define PPL_GPIO_SetAFMode          STM32F4_GPIO_SetAFMode
    #define PPL_GPIO_Lock               STM32F4_GPIO_Lock
#elif defined(PPL_PLATFORM_GD32)
    #include "gd32f10x.h"
    #include "ppl_gpio_gd32.h"
    #define PPL_GPIO_Instance           GD32_GPIO
    #define PPL_GPIO_Config             GD32_GPIO_Config
    #define PPL_GPIO_Create             GD32_GPIO_Create
    #define PPL_GPIO_Remap              GD32_GPIO_Remap
    #define PPL_GPIO_Lock               GD32_GPIO_Lock
#endif

#endif
