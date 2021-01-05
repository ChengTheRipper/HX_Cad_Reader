#include "PathGen.h"


int is_vertical(point4w *input)
{
    if(input->sx == input->ex)
        return true;
    else
        return false;
    return false;
}



void fill_bp_tape( back_plate* const bp_ptr, const Dwg_Entity_LINE** input, const unsigned int len, const char dir)
{
    int i = 0, j = 1, sublen = 0;

    dwg_point_3d dir_vec;//��������
    Dwg_Entity_LINE* line_SS = (input[i]) ;//line_sub_start
    Dwg_Entity_LINE* line_SE = (input[j]);//line_sub_end
    //�����������
    while(j < len)
    {
        line_SS = (input[i]) ;//line_sub_start
        line_SE = (input[j]);//line_sub_end

        //�������Ϊ��ֱ��
        if(dir == DIR_V)
        {
            double result = fabs(line_SS->start.x - line_SE->start.x);
            if( result > 0.001)
            {//���x����ȣ�˵���Ѿ���λ��
                sublen = j - i;
                if(sublen == 1)
                {
                    //�ж��Ƿ��������»�
                    if(fabs(line_SS->start.y - line_SE->start.y) > 0.001)
                    {
                        //��������ָ��,����substart
//                        input[j] = input[i];
                        i = j++;
                        continue;
                    }
                }

                for(int k = 0; k < sublen; ++k)
                {
                    Dwg_Entity_LINE* left = (input[i + k]);
                    Dwg_Entity_LINE* right = (input[j + k]);


                    tape* VT_pt = bp_ptr->vertical_tape + bp_ptr->tape_VT_index;

                    //���е����罺·��
                    VT_pt->path_origin = midpoint( &(left->start), &(right->start) );
                    VT_pt->path_end    = midpoint( &(left->end), &(right->end) );

                    // //Ԥ��������֮��ĳ��Ȳ�һ��
                    // VT_pt->path_origin.y = maxf(left->start.y, right->start.y);
                    // VT_pt->path_end.y    = minf(left->end.y, right->end.y);
                    //��������ֱ��֮��ľ���
                    dir_vec.x = (VT_pt->path_end.x) - (VT_pt->path_origin.x);
                    dir_vec.y = (VT_pt->path_end.y) - (VT_pt->path_origin.y);
                    VT_pt->dir = dir_vec;

                    int g = (bp_ptr->tape_VT_index)++;

                    double sx = (bp_ptr->vertical_tape[g]).path_origin.x;
                    double sy = (bp_ptr->vertical_tape[g]).path_origin.y;
                    double ex = (bp_ptr->vertical_tape[g]).path_end.x;
                    double ey = (bp_ptr->vertical_tape[g]).path_end.y;
                    printf(" V(%.3f, %.3f), (%.3f, %.3f) \n", sx, sy, ex, ey);

                }
                i = j + sublen;
                //��λ����һ��˫��
                j = i + 1;
            }
            else
            //�����ȣ���̽
            {
                ++j;
            }


        }
        else// dir == DIR_H ˮƽ�ߴ���
        {
            /* code */
            double result = fabs(line_SS->start.y - line_SE->start.y);
            if( result > 0.001)
            //���x�����
            {
                sublen = j - i;

                int k = 0;
                for(; k < sublen; ++k)
                {
                    Dwg_Entity_LINE* up = (input[i + k]);
                    Dwg_Entity_LINE* down = (input[j + k]);

                    tape* HZ_pt = bp_ptr->horizontal_tape + bp_ptr->tape_HZ_index;
                    HZ_pt->path_origin = midpoint( &(up->start), &(down->start) );
                    HZ_pt->path_end    = midpoint( &(up->end), &(down->end) );

                    //Ԥ������ֱ�߲�һ��
                    HZ_pt->path_origin.x = minf(up->start.x, down->start.x);
                    HZ_pt->path_end.x    = maxf(up->end.x, down->end.x);

                    dir_vec.x = (HZ_pt->path_end.x) - (HZ_pt->path_origin.x);
                    dir_vec.y = (HZ_pt->path_end.y) - (HZ_pt->path_origin.y);
                    HZ_pt->dir = dir_vec;

                    int g = (bp_ptr->tape_HZ_index)++;

                    double sx = (bp_ptr->horizontal_tape[g]).path_origin.x;
                    double sy = (bp_ptr->horizontal_tape[g]).path_origin.y;
                    double ex = (bp_ptr->horizontal_tape[g]).path_end.x;
                    double ey = (bp_ptr->horizontal_tape[g]).path_end.y;
                    printf(" H(%.3f, %.3f), (%.3f, %.3f) \n", sx, sy, ex, ey);


                }
                i = j + sublen;
                //��λ����һ��˫��
                j = i + 1;
            }
            else
            //�����ȣ���̽
            {
                ++j;
            }
        }
    }





}

