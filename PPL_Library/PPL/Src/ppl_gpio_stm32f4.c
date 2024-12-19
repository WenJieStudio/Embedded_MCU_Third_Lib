/**
  ******************************************************************************
  * @file       ppl_gpio_stm32f4.c
  * @author     WenJie_Studio
  * @version    V 0.2.1
  * @date       2024-12-15
  * @brief      STM32F4系列GPIO实现
  ******************************************************************************
  * @attention
  *
  * 版权所有 (c) [2024] [WenJie_Studio]
  ******************************************************************************
  */ 

#include "ppl_gpio_stm32f4.h"

#ifdef PPL_PLATFORM_STM32F4

#include "ppl_gpio.h"
#include "ppl_gpio_private.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stddef.h>
#include <stdint.h>

/* 私有函数声明 */
static void EnableGPIOClock(GPIO_TypeDef* port);

/* STM32F4 GPIO操作函数表 */
static GPIO_Operations stm32f4_gpio_ops = {
    .init = GPIO_Init_Internal,
    .write = GPIO_Write_Internal,
    .read = GPIO_Read_Internal,
    .toggle = GPIO_Toggle_Internal,
    .set_mode = NULL,
    .set_speed = NULL
};

/* 私有函数实现 */
static void EnableGPIOClock(GPIO_TypeDef* port) {
    if(port == GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    else if(port == GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    else if(port == GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    else if(port == GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    else if(port == GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    else if(port == GPIOF) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    else if(port == GPIOG) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    else if(port == GPIOH) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
}

/* 内部函数实现 */
PPL_GPIO_Status GPIO_Init_Internal(GPIO_Base* self) {
    STM32F4_GPIO* gpio = (STM32F4_GPIO*)self->hw_instance;
    if (!gpio) return PPL_GPIO_INVALID_PARAM;
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    EnableGPIOClock(gpio->port);
    
    GPIO_InitStruct.GPIO_Pin = gpio->pin;
    GPIO_InitStruct.GPIO_Mode = gpio->mode;
    GPIO_InitStruct.GPIO_OType = gpio->otype;
    GPIO_InitStruct.GPIO_Speed = gpio->speed;
    GPIO_InitStruct.GPIO_PuPd = gpio->pupd;
    
    GPIO_Init(gpio->port, &GPIO_InitStruct);
    
    // 如果是复用模式，配置AF
    if(gpio->mode == GPIO_Mode_AF) {
        uint8_t pinpos = 0;
        while(pinpos < 16) {
            if(gpio->pin & (1 << pinpos)) {
                GPIO_PinAFConfig(gpio->port, pinpos, gpio->af_mode);
            }
            pinpos++;
        }
    }
    
    return PPL_GPIO_OK;
}

PPL_GPIO_Status GPIO_Write_Internal(GPIO_Base* self, uint8_t state) {
    STM32F4_GPIO* gpio = (STM32F4_GPIO*)self->hw_instance;
    if (!gpio) return PPL_GPIO_INVALID_PARAM;
    
    if (!gpio->is_initialized) return PPL_GPIO_NOT_INITIALIZED;
    if (gpio->is_locked) return PPL_GPIO_LOCKED;
    
    __disable_irq();  // 关中断
    if (state) {
        GPIO_SetBits(gpio->port, gpio->pin);
    } else {
        GPIO_ResetBits(gpio->port, gpio->pin);
    }
    __enable_irq();   // 开中断
    
    return PPL_GPIO_OK;
}

uint8_t GPIO_Read_Internal(GPIO_Base* self) {
    STM32F4_GPIO* gpio = (STM32F4_GPIO*)self->hw_instance;
    if (!gpio) return 0;
    
    return GPIO_ReadInputDataBit(gpio->port, gpio->pin);
}

PPL_GPIO_Status GPIO_Toggle_Internal(GPIO_Base* self) {
    STM32F4_GPIO* gpio = (STM32F4_GPIO*)self->hw_instance;
    if (!gpio) return PPL_GPIO_INVALID_PARAM;
    
    if (!gpio->is_initialized) return PPL_GPIO_NOT_INITIALIZED;
    if (gpio->is_locked) return PPL_GPIO_LOCKED;
    
    gpio->port->ODR ^= gpio->pin;
    
    return PPL_GPIO_OK;
}

/* 公共函数实现 */
PPL_GPIO_Status STM32F4_GPIO_Create(STM32F4_GPIO* gpio, STM32F4_GPIO_Config* config) {
    if (!gpio || !config) return PPL_GPIO_INVALID_PARAM;
    if (!PPL_IS_GPIO_PORT(config->port)) return PPL_GPIO_INVALID_PORT;
    if (!PPL_IS_GPIO_PIN(config->pin)) return PPL_GPIO_INVALID_PIN;
    if (!PPL_IS_GPIO_MODE(config->mode)) return PPL_GPIO_INVALID_MODE;
    if (!PPL_IS_GPIO_SPEED(config->speed)) return PPL_GPIO_INVALID_SPEED;
    
    gpio->base.hw_instance = gpio;
    gpio->base.ops = &stm32f4_gpio_ops;
    
    gpio->port = config->port;
    gpio->pin = config->pin;
    gpio->mode = config->mode;
    gpio->otype = config->otype;
    gpio->speed = config->speed;
    gpio->pupd = config->pupd;
    gpio->af_mode = config->af_mode;
    gpio->is_locked = false;
    gpio->is_initialized = false;
    
    PPL_GPIO_Status status = GPIO_Init_Internal(&gpio->base);
    if (status == PPL_GPIO_OK) {
        gpio->is_initialized = true;
    }
    
    return status;
}

PPL_GPIO_Status STM32F4_GPIO_SetAFMode(STM32F4_GPIO* gpio, uint8_t af_mode) {
    if (!gpio) return PPL_GPIO_INVALID_PARAM;
    if (!gpio->is_initialized) return PPL_GPIO_NOT_INITIALIZED;
    if (gpio->is_locked) return PPL_GPIO_LOCKED;
    if (gpio->mode != GPIO_Mode_AF) return PPL_GPIO_INVALID_MODE;
    
    uint8_t pinpos = 0;
    while(pinpos < 16) {
        if(gpio->pin & (1 << pinpos)) {
            GPIO_PinAFConfig(gpio->port, pinpos, af_mode);
        }
        pinpos++;
    }
    
    gpio->af_mode = af_mode;
    return PPL_GPIO_OK;
}

PPL_GPIO_Status STM32F4_GPIO_Lock(STM32F4_GPIO* gpio) {
    if (!gpio) return PPL_GPIO_INVALID_PARAM;
    if (!gpio->is_initialized) return PPL_GPIO_NOT_INITIALIZED;
    if (gpio->is_locked) return PPL_GPIO_ALREADY_INITIALIZED;
    
    GPIO_PinLockConfig(gpio->port, gpio->pin);
    gpio->is_locked = true;
    
    return PPL_GPIO_OK;
}

#endif // PPL_PLATFORM_STM32F4
