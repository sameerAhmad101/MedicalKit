#include "medicalkit.h"
#include "ui_medicalkit.h"

MedicalKit::MedicalKit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MedicalKit)
{
    ui->setupUi(this);
	connect(ui->pushButton_openProfile, SIGNAL(clicked(bool)), this, SLOT(openSingleFileUi()));
	connect(ui->pushButton_addProfile, SIGNAL(clicked(bool)), this, SLOT(addProfile()));
    connect(ui->pushButton_addSingleImage, SIGNAL(clicked(bool)), this, SLOT(openSingleImage()));
    connect(ui->pushButton_addImageSeries, SIGNAL(clicked(bool)), this, SLOT(openSingleImagesDirectory()));
	connect(ui->tableWidget_patients, SIGNAL(cellClicked(int, int)), this, SLOT(clickedCell(int, int)));
	connect(ui->pushButton, SIGNAL(clicked(bool)), SLOT(patientSearch()));
	connect(ui->lineEdit_search, SIGNAL(textChanged( QString )), this, SLOT(searchFieldChanged()));
    connect(ui->pushButton_addPersonalImage, SIGNAL(clicked(bool)), this, SLOT(addPersonalImage()));
    m_fileDialog = new QFileDialog();
	m_database = std::make_shared<Database>();
	m_patientService = new PatientService(m_database);
	if (m_database->connectToDb(QString("E:/MedicalKit.sqlite")))
	{
		m_database->addTables();
		m_patientService->initPatients();
		fillTable();
	}
	else {
		std::cout << "ERROR While Getting users." << std::endl;
	}

	QRegExp textFieldsExp("[^ء-يA-Za-z]+");
	QRegExpValidator *textFieldvalidator = new QRegExpValidator(textFieldsExp, this);
	QRegExp numberFieldsExp("[0-9]+");
	QRegExpValidator * numberFieldvalidator = new QRegExpValidator(textFieldsExp, this);
	ui->lineEdit_firstName->setValidator(textFieldvalidator);
	ui->lineEdit_lastName->setValidator(textFieldvalidator);
	ui->lineEdit_location->setValidator(textFieldvalidator);
	ui->lineEdit_search->setValidator(textFieldvalidator);
	ui->lineEdit_nationalNumber->setValidator(numberFieldvalidator);
	ui->lineEdit_age->setValidator(numberFieldvalidator);
	auto header = ui->tableWidget_patients->horizontalHeader();
	header->setSectionResizeMode(QHeaderView::Stretch);
}


MedicalKit::~MedicalKit()
{
    delete ui;
}

void MedicalKit::openSingleFileUi()
{
	if (!m_selectedPatient) {
		messageBox(u8"اختر مريض ليتم عرض بياناته!");
	}
	else {
		m_singleImageUi = new SingleImage(m_selectedPatient);
		m_singleImageUi->show();
	}
}

