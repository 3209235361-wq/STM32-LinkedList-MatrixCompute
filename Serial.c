#include"stm32f10x.h"
#include"Serial.h"
#include<stdio.h>
#include<stdarg.h>
char Serial_RxPacket[100];
uint8_t Serial_RxFlag;
void SerialInit(void){
    //1.使能USART1,GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //2.1初始化GPIOA9为复用推挽输出，供USART1_TX使用
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //2.2初始化GPIOA10为复用推挽输出，供USART1_RX使用
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //3.初始化USART1
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//八位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
    USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//使能发送接收模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流控制
    USART_Init(USART1, &USART_InitStructure);
    //4.使能接收中断,优先级为0
    //开启RXNE的标志位到NVIC的输出,RXNE一旦为1,就会触发USART1_IRQn中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    //先分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //5.使能USART1
    USART_Cmd(USART1, ENABLE);
}
void SerialSendByte(uint8_t byte){
    USART_SendData(USART1, byte);
    //等待发送完成
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void SerialSendString(uint8_t *str){
    for(int i=0;str[i]!='\0';i++){
        SerialSendByte(str[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
uint32_t Power(uint32_t base,uint32_t exp){
    uint32_t result=1;
    for(int i=0;i<exp;i++){
        result*=base;
    }
    return result;
}
void SerialSendNumber(uint32_t num,uint16_t len){
    for(int i=len;i>0;i--){
        SerialSendByte(num/(Power(10,i-1))%10+'0');
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
void SerialSendFloat(float num, uint16_t decimal_places){
    // 1. 处理负数符号
    if(num < 0){
        SerialSendByte('-');
        num = -num;
    }
    
    // 2. 分离整数部分和小数部分
    uint32_t integer_part = (uint32_t)num;
    //2.1计算整数位数
    uint8_t digits=GetNumberDigits(integer_part);
    float fractional_part = num - integer_part;
    
    // 3. 发送整数部分
    SerialSendNumber(integer_part, digits); // 发送整数部分，补齐位数
    
    // 4. 发送小数点
    SerialSendByte('.');
    
    // 5. 发送小数部分（逐位移位提取）
    for(uint16_t i=0; i<decimal_places; i++){
        fractional_part *= 10;
        uint8_t digit = (uint8_t)fractional_part;
        SerialSendByte(digit + '0');
        fractional_part -= digit;
    }
}
//计算整数位数
uint8_t GetNumberDigits(uint32_t num){
    if(num==0){return 1;}
    uint8_t digits=0;
    while(num>0){
        num/=10;
        digits++;
    }
    return digits;
}
void Serial_SendArray(uint8_t*str,int len){
    for(int i=0;i<len;i++){
        SerialSendByte(str[i]);
    }
}
void USART1_IRQHandler(void){
    static uint8_t RxState=0;
    static uint8_t RxCount=0;
    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
        uint8_t RxData=USART_ReceiveData(USART1);
        if(RxState==0){
            RxCount=0;
            if(RxData=='@'&&Serial_RxFlag==0)//保证上一次数据接收完整
            {
                RxState=1;
            }
        }
        else if(RxState==1){
            //接受包尾帧
            if(RxData=='\r'){
                RxState=2;
            }
            else{
                Serial_RxPacket[RxCount]=RxData;
                RxCount++;
            }
        }
        else if(RxState==2){
            if(RxData=='\n'){
                RxState=0;
                Serial_RxPacket[RxCount]='\0';
                Serial_RxFlag=1;//接收完成
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除接收中断标志位
    }
}