void bubble_sort_RL( back_plate* const bp_ptr,  Dwg_Entity_LINE** const input, const unsigned int len, const char dir, const char sub_flag)
{
        int i, j;
        dwg_object* tmp;
        static char sub_sort_needed = 1;

        if(len == 0)
            return;

        for(i = 0; i < len - 1 ; ++i)
        {
            for(j = 0; j < len - 1 ; ++j)
            {
                if( DIR_V == dir)//����Ǵ�ֱ���ߣ�������x
                {
                    double x_1 = input[j]->start.x;
                    double x_2 = input[j + 1]->start.x;
                    if(x_1 > x_2)
                    {
                        tmp = input[j];
                        input[j] = input[j + 1];
                        input[j + 1] = tmp;
                    }

                }
                else if( DIR_H == dir )//�����ˮƽ���ߣ�������y
                {
                    double y_1 = (input[j])->start.y;
                    double y_2 = (input[j + 1])->start.y;
                    if(y_1 > y_2)
                    {
                        tmp = input[j];
                        input[j] = input[j + 1];
                        input[j + 1] = tmp;
                    }
                }
                else
                {
                    printf("fuck\n");
                }

            }
        }

        double buf[len];

        //ʹ��һ������������洢���е�����,��ʵ��һ��stable sort,�����������������겻�䣬
        //ֻ������һ������һ�µĴ�������

        for(i = 0 ; i < len ; ++i)
        {
            if( DIR_V == dir )
            {
                buf[i] = (input[i])->start.x;
            }
            else if( DIR_H == dir )
            {
                buf[i] = (input[i])->start.y;
            }
        }
        //�����Ҫ������������
        if(sub_flag)
        {
            i = j = 0;


            while(j < len + 1)
            {
                // ��right_val��ͬ�����з�Ӧ
                if( fabs(buf[j] - buf[i]) > 0.001)
                {
                    //�����෴����ĵ���
                    bubble_sort_RL(bp_ptr, input + i, j - i, -dir, 0);

                    const double sublen = j - i;
                    if(sublen > 1)
                    {
                        i = j;
                        ++j;
                    }
                    else
                    {
                        //�������ֻ��һ
                        i = ++j;
                        j = i + 1;
                    }



                }
                else
                {
                    ++j;
                }

            }
            //δ���й�����
            if(i == 0)
            {
                bubble_sort_RL(bp_ptr, input + i, j - i, -dir, 0);
            }
            printf("test sort\n");
            for(i = 0; i < len ; ++i)
            {
                double sx = (input[i])->start.x;
                double sy = (input[i])->start.y;
                double ex = (input[i])->end.x;
                double ey = (input[i])->end.y;
                printf(" (%.3f, %.3f), (%.3f, %.3f) \n", sx, sy, ex, ey);
            }
            printf("dir: %s\n\n\n", dir > 0 ? "DIR_V" : "DIR_H");
            //������ϣ����нṹ�����
            fill_bp_tape(bp_ptr, input, len, dir);
        }

}

void output_TAPE(back_plate* bp_ptr)
{
    //�������н���֮ǰ����Ҫ��������
    //��ð����������
    bubble_sort_RL(bp_ptr, bp_ptr->VT_red_line, bp_ptr->RL_VT_index, DIR_V, 1);

    if(bp_ptr->RL_HZ_index)
        //��ð����������
        bubble_sort_RL(bp_ptr, bp_ptr->HZ_red_line, bp_ptr->RL_HZ_index, DIR_H, 1);

}


