#ifndef Raw_FILE_SERVICE
#define Raw_FILE_SERVICE

/// System inclues.
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


/// Qt incudes.
#include <qstring.h> 
#include  <qbytearray.h>

namespace service
{
	class  RawFileService;
}

class RawFileService
{
public:
	RawFileService(QString filepath, vtkSmartPointer<vtkRenderWindow> renderWindow);
	~RawFileService();

	bool openRawFile();

	void setupImage();

/// Setters & getters.
	void setSkinColor( double red, double green, double blue);

	bool readHeaderFile();

	enum RAW_FILE_TYPES
	{
		MET_SHORT = 1
	};

private:
	QString m_rawFilePath;
	std::array<double, 3> m_skinColor;

	std::string  m_rawFileType;
	vtkSmartPointer<vtkMetaImageReader> m_rawImageReader;
	vtkSmartPointer<vtkActor> m_skin;
	vtkSmartPointer<vtkRenderWindow> m_renderWindow;

	vtkSmartPointer<vtkMarchingCubes> m_skinExtractor;

	vtkSmartPointer<vtkRenderer> m_renderer;
};
#endif // !Raw_FILE_SERVICE
