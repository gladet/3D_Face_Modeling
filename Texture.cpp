// Texture.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Texture.h"
//#include "OGLEventDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTexture

IMPLEMENT_DYNCREATE(CTexture, CView)

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}


BEGIN_MESSAGE_MAP(CTexture, CView)
	//{{AFX_MSG_MAP(CTexture)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTexture drawing

void CTexture::OnDraw(CDC* pDC)
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!pDoc->cylindrical_map || !pDoc->txt_mapped)
	{
		unsigned char* pBitmap;
		if(!pDoc->transformed)
			pBitmap= pDoc->m_pBitmap;
		else pBitmap = pDoc->temp_view;//在计算了映射矩阵后，所有视图指针都已存入链表，可以遍历显示各视图
		if(pBitmap == NULL)
			return;
		LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
		unsigned char* pBitmapData = pBitmap + ((LPBITMAPFILEHEADER)pBitmap)->bfOffBits;
		unsigned long biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
		unsigned long biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
		unsigned long biAlign = (biWidth*3+3)/4 *4;
		int i,j,k;//x,y,cur;
		list_intpt::iterator itrt;
						
		CRect rect;
		GetClientRect(&rect);
		long originx =0;
		long originy =0;
		if((unsigned long)(rect.Width()) > biWidth)
			originx = (rect.Width() - biWidth)>>1;
		if((unsigned long)(rect.Height()) > biHeight)
			originy = (rect.Height() - biHeight)>>1;
		SetDIBitsToDevice(pDC->m_hDC, originx, originy, biWidth, biHeight, 0, 0, 0, biHeight, pBitmapData, 	
			lpBitmapInfo, DIB_RGB_COLORS);
			
		
		/*
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//检验OnMap的模块
		if(pDoc->mapped)
		{
			for(i = 0; i < biWidth; i++)
			{
				for(j = 0; j < biHeight; j++)
				{
					if(pDoc->pxl_map[pDoc->iter_index-1][j*biWidth+i] != -1)//存在对应网格顶点
					{
						pDC->SetPixel(CPoint(originx+i, originy+biHeight-1-j), RGB(255, 0, 255));
					}
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		*/

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		if(pDoc->data_cmpt)
		{
			double cur_gray;//当前遍历像素的灰度值
			unsigned long u, v;
			char vertex_gray;
			for(v = 0; v < pDoc->biHeight[pDoc->iter_index-1]; v++)//遍历行
			{
				for(u = 0; u < pDoc->biWidth[pDoc->iter_index-1]; u++)//遍历列
				{
					cur_gray = pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u+2] * 0.3 + pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u+1] * 0.59 + pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u] * 0.11;										
						
					if(pDoc->pxl_map[pDoc->iter_index-1][v*pDoc->biWidth[pDoc->iter_index-1]+u] != -1)//对一个拥有对应网格顶点的像素构造其在Jacaobian Matrix中的相应元素
					{
						vertex_gray = pDoc->vtx_gray[pDoc->pxl_map[pDoc->iter_index-1][v*pDoc->biWidth[pDoc->iter_index-1]+u]];
						pDC->SetPixel(CPoint(originx+u, originy+pDoc->biHeight[pDoc->iter_index-1]-1-v), RGB((char)vertex_gray, (char)vertex_gray, (char)vertex_gray));//RGB((char)fabs(vertex_gray-cur_gray), (char)fabs(vertex_gray-cur_gray), (char)fabs(vertex_gray-cur_gray)));						
					}
					else
					{
						pDC->SetPixel(CPoint(originx+u, originy+pDoc->biHeight[pDoc->iter_index-1]-1-v), RGB(0, 0, 0));//(char)cur_gray, (char)cur_gray, (char)cur_gray));
					}
				}
			}			
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		if(pDoc->img_pt_num>0)
		{
			if(pDoc->which_loc == 1)
			{
				//在计算出实际映射点之后显示标定点与实际对应点，以观察其误差
				
				//显示temp_img_pts所指向的链表中的图像标记点
				for(itrt = pDoc->temp_img_pts[0].begin(); itrt != pDoc->temp_img_pts[0].end(); ++itrt)
				{
					for(j = -1; j < 2; j++)
					{
						if(j == 0)
						{
							for(k = -1; k < 2; k++)
							{
								pDC->SetPixel(CPoint(originx+(*itrt)[0]+j, originy+biHeight-1-(*itrt)[1]+k), RGB(255, 0, 255));
							}
						}
						else
							pDC->SetPixel(CPoint(originx+(*itrt)[0]+j, originy+biHeight-1-(*itrt)[1]), RGB(255, 0, 255));
					}
				}
				
				if(pDoc->textured)
				{
					//用青色来显示
					for(i = 0; i < pDoc->mdl_pt_num; i++)
					{
						for(j = -1; j < 2; j++)
						{
							if(j == 0)
							{
								for(k = -1; k < 2; k++)
								{
									pDC->SetPixel(CPoint(originx+pDoc->act_img_pts_list[pDoc->iter_index-1][i][0]+j, originy+biHeight-1-pDoc->act_img_pts_list[pDoc->iter_index-1][i][1]+k), RGB(0, 255, 255));
								}
							}
							else
								pDC->SetPixel(CPoint(originx+pDoc->act_img_pts_list[pDoc->iter_index-1][i][0]+j, originy+biHeight-1-pDoc->act_img_pts_list[pDoc->iter_index-1][i][1]), RGB(0, 255, 255));
						}
					}
				}

			}
			else
			{
				for(i = 0; i <  pDoc->img_pt_num; i++)
				{
					for(j = -1; j < 2; j++)
					{
						if(j == 0)
						{
							for(k = -1; k < 2; k++)
							{
								pDC->SetPixel(CPoint(originx+pDoc->img_temp_pt[i].x+j, originy+biHeight-1-pDoc->img_temp_pt[i].y+k), RGB(255, 0, 255));
							}
						}
						else
							pDC->SetPixel(CPoint(originx+pDoc->img_temp_pt[i].x+j, originy+biHeight-1-pDoc->img_temp_pt[i].y), RGB(255, 0, 255));
					}
				}				
			}			
		}
		
		//用品红来显示
		if(i != 0 && pDoc->mesh_projed && pDoc->textured)//的确已经计算了实际映射点，同时也意味着一定生成了纹理坐标
		{
			//想进一步把网格画(unfold)到图像上
			//可能已经映射完点，但是没有将指针加入链表
			if(pDoc->num_view > 0)
			{
				if(pDoc->mapped)
				{
					unsigned long tri_coor[3][2];			
			
					//遍历每个三角形
					for(i = 0; i < pDoc->L1; i++)
					{
						//读出三角形上的三个顶点的纹理坐标
						for(j = 0; j < 3; j++)
						{
							for(k = 0; k < 2; k++)
							{
								tri_coor[j][k] = pDoc->vtx_map[pDoc->iter_index-1][pDoc->triangles1[i][j]][k];//记录三角形各顶点在当前视图中对应的像素坐标														
							}
						}
						if(tri_coor[0][0] != -1 && tri_coor[1][0] != -1 && tri_coor[2][0] != -1)//三角形的所有顶点都已确定了对应的像素
						{
							pDC->MoveTo(CPoint(originx+tri_coor[0][0], originy+biHeight-1-tri_coor[0][1]));
							pDC->LineTo(originx+tri_coor[1][0], originy+biHeight-1-tri_coor[1][1]);
							pDC->LineTo(originx+tri_coor[2][0], originy+biHeight-1-tri_coor[2][1]);
							pDC->LineTo(originx+tri_coor[0][0], originy+biHeight-1-tri_coor[0][1]);
						}
						for(k = 0; k < 25000; k++)
						{
							;
						}					
					}
				}
				else
				{
					double tri_txts[3][2];			
			
					//遍历每个三角形
					for(i = 0; i < pDoc->L1; i++)
					{
						//读出三角形上的三个顶点的纹理坐标
						for(j = 0; j < 3; j++)
						{
							for(k = 0; k < 2; k++)
							{
								tri_txts[j][k] = pDoc->temp_txt[pDoc->triangles1[i][j]][k];//纹理坐标序号与顶点序号是严格对应的						
								while(tri_txts[j][k] < 0)
									tri_txts[j][k] += 1;
								while(tri_txts[j][k] > 1)
									tri_txts[j][k] -= 1;
								if(tri_txts[j][k] <= 0.0125)
								{
									tri_txts[j][k] = tri_txts[j][k];
								}
							}
						}
						pDC->MoveTo(CPoint(originx+(int)(tri_txts[0][0]*(biWidth-1))+j, originy+biHeight-1-(int)(tri_txts[0][1]*(biHeight-1))));
						pDC->LineTo(originx+(int)(tri_txts[1][0]*(biWidth-1))+j, originy+biHeight-1-(int)(tri_txts[1][1]*(biHeight-1)));
						pDC->LineTo(originx+(int)(tri_txts[2][0]*(biWidth-1))+j, originy+biHeight-1-(int)(tri_txts[2][1]*(biHeight-1)));
						pDC->LineTo(originx+(int)(tri_txts[0][0]*(biWidth-1))+j, originy+biHeight-1-(int)(tri_txts[0][1]*(biHeight-1)));
					}
				}
			}

			//显示temp_img_pts所指向的链表中的图像标记点
			for(itrt = pDoc->temp_img_pts[0].begin(); itrt != pDoc->temp_img_pts[0].end(); ++itrt)
			{
				for(j = -1; j < 2; j++)
				{
					if(j == 0)
					{
						for(k = -1; k < 2; k++)
						{
							pDC->SetPixel(CPoint(originx+(*itrt)[0]+j, originy+biHeight-1-(*itrt)[1]+k), RGB(255, 0, 255));
						}
					}
					else
						pDC->SetPixel(CPoint(originx+(*itrt)[0]+j, originy+biHeight-1-(*itrt)[1]), RGB(255, 0, 255));
				}
			}

			//用青色来显示
			for(i = 0; i < pDoc->mdl_pt_num; i++)
			{
				for(j = -1; j < 2; j++)
				{
					if(j == 0)
					{
						for(k = -1; k < 2; k++)
						{
							pDC->SetPixel(CPoint(originx+pDoc->act_img_pts_list[pDoc->iter_index-1][i][0]+j, originy+biHeight-1-pDoc->act_img_pts_list[pDoc->iter_index-1][i][1]+k), RGB(0, 255, 255));
						}
					}
					else
						pDC->SetPixel(CPoint(originx+pDoc->act_img_pts_list[pDoc->iter_index-1][i][0]+j, originy+biHeight-1-pDoc->act_img_pts_list[pDoc->iter_index-1][i][1]), RGB(0, 255, 255));
				}
			}
		}
	}	
	else
	{
		unsigned char* pBitmap = pDoc -> m_pTransfered;
		if(pBitmap == NULL)
			return;
		
		unsigned long biHeight = pDoc->height;
		unsigned long biWidth = pDoc->width;		
		
		int i, j, cur;
		unsigned char r, g, b;
		CRect rect;
		GetClientRect(&rect);
		long originx =0;
		long originy =0;
		if((unsigned long)(rect.Width()) > biWidth)
			originx = (rect.Width() - biWidth)>>1;
		if((unsigned long)(rect.Height()) > biHeight)
			originy = (rect.Height() - biHeight)>>1;
		
		for(j = 0; j < biHeight; j++)
		{
			for(i = 0; i < biWidth; i++)
			{
				cur = (biHeight-1-j)*biWidth*3+i*3;//注意绘图坐标系的y轴正方向是由上至下的
				r = pBitmap[cur+2];
				g = pBitmap[cur+1];
				b = pBitmap[cur];
				pDC->SetPixel(CPoint(originx+i, originy+j), RGB(r, g, b));
			}
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CTexture diagnostics

#ifdef _DEBUG
void CTexture::AssertValid() const
{
	CView::AssertValid();
}

void CTexture::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COGLEventDoc* CTexture::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COGLEventDoc)));
	return (COGLEventDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTexture message handlers

void CTexture::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		

	if(pDoc->img_fp_lc == FALSE)
	{
		unsigned char* pBitmap = pDoc -> m_pBitmap;
		if(pBitmap == NULL)
			return;
		LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
		unsigned char* pBitmapData = pBitmap + ((LPBITMAPFILEHEADER)pBitmap)->bfOffBits;
		unsigned long biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
		unsigned long biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
		
		CRect rect;
		GetClientRect(&rect);
		
		long originx =0;
		long originy =0;
		if((unsigned long)(rect.Width()) > biWidth)
			originx = (rect.Width() - biWidth)>>1;
		if((unsigned long)(rect.Height()) > biHeight)
			originy = (rect.Height() - biHeight)>>1;
		
		if(pDoc->which_loc == 1)
		{
			//这是供显示时使用的图像数组中的位置，也是为后面的映射服务的，y轴正方向是向上的
			int *temp_intpt = new int[2];

			temp_intpt[0] = point.x - originx;
			temp_intpt[1] = biHeight - (point.y - originy) - 1;

			pDoc->temp_img_pts[0].insert(pDoc->temp_img_pts[0].end(), temp_intpt);	
			
			//这是供位姿调整时使用的图像坐标系的坐标值，x轴正方向是向上的
			temp_intpt = new int[2];

			temp_intpt[0] = biHeight - (point.y - originy) - 1;
			temp_intpt[1] = point.x - originx;

			pDoc->temp_img_pt_cd[0].insert(pDoc->temp_img_pt_cd[0].end(), temp_intpt);	

			pDoc->img_pt_num++;//记录总的标记点数		
			pDoc->temp_img_pt_num++;//记录实际的标记点数
			if(pDoc->img_pt_num == pDoc->mdl_pt_num)//标记点数已经足够
			{
				pDoc->img_fp_lc = true;
				pDoc->img_pt_num_list.insert(pDoc->img_pt_num_list.end(),pDoc->temp_img_pt_num);
			}
		}
		else
		{
			pDoc->img_temp_pt[pDoc->img_pt_num].x = point.x - originx;
			pDoc->img_temp_pt[pDoc->img_pt_num].y = biHeight - (point.y - originy) - 1;//?			

			//初始化
			if(pDoc->img_pt_num%4 == 0)
			{
				pDoc->img_ft_pt[pDoc->img_pt_num/4].x = 0;
				pDoc->img_ft_pt[pDoc->img_pt_num/4].y = 0;//?			
			}

			//累加
			pDoc->img_ft_pt[pDoc->img_pt_num/4].x += point.x - originx;
			pDoc->img_ft_pt[pDoc->img_pt_num/4].y += biHeight - (point.y - originy) - 1;//?			

			pDoc->img_pt_num++;	

			//求平均
			if(pDoc->img_pt_num%4 == 0)
			{
				pDoc->img_ft_pt[pDoc->img_pt_num/4-1].x /= 4;
				pDoc->img_ft_pt[pDoc->img_pt_num/4-1].y /= 4;//?				
			}
			
			if(pDoc->img_pt_num == 8)		
			{
				pDoc->img_fp_lc = TRUE;
			}
		}		
		
		InvalidateRect(NULL, FALSE);
		//OnDraw(pDC);//也无非是重绘图像添加标定点罢了
	}
	
	CView::OnLButtonUp(nFlags, point);
	
}


