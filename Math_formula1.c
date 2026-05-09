#include<stdio.h>
#include<stdlib.h>
#include"Math_formula1.h"
#include<math.h>
#include"Serial.h"
//1.1两个n*n矩阵乘法运算：version1 and version2
//version2：使用数组
void matrix_multiplication1(float mat1[][2],float mat2[][2],float target_matrix[][2],int len){
    for(int i=0;i<len;i++){
        for(int j=0;j<len;j++){
            target_matrix[i][j]=0;
            for(int k=0;k<len;k++){
                target_matrix[i][j]+=mat1[i][k]*mat2[k][j];
            }
        }
    }
}
void show_target_matrix1(float tar[][2],int LEN){
    for(int i=0;i<LEN;i++){
        for(int j=0;j<LEN;j++){
            SerialSendFloat(tar[i][j],2);
            SerialSendByte(' ');
            if((j+1)%LEN==0){
                SerialSendString("\r\n");
            }
        }
    }
    SerialSendByte('\n');
}
//4.matrix transfer:转置矩阵2*2
void matrix_transfer(float mat[2][2],float target_matrix[2][2]){
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            target_matrix[i][j]=mat[j][i];
        }
    }
}
void show_target_matrix3(float target_matrix[2][2]){
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            SerialSendFloat(target_matrix[i][j],2);
            SerialSendByte(' ');
            if((j+1)%2==0){
                SerialSendString("\r\n");
            }
        }
    }
    SerialSendByte('\n');
}
//7.2维向量旋转
void create_rotate_matrix(float rotate[2][2],float angle){
    rotate[0][0]=cos(angle);
    rotate[0][1]=-sin(angle);
    rotate[1][0]=sin(angle);
    rotate[1][1]=cos(angle);
}
void vector_rotate(float vec[2],float target[2],float rotate[2][2],float angle){
    //初始化target向量
    target[0]=0;
    target[1]=0;
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            target[i]+=rotate[i][j]*vec[j];
        }
    }
}

void print_vector(float vec[2]){
    SerialSendFloat(vec[0],2);
    SerialSendByte(' ');
    SerialSendFloat(vec[1],2);
    SerialSendString("\r\n");
}
