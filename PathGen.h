#ifndef PATHGEN_H
#define PATHGEN_H

#include "back_plate.h"

#define T_VERTICAL 16
#define T_HORIZONTAL 0
#define T_SPRAY 1


int is_vertical(point4w *input);




//��back_plate�ṹ�壬������ֱ��, ���������ԭ�� x��ȵ��߶��ᱻ����һ����ǰ�棬�����ҵ���һ��x����ȵ�λ��
void fill_bp_tape( back_plate* const bp_ptr, const Dwg_Entity_LINE** input, const unsigned int len, const char dir);


//�Ժ���������ݷ������ð������
void bubble_sort_RL( back_plate* const bp_ptr,  Dwg_Entity_LINE** const input, const unsigned int len, const char dir, const char sub_flag);

//���򽺴��������������Ϣ
void output_TAPE(back_plate* bp_ptr);

//Ϊ�������·��
void Generate_path(back_plate* const bp_ptr);



#endif // PATHGEN_H
