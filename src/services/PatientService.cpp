#include "patientservice.h"
#include <iostream>

PatientService::PatientService(std::shared_ptr<Database> db)
{
	m_database = db;
	//m_database->addPatient(m_firstName, m_lastName, m_location, m_gender, m_nationalNumber, m_images_v, m_imageSeries);
}

PatientService::~PatientService()
{
  //;
}

std::map<std::string, Patient*> PatientService::getPatients()
{
	return m_patients_v;
}

bool PatientService::addPatient(std::string &firstName, std::string &lastName, int age, std::string &location,
                std::string &gender, std::string &nationalNumber, std::string &disease,
                std::string &personalImage, std::vector<std::string> &images_v, std::string &imageSeriesDir) {
	bool status = false;
	try {
        m_patient = new Patient(firstName, lastName, age, location, gender, nationalNumber, disease, personalImage, images_v, imageSeriesDir);
		m_patients_v.emplace(nationalNumber, m_patient);
        m_database->addPatient(m_patient);
		status = true;
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return status;
}

bool PatientService::initPatients() {
    m_patients_v = m_database->getPatients();
    if(m_patients_v.size() > 0){
        return true;
    }
    else{
        return false;
    }

}
