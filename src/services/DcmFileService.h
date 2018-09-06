#pragma once
#pragma warning (push, 0)
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkImageData.h>
#include <vtkImageShiftScale.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkPNGWriter.h>
#include <vtkImageShiftScale.h>
#include <QObject>
#include <vtkTextMapper.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#pragma warning (pop)

#include <sstream>
namespace service {
	class DcmFileService;
}

class DcmFileService : public QObject{
    Q_OBJECT

public:
    DcmFileService(vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow);
	~DcmFileService();
	void openSingleFile(std::string filename);
	void openFileSeries(std::string fileDirectory);
    int getSlicesCount();
	std::string dcm2png(const std::string &fileName);
    //vtkStringArray getFilesNames();
 public slots:
    void requestSlice(int slice);
	void updateRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow);
private:
	std::string m_dcmfileName;
	std::string m_directoryName;
    int m_slicesCount;
    int m_firstSlice;
    int m_currentSlice;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_renderWindow;
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkDICOMImageReader> m_dcmImageReader;
	vtkSmartPointer<vtkImageViewer2> m_dcmImageViewer;
	vtkSmartPointer<vtkImageActor> m_imageActor; 
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
	vtkSmartPointer<vtkImageData> m_imageData;
	vtkSmartPointer<vtkImageShiftScale> m_imageShiftScale;
	vtkSmartPointer<vtkLookupTable> m_lookupTable;
	vtkSmartPointer<vtkImageMapToColors> m_imageColor;



	vtkSmartPointer<vtkActor2D> m_sliceCounterActor;
	vtkSmartPointer<vtkTextMapper> m_sliceCounterMapper;
	vtkSmartPointer<vtkTextProperty> m_sliceCounterProperty;
	std::stringstream m_sliceCounterText;

	vtkSmartPointer<vtkActor2D> m_patientInfoActor;
	vtkSmartPointer<vtkTextMapper> m_patientInfoMapper;
	vtkSmartPointer<vtkTextProperty> m_patientInfoProperty;
	std::stringstream m_m_patientInfoText;

	void updateSlicesCount(const int sliceNum);
};
