for(i = 0; i < M; i++)
{
	for(i = 0; i < 3; i++)
	{
		nml[i] = face1_normals[pxl_map[k][v*biWidth[k]+u]][i];//����������Ȩֵʱ��һ��Ҫ�õ���
	}
	
	//�����ۼ��Ҫ��ʼ��0
	deno_of_weight = 0;	//Ȩ�صķ�ĸ���ֳ�ʼ��Ϊ0	
	vtx_gray[i] = 0;//���ض�Ӧ�Ҷȳ�ʼ��Ϊ0
			
	//�ڶ���ѭ���ڸ���ͼ
	for(k = 0; k < num_view; k++)
	{
		if(vtx_map[k][i][0] != -1)//�������ڸ���ͼ�д���һ����Ӧ����
		{
			//��ʼ��			
			vis_img[k][i] = true;
			if(r3[k]*nml>0)//�ж����񶥵�����ͼk���Ƿ�ɼ����ϸ񽲲���ȷ���ڻ�����0�Ƿ���ɼ����жϵȼۣ�
				vis_img[k][i] = false;
			else
				deno_of_weight += r3[k]*nml;//�ۼ��Լ����ĸ���֣������񶥵������пɼ�����ͼ��Ӧ���ڻ�����ͣ�����Ѹ��Ŷ�����
		}
	}
	//�������񶥵��ڸ���ͼ�����������пɼ����ж�Ӧ��Ȩֵ����Ӧ���صĻҶȣ�����һ���ģ��ڶ�Ӧ���ش�����X��Y�����ƫ��
	for(k = 0; k < num_view; k++)
	{
		if(vis_img[k][i])//�������񶥵�����ͼk�пɼ�
		{
			//��������񶥵��Ӧ����ͼk��Ȩֵ
			weight[k][i] = r3[k]*nml/deno_of_weight;
			//��������񶥵�����ͼk�ж�Ӧ���ش����ڸ�������ɫ��ƫ��
			//����ָ�����ص����꣬��3*3���������
			//��¼���񶥵�i����ͼk�ж�Ӧ�����ص�����
			u_k = vtx_map[k][i]][0];
			v_k = vtx_map[k][i]][1];
			//����gray_k���༴ָ��������Χ8����ĻҶ�ֵ
			for(l = -1; l < 2; l++)//������
			{
				for(m = -1; m < 2; m++)//������
				{
					//�����Ѿ��ٶ�ָ������һ�����ڱ߽��ϣ����򽫿��ܳ��ַ���Խ�磡�����ڵ���Щͼ�񿴣����п��ܳ����ڱ߽��ϵģ�
					if(v_k+m > -1 && v_k+m < biHeight[k] && u_k+l > -1 && u_k+l < biWidth[k])
					{
						gray_k[(l+1)*3+(m+1)] = view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)+2] * 0.3 + view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)+1] * 0.59 + view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)] * 0.11;
						if(l == 0 && m == 0)//��Ĥ����λ�ã������񶥵��Ӧ���ش�
							vtx_map_gray[k][i] = gray_k[4];//��¼���񶥵�i����ͼk�ж�Ӧ�����صĻҶ�
					}
					else//���������
					{
						if(v_k+m = -1)//�ϱ߽�
						{
							if(u_k+l = -1)//��߽�
								gray_k[(l+1)*3+(m+1)] = view_data[k][2] * 0.3 + view_data[k][1] * 0.59 + view_data[k][0] * 0.11;
							else
							{
								if(u_k+l = biWidth[k])//�ұ߽�
									gray_k[(l+1)*3+(m+1)] = view_data[k][3*(biWidth[k]-1)+2] * 0.3 + view_data[k][3*(biWidth[k]-1)+1] * 0.59 + view_data[k][3*(biWidth[k]-1)] * 0.11;
								else
									view_data[k][3*(u_k+l)+2] * 0.3 + view_data[k][3*(u_k+l)+1] * 0.59 + view_data[k][3*(u_k+l)] * 0.11;
							}												
						}	
						if(v_k+m = biHeight[k])//�±߽�
						{
							if(u_k+l = -1)//��߽�
								gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+2] * 0.3 + view_data[(biHeight[k]-1)*biAlign[k]+k][1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]] * 0.11;
							else
							{
								if(u_k+l = biWidth)//�ұ߽�
									gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)] * 0.11;
								else
									view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)] * 0.11;
							}												
						}
					}
				}
			}
			//�ں����������Sobel��Ĥ����ֱ�õ�u,v�����ƫ��
			//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���ƫ��
			derive_x[k][i] = gray_k[2]-gray_k[0]+2*(gray_k[5]-gray_k[3])+gray_k[8]-gray_k[6];
			derive_y[k][i] = gray_k[6]-gray_k[0]+2*(gray_k[7]-gray_k[1])+gray_k[8]-gray_k[2];	
			//�������񶥵��Ӧ�ĻҶȣ�ʵ���������ڸ���ͼ�ж�Ӧ���ػҶȵļ�Ȩƽ��
			vtx_gray[i] += weight[k][i]*vtx_map[k][i];
		}
	}
}