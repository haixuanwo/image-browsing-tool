#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->image->setScaledContents(true);
    this->setAcceptDrops(true);
    setWindowTitle("PictureBrowserV1.0");

    // 创建一个 QIntValidator 对象，设置最小值为 0，最大值为 10000
    QIntValidator *widthValidator = new QIntValidator(0, 10000, ui->width);
    ui->width->setValidator(widthValidator);

    QIntValidator *heightValidator = new QIntValidator(0, 10000, ui->height);
    ui->width->setValidator(heightValidator);

    ui->width->setText("640");
    ui->height->setText("480");

    this->width = 640;
    this->height = 480;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //对拖放事件进行筛选
    if (true)
    {
        e->acceptProposedAction();    //放行，否则不会执行dropEvent()函数
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;

    qDebug()<< urls.size();
    foreach (QUrl u, urls)
    {
        QString path = u.toLocalFile();
        show_image(path);
    }
}

/**
 * @brief NV12ToRGB，颜色有点色差
 * @param nv12
 * @param rgb
 * @param width
 * @param height
 */
void NV12ToRGB(unsigned char *nv12, unsigned char *rgb, int width, int height)
{
    int size = width * height;
    int w, h, u, v, yIndex, uvIndex, rIndex, gIndex, bIndex;
    int y1192, r, g, b, uv448, uv_128;

    for (h = 0; h < height; h++)
    {
        for (w = 0; w < width; w++) {
            yIndex = h * width + w;
            uvIndex = (h / 2) * width + (w & (-2)) + size;
            u = nv12[uvIndex];
            v = nv12[uvIndex + 1];

            // YUV to RGB
            y1192 = 1192 * (nv12[yIndex] - 16);
            uv448 = 448 * (u - 128);
            uv_128 = 128 * (v - 128);
            r = (y1192 + uv448) >> 10;
            g = (y1192 - uv_128 - uv448) >> 10;
            b = (y1192 + uv_128) >> 10;

            // RGB clipping
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            // Save RGB values
            rIndex = yIndex * 3;
            gIndex = rIndex + 1;
            bIndex = gIndex + 1;
            rgb[rIndex] = r;
            rgb[gIndex] = g;
            rgb[bIndex] = b;
        }
    }
}

void MainWindow::show_image(QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray img = file.readAll();

    QImage showImg;
    QFileInfo fileInfo(path);
    if (fileInfo.suffix().contains("jpg", Qt::CaseInsensitive)
            || fileInfo.suffix().contains("png", Qt::CaseInsensitive)
            || fileInfo.suffix().contains("bmp", Qt::CaseInsensitive))
    {
        // jpg、png图片
        QImage image;
        image.loadFromData(img);
        showImg = image;
    }
    else if (fileInfo.suffix().contains("nv12", Qt::CaseInsensitive))
    {
        // nv12图
        QByteArray rgbImg;
        rgbImg.resize(img.size()*2);

        qDebug()<<"rgbImg size:"<<rgbImg.size()<<" nv12 size: "<<img.size() << " width:"<<width<<" height:"<<height;
        NV12ToRGB((uint8_t *)img.data(), (uint8_t *)rgbImg.data(), width, height);

        QImage image((uint8_t *)rgbImg.data(), width, height, QImage::Format_RGB888);
        showImg = image;
    }
    else if (fileInfo.suffix().contains("rgb", Qt::CaseInsensitive))
    {
        QImage image((uint8_t *)img.data(), width, height, QImage::Format_RGB888);
        showImg = image;
    }
    else if(fileInfo.suffix().contains("ico", Qt::CaseInsensitive))
    {
         QIcon icon(path);
         QPixmap pixmap = icon.pixmap(QSize(64, 64)); // 获取 64x64 大小的图标
         // 将 QPixmap 对象转换为 QImage 对象
         showImg = pixmap.toImage();
    }
    else
    {
        // raw图
        QImage image((uint8_t *)img.data(), width, height, QImage::Format_Grayscale8);
        showImg = image;
    }

    QPixmap pixmap = QPixmap::fromImage(showImg);
    ui->image->setPixmap(pixmap);
    ui->image->show();
    ui->pathFile->setText(path);
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "open", "../", "TXT(*.*)");
    if (path.isEmpty())
    {
        qDebug()<<"getOpenFileName fail";
        return;
    }

    show_image(path);
}

void MainWindow::on_width_editingFinished()
{
    bool result = false;
    uint32_t width = ui->width->text().toUInt(&result);
    if (!result)
    {
        return;
    }

    qDebug()<<"on_width_editingFinished width: "<< width << "height: " << height;
    if (width >= 10000)
    {
        return;
    }

    this->width = width;
}

void MainWindow::on_height_editingFinished()
{
    bool result = false;
    uint32_t height = ui->height->text().toUInt(&result);
    if (!result)
    {
        return;
    }

    qDebug()<<"on_height_editingFinished width: "<< width << "height: " << height;
    if (height >= 10000)
    {
        return;
    }

    this->height = height;
}
