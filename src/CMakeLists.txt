cmake_minimum_required(VERSION 2.8.11)

set(PROJECT_BINARY_NAME MedicalTool)

# qt package
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets REQUIRED QUIET)
FIND_PACKAGE(Qt5Core REQUIRED)


# vtk package
set(VTK_DIR F:/vtk-64/bin)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 

# define source dir
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

#define view dir
set(VIEW_DIR ${SOURCE_DIR}/view)

#define service directory
SET(SERVICE_DIR ${SOURCE_DIR}/services)

add_subdirectory(${VIEW_DIR})
		 
# define sources
set(CPP_FILES
        
	${SOURCE_DIR}/main.cpp
        #view
	${VIEW_DIR}/medicalkit.cpp
	${VIEW_DIR}/OpenFileDialog.cpp
	
	
	#services
	${SERVICE_DIR}/RawFileService.cpp
	${SERVICE_DIR}/TextLabelService.cpp
	${SERVICE_DIR}/VtkTextCallBack.cpp
	${SERVICE_DIR}/LabelingService.cpp
    ${SERVICE_DIR}/MedicalKitSettings.cpp
    ${SERVICE_DIR}/DcmFileService.cpp
	
)

# define headers
set(HEDER_FILES
       #view
       ${VIEW_DIR}/medicalkit.h
	   ${VIEW_DIR}/OpenFileDialog.h
	  #services
	  ${SERVICE_DIR}/RawFileService.h
	  ${SERVICE_DIR}/TextLabelService.h
	  ${SERVICE_DIR}/LabelingService.h
      ${SERVICE_DIR}/VtkTextCallBack.h
      ${SERVICE_DIR}/MedicalKitSettings.h
      ${SERVICE_DIR}/DcmFileService.h
)


# define ui	
set(RAP_UI_FILES 
       ${VIEW_DIR}/medicalkit.ui
)

qt5_wrap_ui(PROJECT_FORMS_HEADERS ${WRAP_UI_FILES})


# create exe project
add_executable(${PROJECT_BINARY_NAME}
    ${CPP_FILES}
    ${PROJECT_FORMS_HEADERS}
    ${HEDER_FILES}
    ${RAP_UI_FILES}
    )
	
# required qt modules	
qt5_use_modules(${PROJECT_BINARY_NAME} Core Gui)
  
# link project with dependencies
target_link_libraries(${PROJECT_BINARY_NAME}
      ${VTK_LIBRARIES}
      Qt5::Widgets)