void MedicalKit::openImageSeriesFileUi()
{
     m_imageSeriesUi = new ImageSeries();
     m_imageSeriesUi->show();
}

 void MedicalKit::closeEvent(QCloseEvent *event)
 {
     if(m_imageSeriesUi)
     {
         m_imageSeriesUi->close();
     }
     if(m_singleImageUi)
     {
         m_singleImageUi->close();
     }
     event->accept();
 }

 void MedicalKit::fillTable(int mode)
 {
	 QTableWidgetItem *item_fName;
	 QTableWidgetItem *item_lName;
	 QTableWidgetItem *item_nationalNum;
	 QTableWidgetItem *item_age;
	 ui->tableWidget_patients->clear();
	 ui->tableWidget_patients->setRowCount(1);
	 QStringList labels;
	 labels << tr(u8"الرقم الوطني") << tr(u8"الاسم الأول") << tr(u8"الاسم الثاني") << tr(u8"العمر");
	 if (mode == 0 && m_patientService->getPatients().size() != 0)
	 {
         for (auto &patient : m_patientService->getPatients())
         {
			 int rowsCount = ui->tableWidget_patients->rowCount();
			 ui->tableWidget_patients->insertRow(rowsCount - 1);
			 item_fName = new QTableWidgetItem(QString().fromStdString(patient.second->getFirstName()));
			 item_lName = new QTableWidgetItem(QString().fromStdString(patient.second->getLastName()));
			 item_nationalNum = new QTableWidgetItem(QString().fromStdString(patient.first));
			 item_age = new QTableWidgetItem(QString().fromStdString(std::to_string(patient.second->getAge())));
			 ui->tableWidget_patients->setItem(rowsCount - 1, 0, item_nationalNum);
			 ui->tableWidget_patients->setItem(rowsCount - 1, 1, item_fName);
			 ui->tableWidget_patients->setItem(rowsCount - 1, 2, item_lName);
			 ui->tableWidget_patients->setItem(rowsCount - 1, 3, item_age);
         }
		 ui->tableWidget_patients->setHorizontalHeaderLabels(labels);
	 }
	 else if (mode == 1) {
		 if (m_searchResults.size() != 0)
		 {
			 for (auto &patient : m_searchResults)
			 {
				 int rowsCount = ui->tableWidget_patients->rowCount();
				 ui->tableWidget_patients->insertRow(rowsCount - 1);
				 item_fName = new QTableWidgetItem(QString().fromStdString(patient.second->getFirstName()));
				 item_lName = new QTableWidgetItem(QString().fromStdString(patient.second->getLastName()));
				 item_nationalNum = new QTableWidgetItem(QString().fromStdString(patient.first));
				 item_age = new QTableWidgetItem(QString().fromStdString(std::to_string(patient.second->getAge())));
				 ui->tableWidget_patients->setItem(rowsCount - 1, 0, item_nationalNum);
				 ui->tableWidget_patients->setItem(rowsCount - 1, 1, item_fName);
				 ui->tableWidget_patients->setItem(rowsCount - 1, 2, item_lName);
				 ui->tableWidget_patients->setItem(rowsCount - 1, 3, item_age);
			 }
		 }
	 }
 }

 void MedicalKit::addProfile()
 {
     int age  = std::stoi(ui->lineEdit_age->text().toStdString());
     std::string firstname = ui->lineEdit_firstName->text().toStdString();
     std::string lasttname = ui->lineEdit_lastName->text().toStdString();
     std::string location = ui->lineEdit_location->text().toStdString();
     std::string nationalNmber = ui->lineEdit_nationalNumber->text().toStdString();
     std::string gender = ui->comboBox_gender->currentText().toStdString();
     std::string disease = ui->lineEdit_5->text().toStdString();
     if(age == NULL || firstname == "" || lasttname == "" || location == "" ||
         nationalNmber == "" || disease == "")
     {
        return;
     }
     else{
         m_patientService->addPatient(firstname, lasttname, age,  location, gender, nationalNmber,
                                      disease, m_presonalImagePath, m_images_v, m_slicesDirectory);
         fillTable();
         viewPatientImage(m_presonalImagePath);
         initPatientInfo(m_patientService->getPatients()[nationalNmber]);
     }
}

 void MedicalKit::openSingleImage()
 {
	 m_filesNames = m_fileDialog->getOpenFileNames(this,
         tr("Open Image"), QDir::currentPath(), tr(" All Files(*.*);; Raw File( *.mhd, *.txt);; DICOM File(*.dcm)"));
	 if (!m_filesNames.isEmpty())
	 {
		 m_images_v.clear();
		 for (auto &fileName : m_filesNames)
		 {
			 m_images_v.push_back(fileName.toStdString());
		 }
	 }
 }

 void MedicalKit::openSingleImagesDirectory(){
     QString slicesDirectory =
             m_fileDialog->getExistingDirectory(this, tr("Open Directory"),"/home",
                                        QFileDialog::ShowDirsOnly
                                        | QFileDialog::DontResolveSymlinks);
	 m_slicesDirectory = slicesDirectory.toStdString();
 }

 void MedicalKit::initPatients()
 {
	//;
 }

 void MedicalKit::clickedCell(int row, int column){
	 if (ui->tableWidget_patients->rowCount() == row + 1)
		 return;
	 m_selectedPatient = m_patientService->getPatients()[ui->tableWidget_patients->item(row, 0)->text().toStdString()];
     if(m_selectedPatient){
         initPatientInfo(m_selectedPatient);
     }
 }

 void MedicalKit::messageBox(const std::string &message){
	 QMessageBox msgBox;
	 msgBox.setText(QString::fromStdString(message));
	 QPushButton *connectButton = msgBox.addButton(tr("OK"), QMessageBox::ActionRole);
	 msgBox.exec();
 }
 
 std::map<std::string, Patient*> MedicalKit::searchBySting(const std::string &string) {
	 m_searchResults.clear();
	 for (auto &patient : m_patientService->getPatients()) {
		 if (m_searchResults.find(patient.first) != m_searchResults.end()) {
			 continue;
		 }
		 else {
			 if (patient.second->getFirstName() == string || patient.second->getLocation() == string || patient.second->getLastName() == string) {
				 m_searchResults.emplace(std::make_pair(patient.first, patient.second));
			 }
		 }
	 }
	 return m_searchResults;
 }

 void MedicalKit::patientSearch() {
	 searchBySting(ui->lineEdit_search->text().toStdString());
	 fillTable(1);
 }

 void MedicalKit::searchFieldChanged() {
	if(ui->lineEdit_search->text() == NULL){
		fillTable();
	 }
 }

 void MedicalKit::addPersonalImage(){
     m_presonalImagePath = QFileDialog::getOpenFileName(this,
         tr("Open personalImage Image"), QDir::currentPath(), tr("PNG(*.png);; JPG(*.jpg)")).toStdString();
 }

 void MedicalKit::viewPatientImage(std::string &imagePath) {
     QImage image;
     image.load(QString::fromStdString(imagePath));
     ui->label_personalImage->setPixmap(QPixmap::fromImage(image));
 }

 void MedicalKit::initPatientInfo(Patient *patient){
    ui->label_patientAge->setText(QString(patient->getAge()));
    ui->label_patientDisease->setText(QString::fromStdString(patient->getDisease()));
    ui->label_patientLastName->setText(QString::fromStdString(patient->getLastName()));
    std::cout << patient->getAge();
    ui->label_patientFirstName->setText(QString::fromStdString(patient->getFirstName()));
    viewPatientImage(patient->getPersonalImage());
 }
