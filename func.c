#include"func.h"
#include"Math_formula1.h"
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include"Serial.h"
uint8_t *Device_ID_str[3]={"Device_0\r\n","Device_1\r\n","Device_2\r\n"};
int Get_NodeSize(const Plist*head){
    if(*head==NULL){
        return 0;
    }
    List*pnode=*head;
    int count=0;
    while(pnode!=NULL){
        count++;
        pnode=pnode->Next;
    }
    return count;
}
bool EnAdd_Node(Plist*head,Node*data,int count){
    if(count>=3){
        SerialSendString("Error: The count is out of range!\r\n");
        return false;
    }
    List *pnew;
    List *Scan=*head;
    if(list_isFull(head)==true){
        SerialSendString("Error Memory assign\r\n");
        return false;
    }
    // // 找到最小的可用ID (0-2)
    // //好好琢磨一下这个算法
    // bool id_used[3] = {false, false, false};
    // List *temp = *head;
    // while(temp != NULL) {
    //     if(temp->node.ID < 3) {
    //         id_used[temp->node.ID] = true;
    //     }
    //     temp = temp->Next;
    // }
    // uint8_t new_id = 0;
    // while(new_id < 3 && id_used[new_id]) {
    //     new_id++;
    // }换了套逻辑用不上了
    pnew=(List*)malloc(sizeof(List));
    //Initialize the new node
    pnew->Matrix_transform=matrix_operator;
    Node_Init(pnew,data,data->Mode);
    pnew->Next=NULL;
    if(Scan==NULL){
        *head=pnew;
    }
    else{
        while(Scan->Next!=NULL){
            Scan=Scan->Next;
        }
        Scan->Next=pnew;
    }
    SerialSendString("Add success\r\n");
    return true;
}
void Node_Init(List*pnew,Node*data,int mode){
    switch(mode){
        case first:
            //二维数组的复制
            memcpy(pnew->node.mat1,data->mat1,sizeof(data->mat1));
            memcpy(pnew->node.mat2,data->mat2,sizeof(data->mat2));
            pnew->node.Mode=data->Mode;
            pnew->node.ID=data->ID;
            break;
        case second:
            memcpy(pnew->node.transfer,data->transfer,sizeof(data->transfer));
            pnew->node.Mode=data->Mode;
            pnew->node.ID=data->ID;
            break;
        case third:
            memcpy(pnew->node.vector,data->vector,sizeof(data->vector));
            pnew->node.Mode=data->Mode;
            pnew->node.angle=data->angle;
            pnew->node.ID=data->ID;
            break;
    }
}
bool EnDelete_Node(Plist*head){
    if(list_isEmpty(head)==true){
        SerialSendString("Error: The linked list is empty!\r\n");
        return false;
    }
    //Case 1 : only one node in the linked list
    if((*head)->Next==NULL){
        free(*head);
        *head=NULL;
        SerialSendString("Delete success\r\n");
        return true;
    }
    //Case 2 : more than one node in the linked list
    List*pnode=*head;
    //Find the second last node in the linked list
    while(pnode->Next->Next!=NULL)//Awesome way!
    {
        pnode=pnode->Next;
    }
    free(pnode->Next);
    pnode->Next=NULL;
    SerialSendString("Delete success\r\n");
    return true;
}
bool list_isFull(const Plist*head){
    Node*pt;
    bool full;
    pt=(Node*)malloc(sizeof(Node));
    if(pt==NULL){
        full=true;
    }
    else{
        full=false;
    }
    free(pt);
    return full;
}
bool list_isEmpty(const Plist*head){
    if(*head==NULL){
        return true;
    }
    else{
        return false;
    }
}
void Free_linked_list(Plist*head){
    List*ptr=NULL;
    while(*head!=NULL){
        ptr=(*head)->Next;
        free(*head);
        *head=ptr;
    }
    SerialSendString("Free success\r\n");
}
void Init_linked_list(Plist*head){
    *head=NULL;
    SerialSendString("Init success\r\n");
}
void Print_linked_list(const Plist*head,int current_pos){
    List*pnode=*head;
    if(pnode==NULL){
        SerialSendString("The linked list is empty!\r\n");
        return;
    }
    find_Designate_node(pnode,current_pos);
    switch(pnode->node.Mode){
        case first:
            show_target_matrix1(pnode->node.mat1,2);
            show_target_matrix1(pnode->node.mat2,2);
            SerialSendString("Multiplication operation:\r\n");
            show_target_matrix1(pnode->node.outcome,2);
            break;
        case second:
            show_target_matrix3(pnode->node.transfer);
            SerialSendString("Transfer operation:\r\n");
            show_target_matrix3(pnode->node.outcome);
            break;
        case third:
            print_vector(pnode->node.vector);
            SerialSendString("Rotate Angle:");
            SerialSendFloat(pnode->node.angle,2);
            SerialSendString("\r\n");
            print_vector(pnode->node.vector_rotate);
            break;
    }
}
List* find_Designate_node(List*pnode,int current_pos){
    if(pnode==NULL){
        SerialSendString("Error: The linked list is empty!\r\n");
        return NULL;
    }
    int position=1;
    //注意逻辑问题，position从1开始，current_pos从1开始，所以position<current_pos
    while(pnode!=NULL&&position<current_pos){
        pnode=pnode->Next;
        position++;
    }
    return pnode;
}
void matrix_operator(Node*data,int mode){
    switch(mode){
    //first: multiply two matrices
        case first:
            matrix_multiplication1(data->mat1,data->mat2,data->outcome,2);
            break;
    //second: 2x2 matrix transform
        case second:
            matrix_transfer(data->transfer,data->outcome);
            break;
    //third: 2 dimension vector rotate
        case third:
            create_rotate_matrix(data->matrix_rotate,data->angle);
            vector_rotate(data->vector,data->vector_rotate,data->matrix_rotate,data->angle);
            break;
    }
}