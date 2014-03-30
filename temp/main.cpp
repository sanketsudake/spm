#include <cvimagewidget.h>

#include <QDialog>
#include <QApplication>
#include <QMainWindow>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QMainWindow window;

    // Create the image widget
    CVImageWidget* imageWidget = new CVImageWidget();
    window.setCentralWidget(imageWidget);

    // Load an image
    cv::Mat image = cv::imread("somepicture.jpg", true);
    imageWidget->showImage(image);

    window.show();

    return app.exec();
}
