/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2023-04-02 23:07:58
 * @LastEditors: Clark
 * @LastEditTime: 2023-12-25 14:47:52
 * @Description: file content
 */
#include "jh_util.h"
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

jh_clock_t g_clock = {
    .start = 0,
    .end = 0,
};

void jh_clock_start(jh_clock_t *clock)
{
    struct timeval tv;

    assert (clock != NULL);

    gettimeofday(&tv, NULL);
    clock->start = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return;
}

void jh_clock_end(jh_clock_t *clock)
{
    struct timeval tv;

    assert (clock != NULL);

    gettimeofday(&tv, NULL);
    clock->end = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return;
}

uint32_t jh_clock_diff(jh_clock_t *clock)
{
    assert (clock != NULL);

    return clock->end - clock->start;
}

/**
 * @brief raw图像逆时针旋转90度
 * @param imgU8
 * @param imgU8_t
 * @param width
 * @param height
 */
void anticlockwise_rotate_raw_image(unsigned char *imgU8, unsigned char *imgU8_t, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imgU8_t[height * j + height - i - 1] = imgU8[(width*height-1)-(width * i + j)];  // 图像逆时针旋转90度
        }
    }
}

/**
 * @brief raw图像图像顺时针旋转90度
 * @param imgU8     原图像
 * @param imgU8_t   旋转后图像
 * @param width     原始宽
 * @param height    原始高
 */
void clockwise_rotate_raw_image(unsigned char *imgU8, unsigned char *imgU8_t, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imgU8_t[height * j + height - i - 1] = imgU8[width * i + j];
        }
    }
}

/**
 * @brief gray图上下翻转
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void horizontal_flip(unsigned char *imgU8,int width, int height)
{
    int index = 0;
    uint8_t tmp = 0;
    for (int i = 0; i < height; i++)
    {
        index = width - 1;
        for (int j = 0; j < index; j++)
        {
            tmp = imgU8[width*i+j];
            imgU8[width*i+j] = imgU8[width*i+index];
            imgU8[width*i+index] = tmp;
            index--;
        }
    }
}

/**
 * @brief gray图左右翻转
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void vertical_flip(unsigned char* img,int width, int height)
{
    int i, j;
    unsigned char tmp;
    for (i = 0; i < height / 2; i++) {
        for (j = 0; j < width; j++) {
            tmp = img[i * width + j];
            img[i * width + j] = img[(height - i - 1) * width + j];
            img[(height - i - 1) * width + j] = tmp;
        }
    }
}

/**
 * @brief gray图旋转180度
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void rotate_raw_image_180_degrees(unsigned char* img, uint32_t width, uint32_t height)
{
    unsigned char tmp;
    uint32_t imageSize = height*width;

    for (int i = 0; i < imageSize/2; i++)
    {
        tmp = img[imageSize - 1 - i];
        img[imageSize - 1 - i] = img[i];
        img[i] = tmp;
    }
}

/**
 * @brief RAW10转Gray8
 * @param _dst  原数据
 * @param _src  目的
 * @param xres  宽
 * @param yres  高
 */
void raw10_to_gray8(void *_dst, void *_src, int xres, int yres)
{
#if 0
    int len = xres * yres;
    unsigned short *src = (unsigned short *) _src;
    unsigned char *dst = (unsigned char *) _dst;

    for(int i = 0; i < len; i++)
    {
        unsigned char tmp = src[i] & 0x0003;
        dst[i] = ((src[i] >> 8) & 0x00ff) + tmp;
        dst[i] = (dst[i] <= 255) ? dst[i] : 255;
    }
#else
    int len = xres * yres;
    unsigned char *src = _src + 1;
    unsigned char *dst = _dst;

    int i = len * 2 / 32;
    while (i--) {
        unsigned char s0 = src[0*2];
        unsigned char s1 = src[1*2];
        unsigned char s2 = src[2*2];
        unsigned char s3 = src[3*2];
        unsigned char s4 = src[4*2];
        unsigned char s5 = src[5*2];
        unsigned char s6 = src[6*2];
        unsigned char s7 = src[7*2];
        unsigned char s8 = src[8*2];
        unsigned char s9 = src[9*2];
        unsigned char s10 = src[10*2];
        unsigned char s11 = src[11*2];
        unsigned char s12 = src[12*2];
        unsigned char s13 = src[13*2];
        unsigned char s14 = src[14*2];
        unsigned char s15 = src[15*2];

        dst[0] = s0;
        dst[1] = s1;
        dst[2] = s2;
        dst[3] = s3;
        dst[4] = s4;
        dst[5] = s5;
        dst[6] = s6;
        dst[7] = s7;
        dst[8] = s8;
        dst[9] = s9;
        dst[10] = s10;
        dst[11] = s11;
        dst[12] = s12;
        dst[13] = s13;
        dst[14] = s14;
        dst[15] = s15;

        src += 32;
        dst += 16;
    }

    memset(dst, 0x80, len / 2);
#endif
}

