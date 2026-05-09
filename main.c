#include<stdio.h>
#include "stm32f10x.h"// Device header
#include "OLED.h"
#include"Delay_ms.h"
#include"Serial.h"
#include"LED.h"
#include"Key.h"
#include<string.h>
#include"func.h"
#define PI 3.1415
int key=0;
int Node_count=0;
int Node_pos=0;
Node data1={
	.ID=0,
	.Mode=first,
	.mat1={{1,2},{3,4}},
	.mat2={{5,6},{7,8}},
};
Node data2={
	.ID=1,
	.Mode=second,
	.transfer={{1,2},{3,4}},
};
Node data3={
	.ID=2,
	.Mode=third,
	.vector={1,2},
	.angle=PI/4,
};
int	main(void)  {
	OLED_Init();
	SerialInit();
	LED_Init();
	Key_Init();
	OLED_ShowString(1,1,"Node total:");
	OLED_ShowString(2,1,"Device ID:");
	OLED_ShowString(3,1,"Mode:");
	OLED_ShowString(3,8,"Pos:");
	Plist head=NULL;
	Init_linked_list(&head);

	while(1){
		Node_count=Get_NodeSize(&head);
		OLED_ShowNum(1,13,Node_count,1);		
		key=Key_Scan();
		if(key==1){//轮切节点,并显示当前节点的ID和Mode
			if(head==NULL){
				continue;
			}
			if(Node_pos>=Node_count){
				Node_pos=0;
			}
			List*pnode=find_Designate_node(head,Node_pos+1);//head值传递
			if(pnode!=NULL){
				OLED_ShowNum(2,11,pnode->node.ID,1);
				OLED_ShowNum(3,6,pnode->node.Mode,1);
			}
			Node_pos++;
			OLED_ShowNum(3,12,Node_pos,1);

		}		
		else if(key==11){//运行当前节点的操作
			if(head==NULL){
				continue;
			}
			List*pnode=find_Designate_node(head,Node_pos);//head值传递
			if(pnode!=NULL){
				matrix_operator(&pnode->node,pnode->node.Mode);
				Print_linked_list(&pnode,Node_pos);
			}
		}
		else if(key==6){
			SerialSendString("-------------------------\r\n");
			SerialSendString("Mode 0.Matrix multiply\r\n");
			SerialSendString("Mode 1.Matrix transform\r\n");
			SerialSendString("Mode 2.Vector rotate\r\n");
			SerialSendString("Mode 3.Clear linked list\r\n");
			SerialSendString("Choice initialize select:\r\n");
			SerialSendString("-------------------------\r\n");
		}		
		if(Serial_RxFlag==1){
			if(strcmp(Serial_RxPacket,"Mode 0")==0){
				EnAdd_Node(&head,&data1,Node_count);
				OLED_ShowString(4,1,"                   ");
				OLED_ShowString(4,1,"init mode0!");	
				LED_Set(1,1);
				Delay_ms(500);
				LED_Set(1,0);
			}
			else if(strcmp(Serial_RxPacket,"Mode 1")==0){
				EnAdd_Node(&head,&data2,Node_count);
				OLED_ShowString(4,1,"                   ");
				OLED_ShowString(4,1,"init mode1!");
				LED_Set(2,1);
				Delay_ms(500);
				LED_Set(2,0);
			}
			else if(strcmp(Serial_RxPacket,"Mode 2")==0){
				EnAdd_Node(&head,&data3,Node_count);				
				OLED_ShowString(4,1,"                   ");
				OLED_ShowString(4,1,"init mode2!");
				LED_Set(3,1);
				Delay_ms(500);
				LED_Set(3,0);
			}
			else if(strcmp(Serial_RxPacket,"Mode 3")==0){
				Free_linked_list(&head);
				OLED_ShowString(4,1,"                   ");
				OLED_ShowString(4,1,"Clear all!");
			}
			else{
				SerialSendString("Invalid command\r\n");
				OLED_ShowString(4,1,"                  ");
				OLED_ShowString(4,1,"Invalid command");
			}
			Serial_RxFlag=0;//接收完成,重置接收标志位
		}
	}
}

