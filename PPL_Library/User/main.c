#include "stm32f10x.h"
#include "ppl_gpio_stm32f1.h"

int main(void) {
    // 定义PC13的GPIO配置
    STM32F1_GPIO_Config led_config = {
        .port = GPIOC,               // LED连接到GPIOC
        .pin = GPIO_Pin_13,          // 使用PC13引脚
        .mode = STM32F1_MODE_OUTPUT_OD,  // 推挽输出模式
        .speed = STM32F1_SPEED_2MHZ,     // 2MHz输出速度
        .remap = DISABLE,            // 不需要重映射
        .remap_func = 0              // 重映射功能未使用
    };
    
    // 定义LED的GPIO对象
    STM32F1_GPIO led;
    
    // 创建LED GPIO实例
    STM32F1_GPIO_Create(&led, &led_config);
    
    while(1) {
        // 点亮LED（对于低电平点亮的LED）
        led.base.ops->write(&led.base, 0);
        
        // 简单的延时
        for(int i = 0; i < 500000; i++) {
            __NOP();  // 空操作延时
        }
        
        // 熄灭LED
        led.base.ops->write(&led.base, 1);
        
        // 简单的延时
        for(int i = 0; i < 500000; i++) {
            __NOP();  // 空操作延时
        }
    }
}