/**
 * @brief raw图像均值滤波，滤除噪点
 * @param originImage 原始图像
 * @param dstnImage   滤波后图像
 * @param height      原始高
 * @param width       原始宽
 * @param filter_size 滤波器大小，默认3
 */
void raw_image_average_filter(uint8_t *originImage, uint8_t *dstnImage, int height, int width, int filter_size)
{
    int i, j, k, l;
    int half_size = filter_size / 2;

    // 对每个像素进行滤波操作
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            int sum = 0;
            int count = 0;

            // 对滤波器中的每个像素进行计算
            for (k = -half_size; k <= half_size; k++)
            {
                for (l = -half_size; l <= half_size; l++)
                {
                    int x = i + k;
                    int y = j + l;
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    { // 检查像素是否越界
                        sum += originImage[x * width + y];
                        count++;
                    }
                }
            }

            // 计算平均值并存储到临时数组中
            dstnImage[i * width + j] = sum / count;
        }
    }
}

/**
 * @brief raw图像中值滤波，去掉噪点
 * @param img       图像
 * @param width     宽
 * @param height    高
 */
void raw_image_median_filter(unsigned char* img, int width, int height)
{
    int i, j, k, l, n;
    unsigned char window[9];
    for (i = 1; i < height - 1; i++) {
        for (j = 1; j < width - 1; j++) {
            n = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    window[n++] = img[(i + k) * width + j + l];
                }
            }

            // 计算中值
            for (k = 0; k < n - 1; k++) {
                for (l = k + 1; l < n; l++) {
                    if (window[k] > window[l]) {
                        unsigned char temp = window[k];
                        window[k] = window[l];
                        window[l] = temp;
                    }
                }
            }
            img[i * width + j] = window[n / 2];
        }
    }
}

/**
 * @brief 两帧叠加取均值去图像噪点
 * @param nv12      nv12数据
 * @param tmpSpace  数据处理需使用的临时空间
 * @return true
 * @return false    缓存第一帧时返回失败
 */
bool nv12_image_two_frame_mean_filter(uint8_t *nv12, uint8_t *tmpSpace)
{
    uint32_t width = get_rgb_width();
    uint32_t height = get_rgb_height();
    uint32_t len = width*height*3/2;

    static uint8_t *buf = NULL;
    if (!buf)
    {
        buf = malloc(len);
        memcpy(buf, nv12, len);
        log_debug("JH --- nv12_image_two_frame_mean_filter first frame\n");
        return false;
    }

    for (int i = 0; i < len; i++)
    {
            tmpSpace[i] = (buf[i] + nv12[i])/2;
    }
    memcpy(buf, nv12, len);
    memcpy(nv12, tmpSpace, len);

    return true;
}

/**
 * @brief NV12数据顺时针旋转90度
 * @param src       源数据
 * @param dst       旋转后的数据
 * @param width     原始宽
 * @param height    原始高
 */
