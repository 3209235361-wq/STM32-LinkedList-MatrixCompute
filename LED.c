#include "stm32f10x.h"
#include "LED.h"
void LED_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);//默认关闭LED
}
void LED_Set(uint8_t LEDx,uint8_t State){
    if(LEDx == 1){
        if(State==0){
            GPIO_SetBits(GPIOA,GPIO_Pin_1);//关闭LED
        }
        else{
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);//打开LED
        }
    }
    else if(LEDx == 2){
        if(State==0){
            GPIO_SetBits(GPIOA,GPIO_Pin_2);//关闭LED
        }
        else{
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);//打开LED
        }
    }
    else if(LEDx == 3){
        if(State==0){
            GPIO_SetBits(GPIOA,GPIO_Pin_3);//关闭LED
        }
        else{
            GPIO_ResetBits(GPIOA,GPIO_Pin_3);//打开LED
               }
    }
}
void LED1_Turn(void){
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)==0){//如果LED1是亮的,即输出低电平
        GPIO_SetBits(GPIOA,GPIO_Pin_1);//关闭LED1
    }
    else{
        GPIO_ResetBits(GPIOA,GPIO_Pin_1);//打开LED1
    }
}
void LED2_Turn(void){
    if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==0){//如果LED2是亮的，即输出低电平
        GPIO_SetBits(GPIOA,GPIO_Pin_2);//关闭LED2
    }
    else{
        GPIO_ResetBits(GPIOA,GPIO_Pin_2);//打开LED2
    }
}
