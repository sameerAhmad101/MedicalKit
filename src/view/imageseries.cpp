#include "imageseries.h"
#include "ui_imageseries.h"

ImageSeries::ImageSeries(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageSeries)
{
    ui->setupUi(this);
    m_vtkWidget = new QVTKOpenGLWidget;
    ui->verticalLayout_qvtkWidget->addWidget(m_vtkWidget);
    m_renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    m_vtkWidget->SetRenderWindow(m_renderWindow);
    m_menuBar = new QMenuBar();
    m_fileMenu = new QMenu("File");
    m_menuBar->addMenu(m_fileMenu);
    m_fileMenu->addAction("Open Dicom Directory");
    connect(m_fileMenu->actions()[0], SIGNAL(triggered(bool)), this, SLOT(openDicomdirectory()));
    this->layout()->setMenuBar(m_menuBar);
    m_dcmFileService= new DcmFileService(m_renderWindow);
//    connect(ui->horizontal, SIGNAL(valueChanged(int)), m_dcmFileService, SLOT(requestSlice(int)));
//    connect(ui->horizontalSlider_slicesSlider, SIGNAL(valueChanged(int)), m_dcmFileService, SLOT(requestSlice(int)));
    connect(ui->horizontalSlider_slicesSlider, SIGNAL(valueChanged(int)), m_dcmFileService, SLOT(requestSlice(int)));
}

ImageSeries::~ImageSeries()
{
    delete ui;
}

void ImageSeries::openDicomdirectory(){
    QString slicesDirectory =
            m_fileDialog->getExistingDirectory(this, tr("Open Directory"),"/home",
                                       QFileDialog::ShowDirsOnly
                                       | QFileDialog::DontResolveSymlinks);
    if(!m_dcmFileService){
         m_dcmFileService = new DcmFileService(m_renderWindow);
    }

    m_dcmFileService->openFileSeries(slicesDirectory.toStdString());
    ui->horizontalSlider_slicesSlider->setRange(0,m_dcmFileService->getSlicesCount());
}


void ImageSeries::seekSlicesSlider(int value)
{
    m_dcmFileService->requestSlice(value);
}