void clockwise_rotate_NV12(unsigned char* src, unsigned char* dst, int width, int height)
{
    int i, j;
    // 顺时针旋转90°后，宽高交换
    int dstWidth = height;
    int dstHeight = width;

    // 旋转Y分量
    for (i = 0; i < dstHeight; i++) {
        for (j = 0; j < dstWidth; j++) {
            // 计算旋转后的坐标
            int dstIndex = i * dstWidth + j;
            int srcIndex = (height - j - 1) * width + i;
            dst[dstIndex] = src[srcIndex];
        }
    }

    // 旋转UV分量
    int srcUVIndex = width * height; // UV分量在原数组中的起始位置
    int dstUVIndex = dstWidth * dstHeight; // UV分量在目标数组中的起始位置
    for (i = 0; i < dstHeight / 2; i++) {
        for (j = 0; j < dstWidth / 2; j++) {
            // 计算旋转后的坐标
            int dstIndex = dstUVIndex + i * dstWidth + j * 2;
            int srcIndex = srcUVIndex + (height / 2 - j - 1) * width + i * 2;
            dst[dstIndex] = src[srcIndex];
            dst[dstIndex + 1] = src[srcIndex + 1];
        }
    }
}

/**
 * @brief NV12数据逆时针旋转90度
 * @param src       源数据
 * @param dst       旋转后的数据
 * @param width     原始宽
 * @param height    原始高
 */
void anticlockwise_rotate_NV12(unsigned char * src, unsigned char * dst, int srcW, int srcH)
{
    int wide = srcW;
    int high = srcH;

    unsigned char * srcUV = src + wide  * high;
    unsigned char * destUV = dst + wide  * high;

    int i, j;
    for (int i = 0; i < high; i++)
    {
        for (int j = 0; j < wide; j++)
        {
            dst[(wide - j - 1) * high + i] = src[i * wide + j];
            destUV[(((wide - j - 1) / 2) * (high / 2) + (i/ 2)) * 2 + 0] = srcUV[((i / 2) * (wide / 2) + (j / 2)) * 2 + 0];
            destUV[(((wide - j - 1) / 2) * (high / 2) + (i/ 2)) * 2 + 1] = srcUV[((i / 2) * (wide / 2) + (j / 2)) * 2 + 1];
        }
    }
}

/**
 * @brief NV12图像旋转180度
 * @param src       原图像
 * @param dst       旋转之后的图像
 * @param width     原宽
 * @param height    原高
 */
void nv12_rotate_180_degree(uint8_t *src,uint32_t width, uint32_t height)
{
    uint8_t tmp = 0;
    uint8_t *rawUV = src + width*height;
    rotate_raw_image_180_degrees(src, width, height);

    int size = height*width/4;
    for (int i=0; i<size/2; i++)
    {
        tmp = rawUV[2*i];
        rawUV[2*i] = rawUV[2*size - 2*i - 2];
        rawUV[2*size - 2*i - 2] = tmp;

        tmp = rawUV[2*i+1];
        rawUV[2*i+1] = rawUV[2*size - 2*i - 1];
        rawUV[2*size - 2*i - 1] = tmp;
    }
}

/**
 * @brief 裁剪yuv数据
 * @param tarYuv    裁剪后的数据
 * @param srcYuv    源数据
 * @param startW    裁剪宽起点
 * @param startH    裁剪高起点
 * @param cutW      裁剪宽度
 * @param cutH      裁剪高度
 * @param srcW      原始宽
 * @param srcH      原始高
 */
void cut_yuv(unsigned char *tarYuv, unsigned char *srcYuv, int startW, int startH, int cutW, int cutH, int srcW, int srcH)
{
    int i;
    int j = 0;
    int k = 0;

    //分配一段内存，用于存储裁剪后的Y分量
    unsigned char *tmpY = (unsigned char *)malloc(cutW*cutH);
    //分配一段内存，用于存储裁剪后的UV分量
    unsigned char *tmpUV = (unsigned char *)malloc(cutW*cutH/2);

    for (i=startH; i<cutH+startH; i++)
    {
        // 逐行拷贝Y分量，共拷贝cutW*cutH
        memcpy(tmpY+j*cutW, srcYuv+startW+i*srcW, cutW);
        j++;
    }

    for (i=startH/2; i<(cutH+startH)/2; i++)
    {
        //逐行拷贝UV分量，共拷贝cutW*cutH/2
        memcpy(tmpUV+k*cutW, srcYuv+startW+srcW*srcH+i*srcW, cutW);
        k++;
    }

    //将拷贝好的Y，UV分量拷贝到目标内存中
    memcpy(tarYuv, tmpY, cutW*cutH);
    memcpy(tarYuv+cutW*cutH, tmpUV, cutW*cutH/2);
    free(tmpY);
    free(tmpUV);
}

