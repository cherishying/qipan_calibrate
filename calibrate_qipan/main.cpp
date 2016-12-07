#include <header.h>




int main()
{

    Mat img = imread("/home/strong/qtcreator-2.4.1/project/calibrate_qipan/picture/1.jpg");
    if(!img.data)
    {
        return -1;
    }
    //imshow("img", img);
    IplImage image = img;
    // remap
    CvMat * intrinsic = (CvMat *)cvLoad("/home/strong/qtcreator-2.4.1/project/calibrate_qipan/Intrinsics.xml");
    CvMat * distortion = (CvMat *)cvLoad("/home/strong/qtcreator-2.4.1/project/calibrate_qipan/Distortion.xml");

    IplImage * mapx = cvCreateImage(cvGetSize(&image), IPL_DEPTH_32S, 1);
    IplImage * mapy = cvCreateImage(cvGetSize(&image), IPL_DEPTH_32S, 1);

    //remap
    /*cvInitUndistortMap(intrinsic, distortion, mapx, mapy); //mapx,mapy
    cvRemap(&image, &image, mapx, mapy);
    cvShowImage("image", &image);*/

    //undistort
    IplImage *image2 = cvLoadImage("/home/strong/qtcreator-2.4.1/project/calibrate_qipan/picture/1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *image3 = cvCreateImage(cvGetSize(image2), image2->depth, 1);
    cvUndistort2(image2, image3, intrinsic, distortion);


    cvSetImageROI(image3, cvRect(75, 25, 470, 440));


    Mat qipan = image3;
    Mat img1 = qipan.clone();
    IplImage image10 = img1;
    //IplImage *image4 = cvCreateImage(cvGetSize(image3), image3->depth, 1);
    Mat qipan_gray(qipan);
    //cvtColor(qipan, qipan_gray, CV_BGR2GRAY);

    //cvShowImage("undistort", image3);



//    // harris detect
    int thresh = 100;
    Mat dst, dst_norm_scaled, dst_norm_tran;


    dst = Mat::zeros( qipan.size(), CV_32SC1);
    dst_norm_tran = Mat::zeros( qipan.size(), CV_32SC1);
    int blockSize = 3;
    int apertureSize = 3;
    double k = 0.04;
    cornerHarris(qipan_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

//    //one
//    const CvMat dst1 = dst;
//    CvMat dst2 = dst_norm_tran;
//    cvNormalize(&dst1, &dst2 , 0, 255, NORM_MINMAX, NULL);

//    Mat dst_norm(&dst2);

//    convertScaleAbs(dst_norm, dst_norm_scaled );
//    cout << "***************" << dst_norm.rows;

//    for(int j = 0; j < dst_norm_scaled.rows; j++)
//    {for(int i = 0; i < dst_norm_scaled.cols; i++)
//        {
//            if( (int) dst_norm.at<float>(j,i) > thresh )
//            {
//               circle(dst_norm_scaled, Point(i, j), 3, Scalar(0, 0, 255), 1, 8, 0);
//               circle(qipan, Point(i, j), 3, Scalar(255, 0, 0), 1, 8, 0);
//            }
//        }

//    }


    //second
    Mat harrisCorner;
    threshold(dst, harrisCorner, 0.00001, 1, THRESH_BINARY);
//cvCircle();

    IplImage QI = harrisCorner;
    IplImage *dilate = cvCloneImage(&QI);

    cvDilate(&QI, dilate, 0, 2);    
    IplImage *a = cvCreateImage(cvGetSize(image3), image3->depth, 1);
    IplImage *dilate8UC1 = cvCreateImage(cvGetSize(image3), IPL_DEPTH_8U, 1);
    cvConvertScale(dilate, dilate8UC1, 255);

    cvMul(image3, dilate, a, 1);
    cvShowImage("a", a);

   // threshold(a, a, 128, 255, CV_THRESH_BINARY);
//    问题卡在这里，为什么findcontour函数用不了，类型转换怎么搞，现在想法是先让它变回原图，在取阈值，再处理16/11/28

//    IplImage *temp = cvCreateImage(cvGetSize(&QI), QI.depth, 1);
//    cout << dilate->depth;
//    cvCopyImage(dilate, temp);
//    cvScale(temp, temp, CV_32SC1/temp->depth);

    CvMemStorage* mem_storage = cvCreateMemStorage(0);
    CvMemStorage* mem_storage1 = cvCreateMemStorage(0);
    CvSeq  *contour = NULL, *c = NULL;
    CvSeq *contour1 = NULL;
    vector<vector<Point> >contours;
    vector<Vec4i> hierarchy;


//    findContours( a, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    int num = cvFindContours(dilate8UC1, mem_storage, &contour);
    cout << "the number of point in chessboard ："<< num;
   // cvDrawContours(dilate8UC1, contour1, cvScalar(100), cvScalar(100), 1);
//    vector<Rect>boundRect(contours.size());
//    vector<vector<Point> >contours_poly(contours.size());

//    for(int i = 0; i < contours.size(); i++)
//    {
//        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
//        boundRect[i] = boundingRect( Mat(contours_poly[i]));
//    }
    // 16/11/29   看opencv的轮廓那一章，有关序列，尝试将序列转换成vector的数据，然后再用cvboundingRect
    //http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html
    int j = 0;
    double Area[num];
    Rect rect[num];
    for(;j < num; contour = contour->h_next, j++)
    {
        for(int i = 0;i < contour->total; i++)
        {
            CvPoint *q = (CvPoint*)cvGetSeqElem(contour, i);
//            cout<< q->x;
        }
//        cout << "\n\n";
        contour1 = cvApproxPoly(contour, sizeof(CvContour), mem_storage1, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02,0);
        rect[j] = cvBoundingRect(contour1, 0);
        Area[j] = cvContourArea(contour1, CV_WHOLE_SEQ);
//        Area[j] = rect[j].height * rect[j].width;
        cvDrawContours(dilate8UC1, contour1, cvScalar(100), cvScalar(100), 1);
    }


//    cout << "rect" << rect[10].width;


    int Num = 0;
    Rect result[num];
    for (int i= 0; i < num; i++)
    {
        if (Area[i] > 40){
            result[i] = rect[i];
            result[i] = rect[i];
            Num++;
        }
    }

    Mat img2 = dilate8UC1;
    int radius = 3;
    for(int i= 0; i < num; i++)
    {
        circle(img2, Point(result[i].x+result[i].width/2, result[i].y + result[i].height/2), radius, Scalar(255, 0, 0), 1);

    }
    cout << "  Num:   " << Num;
    //16/11/30 发现问题，找到的点中有的轮廓是不连续的（猜测）导致以面积区分角点效果不好，试试能不呢把ROI圈出来，找到一个较好的办法



    //***************test Test ***********************//
//    char cc[303];
//    for(int i = 0; i < 303; i++){
//        sprintf(cc, "%d", i);
//        const char *b = cc;
//        cvText(&image10, b, rect[i].x, rect[i].y);
//        cout << "Area:" << Area[i] << "  "<< "x: " <<rect[i].x << "y:" << rect[i].y << "contour num: "<< i;
//        cout << "\n";
//    }



    namedWindow("contour1", 0);
    cvShowImage("contour1", dilate8UC1);
//    cvNamedWindow("name", 0);
    cvShowImage("name", &image10);
//    imshow("image", img1);
    waitKey(0);
    return 0;
}



