// 1.0.cpp : 定义控制台应用程序的入口点。
//整合可以用版本
#include "stdafx.h"
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include <cv.h>
#include "opencv2/core/core.hpp"
void InitCorners3D(CvMat *Corners3D, CvSize ChessBoardSize, int NImages, float SquareSize);
//下面这段可以通过添加头文件来解决
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
	//	printf("无法捕获摄像头设备！\n\n");
	//	return 0;
	//}
	//else
	//{
	//	printf("捕获摄像头设备成功！！\n\n");
	//}

	//IplImage* frame = NULL;

	//cvNamedWindow("摄像机帧截取窗口", 1);

	//printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");

	//int number_image = 1;
	//char *str1;
	//str1 = ".jpg";
	//char filename[20] = "";

	//while (true)
	//{
	//	frame = cvQueryFrame(capture);
	//	if (!frame)
	//		break;
	//	cvShowImage("摄像机帧截取窗口", frame);

	//	if (cvWaitKey(10) == 'c')
	//	{
	//		sprintf_s(filename, "%d.jpg", number_image);
	//		cvSaveImage(filename, frame);
	//		cout << "成功获取当前帧，并以文件名" << filename << "保存...\n\n";
	//		printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");
	//		number_image++;
	//	}
	//	else if (cvWaitKey(10) == 'q')
	//	{
	//		printf("截取图像帧过程完成...\n\n");
	//		cout << "共成功截取" << --number_image << "帧图像！！\n\n";
	//		break;
	//	}
	//}

	//cvReleaseImage(&frame);
	//cvDestroyWindow("摄像机帧截取窗口");

	char filename[17];
	int number_image = 17;
	int cube_length = 10; //猜测：随便赋值，对结果影响不大，应该说的就是棋盘格子边长

