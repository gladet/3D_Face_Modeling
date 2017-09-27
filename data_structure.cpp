for(i = 0; i < M; i++)
{
	for(i = 0; i < 3; i++)
	{
		nml[i] = face1_normals[pxl_map[k][v*biWidth[k]+u]][i];//法向量在求权值时是一定要用到的
	}
	
	//凡是累加项都要初始清0
	deno_of_weight = 0;	//权重的分母部分初始化为0	
	vtx_gray[i] = 0;//像素对应灰度初始化为0
			
	//第二层循环于各视图
	for(k = 0; k < num_view; k++)
	{
		if(vtx_map[k][i][0] != -1)//该像素在该视图中存在一个对应像素
		{
			//初始化			
			vis_img[k][i] = true;
			if(r3[k]*nml>0)//判断网格顶点在视图k中是否可见，严格讲并不确定内积大于0是否与可见性判断等价？
				vis_img[k][i] = false;
			else
				deno_of_weight += r3[k]*nml;//累加以计算分母部分，是网格顶点在其中可见的视图对应的内积的求和，这里把负号丢掉了
		}
	}
	//计算网格顶点在各视图（顶点在其中可见）中对应的权值，对应像素的灰度，及进一步的，在对应像素处关于X和Y方向的偏导
	for(k = 0; k < num_view; k++)
	{
		if(vis_img[k][i])//若该网格顶点在视图k中可见
		{
			//计算该网格顶点对应于视图k的权值
			weight[k][i] = r3[k]*nml/deno_of_weight;
			//计算该网格顶点在视图k中对应像素处关于该像素颜色的偏导
			//计算指定像素的坐标，是3*3区域的中心
			//记录网格顶点i在视图k中对应的像素的坐标
			u_k = vtx_map[k][i]][0];
			v_k = vtx_map[k][i]][1];
			//计算gray_k，亦即指定像素周围8邻域的灰度值
			for(l = -1; l < 2; l++)//遍历行
			{
				for(m = -1; m < 2; m++)//遍历列
				{
					//这里已经假定指定像素一定不在边界上，否则将可能出现访问越界！从现在的这些图像看，是有可能出现在边界上的！
					if(v_k+m > -1 && v_k+m < biHeight[k] && u_k+l > -1 && u_k+l < biWidth[k])
					{
						gray_k[(l+1)*3+(m+1)] = view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)+2] * 0.3 + view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)+1] * 0.59 + view_data[k][(v_k+m)*biAlign[k]+3*(u_k+l)] * 0.11;
						if(l == 0 && m == 0)//掩膜中心位置，即网格顶点对应像素处
							vtx_map_gray[k][i] = gray_k[4];//记录网格顶点i在视图k中对应的像素的灰度
					}
					else//分情况讨论
					{
						if(v_k+m = -1)//上边界
						{
							if(u_k+l = -1)//左边界
								gray_k[(l+1)*3+(m+1)] = view_data[k][2] * 0.3 + view_data[k][1] * 0.59 + view_data[k][0] * 0.11;
							else
							{
								if(u_k+l = biWidth[k])//右边界
									gray_k[(l+1)*3+(m+1)] = view_data[k][3*(biWidth[k]-1)+2] * 0.3 + view_data[k][3*(biWidth[k]-1)+1] * 0.59 + view_data[k][3*(biWidth[k]-1)] * 0.11;
								else
									view_data[k][3*(u_k+l)+2] * 0.3 + view_data[k][3*(u_k+l)+1] * 0.59 + view_data[k][3*(u_k+l)] * 0.11;
							}												
						}	
						if(v_k+m = biHeight[k])//下边界
						{
							if(u_k+l = -1)//左边界
								gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+2] * 0.3 + view_data[(biHeight[k]-1)*biAlign[k]+k][1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]] * 0.11;
							else
							{
								if(u_k+l = biWidth)//右边界
									gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth-1)] * 0.11;
								else
									view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+l)] * 0.11;
							}												
						}
					}
				}
			}
			//在横向和纵向用Sobel掩膜计算分别得到u,v方向的偏导
			//记录网格顶点在各视图中对应像素处的偏导
			derive_x[k][i] = gray_k[2]-gray_k[0]+2*(gray_k[5]-gray_k[3])+gray_k[8]-gray_k[6];
			derive_y[k][i] = gray_k[6]-gray_k[0]+2*(gray_k[7]-gray_k[1])+gray_k[8]-gray_k[2];	
			//计算网格顶点对应的灰度，实际上是其在各视图中对应像素灰度的加权平均
			vtx_gray[i] += weight[k][i]*vtx_map[k][i];
		}
	}
}