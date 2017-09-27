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
		else pBitmap = pDoc->temp_view;//�ڼ�����ӳ������������ͼָ�붼�Ѵ����������Ա�����ʾ����ͼ
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
		//����OnMap��ģ��
		if(pDoc->mapped)
		{
			for(i = 0; i < biWidth; i++)
			{
				for(j = 0; j < biHeight; j++)
				{
					if(pDoc->pxl_map[pDoc->iter_index-1][j*biWidth+i] != -1)//���ڶ�Ӧ���񶥵�
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
			double cur_gray;//��ǰ�������صĻҶ�ֵ
			unsigned long u, v;
			char vertex_gray;
			for(v = 0; v < pDoc->biHeight[pDoc->iter_index-1]; v++)//������
			{
				for(u = 0; u < pDoc->biWidth[pDoc->iter_index-1]; u++)//������
				{
					cur_gray = pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u+2] * 0.3 + pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u+1] * 0.59 + pDoc->view_data[pDoc->iter_index-1][v*pDoc->biAlign[pDoc->iter_index-1]+3*u] * 0.11;										
						
					if(pDoc->pxl_map[pDoc->iter_index-1][v*pDoc->biWidth[pDoc->iter_index-1]+u] != -1)//��һ��ӵ�ж�Ӧ���񶥵�����ع�������Jacaobian Matrix�е���ӦԪ��
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
				//�ڼ����ʵ��ӳ���֮����ʾ�궨����ʵ�ʶ�Ӧ�㣬�Թ۲������
				
				//��ʾtemp_img_pts��ָ��������е�ͼ���ǵ�
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
					//����ɫ����ʾ
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
		
		//��Ʒ������ʾ
		if(i != 0 && pDoc->mesh_projed && pDoc->textured)//��ȷ�Ѿ�������ʵ��ӳ��㣬ͬʱҲ��ζ��һ����������������
		{
			//���һ��������(unfold)��ͼ����
			//�����Ѿ�ӳ����㣬����û�н�ָ���������
			if(pDoc->num_view > 0)
			{
				if(pDoc->mapped)
				{
					unsigned long tri_coor[3][2];			
			
					//����ÿ��������
					for(i = 0; i < pDoc->L1; i++)
					{
						//�����������ϵ������������������
						for(j = 0; j < 3; j++)
						{
							for(k = 0; k < 2; k++)
							{
								tri_coor[j][k] = pDoc->vtx_map[pDoc->iter_index-1][pDoc->triangles1[i][j]][k];//��¼�����θ������ڵ�ǰ��ͼ�ж�Ӧ����������														
							}
						}
						if(tri_coor[0][0] != -1 && tri_coor[1][0] != -1 && tri_coor[2][0] != -1)//�����ε����ж��㶼��ȷ���˶�Ӧ������
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
			
					//����ÿ��������
					for(i = 0; i < pDoc->L1; i++)
					{
						//�����������ϵ������������������
						for(j = 0; j < 3; j++)
						{
							for(k = 0; k < 2; k++)
							{
								tri_txts[j][k] = pDoc->temp_txt[pDoc->triangles1[i][j]][k];//������������붥��������ϸ��Ӧ��						
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

			//��ʾtemp_img_pts��ָ��������е�ͼ���ǵ�
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

			//����ɫ����ʾ
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
				cur = (biHeight-1-j)*biWidth*3+i*3;//ע���ͼ����ϵ��y�����������������µ�
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
			//���ǹ���ʾʱʹ�õ�ͼ�������е�λ�ã�Ҳ��Ϊ�����ӳ�����ģ�y�������������ϵ�
			int *temp_intpt = new int[2];

			temp_intpt[0] = point.x - originx;
			temp_intpt[1] = biHeight - (point.y - originy) - 1;

			pDoc->temp_img_pts[0].insert(pDoc->temp_img_pts[0].end(), temp_intpt);	
			
			//���ǹ�λ�˵���ʱʹ�õ�ͼ������ϵ������ֵ��x�������������ϵ�
			temp_intpt = new int[2];

			temp_intpt[0] = biHeight - (point.y - originy) - 1;
			temp_intpt[1] = point.x - originx;

			pDoc->temp_img_pt_cd[0].insert(pDoc->temp_img_pt_cd[0].end(), temp_intpt);	

			pDoc->img_pt_num++;//��¼�ܵı�ǵ���		
			pDoc->temp_img_pt_num++;//��¼ʵ�ʵı�ǵ���
			if(pDoc->img_pt_num == pDoc->mdl_pt_num)//��ǵ����Ѿ��㹻
			{
				pDoc->img_fp_lc = true;
				pDoc->img_pt_num_list.insert(pDoc->img_pt_num_list.end(),pDoc->temp_img_pt_num);
			}
		}
		else
		{
			pDoc->img_temp_pt[pDoc->img_pt_num].x = point.x - originx;
			pDoc->img_temp_pt[pDoc->img_pt_num].y = biHeight - (point.y - originy) - 1;//?			

			//��ʼ��
			if(pDoc->img_pt_num%4 == 0)
			{
				pDoc->img_ft_pt[pDoc->img_pt_num/4].x = 0;
				pDoc->img_ft_pt[pDoc->img_pt_num/4].y = 0;//?			
			}

			//�ۼ�
			pDoc->img_ft_pt[pDoc->img_pt_num/4].x += point.x - originx;
			pDoc->img_ft_pt[pDoc->img_pt_num/4].y += biHeight - (point.y - originy) - 1;//?			

			pDoc->img_pt_num++;	

			//��ƽ��
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
		//OnDraw(pDC);//Ҳ�޷����ػ�ͼ����ӱ궨�����
	}
	
	CView::OnLButtonUp(nFlags, point);
	
}


