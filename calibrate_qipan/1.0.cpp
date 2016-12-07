// 1.0.cpp : �������̨Ӧ�ó������ڵ㡣
//���Ͽ����ð汾
#include "stdafx.h"
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include <cv.h>
#include "opencv2/core/core.hpp"
void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize);
//������ο���ͨ�����ͷ�ļ������
//#pragma comment(lib, "ml.lib")
//#pragma comment(lib, "cv.lib")
//#pragma comment(lib, "cvaux.lib")
//#pragma comment(lib, "cvcam.lib")
//#pragma comment(lib, "cxcore.lib")
//#pragma comment(lib, "cxts.lib")
//#pragma comment(lib, "highgui.lib")
//#pragma comment(lib, "cvhaartraining.lib")

using namespace std;

int main()
{
	

	//CvCapture* capture;

	//capture = cvCreateCameraCapture(0);

	//if (capture == 0)
	//{
	//	printf("�޷���������ͷ�豸��\n\n");
	//	return 0;
	//}
	//else
	//{
	//	printf("��������ͷ�豸�ɹ�����\n\n");
	//}

	//IplImage* frame = NULL;

	//cvNamedWindow("�����֡��ȡ����", 1);

	//printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");

	//int number_image = 1;
	//char *str1;
	//str1 = ".jpg";
	//char filename[20] = "";

	//while (true)
	//{
	//	frame = cvQueryFrame(capture);
	//	if (!frame)
	//		break;
	//	cvShowImage("�����֡��ȡ����", frame);

	//	if (cvWaitKey(10) == 'c')
	//	{
	//		sprintf_s(filename, "%d.jpg", number_image);
	//		cvSaveImage(filename, frame);
	//		cout << "�ɹ���ȡ��ǰ֡�������ļ���" << filename << "����...\n\n";
	//		printf("����C������ȡ��ǰ֡������Ϊ�궨ͼƬ...\n����Q�����˳���ȡ֡����...\n\n");
	//		number_image++;
	//	}
	//	else if (cvWaitKey(10) == 'q')
	//	{
	//		printf("��ȡͼ��֡�������...\n\n");
	//		cout << "���ɹ���ȡ" << --number_image << "֡ͼ�񣡣�\n\n";
	//		break;
	//	}
	//}

	//cvReleaseImage(&frame);
	//cvDestroyWindow("�����֡��ȡ����");

	char filename[17];
	int number_image = 17;
	int cube_length = 10; //�²⣺��㸳ֵ���Խ��Ӱ�첻��Ӧ��˵�ľ������̸��ӱ߳�

//*************************�ڶ�����*********************************//
	IplImage * show;
	cvNamedWindow("RePlay", 1);

	int a = 1;
	int number_image_copy = number_image;

	CvSize board_size = cvSize(9, 6);//cvsize
	//CvSize board_size = cvSize(6, 4);
	int board_width = board_size.width;//
	int board_height = board_size.height;//
	int total_per_image = board_width*board_height; //�ܵ���
	//new��ʲô��������������������
	CvPoint2D32f * image_points_buf = new CvPoint2D32f[total_per_image]; //ʶ�𵽵ĸ����ǵ�����
	CvMat * image_points = cvCreateMat(number_image*total_per_image, 2, CV_32FC1); //����ͼƬ�����������е���Ϊ����
	CvMat * object_points = cvCreateMat(number_image*total_per_image, 3, CV_32FC1);//�ǵ����������ϵ����ά
	CvMat * point_counts = cvCreateMat(number_image, 1, CV_32SC1);
	CvMat * intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1); //�ڲ�������
	CvMat * distortion_coeffs = cvCreateMat(5, 1, CV_32FC1); //����ϵ��
	//�������ϵ�
	/*IplImage * eig_image = cvCreateImage(cvGetSize(show), IPL_DEPTH_32F, 1);
	IplImage * temp_image = cvCloneImage(eig_image);
	CvPoint2D32f* corners = new CvPoint2D32f[1000];
	int corner_count = 1000;
	double quality = 0.1;*/



	int count;
	int found;
	int step;
	int successes = 0;//�ɹ��궨��ͼƬ����

	while (a <= number_image_copy)
	{
		
		//��ȡͼƬ
		sprintf(filename, "E:\\catlab\\150�Ⱦ�ͷ1��\\%d.jpg", a);
		show = cvLoadImage(filename, -1);
		//Ѱ������ͼ���ڽǵ�λ��
		found = cvFindChessboardCorners(show, board_size, image_points_buf, &count,
 			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
		//���нǵ㶼û���ҵ������
		if (found == 0)
		{
			cout << "��" << a << "֡ͼƬ�޷��ҵ����̸����нǵ�!\n\n";
			cvNamedWindow("RePlay", 1);
			cvShowImage("RePlay", show);
			cvWaitKey(0);

		}
		else
		{
			cout << "��" << a << "֡ͼ��ɹ����" << count << "���ǵ�...\n";

			cvNamedWindow("RePlay", 1);

			IplImage * gray_image = cvCreateImage(cvGetSize(show), 8, 1);
			cvCvtColor(show, gray_image, CV_BGR2GRAY);
			cout << "��ȡԴͼ��Ҷ�ͼ�������...\n";
			
			//�����ǿ�����cvGoodFeaturesToTrack���ṩcvFindCornerSubPix��������Ĳ����ģ�����ǰ���Ѿ��ṩ��cvFindChessboardCorners������һ���ĵ���
			//cvGoodFeaturesToTrack(gray_image, eig_image, temp_image, corners, &corner_count, quality, 5);
			//��⵽�Ľǵ�������ܷ�����image_points_buf��
			cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1),
				cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));//����Ǹ����캯���ǹ��ڵ�����
			//cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1),
				//cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			


			cout << "�Ҷ�ͼ�����ػ��������...\n";
			
			cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
			//cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
			
			cout << "��Դͼ���ϻ��ƽǵ�������...\n\n";
			cvShowImage("RePlay", show);

			cvWaitKey(0);
		}
        
		//****��������һ�����Ǹ����****//
		//count�ǵ�����洢��count�� ��total_per_image���û���������Ľǵ���
		//success���Ǳ궨�ɹ���ͼƬ����
		InitCorners3D(object_points, board_size, number_image, cube_length);
		
		if (total_per_image == count)
		{
		 	
			step = successes*total_per_image;  
			for (int i = step, j = 0; j<total_per_image; ++i, ++j)
			{
				//opencv���������ʾ���ÿ��Ԫ�صĺ꣬�����ֻ�Ե�ͨ��������Ч����ͨ���ᱨ��
				
				CV_MAT_ELEM(*image_points, float, i, 0) = image_points_buf[j].x;
				CV_MAT_ELEM(*image_points, float, i, 1) = image_points_buf[j].y;//��ʶ�𵽵���������image_points���������,�����ݷ��ڿ����������
				//CV_MAT_ELEM(*object_points, float, i, 0) = (float)(j /cube_length); // cube ����˵�ľ��Ǳ궨ֽ�Ϸ���ı߳�
				//CV_MAT_ELEM(*object_points, float, i, 1) = (float)(j %cube_length);
				//CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
			}
			CV_MAT_ELEM(*point_counts, int, successes, 0) = total_per_image; //�����ܵ����ľ���
			successes++;
		}
		a++;
	}
	//****************************************//
	
	cvReleaseImage(&show);
	cvDestroyWindow("RePlay");


	cout << "*********************************************\n";
	cout << number_image << "֡ͼƬ�У��궨�ɹ���ͼƬΪ" << successes << "֡...\n";
	cout << number_image << "֡ͼƬ�У��궨ʧ�ܵ�ͼƬΪ" << number_image - successes << "֡...\n\n";
	cout << "*********************************************\n\n";

	cout << "���������ʼ����������ڲ���...\n\n";
	
	//*************������һ����Ϊ�˻�ȡͼƬ������***********************//
	//CvCapture* capture1;
	//capture1 = cvCreateCameraCapture(0);
	//IplImage * show_colie;
	//show_colie = cvQueryFrame(capture1);
    
	//***�滻Ϊֱ�Ӷ�ȡһ��ͼƬ******//
	IplImage *show_colie;
	IplImage *img;
	//img = cvLoadImage("E:\\catlab\\catlab_black\\1.jpg");	
	show_colie = cvLoadImage("E:\\catlab\\150�Ⱦ�ͷ1��\\15.jpg");
	img = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_8U, 3);

	//**������궨�ɹ��ĵ㸴������*******//
	CvMat * object_points2 = cvCreateMat(successes*total_per_image, 3, CV_32FC1);
	CvMat * image_points2 = cvCreateMat(successes*total_per_image, 2, CV_32FC1);
	CvMat * point_counts2 = cvCreateMat(successes, 1, CV_32SC1);

 	for (int i = 0; i<successes*total_per_image; ++i)
	{
		CV_MAT_ELEM(*image_points2, float, i, 0) = CV_MAT_ELEM(*image_points, float, i, 0);
		CV_MAT_ELEM(*image_points2, float, i, 1) = CV_MAT_ELEM(*image_points, float, i, 1);
		CV_MAT_ELEM(*object_points2, float, i, 0) = CV_MAT_ELEM(*object_points, float, i, 0);
		CV_MAT_ELEM(*object_points2, float, i, 1) = CV_MAT_ELEM(*object_points, float, i, 1);
		CV_MAT_ELEM(*object_points2, float, i, 2) = CV_MAT_ELEM(*object_points, float, i, 2);
	}

	for (int i = 0; i<successes; ++i)
	{ 
		CV_MAT_ELEM(*point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);
	}
	///////////*******************************************/////////////////

	cvReleaseMat(&object_points);
	cvReleaseMat(&image_points); 
	cvReleaseMat(&point_counts);

	CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
	CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;

	//�궨���㺯��
	cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie),
		intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);//����ͷ�����߶�ʱ��Ҫ����CV_CALIB_FIX_K3���������ܴ�
	//cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie), 
		//intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);
	

	//******************************//
	std::cout << "������ڲ�������Ϊ��\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 1)
		<< "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 2)
		<< "\n\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1)
		<< "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 2)
		<< "\n\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 1)
		<< "          " << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 2)
		<< "\n\n";

	std::cout << "����ϵ������Ϊ��\n";
	std::cout << CV_MAT_ELEM(*distortion_coeffs, float, 0, 0) << "    " << CV_MAT_ELEM(*distortion_coeffs, float, 1, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 2, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 3, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 4, 0)
		<< "\n\n";

	cvSave("Intrinsics.xml", intrinsic_matrix);
	cvSave("Distortion.xml", distortion_coeffs);

	std::cout << "��������󡢻���ϵ�������Ѿ��ֱ�洢����ΪIntrinsics.xml��Distortion.xml�ĵ���\n\n";
	//**********************************************//
	
	//******************У��ͼƬ************************//
	//����xml�ļ�
	CvMat * intrinsic = (CvMat *)cvLoad("Intrinsics.xml");
	CvMat * distortion = (CvMat *)cvLoad("Distortion.xml");

	IplImage * mapx = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_32F, 1);
	IplImage * mapy = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_32F, 1);

	//�����α�����α�ͼ��Ķ�Ӧ
	cvInitUndistortMap(intrinsic, distortion, mapx, mapy); //mapx,mapy 

	cvNamedWindow("ԭʼͼ��", 1);
	cvNamedWindow("�ǻ���ͼ��", 1);

	cout << "����E�����˳���ʾ...\n\n";

	//while (show_colie)
	//{
		IplImage * clone = cvCloneImage(show_colie);
		cvShowImage("ԭʼͼ��", show_colie);
		cvRemap(clone, img, mapx, mapy);  //��ͼ�����У��
		cvReleaseImage(&clone);
		cvShowImage("�ǻ���ͼ��", img);

		//if (cvWaitKey(10) == 'e')
		//{
		//	break;
		//}

		//show_colie = cvQueryFrame(capture1);
		//show_colie = img;
	//}
		cvWaitKey(0);
	return 0;
}

void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize)
{
	int CurrentImage = 0; //�궨ͼƬ����
	int CurrentRow = 0; //���̽ǵ��б���
	int CurrentColumn = 0;//���̽ǵ��б���
	int NPoints = ChessBoardSize.height*ChessBoardSize.width; 
	float * temppoints = new float[NImages*NPoints * 3];

	// for now, assuming we're row-scanning  
	for (CurrentImage = 0; CurrentImage < NImages; CurrentImage++)
	{
		for (CurrentRow = 0; CurrentRow < ChessBoardSize.height; CurrentRow++)
		{
			for (CurrentColumn = 0; CurrentColumn < ChessBoardSize.width; CurrentColumn++)
			{
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3] = (float)CurrentRow*SquareSize;
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3 + 1] = (float)CurrentColumn*SquareSize;
				temppoints[(CurrentImage*NPoints * 3) + (CurrentRow*ChessBoardSize.width +
					CurrentColumn) * 3 + 2] = 0.f;
			}
		}
	}
	(*Corners3D) = cvMat(NImages*NPoints, 3, CV_32FC1, temppoints);
}