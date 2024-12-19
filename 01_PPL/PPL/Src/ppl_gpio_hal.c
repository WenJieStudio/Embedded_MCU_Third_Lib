/**
  ******************************************************************************
  * @file       ppl_gpio_hal.c
  * @author     WenJie_Studio
  * @version    V 0.1.0
  * @date       2024-12-15
  * @brief      PPL硬件抽象层实现文件
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */

#include "ppl_gpio_hal.h"
#include "ppl_platform_config.h"
#include "ppl_gpio_stm32f1.h"
#include "ppl_gpio.h"
#include <stdlib.h>
#include <string.h>

/*------------------------------- 私有变量区 -----------------------------------*/
// GPIO操作函数表实例
static HW_GPIO_Operations gpio_ops = {
#ifdef PPL_PLATFORM_STM32F1
    .init = (GPIO_Init_Func)GPIO_Init_Internal,
    .write = (GPIO_Write_Func)GPIO_Write_Internal,
    .read = (GPIO_Read_Func)GPIO_Read_Internal,
    .toggle = (GPIO_Toggle_Func)GPIO_Toggle_Internal,
    .set_mode = NULL,  // STM32F1不支持动态改变模式
    .set_speed = NULL  // STM32F1不支持动态改变速度
#endif
};

/*------------------------------- 私有函数区 -----------------------------------*/
static GPIO_TypeDef* PPL_HW_GetGPIOPort(const char* port_name) {
#ifdef PPL_PLATFORM_STM32F1
    if(strcmp(port_name, "GPIOA") == 0) return GPIOA;
    if(strcmp(port_name, "GPIOB") == 0) return GPIOB;
    if(strcmp(port_name, "GPIOC") == 0) return GPIOC;
    if(strcmp(port_name, "GPIOD") == 0) return GPIOD;
    if(strcmp(port_name, "GPIOE") == 0) return GPIOE;
    if(strcmp(port_name, "GPIOF") == 0) return GPIOF;
    if(strcmp(port_name, "GPIOG") == 0) return GPIOG;
#endif
    return NULL;
}

/*------------------------------- 公共函数实现区 -------------------------------*/
HW_GPIO_Device* PPL_HW_CreateGPIODevice(const char* port_name, uint16_t pin_number) {
    if (!port_name || pin_number >= PPL_MAX_GPIO_PINS) {
        return NULL;
    }

    // 分配设备内存
    HW_GPIO_Device* device = (HW_GPIO_Device*)malloc(sizeof(HW_GPIO_Device));
    if (!device) {
        return NULL;
    }

    // 初始化设备基本信息
    device->port_name = port_name;
    device->pin_number = pin_number;
    device->ops = &gpio_ops;
    device->is_initialized = false;

    // 创建平台特定的GPIO实例
#ifdef PPL_PLATFORM_STM32F1
    STM32F1_GPIO* gpio = (STM32F1_GPIO*)malloc(sizeof(STM32F1_GPIO));
    if (!gpio) {
        free(device);
        return NULL;
    }

    // 配置GPIO
    STM32F1_GPIO_Config config = {
        .port = PPL_HW_GetGPIOPort(port_name),
        .pin = (uint16_t)(1 << pin_number),
        .mode = STM32F1_MODE_OUTPUT_PP,  // 默认配置，可以根据需要修改
        .speed = STM32F1_SPEED_2MHZ,
        .remap = DISABLE,
        .remap_func = 0
    };

    // 创建GPIO实例
    if (STM32F1_GPIO_Create(gpio, &config) != PPL_GPIO_OK) {
        free(gpio);
        free(device);
        return NULL;
    }

    device->hw_instance = gpio;
#endif

    return device;
}

PPL_GPIO_Status PPL_HW_InitGPIODevice(HW_GPIO_Device* device) {
    if (!device || !device->hw_instance || !device->ops || !device->ops->init) {
        return PPL_GPIO_INVALID_PARAM;
    }

    PPL_GPIO_Status status = device->ops->init((GPIO_Base*)device->hw_instance);
    if (status == PPL_GPIO_OK) {
        device->is_initialized = true;
    }

    return status;
}

void PPL_HW_DestroyGPIODevice(HW_GPIO_Device* device) {
    if (device) {
        if (device->hw_instance) {
            free(device->hw_instance);
        }
        free(device);
    }
}
