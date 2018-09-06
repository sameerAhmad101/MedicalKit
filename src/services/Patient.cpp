#include "Patient.h"

Patient::Patient(std::string &firstName, std::string &lastName, int age,	std::string &location,
    std::string &gender, std::string &nationalNumber ,std::string &disease, std::string &personalImage, std::vector<std::string> &images_v, std::string &imageSeriesDir){
	m_firstName = firstName;
	m_lastName = lastName;
	m_location = location;
	m_gender = gender;
	m_nationalNumber = nationalNumber;
	m_age = age;
	m_images_v = images_v;
	m_imageSeriesDir = imageSeriesDir;
    m_disease = disease;
    m_personalImage = personalImage;
}

Patient::~Patient(){
	//;
}

std::string Patient::getFirstName() {
	return m_firstName;
}

std::string Patient::getLastName() {
	return m_lastName;
}

std::string Patient::getLocation() {
	return m_location;
}

std::string Patient::getprofilePath() {
	return m_profilePath;
}

std::string Patient::getGender() {
	return m_gender;
}

std::string Patient::getNationalNumber() {
	return m_nationalNumber;
}
int Patient::getAge() {
	return m_age;
}
void Patient::setIamgeSeriesDir(std::string &dir)
{
	m_imageSeriesDir = dir;
}

void Patient::setIamges(std::vector<std::string> &images)
{
	m_images_v = images;
}

std::vector<std::string> Patient::getImages() {
	return m_images_v;
}

std::string Patient::getImagesPath() {
	return m_imageSeriesDir;
}

std::string Patient::getPersonalImage(){
    return m_personalImage;
}

std::string Patient::getDisease(){
    return m_disease;
}
