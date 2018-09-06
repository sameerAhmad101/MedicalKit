#pragma once

/// System inclues.
#pragma warning(push,0)
#include <string>
#include <array>
#include <fstream>
#include <sstream>
/// Vtk library.
#include <vtkSmartPointer.h>
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
#include <vtkBoxWidget2.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkImageReslice.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageViewer2.h>
#include <vtkMatrix4x4.h>
#include <vtkSphere.h>
#include <vtkClipDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkSphereSource.h>
#include <vtkProbeFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPlaneSource.h>
#include <vtkStripper.h>
#include <vtkNamedColors.h>
#include <vtkOutlineFilter.h>
/// Qt incudes.
#include <qstring.h> 
#include  <qbytearray.h>
#include <qobject.h>
#include <QMetaObject>
#include <QMetaMethod>
#pragma warning(pop,0)

#include "MedicalKitSettings.h"


namespace service
{
	class  RawFileService;
}

class RawFileService : public  QObject
{
	Q_OBJECT

public:
    RawFileService(QString filepath, vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow);
	~RawFileService();

	bool openRawFile();

	void setupImage();

/// Setters & getters.
	void setSkinColor( double red, double green, double blue);

	bool readHeaderFile();


public slots:
	void startCutter();
	void moveCutter(int x);
	void extractSourface();
private:
	QString m_rawFilePath;
	std::array<double, 3> m_skinColor;

	std::string  m_rawFileType;
	vtkSmartPointer<vtkMetaImageReader> m_rawImageReader;
	vtkSmartPointer<vtkActor> m_skin;
	vtkSmartPointer<vtkActor> m_boneActor;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;

	vtkSmartPointer<vtkMarchingCubes> m_skinExtractor;
	vtkSmartPointer<vtkMarchingCubes> m_bonesExtractor;

	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> m_renderWindowInteractor;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> m_interactorStyle;
	vtkSmartPointer<vtkPlane> m_cutterplane;
	vtkSmartPointer<vtkCutter> m_cutter;
	vtkSmartPointer<vtkPlaneSource> m_lensModel;
	vtkSmartPointer<vtkPlane> m_clipFunction;
};