dwg_point_3d* fill_vertical_group(point4w* path, tape* tape_pt, int sublen, bool Need_Reverse)
{
    int i = 0;
    int j = 0;
    //�������Ҫ����
    if(!Need_Reverse)
    {
        for( i = 0 ; i > -sublen ; --i)
        {
            path[j].ex = ((tape_pt + i)->path_origin.x) * 10;
            path[j].ey = ((tape_pt + i)->path_origin.y) * 10;
            path[j].sx = ((tape_pt + i)->path_end.x) * 10;
            path[j].sy = ((tape_pt + i)->path_end.y) * 10;
            ++j;
        }
        return &((tape_pt + i)->path_end);
    }

    //��Ҫ����
    else
    {
        /* code */
        for( i = sublen - 1 ; i >= 0 ; --i)
        {
            path[j].sx = ((tape_pt - i)->path_origin.x) * 10;
            path[j].sy = ((tape_pt - i)->path_origin.y) * 10;
            path[j].ex = ((tape_pt - i)->path_end.x) * 10;
            path[j].ey = ((tape_pt - i)->path_end.y) * 10;

            ++j;
           //��ת˳�����㣬�յ�
        }
        return &((tape_pt + i)->path_end);
    }
}
//·���滮��ʵ��·������
int fill_actual_path(point4w* path, back_plate* bp_pt)
{
    //�������У��������Ǹ���y�ֺ���ģ���y�������򣬴����У�x����ε���������һ��ʼ����������еĵײ�������
    int P_i = -1, H_i = 0;
    tape* tape_pt = bp_pt->horizontal_tape;
    //������ں��ߣ�
    if(bp_pt->tape_HZ_index)
    {
        double c_y, n_y;
        //�����ֹ,���Գ���һ��ͬy����
        do
        {
            ++P_i;
            path[P_i].sx = ((tape_pt + P_i)->path_origin.x) * 10;
            path[P_i].sy = ((tape_pt + P_i)->path_origin.y) * 10;
            path[P_i].ex = ((tape_pt + P_i)->path_end.x) * 10;
            path[P_i].ey = ((tape_pt + P_i)->path_end.y) * 10;
            if( (P_i + 1) == (bp_pt->tape_HZ_index) )
            {//�Է�����ͼֻֽ��һ������
                ++P_i;
                break;
            }
            c_y = (tape_pt + P_i)->path_origin.y;
            n_y = (tape_pt + P_i + 1)->path_origin.y;

         } while (fabs(c_y - n_y) < 0.001);
        //��¼���һ����
        ++P_i;
        path[P_i].sx = ((tape_pt + P_i)->path_origin.x) * 10;
        path[P_i].sy = ((tape_pt + P_i)->path_origin.y) * 10;
        path[P_i].ex = ((tape_pt + P_i)->path_end.x) * 10;
        path[P_i].ey = ((tape_pt + P_i)->path_end.y) * 10;
    }
    //������������ں����Լ�ֻ�еײ�һ�����ߵ������P_i == 0 or P_i == -1)
    if(P_i <= 0)
    {
        H_i = ++P_i;
        //���Ϻ��ߵ�y����
    }
    else
        H_i = P_i;

    //��ʼ��ͣ�Ĵ�����

    int V_i = bp_pt->tape_VT_index - 1;
    tape_pt = bp_pt->vertical_tape;

    dwg_point_3d* c_end = NULL, *n_start = NULL, *n_end = NULL;

    //����ɹ������˺�ֱ����
    if(P_i > 1)
    {
        c_end   = &((bp_pt->horizontal_tape + P_i - 1)->path_end);//������������һ������һ�������Ǹ����һ�����߱Ƚϵ�
        n_start = &((tape_pt + V_i)->path_origin);
        n_end   = &((tape_pt + V_i)->path_end);
    }
    //���ɹ��Ļ������ϵ�һ�����ߵ��յ㣬ǿ�з�תֱ��
    else
    {
        c_end   = &((bp_pt->vertical_tape + V_i)->path_end);//��������е��յ㣬��һ�������Ǹ����һ�����߱Ƚϵ�
        n_start = &((tape_pt + V_i)->path_origin);
        n_end   = &((tape_pt + V_i)->path_end);
    }

    //������ɹ�����û�д����κκ���


    bool Need_Reverse = false;
    while( V_i >= 0 )
    {

        size_t sublen = 1;

        //��������β����ʼ,����Եģ���һ��ͬx��ֱ����
        //����sublen
        double c_x, n_x;
        c_x = (tape_pt + V_i)->path_origin.x;
        n_x = (tape_pt + V_i - sublen)->path_origin.x;
        while( (V_i - sublen >= 0) && ( fabs( c_x - n_x ) < 0.001 ))
        {
            c_x = (tape_pt + V_i)->path_origin.x;
            n_x = (tape_pt + V_i - sublen)->path_origin.x;
            ++sublen;
        }

        if(sublen != 1)
            --sublen;

        if(n_start != NULL)
        {
            //�Ҹ�����λ�ý�һ�㣬�����յ��λ�ý�һ�㣿
//            double dis_end_start = getdis_4(c_end->x, c_end->y, n_start->x, n_start->y);
//            double dis_end_end   = getdis_4(c_end->x, c_end->y, n_end->x, n_end->y);
            //���յ�����ͷ�ת
            if(Need_Reverse == false)
            {
                Need_Reverse = true;
            }
            else
            {
                Need_Reverse = false;
            }

        }
        //�����Ƿ���Ҫreverseѡ��deque������������Ƿ������������˳�򣬷���start �� end
        c_end = fill_vertical_group(path + P_i, tape_pt + V_i, (int)sublen, Need_Reverse);
        P_i += sublen;
        V_i -= sublen;

        if(V_i < 0)
        {
            //˵���Ѿ��������к����飬����
            break;
        }
        n_start = &((tape_pt + V_i)->path_origin);
        n_end   = &((tape_pt + V_i)->path_end);

    }


    //�����һ���ĺ���
    tape_pt = bp_pt->horizontal_tape;
    //����������Ϻ���, �Ͳ���
    if(H_i < bp_pt->tape_HZ_index)
    {
        //�������ֻ��һ��
        //�����ֹ,���Գ���һ��ͬy����
        do
        {

            path[P_i].sx = ((tape_pt + H_i)->path_origin.x) * 10;
            path[P_i].sy = ((tape_pt + H_i)->path_origin.y) * 10;
            path[P_i].ex = ((tape_pt + H_i)->path_end.x) * 10;
            path[P_i].ey = ((tape_pt + H_i)->path_end.y) * 10;
            ++P_i;
            if( (H_i + 1) == (bp_pt->tape_HZ_index) )
            {
                break;
            }
            ++H_i;
        } while (fabs((tape_pt + P_i)->path_origin.y - (tape_pt + P_i + 1)->path_origin.y) < 0.001);

    }
    return P_i;

}