/**
 * @brief raw图转至rgb格式
 * @param gray            原图数据
 * @param grayLen         原图大小
 * @param rgb             rgb图像数据
 * @param rgbLen          rgb图像大小
 * @return true             成功
 */
bool gray_to_rgb(uint8_t *gray, uint32_t grayLen, uint8_t *rgb, uint32_t *rgbLen)
{
    for (size_t i = 0; i < grayLen; i++)
    {
        rgb[i*3 + 0] = gray[i];
        rgb[i*3 + 1] = gray[i];
        rgb[i*3 + 2] = gray[i];
    }
    *rgbLen = 3*grayLen;
    return true;
}

// typedef struct picintopic_info{
//     /* data */
//     unsigned char *around_pic;
//     unsigned char *inner_pic;
//     int around_w;
//     int around_h;
//     int inner_w;
//     int inner_h;
//     int inner_x;/*画中画坐标*/
//     int inner_y;
// }picIntopic_Info;

/**
 * @brief 画中画嵌入算法
 * @param pic_info
 */
void putPicIntoPic(struct picintopic_info *pic_info)
{
    int cnt = 0;
    unsigned char *inner = pic_info->inner_pic;
    for(int i = pic_info->inner_y; i < (pic_info->inner_y + pic_info->inner_h); i++)
    {
        int start_x = i * pic_info->around_w + pic_info->inner_x;
        unsigned char *around_pic = pic_info->around_pic + start_x;
        for(int j = 0; j < pic_info->inner_w; j++)
        {
            around_pic[j] = inner[cnt];
            cnt++;
        }
    }

    /*uv值*/
    cnt = 0;
    inner = pic_info->inner_pic + pic_info->inner_h * pic_info->inner_w;
    for(int i = pic_info->inner_y / 2; i < (pic_info->inner_y + pic_info->inner_h) / 2; i++)
    {
        int start_x = i * pic_info->around_w + pic_info->inner_x;
        unsigned char *around_pic = pic_info->around_pic + pic_info->around_h * pic_info->around_w + start_x;
        for(int j = 0; j < pic_info->inner_w; j++)
        {
            around_pic[j] = inner[cnt];
            cnt++;
        }
    }
}

/**
 * @brief NV12图像截取一片区域
 *
 * @param nv12
 * @param in_w
 * @param in_h
 * @param in_x
 * @param in_y
 * @param out_nv12
 * @param out_w
 * @param out_h
 * @return true
 * @return false
 */
bool nv12_copy_area(uint8_t* nv12, int in_w, int in_h, int in_x, int in_y, uint8_t* out_nv12, int out_w, int out_h)
{
	in_x = in_x & (~1);
	in_y = in_y & (~1);

	int size = out_w * out_h;
	memset(out_nv12, 0x7f, size * 3 / 2);

	int offset_in = in_y * in_w;
	uint8_t* line = &nv12[offset_in + in_x];
	uint8_t* line_out = out_nv12;

	int cpy_h = in_h - in_y - 1;
	if (cpy_h <= 1) {
		return true;
	}
	cpy_h = cpy_h < out_h ? cpy_h : out_h;
	int cpy_w = in_w - in_x - 1;
	if (cpy_w <= 1) {
		return true;
	}
	cpy_w = cpy_w < out_w ? cpy_w : out_w;

	for (int j = 0; j < cpy_h; j++) {
		memcpy(line_out, line, cpy_w);
		line += in_w;
		line_out += out_w;
	}

	line = &nv12[in_w * in_h + offset_in / 2 + in_x];
	line_out = &out_nv12[size];
	int cpy_uv_h = cpy_h / 2;

	for (int j = 0; j < cpy_uv_h; j++) {
		memcpy(line_out, line, cpy_w);
		line += in_w;
		line_out += out_w;
	}

	return true;
}
