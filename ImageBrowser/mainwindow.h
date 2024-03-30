#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDropEvent>
#include <QMimeData>
#include <QDragEnterEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void show_image(QString &path);

private slots:
    void on_pushButton_clicked();

    void on_width_editingFinished();

    void on_height_editingFinished();

private:
    uint32_t width;
    uint32_t height;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