void fill_empty_path(point4w* epath, int len, point4w* apath)
{



    for(int i = 0 ; i < len - 1 ; ++i)
    {
        //�жϵ�һ���ߵ��յ���ڶ����ߵ���������������յ����
        epath[i].sx = apath[i].ex;
        epath[i].sy = apath[i].ey;
        epath[i].ex = apath[i + 1].sx;
        epath[i].ey = apath[i + 1].sy;

    }
    epath[len - 1].sx = apath[len - 1].ex;
    epath[len - 1].sy = apath[len - 1].ey;
    epath[len - 1].ex = apath[0].sx;
    epath[len - 1].ey = apath[0].sy;

}

// void Get_minmax_xy(actual_path *apath, point4w* tmp)
// {
//     int min_x, min_y, max_x, max_y;

//     min_x =tmp[0].sx;
//     min_y =tmp[0].sy;
//     max_x =tmp[0].sx;
//     max_y =tmp[0].sy;


//     for(int i = 1 ; i < apath->len ; i++)
//     {
//         int t_x = tmp[i].sx;
//         int t_y = tmp[i].sy;

//         if(t_x < min_x)
//             min_x = t_x;

//         if(t_x < min_y)
//             min_y = t_x;

//         if(t_x > max_x)
//             max_x = t_x;

//         if(t_y > max_y)
//             max_y = t_y;
//     }

//     apath->rsx = min_x;
//     apath->rsy = min_y;
//     apath->length = max_x - min_x;
//     apath->width  = max_y - min_y;

// }


