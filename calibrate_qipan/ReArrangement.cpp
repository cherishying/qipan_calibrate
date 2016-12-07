#include <header.h>

int ReArrangement(Rect *r, int num)
{
    // 20*20 to avoid out of range, but i only need 16*18
    Rect rect[20][20];
    Rect buffer;
    // 将303个数据按照y先排序，即将每一行的数据聚在一块
    for (int i = 0; i < num; i++)
        for(int j = 1; j <= num; j++)
        {
            if ((r[i].y < r[j].y))
            {
                buffer = r[i];
                r[i] = r[j];
                r[j] = buffer;
            }
        }
    // 将数据按网格状分布存进rect里面
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < 18; j++)
        {
            for(int k = 1; k < 18; k++)
            {
                if(r[i*18 + j] > r[i*16 + k])
                {
                    buffer = r[i*18 + j];
                    r[i*18 + j] = r[i*16 + k];
                    r[i*18 + k] = buffer;
                }
            }
            rect[i][j] = r[i*18 + j];
        }
    }
}
