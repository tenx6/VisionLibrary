#ifndef VISIONWIDGET_H
#define VISIONWIDGET_H

#include <QtWidgets/QMainWindow>
#include "ui_visionwidget.h"

class VisionWidget : public QMainWindow
{
    Q_OBJECT

public:
    VisionWidget(QWidget *parent = 0);
    ~VisionWidget();

private slots:
    void on_selectImageBtn_clicked();
	void on_fitCircleBtn_clicked();
    void on_fitLineBtn_clicked();
    void on_fitParallelLineBtn_clicked();
    void on_fitRectBtn_clicked();
    void on_ocrBtn_clicked();
    void on_srchFiducialBtn_clicked();
    void on_checkBoxDisplayGrayScale_clicked(bool checked);
    void on_checkBoxDisplayBinary_clicked(bool checked);
protected:
    bool checkDisplayImage();
private:
    Ui::VisionWidgetClass ui;
    std::string           _sourceImagePath;
    cv::Mat               _matOriginal;
};

#endif // VISIONWIDGET_H
