#pragma once

#pragma warning(push,0)
#include <QVTKWidget.h>
#include <QVTKOpenGLWidget.h>
#include <QFileDialog>
#include <qfile.h>
#include <qpalette.h>
#include <qmenubar.h>
#pragma warning(pop,0)
// My includes
#include "../services/RawFileService.h"
#include "../services/TextLabelService.h"
#include "../services/DcmFileService.h"

namespace Ui {
class ImageSeries;
}

class ImageSeries : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSeries(QWidget *parent = 0);
    ~ImageSeries();
public slots:
    void openDicomdirectory();
    void seekSlicesSlider(int value);
private:
    Ui::ImageSeries *ui;
    QVTKOpenGLWidget * m_vtkWidget;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    QFileDialog * m_fileDialog;
    QMenuBar* m_menuBar;
    QMenu * m_fileMenu;
    DcmFileService * m_dcmFileService = nullptr;
};


