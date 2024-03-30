<!--
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2024-03-30 14:14:48
 * @LastEditors: Clark
 * @LastEditTime: 2024-03-30 14:17:11
 * @Description: file content
-->



YUV420p: I420、YV12
YUV420sp: NV12、NV21


/**
 * 将YUV420P图像数据转换为NV12格式
 * @param yuv420p YUV420P图像数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param nv12 NV12图像数据，需在外部分配内存
 */
void convertYuv420pToNv12(const unsigned char* yuv420p, int width, int height, unsigned char* nv12)
{
    int size = width * height;

    // Y分量复制到NV12图像数据
    memcpy(nv12, yuv420p, size);

    // 转换U、V分量并复制到NV12图像数据
    for (int i = 0; i < size / 4; i++)
    {
        nv12[size + i * 2] = yuv420p[size + i];
        nv12[size + i * 2 + 1] = yuv420p[size + i + size / 4];
    }
}

