#include "SqlDB.h"
#include <iostream>

Database::Database()
{
	m_database = QSqlDatabase::addDatabase("QSQLITE", "OneNameOrOther");
}

Database::~Database()
{
	//;
}

bool Database::connectToDb(QString &dbName)
{
	bool status = false;
	m_database.setDatabaseName(dbName);
	if (!m_database.open())
	{
		std::cout << "SQL ERROR 1 " << QSqlError().text().toStdString() << std::endl;
	}
	else {
		status = true;
	}
	return status;
	}


bool Database::addTables()
{
	bool status = false;
	QSqlQuery query(m_database);
	if (!query.exec(QLatin1String("create table patients(id integer primary key autoincrement , fName varchar, lName varchar, disease varchar , location varchar, gende varchar, nationNum varchar, imagesSeries varchar, personalImage varchar, addDate date)")))
	{
		std::cout << "SQL ERROR 2 " << QSqlError().text().toStdString() << std::endl;
		status = false;
	}
	else
	{
		status = true;
	}
	if (!query.exec(QLatin1String("create table images(id integer primary key autoincrement, patientId integer, path varchar, addDate date)")))
	{
		std::cout << "SQL ERROR 3 " << QSqlError().text().toStdString() << std::endl;
		status = false;
	}
	else
	{
		status = true;
	}
	return status;
}

bool Database::addPatient(Patient *patient)
{
	if (m_database.isOpen()) {
		QSqlQuery query(m_database);// prolem i  date addition(current time/date)
		query.prepare("INSERT INTO patients(  fName , lName , disease  , location , gende , nationNum, imagesSeries, personalImage, addDate ) VALUES(?, ?, ?, ?, ?, ?, ?, ?,CURRENT_TIMESTAMP)");
        query.addBindValue(QVariant(QString::fromStdString(patient->getFirstName())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getLastName())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getDisease())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getLocation())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getGender())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getNationalNumber())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getImagesPath())));
        query.addBindValue(QVariant(QString::fromStdString(patient->getPersonalImage())));
		if (!query.exec())
		{
			std::cout << "SQL ERROR 4 " << QSqlError().text().toStdString() << std::endl;
		}
        for (auto & img : patient->getImages())
		{
			query.prepare("INSERT INTO images(patientId, path, addDate) VALUES(?, ?, CURRENT_TIMESTAMP)");
            query.addBindValue(QVariant(QString::fromStdString(patient->getNationalNumber())));
			query.addBindValue(QVariant(QString::fromStdString(img)));
			if (!query.exec())
			{
				std::cout << "SQL ERROR 5 " << QSqlError().text().toStdString() << std::endl;
			}
		}
        return true;
	}
    else{
        std::cout << "SQL ERROR 6 " << QSqlError().text().toStdString() << std::endl;
        return false;
    }

}


Patient* Database::getPatient(std::string &nationalNumber){
	Patient * patient;
	std::string fName;
	std::string lName;
	std::string disease;
	std::string location;
	std::string gender;
	std::string nationNum;
	std::string imagesSeriesDirectory;
	std::string personalImage;
	std::vector<std::string> images;
	QSqlQuery query(m_database);
	query.prepare("SELECT * from patients WHERE  nationNum = ?");
	query.addBindValue(QVariant(QString::fromStdString(nationalNumber)));
	if (!query.exec()) {
		std::cout << "SQL ERROR 7  " << QSqlError().text().toStdString() << std::endl;
	}
	else {
		while (query.next()) {
			int patientId = query.value(1).toInt();
			fName = query.value(2).toString().toStdString();
		    lName = query.value(3).toString().toStdString();
			disease = query.value(4).toString().toStdString();
			location = query.value(5).toString().toStdString();
			gender = query.value(6).toString().toStdString();
			nationNum = query.value(7).toString().toStdString();
			imagesSeriesDirectory = query.value(8).toString().toStdString();
			personalImage = query.value(9).toString().toStdString();
			QSqlQuery imageQuery(m_database);
			imageQuery.prepare("SELECT * from images WHERE  patientId = ? ");
			imageQuery.addBindValue(QVariant(QString::fromStdString(nationalNumber)));
			if (!imageQuery.exec()) {
				std::cout << "SQL ERROR 8  " << QSqlError().text().toStdString() << std::endl;
			}
			else {
				while (imageQuery.next()) {
					images.push_back(imageQuery.value(2).toString().toStdString());
				}
			}
			patient = new Patient(fName, lName, 10, location, gender, nationNum, disease, personalImage,  images, imagesSeriesDirectory);
		}
	}
	return patient;
}

std::map<std::string, Patient*> Database::getPatients() {

	std::string fName;
	std::string lName;
	std::string disease;
	std::string location;
	std::string gender;
	std::string nationNum;
	std::string imagesSeriesDirectory;
	std::string personalImage;
	std::vector<std::string> images;
	QSqlQuery query(m_database);
	query.prepare("SELECT * from patients");
	if (!query.exec()) {
		std::cout << "SQL ERROR 9  " << QSqlError().text().toStdString() << std::endl;
	}
	else {
		while (query.next()) {
			int patientId = query.value(0).toInt();
			fName = query.value(1).toString().toStdString();
			lName = query.value(2).toString().toStdString();
			disease = query.value(3).toString().toStdString();
			location = query.value(4).toString().toStdString();
			gender = query.value(5).toString().toStdString();
			nationNum = query.value(6).toString().toStdString();
			imagesSeriesDirectory = query.value(7).toString().toStdString();
			personalImage = query.value(8).toString().toStdString();
			QSqlQuery imageQuery(m_database);
			imageQuery.prepare("SELECT * from images WHERE patientId = ?");
			imageQuery.addBindValue(QVariant(QString::fromStdString(nationNum)));
			if (!imageQuery.exec()) {
				std::cout << "SQL ERROR 10  " << QSqlError().text().toStdString() << std::endl;
			}
			else {
				while (imageQuery.next()) {
					images.push_back(imageQuery.value(2).toString().toStdString());
				}
			}
			 m_patient = new Patient(fName, lName, 10, location, gender, nationNum, disease, personalImage, images, imagesSeriesDirectory);
			 m_patients_v.emplace(nationNum, m_patient);
		}
	}
	return m_patients_v;
}


