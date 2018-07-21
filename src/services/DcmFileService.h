#pragma once
#pragma warning (push, 0)
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkActorCollection.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3d.h>
#pragma warning (pop,0)


namespace service {
	class DcmFileService;
}

class DcmFileService{
public:
	DcmFileService(vtkSmartPointer<vtkRenderWindow> renderWindow);
	~DcmFileService();
	void openSingleFile(std::string filename);
	void openFileSeries(std::string fileDirectory);
    int getSlicesCount();
    void requestSlice(int slice);
private:
	std::string m_dcmfileName;
	std::string m_directoryName;
    int m_slicesCount;
    int m_firstSlice;
    int m_currentSlice;
	vtkSmartPointer<vtkRenderWindow> m_renderWindow;
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkSmartPointer<vtkDICOMImageReader> m_dcmImageReader;
	vtkSmartPointer<vtkImageViewer2> m_dcmImageViewer;
	vtkSmartPointer<vtkImageMapper3D> m_imageMapper;
	vtkSmartPointer<vtkImageActor> m_imageActor;
	vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;
};
