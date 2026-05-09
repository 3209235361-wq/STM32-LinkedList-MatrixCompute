#ifndef M_FORMULA_H
#define M_FORMULA_H
#define PI 3.1415
//1.1两个n*n矩阵乘法运算：version1 and version2
//version2：使用数组
void matrix_multiplication1(float mat1[][2],float mat2[][2],float target_matrix[][2],int len);
void show_target_matrix1(float tar[][2],int LEN);
 
//4.matrix transfer:转置矩阵2*2
void matrix_transfer(float mat[2][2],float target_matrix[2][2]);
void show_target_matrix3(float target_matrix[2][2]);


//7.2维向量旋转
void create_rotate_matrix(float rotate[2][2],float angle);
void vector_rotate(float vec[2],float target[2],float rotate[2][2],float angle);
void print_vector(float vec[2]);
 
#endif