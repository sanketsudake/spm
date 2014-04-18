#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include "../include/gui.hpp"

using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        void openVideo(char *video);
        void showProfile(GUI );
        void showGameStat(GUI );

    protected:
        QString FileOpName;
        VideoCapture capture;
        Mat src;

    private slots:

    private:
            Ui::MainWindow *ui;
            QLabel *myLablel;
};

#endif // MAINWINDOW_H
