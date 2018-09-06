cmake_minimum_required(VERSION 2.8.11)

set(PROJECT_BINARY_NAME MedicalTool)

# qt package
find_package(Qt5Widgets REQUIRED QUIET)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Sql   REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

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
    ${VIEW_DIR}/singleimage.cpp
    ${VIEW_DIR}/imageseries.cpp
    ${VIEW_DIR}/Intro.cpp
    ${VIEW_DIR}/DarkStyle.cpp
    ${VIEW_DIR}/framelesswindow/framelesswindow.cpp
    ${VIEW_DIR}/framelesswindow/windowdragger.cpp
	
	#services
	${SERVICE_DIR}/RawFileService.cpp
	${SERVICE_DIR}/TextLabelService.cpp
	${SERVICE_DIR}/VtkTextCallBack.cpp
	#SERVICE_DIR}/LabelingService.cpp
    ${SERVICE_DIR}/MedicalKitSettings.cpp
    ${SERVICE_DIR}/DcmFileService.cpp
    ${SERVICE_DIR}/PatientService.cpp
    ${SERVICE_DIR}/Patient.cpp
    ${SERVICE_DIR}/SqlDB.cpp
)

# define headers
set(HEDER_FILES
       #view
       ${VIEW_DIR}/medicalkit.h
       ${VIEW_DIR}/singleimage.h
       ${VIEW_DIR}/imageseries.h
       ${VIEW_DIR}/Intro.h
       ${VIEW_DIR}/DarkStyle.h
       ${VIEW_DIR}/framelesswindow/framelesswindow.h
       ${VIEW_DIR}/framelesswindow/windowdragger.h
       #services
       ${SERVICE_DIR}/RawFileService.h
       ${SERVICE_DIR}/TextLabelService.h
       #${SERVICE_DIR}/LabelingService.h
       ${SERVICE_DIR}/VtkTextCallBack.h
       ${SERVICE_DIR}/MedicalKitSettings.h
       ${SERVICE_DIR}/DcmFileService.h
       ${SERVICE_DIR}/PatientService.h
       ${SERVICE_DIR}/Patient.h
       ${SERVICE_DIR}/SqlDB.h
)


# define ui	
set(RAP_UI_FILES 
       ${VIEW_DIR}/medicalkit.ui
       ${VIEW_DIR}/singleimage.ui
       ${VIEW_DIR}/Intro.ui
       ${VIEW_DIR}/imageseries.ui
       ${VIEW_DIR}/framelesswindow/framelesswindow.ui
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
qt5_use_modules(${PROJECT_BINARY_NAME} Core Gui )
  
# link project with dependencies
target_link_libraries(${PROJECT_BINARY_NAME}
      ${VTK_LIBRARIES}
      Qt5::Widgets
      Qt5::Sql)
set_target_properties(${VTK_LIBRARIES} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

FILE(GLOB  UI_ICONS ${VIEW_DIR}/darkstyle/*.png)

ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_BINARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		  ${VIEW_DIR}/darkstyle
          ${CMAKE_CURRENT_BINARY_DIR}/view/darkstyle
	)
ADD_CUSTOM_COMMAND(
		TARGET ${PROJECT_BINARY_NAME} POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		  ${VIEW_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/view/assets
	)
