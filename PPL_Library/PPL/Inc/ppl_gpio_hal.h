/**
  ******************************************************************************
  * @file       ppl_gpio_hal.h
  * @author     WenJie_Studio
  * @version    V 0.1.0
  * @date       2024-12-15
  * @brief      硬件抽象层头文件
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */

#ifndef __PPL_GPIO_HAL_H
#define __PPL_GPIO_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ppl_gpio.h"
#include <stdint.h>
#include <stdbool.h>

#define PPL_MAX_GPIO_PINS    16  // 定义最大GPIO引脚数

// GPIO操作函数类型定义
typedef PPL_GPIO_Status (*GPIO_Init_Func)(GPIO_Base* gpio_base);
typedef PPL_GPIO_Status (*GPIO_Write_Func)(GPIO_Base* gpio_base, uint8_t state);
typedef uint8_t (*GPIO_Read_Func)(GPIO_Base* gpio_base);
typedef PPL_GPIO_Status (*GPIO_Toggle_Func)(GPIO_Base* gpio_base);
typedef PPL_GPIO_Status (*GPIO_SetMode_Func)(GPIO_Base* gpio_base, uint32_t mode);
typedef PPL_GPIO_Status (*GPIO_SetSpeed_Func)(GPIO_Base* gpio_base, uint32_t speed);

// GPIO操作函数表结构体
typedef struct {
    GPIO_Init_Func init;
    GPIO_Write_Func write;
    GPIO_Read_Func read;
    GPIO_Toggle_Func toggle;
    GPIO_SetMode_Func set_mode;
    GPIO_SetSpeed_Func set_speed;
} HW_GPIO_Operations;

// GPIO设备结构体
typedef struct {
    const char* port_name;           // 端口名称
    uint16_t pin_number;             // 引脚号
    HW_GPIO_Operations* ops;         // 操作函数表
    void* hw_instance;               // 平台特定的硬件实例
    bool is_initialized;             // 初始化标志
} HW_GPIO_Device;

// 公共函数声明
HW_GPIO_Device* PPL_HW_CreateGPIODevice(const char* port_name, uint16_t pin_number);
PPL_GPIO_Status PPL_HW_InitGPIODevice(HW_GPIO_Device* device);
void PPL_HW_DestroyGPIODevice(HW_GPIO_Device* device);

#ifdef __cplusplus
}
#endif

#endif /* __PPL_GPIO_HAL_H */
