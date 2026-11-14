QT += widgets

CONFIG += c++17
TEMPLATE = app
TARGET = HospitalSystem

INCLUDEPATH += \
    src/include \
    src/model \
    src/gui/include

SOURCES += \
    src/main.cpp \
    src/data/PatientManager.cpp \
    src/data/PriorityQueue.cpp \
    src/data/utils.cpp \
    src/gui/data/mainwindow.cpp \
    src/gui/data/patient.cpp \
    src/gui/data/dashboard.cpp \
    src/gui/data/triage.cpp \
    src/gui/data/emergency.cpp

HEADERS += \
    src/include/PatientManager.h \
    src/include/PriorityQueue.h \
    src/include/Queue.h \
    src/include/utils.h \
    src/model/Patient.h \
    src/model/triage.h \
    src/gui/include/mainwindow.h \
    src/gui/include/dashboard.h \
    src/gui/include/patient.h \
    src/gui/include/triage.h \
    src/gui/include/emergency.h