void Generate_path(back_plate* const bp_ptr)
{


    actual_path Apath;

    memset(&Apath, 0, sizeof(Apath));

    point4w ac_path[50];
    point4w empty_path[50];
    //�ȶ�ʵ��·��

    //ȡ���һ��

    int i = 0, j = 1;
    tape* HT_ptr = bp_ptr->horizontal_tape;
    tape* VT_ptr = bp_ptr->vertical_tape;


    Apath.len = 0;
    Apath.len += fill_actual_path(ac_path, bp_ptr);

    printf("total actual path %d\n", Apath.len);

    for(int i = 0 ; i < Apath.len ; ++i)
    {
        printf("sx: %d, sy: %d, ex: %d, ey: %d\n", ac_path[i].sx, ac_path[i].sy, ac_path[i].ex, ac_path[i].ey);
    }
    printf("\n\n\n");

    fill_empty_path(empty_path, Apath.len, ac_path);
    for(int i = 0 ; i < Apath.len ; ++i)
    {
        printf("sx: %d, sy: %d, ex: %d, ey: %d\n", empty_path[i].sx, empty_path[i].sy, empty_path[i].ex, empty_path[i].ey);
    }
    printf("\n\n\n");

    FILE* fpTXT;
    fpTXT = fopen("path.txt", "w+");
    if(fpTXT == NULL)
    {
        printf("error writing data to txt\n");
    }
    //��ʱ�Ĳ��ԣ�
    // Get_minmax_xy(&Apath, ac_path);

    //�޸Ĵ˴�λ��Ϊ�����Ͻ�Ϊԭ��

    Apath.rsx = bp_ptr->minWidth * 10;
    // Apath.rsy = bp_ptr->minHeight * 10;
    Apath.rsy    = bp_ptr->minHeight * 10;
    Apath.height = (bp_ptr->maxHeight - bp_ptr->minHeight) * 10;
    Apath.width  = (bp_ptr->maxWidth  - bp_ptr->minWidth) * 10;

    const double fileRsx = bp_ptr->minWidth * 10;
    const double fileRsy = bp_ptr->maxHeight * 10;

    j = 0;

    //i == 0
    //��һ�����г̲���������������Լ������
    //��ʼ����
    fprintf(fpTXT, "%d %d %d\n",
                ac_path[0].sx - fileRsx,
                abs(ac_path[0].sy - fileRsy),
                is_vertical(ac_path) ? (T_VERTICAL) : (T_HORIZONTAL)
                );//����,���ݺ����й�·�������Ƿ��紹ֱ����ˮƽ
    for(int i = 0; i < Apath.len; ++i)
    {
        //�ṹ�屣��rawͼֽ����
        Apath.real_path[j++] = ac_path[i];
        Apath.real_path[j++] = empty_path[i];
        //�ļ���������ģ������½�Ϊԭ���ͼֽ����
        point4w tmp_ap = {
                            ac_path[i].sx - fileRsx,
                            abs(ac_path[i].sy - fileRsy),
                            ac_path[i].ex - fileRsx,
                            abs(ac_path[i].ey - fileRsy),
                            };
        point4w tmp_ep = {
                            empty_path[i].sx - fileRsx,
                            abs(empty_path[i].sy - fileRsy),
                            empty_path[i].ex - fileRsx,
                            abs(empty_path[i].ey - fileRsy),
                            };

        // fprintf(fpTXT, "%d %d %d %d 1\n", tmp_ap.sx, tmp_ap.sy, tmp_ap.ex, tmp_ap.ey);
        // fprintf(fpTXT, "%d %d %d %d 0\n", tmp_ep.sx, tmp_ep.sy, tmp_ep.ex, tmp_ep.ey);


        //ʵ��
        fprintf(fpTXT, "%d %d %d\n", tmp_ap.ex, tmp_ap.ey, T_SPRAY);
         //����
        fprintf(fpTXT, "%d %d %d\n", tmp_ep.ex, tmp_ep.ey, is_vertical(ac_path + i + 1) ? (T_VERTICAL) : (T_HORIZONTAL));//����,���ݺ����й�·�������Ƿ��紹ֱ����ˮƽ
    }
    fclose(fpTXT);


    Apath.len *= 2;

    FILE* fp;
    fp = fopen("path.cof", "wb+");//cad_out_file , ʵ�ʾ���Ҫдһ���ṹ���ȥ
    if(fp == NULL)
    {
        printf("error open path.cof\n");
    }
    fwrite((&Apath), sizeof(Apath), 1, fp);

    fclose(fp);

    // fp = fopen("path.cof", "r+") why write a binary file without b option...
    //���Զ�ȡpath�ṹ��
    fp = fopen("path.cof", "rb+");
    actual_path a2;
    fread(&a2, sizeof(a2), 1, fp);
    fclose(fp);

    for(int i = 0; i < Apath.len; ++i)
    {
        printf("%s: sx :%d, sy :%d, ex :%d, ey :%d\n",(i % 2) ? ("empty"):("real"), a2.real_path[i].sx, a2.real_path[i].sy, a2.real_path[i].ex, a2.real_path[i].ey);
    }
    printf("width %d, height %d\n", Apath.width, Apath.height);

}
