#include <header.h>
void cvText(IplImage* img, const char* text, int x, int y)
{
    CvFont font;

    double hscale = 0.5;
    double vscale = 0.5;
    int linewidth = 0.5;
    cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX |CV_FONT_ITALIC,hscale,vscale,0,linewidth);

    CvScalar textColor =cvScalar(0,255,255);
    CvPoint textPos =cvPoint(x, y);

    cvPutText(img, text, textPos, &font,textColor);
}
