/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2023-04-02 23:08:12
 * @LastEditors: Clark
 * @LastEditTime: 2023-12-25 14:47:46
 * @Description: file content */
#ifndef __JH_X2100_UTIL_H__
#define __JH_X2100_UTIL_H__

#include "../jh_ilock/common/common.h"

#define JH_TIME_CLOCK_START()                           jh_clock_start(&g_clock)
#define JH_TIME_CLOCK_END()                             jh_clock_end(&g_clock)
#define JH_TIME_CLOCK_DIFF()                            jh_clock_diff(&g_clock)

typedef struct picintopic_info{
    /* data */
    unsigned char *around_pic;      //外部数据
    unsigned char *inner_pic;       //内部数据
    int around_w;                   //外部宽
    int around_h;                   //外部高
    int inner_w;                    //内部宽
    int inner_h;                    //内部高
    int inner_x;                /*画中画坐标*/
    int inner_y;
}picIntopic_Info;


struct jh_clock_s {
    uint32_t start;
    uint32_t end;
};

typedef struct jh_clock_s jh_clock_t;

extern jh_clock_t g_clock;

void jh_clock_start(jh_clock_t *clock);
void jh_clock_end(jh_clock_t *clock);
uint32_t jh_clock_diff(jh_clock_t *clock);

/**
 * @brief raw图像图像逆时针旋转90度
 * @param imgU8     原图像
 * @param imgU8_t   旋转后图像
 * @param width     原始宽
 * @param height    原始高
 */
void anticlockwise_rotate_raw_image(unsigned char *imgU8, unsigned char *imgU8_t, int width, int height);

/**
 * @brief raw图像图像顺时针旋转90度
 * @param imgU8     原图像
 * @param imgU8_t   旋转后图像
 * @param width     原始宽
 * @param height    原始高
 */
void clockwise_rotate_raw_image(unsigned char *imgU8, unsigned char *imgU8_t, int width, int height);

/**
 * @brief 顺时针旋转90°
 * @param matrix
 * @param width
 * @param height
 */
void rotate_matrix(unsigned char* matrix, int width, int height);


/**
 * @brief gray图上下翻转
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void horizontal_flip(unsigned char *imgU8,int width, int height);


/**
 * @brief gray图左右翻转
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void vertical_flip(unsigned char* img,int width, int height);

/**
 * @brief gray图旋转180度
 * @param imgU8     原图像
 * @param width     原始宽
 * @param height    原始高
 */
void rotate_raw_image_180_degrees(unsigned char* img,  uint32_t width, uint32_t height);

/**
 * @brief RAW10转Gray8
 * @param _dst  原数据
 * @param _src  目的
 * @param xres  宽
 * @param yres  高
 */
void raw10_to_gray8(void *_dst, void *_src, int xres, int yres);

/**
 * @brief raw图像均值滤波，滤除噪点
 * @param originImage 原始图像
 * @param dstnImage   滤波后图像
 * @param height      原始高
 * @param width       原始宽
 * @param filter_size 滤波器大小，默认3
 */
void raw_image_average_filter(uint8_t *originImage, uint8_t *dstnImage, int height, int width, int filter_size);

/**
 * @brief raw图像中值滤波，去掉噪点
 * @param img       图像
 * @param width     宽
 * @param height    高
 */
void raw_image_median_filter(unsigned char* img, int width, int height);

/**
 * @brief 两帧叠加取均值去图像噪点
 * @param nv12      nv12数据
 * @param tmpSpace  数据处理需使用的临时空间
 * @return true
 * @return false    缓存第一帧时返回失败
 */
bool nv12_image_two_frame_mean_filter(uint8_t *nv12, uint8_t *tmpSpace);

/**
 * @brief NV12数据顺时针旋转90度
 * @param src       源数据
 * @param dst       旋转后的数据
 * @param width     原始宽
 * @param height    原始高
 */
void clockwise_rotate_NV12(unsigned char* src, unsigned char* dst, int width, int height);

/**
 * @brief NV12数据逆时针旋转90度
 * @param src       源数据
 * @param dst       旋转后的数据
 * @param width     原始宽
 * @param height    原始高
 */
void anticlockwise_rotate_NV12(unsigned char * src, unsigned char * dst, int srcW, int srcH);

/**
 * @brief NV12图像旋转180度
 * @param src       原图像
 * @param width     原宽
 * @param height    原高
 */
void nv12_rotate_180_degree(uint8_t *src,uint32_t width, uint32_t height);

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
void cut_yuv(unsigned char *tarYuv, unsigned char *srcYuv, int startW, int startH, int cutW, int cutH, int srcW, int srcH);

/**
 * @brief raw图转至rgb格式
 * @param gray            原图数据
 * @param grayLen         原图大小
 * @param rgb             rgb图像数据
 * @param rgbLen          rgb图像大小
 * @return true             成功
 */
bool gray_to_rgb(uint8_t *gray, uint32_t grayLen, uint8_t *rgb, uint32_t *rgbLen);

/**
 * @brief 画中画嵌入算法
 * @param pic_info
 */
void putPicIntoPic(struct picintopic_info *pic_info);

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
bool nv12_copy_area(uint8_t* nv12, int in_w, int in_h, int in_x, int in_y, uint8_t* out_nv12, int out_w, int out_h);

#endif  /* __JH_X2100_UTIL_H__ */
