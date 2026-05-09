#ifndef __FUNC_H__
#define __FUNC_H__
#define MAX_SIZE 50
#define MAX_Mode 3
enum Mode{first,second,third};
#include"stm32f10x.h"
#include<stdbool.h>
//节点结构体
typedef struct Node{
    uint8_t ID;
    uint8_t Mode;
    //1.matrix multiply
    float  mat1[2][2];
    float  mat2[2][2];
    float  outcome[2][2];//1和2一起用
    //2.matrix transfer
    float  transfer[2][2];
    //3.vector rotate
    float vector[2];
    float angle;
    float matrix_rotate[2][2];
    float vector_rotate[2];
}Node;
//全局变量
typedef struct List{
    Node node;
    //处理函数
    void (*Matrix_transform)(Node*data,int mode);
    struct List*Next;
}List;
//指针类型
typedef List* Plist;
void Init_linked_list(Plist*head);
int Get_NodeSize(const Plist*head);
bool EnAdd_Node(Plist*head,Node*data,int count);
bool EnDelete_Node(Plist*head);
bool list_isFull(const Plist*head);
bool list_isEmpty(const Plist*head);
void Free_linked_list(Plist*head);
List* find_Designate_node(List*pnode,int current_pos);
void Print_linked_list(const Plist*head,int current_pos);
void Node_Init(List*pnew,Node*data,int mode);
void matrix_operator(Node*data,int mode);
#endif