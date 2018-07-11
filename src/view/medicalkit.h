#pragma once

/// Qt includes
#include <QMainWindow>
#include <QVTKWidget.h>
#include <QVTKOpenGLWidget.h>
#include <QFileDialog>
#include <qfile.h>
#include <qpalette.h>
/// Vtk includes
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkMetaImageReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkBoxWidget.h>
#include <vtkMarchingCubes.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleSwitch.h>
#include <array>
/// My Includes.
#include "../services/RawFileService.h"
#include "../services/TextLabelService.h"

namespace Ui {
class MedicalKit;
}

class MedicalKit : public QMainWindow
{
    Q_OBJECT

public:
    explicit MedicalKit(QWidget *parent = 0);
    ~MedicalKit();
	void initRGBSliders();
	public slots:
	void openMedicalFile();
	void seekSlider(int value);
    void addText();

private:
    Ui::MedicalKit *ui;
    QVTKOpenGLWidget * m_vtkWidget;
	vtkSmartPointer<vtkMetaImageReader> m_rawImageReader;
	vtkSmartPointer<vtkRenderWindow> m_renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> m_renderWindowInteractor;


	std::array<double, 3>  m_rgbColor;

	RawFileService  * m_rawFileService = nullptr;
    TextLabelService * m_textLabel;
	void initUI();
};