//*************************第二部分*********************************//
	IplImage * show;
	cvNamedWindow("RePlay", 1);

	int a = 1;
	int number_image_copy = number_image;

	CvSize board_size = cvSize(9, 6);//cvsize
	//CvSize board_size = cvSize(6, 4);
	int board_width = board_size.width;//
	int board_height = board_size.height;//
	int total_per_image = board_width*board_height; //总点数
	//new是什么？？？？？？？？？？
	CvPoint2D32f * image_points_buf = new CvPoint2D32f[total_per_image]; //识别到的各个角点坐标
	CvMat * image_points = cvCreateMat(number_image*total_per_image, 2, CV_32FC1); //所有图片加起来的所有点数为行数
	CvMat * object_points = cvCreateMat(number_image*total_per_image, 3, CV_32FC1);//角点的世界坐标系，三维
	CvMat * point_counts = cvCreateMat(number_image, 1, CV_32SC1);
	CvMat * intrinsic_matrix = cvCreateMat(3, 3, CV_32FC1); //内参数矩阵
	CvMat * distortion_coeffs = cvCreateMat(5, 1, CV_32FC1); //畸变系数
	//后来加上的
	/*IplImage * eig_image = cvCreateImage(cvGetSize(show), IPL_DEPTH_32F, 1);
	IplImage * temp_image = cvCloneImage(eig_image);
	CvPoint2D32f* corners = new CvPoint2D32f[1000];
	int corner_count = 1000;
	double quality = 0.1;*/



	int count;
	int found;
	int step;
	int successes = 0;//成功标定的图片数量

	while (a <= number_image_copy)
	{
		
		//读取图片
		sprintf(filename, "E:\\catlab\\150度镜头1号\\%d.jpg", a);
		show = cvLoadImage(filename, -1);
		//寻找棋盘图的内角点位置
		found = cvFindChessboardCorners(show, board_size, image_points_buf, &count,
 			CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
		//所有角点都没法找到的情况
		if (found == 0)
		{
			cout << "第" << a << "帧图片无法找到棋盘格所有角点!\n\n";
			cvNamedWindow("RePlay", 1);
			cvShowImage("RePlay", show);
			cvWaitKey(0);

		}
		else
		{
			cout << "第" << a << "帧图像成功获得" << count << "个角点...\n";

			cvNamedWindow("RePlay", 1);

			IplImage * gray_image = cvCreateImage(cvGetSize(show), 8, 1);
			cvCvtColor(show, gray_image, CV_BGR2GRAY);
			cout << "获取源图像灰度图过程完成...\n";
			
			//本来是可以用cvGoodFeaturesToTrack来提供cvFindCornerSubPix这个函数的参数的，但是前文已经提供了cvFindChessboardCorners，所以一样的道理
			//cvGoodFeaturesToTrack(gray_image, eig_image, temp_image, corners, &corner_count, quality, 5);
			//检测到的角点坐标可能放在了image_points_buf中
			cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1),
				cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));//最后那个构造函数是关于迭代的
			//cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1),
				//cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			


			cout << "灰度图亚像素化过程完成...\n";
			
			cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
			//cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
			
			cout << "在源图像上绘制角点过程完成...\n\n";
			cvShowImage("RePlay", show);

			cvWaitKey(0);
		}
        
		//****不明白这一部分是干嘛的****//
		//count角点个数存储在count中 ，total_per_image是用户事先输入的角点数
		//success就是标定成功的图片数量
		InitCorners3D(object_points, board_size, number_image, cube_length);
		
		if (total_per_image == count)
		{
		 	
			step = successes*total_per_image;  
			for (int i = step, j = 0; j<total_per_image; ++i, ++j)
			{
				//opencv中用来访问矩阵每个元素的宏，这个宏只对单通道矩阵有效，多通道会报错
				
				CV_MAT_ELEM(*image_points, float, i, 0) = image_points_buf[j].x;
				CV_MAT_ELEM(*image_points, float, i, 1) = image_points_buf[j].y;//将识别到的坐标塞入image_points这个矩阵中,且数据放在靠近矩阵后面
				//CV_MAT_ELEM(*object_points, float, i, 0) = (float)(j /cube_length); // cube 可能说的就是标定纸上方格的边长
				//CV_MAT_ELEM(*object_points, float, i, 1) = (float)(j %cube_length);
				//CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
			}
			CV_MAT_ELEM(*point_counts, int, successes, 0) = total_per_image; //放置总点数的矩阵
			successes++;
		}
		a++;
	}
	//****************************************//
	
	cvReleaseImage(&show);
	cvDestroyWindow("RePlay");


	cout << "*********************************************\n";
	cout << number_image << "帧图片中，标定成功的图片为" << successes << "帧...\n";
	cout << number_image << "帧图片中，标定失败的图片为" << number_image - successes << "帧...\n\n";
	cout << "*********************************************\n\n";

	cout << "按任意键开始计算摄像机内参数...\n\n";
	
	//*************下面这一段是为了获取图片并矫正***********************//
	//CvCapture* capture1;
	//capture1 = cvCreateCameraCapture(0);
	//IplImage * show_colie;
	//show_colie = cvQueryFrame(capture1);
    
	//***替换为直接读取一张图片******//
	IplImage *show_colie;
	IplImage *img;
	//img = cvLoadImage("E:\\catlab\\catlab_black\\1.jpg");	
	show_colie = cvLoadImage("E:\\catlab\\150度镜头1号\\15.jpg");
	img = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_8U, 3);

	//**将上面标定成功的点复制下来*******//
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

	//标定计算函数
	cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie),
		intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);//摄像头并不高端时不要设置CV_CALIB_FIX_K3，否则误差很大
	//cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie), 
		//intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);
	

	//******************************//
	std::cout << "摄像机内参数矩阵为：\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 1)
		<< "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 0, 2)
		<< "\n\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1)
		<< "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 1, 2)
		<< "\n\n";
	std::cout << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 0) << "    " << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 1)
		<< "          " << CV_MAT_ELEM(*intrinsic_matrix, float, 2, 2)
		<< "\n\n";

	std::cout << "畸变系数矩阵为：\n";
	std::cout << CV_MAT_ELEM(*distortion_coeffs, float, 0, 0) << "    " << CV_MAT_ELEM(*distortion_coeffs, float, 1, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 2, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 3, 0)
		<< "    " << CV_MAT_ELEM(*distortion_coeffs, float, 4, 0)
		<< "\n\n";

	cvSave("Intrinsics.xml", intrinsic_matrix);
	cvSave("Distortion.xml", distortion_coeffs);

	std::cout << "摄像机矩阵、畸变系数向量已经分别存储在名为Intrinsics.xml、Distortion.xml文档中\n\n";
	//**********************************************//
	
	//******************校正图片************************//
	//加载xml文件
	CvMat * intrinsic = (CvMat *)cvLoad("Intrinsics.xml");
	CvMat * distortion = (CvMat *)cvLoad("Distortion.xml");

	IplImage * mapx = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_32F, 1);
	IplImage * mapy = cvCreateImage(cvGetSize(show_colie), IPL_DEPTH_32F, 1);

	//计算形变与非形变图像的对应
	cvInitUndistortMap(intrinsic, distortion, mapx, mapy); //mapx,mapy 

	cvNamedWindow("原始图像", 1);
	cvNamedWindow("非畸变图像", 1);

	cout << "按‘E’键退出显示...\n\n";

	//while (show_colie)
	//{
		IplImage * clone = cvCloneImage(show_colie);
		cvShowImage("原始图像", show_colie);
		cvRemap(clone, img, mapx, mapy);  //对图像进行校正
		cvReleaseImage(&clone);
		cvShowImage("非畸变图像", img);

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
	int CurrentImage = 0; //标定图片变量
	int CurrentRow = 0; //棋盘角点行变量
	int CurrentColumn = 0;//棋盘角点列变量
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