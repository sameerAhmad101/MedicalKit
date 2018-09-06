#pragma once

#pragma warning(push,0)
#include <QVTKWidget.h>
#include <QVTKOpenGLWidget.h>
#include <QFileDialog>
#include <qfile.h>
#include <qpalette.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <vtkStringArray.h>
#include <QScrollArea>
#include <QMouseEvent>
#include <qtoolbar.h>
#include <vector>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qsplitter.h>
#include <qsizepolicy.h>
#pragma warning(pop,0)
// My includes
#include "../services/RawFileService.h"
#include "../services/TextLabelService.h"
#include "../services/DcmFileService.h"
#include "../services/Patient.h"

namespace Ui {
class SingleImage;
class ImageFrame;
}

class SingleImage : public QWidget
{
    Q_OBJECT

public:
    explicit SingleImage(Patient *patient, QWidget *parent = 0);
    ~SingleImage();
	bool eventFilter(QObject *, QEvent*);
public slots:
    void openMedicalFile(DcmFileService *dcmFileService);
    void seekRGBSlider(int value);
    void openDicomdirectory(DcmFileService *dcmFileService);
	void patientImageCliked();
	void addNewViewPort();
	void openDicomFileViewport();
private:
    Ui::SingleImage *ui;

    QVTKOpenGLWidget * m_vtkWidget;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkMetaImageReader> m_rawImageReader;
    QFileDialog * m_fileDialog;
    QMenuBar* m_menuBar;
    QMenu * m_fileMenu;
    std::array<double, 3>  m_rgbColor;
    RawFileService  * m_rawFileService = nullptr;
    DcmFileService *m_dcmFileService = nullptr;
    //  TextLabelService * m_textLabel;
	Patient *m_patient;
	QPixmap * m_labelImage;
	QLabel * m_label;
	QToolBar *m_toolsBar;
	std::map<std::string, vtkSmartPointer<vtkGenericOpenGLRenderWindow>> m_renderWindows_v;
	std::map<std::string, DcmFileService*> m_OpenedDcmFiles_v;

	// Viewports 
    void initUI();
    void initRGBSliders();
	void initImageslist();
    void initPatientInfo();
    void viewPatientImage(std::string &imagePath);
	std::string generateReinderWindowId();
};


class ImageFrame : public QLabel {
	Q_OBJECT;
public:
	explicit ImageFrame(QWidget* parent = 0);
	 ~ImageFrame();
	 void setImagePath(std::string &imagePath);
	 std::string getImagePath();
 signals:
	 void clicked();

protected:
	 void mousePressEvent(QMouseEvent *e);
	
private:
	std::string m_imagePath;
	 
};
