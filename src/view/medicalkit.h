#pragma once

/// Qt includes
#pragma warning (push,0)
#include <QMainWindow>
#include <QCloseEvent>
#include <QString>
#include <QFileDialog>
#include <QStringList>
#include <qtablewidget.h>
#include <QTableWidgetItem>
#include <QtSql>
#include <QRegularExpressionValidator>
/// Vtk includes
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

// System includes
#include <array>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <experimental/filesystem>
#include <qdebug.h>
#include <QMessageBox>
#pragma warning (pop)
// My Includes.

#include "singleimage.h"
#include "imageseries.h"
#include "../services/patientservice.h"
#include "../services/SqlDB.h"
namespace Ui {
class MedicalKit;
}

class MedicalKit : public QMainWindow
{
  Q_OBJECT

public:
    explicit MedicalKit(QWidget *parent = 0);
    ~MedicalKit();
    void closeEvent(QCloseEvent *event);
    void fillTable(int mode = 0);
public slots:
    void openSingleFileUi();
    void openImageSeriesFileUi();
    void addProfile();
    void openSingleImage();
    void openSingleImagesDirectory();
	void clickedCell(int , int );
	void patientSearch();
	void searchFieldChanged();
    void addPersonalImage();
	std::map<std::string, Patient*> searchBySting(const std::string &string);
private:
    Ui::MedicalKit *ui;
    SingleImage * m_singleImageUi = nullptr;
    ImageSeries * m_imageSeriesUi = nullptr;
    PatientService * m_patientService;
	std::map<std::string, Patient *> m_patients_v;
	std::map<std::string, Patient *> m_searchResults;
    QFileDialog *m_fileDialog;
    std::vector<std::string> m_images_v;
    QStringList m_filesNames;
    std::string m_slicesDirectory;
    std::fstream  m_profile;
    void initPatients();
    Patient* m_selectedPatient = nullptr;
    std::shared_ptr<Database> m_database;
    std::string m_presonalImagePath;

    void messageBox(const std::string &message);
    void viewPatientImage(std::string &imagePath);
    void initPatientInfo(Patient *patient);
};


