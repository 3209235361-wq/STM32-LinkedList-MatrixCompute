#include"Key.h"
#include"Delay_ms.h"
void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入,按键未按下时为高电平
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入,按键未按下时为高电平
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

}
int Key_Scan(void){
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
        Delay_ms(20);//去抖
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);//等待按键释放
        Delay_ms(20);//去抖
        return 1;
    }
    else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0){
        Delay_ms(20);//去抖
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);//等待按键释放
        Delay_ms(20);//去抖
        return 11;
    }
    else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0){
        Delay_ms(20);//去抖
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);//等待按键释放
        Delay_ms(20);//去抖
        return 6;
    }
    return 0;//没有按键被按下
}
