cmake_minimum_required(VERSION 3.16)

project(HospitalEmergency LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Project headers
set(PROJECT_HEADERS
    src/include/PatientManager.h
    src/include/PriorityQueue.h
    src/include/Queue.h
    src/include/utils.h
    src/model/Patient.h
    src/model/triage.h
    src/gui/include/mainwindow.h
    src/gui/include/dashboard.h
    src/gui/include/patient.h
    src/gui/include/triage.h
    src/gui/include/emergency.h
    src/gui/include/treatment.h
)

# Project include directories
include_directories(
    src/include
    src/model
    src/gui/include
)

# Target: Standalone Emergency Queue GUI
add_executable(emergency_test
    src/emergency_main.cpp
    src/gui/data/emergency.cpp
    src/gui/include/emergency.h
    src/data/PatientManager.cpp
    src/data/PriorityQueue.cpp
    src/data/utils.cpp
    resources/resources.qrc
)

target_include_directories(emergency_test PRIVATE
    src/include
    src/model
    src/gui/include
)

target_link_libraries(emergency_test PRIVATE
    Qt6::Widgets
)

# Target: Full Hospital Management System
add_executable(HospitalSystem
    src/main.cpp
    src/data/PatientManager.cpp
    src/data/PriorityQueue.cpp
    src/data/utils.cpp
    src/gui/data/mainwindow.cpp
    src/gui/data/patient.cpp
    src/gui/data/dashboard.cpp
    src/gui/data/triage.cpp
    src/gui/data/emergency.cpp
    src/gui/data/treatment.cpp
    ${PROJECT_HEADERS}
    resources/resources.qrc
)

target_include_directories(HospitalSystem PRIVATE
    src/include
    src/model
    src/gui/include
)

target_link_libraries(HospitalSystem PRIVATE
    Qt6::Widgets
)
