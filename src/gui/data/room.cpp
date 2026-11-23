#include "../include/room.h"
#include "../../include/RoomManager.h"
#include "../../include/PatientManager.h"
#include "../../model/Patient.h"
#include "../../model/room.h"

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QFont>
#include <QColor>

RoomPage::RoomPage(RoomManager *rMgr, PatientManager *pMgr, QWidget *parent)
    : QWidget(parent),
      roomManager(rMgr),
      patientManager(pMgr),
      roomSelector(nullptr),
      patientSelector(nullptr),
      roomTable(nullptr),
      totalValue(nullptr),
      availableValue(nullptr),
      occupiedValue(nullptr)
{
    buildInterface();
    refreshAll();
}

void RoomPage::refreshAll()
{
    refreshRoomTable();
    refreshRoomSelector();
    refreshPatients();
    updateSummaryCards();
}

QWidget *RoomPage::buildHeader()
{
    QWidget *header = new QWidget;
    header->setMinimumHeight(95);
    header->setStyleSheet("background-color: #EEF6FF; border-radius: 12px; margin-bottom: 4px;");

    QHBoxLayout *layout = new QHBoxLayout(header);
    layout->setContentsMargins(25, 15, 25, 15);

    QLabel *icon = new QLabel("🛏");
    icon->setStyleSheet("font-size: 38px; color: #1766A8;");
    layout->addWidget(icon);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setSpacing(3);

    QLabel *title = new QLabel("Room Management");
    title->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 24px; font-weight: bold; color: #102F50;");

    QLabel *subtitle = new QLabel("Manage emergency hospital rooms and patient assignments in real time");
    subtitle->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 13px; color: #335372;");

    textLayout->addWidget(title);
    textLayout->addWidget(subtitle);
    layout->addLayout(textLayout);
    layout->addStretch();

    return header;
}

QFrame *RoomPage::buildSummaryCard(
    const QString &title,
    QLabel *&valueLabel,
    const QString &background,
    const QString &numberColor)
{
    QFrame *card = new QFrame;
    card->setMinimumHeight(105);
    card->setStyleSheet(
        QString(
            "QFrame { background-color: %1; border-radius: 12px; border: 1px solid #E2E8F0; }"
            "QLabel { border: none; }")
            .arg(background));

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 14, 22, 14);
    layout->setSpacing(4);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 15px; font-weight: bold; color: #1E293B;");

    valueLabel = new QLabel("0");
    valueLabel->setStyleSheet(
        QString("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 30px; font-weight: bold; color: %1;")
            .arg(numberColor));

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);

    return card;
}

QFrame *RoomPage::buildPanel(const QString &title)
{
    QFrame *panel = new QFrame;
    panel->setStyleSheet(
        "QFrame {"
        " background-color: #FFFFFF;"
        " border: 1px solid #CBD5E1;"
        " border-radius: 12px;"
        "}");

    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *heading = new QLabel("  " + title);
    heading->setMinimumHeight(48);
    heading->setStyleSheet(
        "QLabel {"
        " font-family: 'Gotham', 'Montserrat', sans-serif;"
        " background-color: #1E3A8A;"
        " color: #FFFFFF;"
        " font-size: 16px;"
        " font-weight: bold;"
        " border: none;"
        " border-top-left-radius: 11px;"
        " border-top-right-radius: 11px;"
        " padding-left: 14px;"
        "}");

    layout->addWidget(heading);
    return panel;
}

QPushButton *RoomPage::makeActionButton(
    const QString &text,
    const QString &background)
{
    QPushButton *button = new QPushButton(text);
    button->setMinimumHeight(46);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        QString(
            "QPushButton {"
            " font-family: 'Gotham', 'Montserrat', sans-serif;"
            " background-color: %1;"
            " color: #FFFFFF;"
            " font-size: 14px;"
            " font-weight: bold;"
            " border: none;"
            " border-radius: 8px;"
            " padding: 8px 16px;"
            "}"
            "QPushButton:hover { opacity: 0.9; filter: brightness(1.1); }"
            "QPushButton:pressed { filter: brightness(0.9); }")
            .arg(background));
    return button;
}

void RoomPage::buildInterface()
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(22, 18, 22, 22);
    rootLayout->setSpacing(16);

    rootLayout->addWidget(buildHeader());

    // Summary cards
    QHBoxLayout *cardsLayout = new QHBoxLayout;
    cardsLayout->setSpacing(14);

    QFrame *totalCard = buildSummaryCard(
        "Total Rooms", totalValue, "#F0F9FF", "#0284C7");

    QFrame *availableCard = buildSummaryCard(
        "Available Rooms", availableValue, "#F0FDF4", "#16A34A");

    QFrame *occupiedCard = buildSummaryCard(
        "Occupied Rooms", occupiedValue, "#FEF2F2", "#DC2626");

    cardsLayout->addWidget(totalCard);
    cardsLayout->addWidget(availableCard);
    cardsLayout->addWidget(occupiedCard);
    rootLayout->addLayout(cardsLayout);

    // Main content: actions and table
    QHBoxLayout *mainContentLayout = new QHBoxLayout;
    mainContentLayout->setSpacing(16);

    // Room actions panel
    QFrame *actionsPanel = buildPanel("⚙  Room Actions");
    actionsPanel->setMinimumWidth(320);
    actionsPanel->setMaximumWidth(390);

    QVBoxLayout *actionsLayout =
        qobject_cast<QVBoxLayout *>(actionsPanel->layout());

    QWidget *actionsBody = new QWidget;
    QVBoxLayout *bodyLayout = new QVBoxLayout(actionsBody);
    bodyLayout->setContentsMargins(18, 16, 18, 18);
    bodyLayout->setSpacing(10);

    QLabel *roomLabel = new QLabel("Room (Select Room)");
    roomLabel->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 14px; font-weight: bold; color: #1E293B;");

    const QString comboStyle =
        "QComboBox {"
        " font-family: 'Gotham', 'Montserrat', sans-serif;"
        " background-color: #FFFFFF;"
        " color: #0F172A;"
        " border: 1.5px solid #CBD5E1;"
        " border-radius: 8px;"
        " padding: 8px 12px;"
        " font-size: 13px;"
        " font-weight: 500;"
        "}"
        "QComboBox:hover {"
        " border-color: #2563EB;"
        " background-color: #F8FAFC;"
        "}"
        "QComboBox:focus {"
        " border-color: #2563EB;"
        " background-color: #FFFFFF;"
        "}"
        "QComboBox::drop-down {"
        " subcontrol-origin: padding;"
        " subcontrol-position: top right;"
        " width: 32px;"
        " border-left: 1px solid #E2E8F0;"
        " border-top-right-radius: 8px;"
        " border-bottom-right-radius: 8px;"
        " background-color: #F1F5F9;"
        "}"
        "QComboBox::down-arrow {"
        " width: 0; height: 0;"
        " border-left: 5px solid transparent;"
        " border-right: 5px solid transparent;"
        " border-top: 6px solid #475569;"
        " margin-right: 2px;"
        "}"
        "QComboBox QAbstractItemView {"
        " font-family: 'Gotham', 'Montserrat', sans-serif;"
        " background-color: #FFFFFF;"
        " color: #0F172A;"
        " border: 1.5px solid #94A3B8;"
        " border-radius: 8px;"
        " selection-background-color: #EFF6FF;"
        " selection-color: #1E3A8A;"
        " padding: 4px;"
        " font-size: 13px;"
        " outline: none;"
        "}"
        "QComboBox QAbstractItemView::item {"
        " min-height: 34px;"
        " padding: 6px 10px;"
        " border-radius: 4px;"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        " background-color: #F8FAFC;"
        " color: #0F172A;"
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        " background-color: #DBEAFE;"
        " color: #1D4ED8;"
        " font-weight: bold;"
        "}";

    roomSelector = new QComboBox;
    roomSelector->setMinimumHeight(44);
    roomSelector->setStyleSheet(comboStyle);
    roomSelector->setMaxVisibleItems(10);
    roomSelector->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    roomSelector->view()->setMinimumWidth(300);

    QLabel *patientLabel = new QLabel("Patient (Select Patient)");
    patientLabel->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 14px; font-weight: bold; color: #1E293B; margin-top: 6px;");

    patientSelector = new QComboBox;
    patientSelector->setMinimumHeight(44);
    patientSelector->setStyleSheet(comboStyle);
    patientSelector->setMaxVisibleItems(10);
    patientSelector->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    patientSelector->view()->setMinimumWidth(300);

    bodyLayout->addWidget(roomLabel);
    bodyLayout->addWidget(roomSelector);
    bodyLayout->addWidget(patientLabel);
    bodyLayout->addWidget(patientSelector);
    bodyLayout->addSpacing(10);

    QPushButton *assignButton =
        makeActionButton("♟  Assign Patient", "#2563EB");
    QPushButton *releaseButton =
        makeActionButton("↩  Release Room", "#DC2626");
    QPushButton *addButton =
        makeActionButton("＋  Add New Room", "#16A34A");

    bodyLayout->addWidget(assignButton);
    bodyLayout->addWidget(releaseButton);
    bodyLayout->addWidget(addButton);
    bodyLayout->addStretch();

    actionsLayout->addWidget(actionsBody);

    // Room list panel
    QFrame *tablePanel = buildPanel("☷  Emergency Rooms Status");
    QVBoxLayout *tablePanelLayout =
        qobject_cast<QVBoxLayout *>(tablePanel->layout());

    roomTable = new QTableWidget;
    roomTable->setColumnCount(5);
    roomTable->setHorizontalHeaderLabels({"Room ID",
                                          "Room Number",
                                          "Patient ID",
                                          "Patient Name",
                                          "Status"});

    roomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    roomTable->setSelectionMode(QAbstractItemView::SingleSelection);
    roomTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    roomTable->setAlternatingRowColors(true);
    roomTable->verticalHeader()->setVisible(false);
    roomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    roomTable->setMinimumHeight(400);
    roomTable->setStyleSheet(
        "QTableWidget {"
        " font-family: 'Gotham', 'Montserrat', sans-serif;"
        " background-color: #FFFFFF;"
        " alternate-background-color: #F8FAFC;"
        " gridline-color: #E2E8F0;"
        " border: none;"
        " font-size: 14px;"
        " color: #1E293B;"
        "}"
        "QHeaderView::section {"
        " font-family: 'Gotham', 'Montserrat', sans-serif;"
        " background-color: #F1F5F9;"
        " color: #334155;"
        " font-size: 13px;"
        " font-weight: bold;"
        " padding: 10px 6px;"
        " border: none;"
        " border-bottom: 1.5px solid #CBD5E1;"
        "}"
        "QTableWidget::item { padding: 6px; }"
        "QTableWidget::item:selected {"
        " background-color: #DBEAFE;"
        " color: #1E3A8A;"
        " font-weight: bold;"
        "}");

    tablePanelLayout->addWidget(roomTable);
    mainContentLayout->addWidget(actionsPanel, 1);
    mainContentLayout->addWidget(tablePanel, 3);

    rootLayout->addLayout(mainContentLayout);

    // Button actions
    connect(addButton, &QPushButton::clicked, this, [this]() {
        addRoom();
    });

    connect(assignButton, &QPushButton::clicked, this, [this]() {
        assignPatient();
    });

    connect(releaseButton, &QPushButton::clicked, this, [this]() {
        releaseRoom();
    });

    // Synchronize row click in table with room selector dropdown
    connect(roomTable, &QTableWidget::cellClicked, this, [this](int row, int) {
        if (!roomManager || row < 0 || row >= roomManager->getRoomCount())
            return;

        Room *room = roomManager->getRoomAt(row);
        if (!room)
            return;

        for (int i = 0; i < roomSelector->count(); ++i)
        {
            if (roomSelector->itemData(i).toInt() == room->roomID)
            {
                roomSelector->blockSignals(true);
                roomSelector->setCurrentIndex(i);
                roomSelector->blockSignals(false);
                break;
            }
        }
    });

    // Synchronize room selector dropdown with table row selection
    connect(roomSelector, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (!roomManager || index < 0)
            return;

        int roomId = roomSelector->itemData(index).toInt();
        for (int r = 0; r < roomManager->getRoomCount(); ++r)
        {
            Room *room = roomManager->getRoomAt(r);
            if (room && room->roomID == roomId)
            {
                roomTable->selectRow(r);
                break;
            }
        }
    });
}

void RoomPage::refreshRoomTable()
{
    if (!roomManager)
        return;

    int totalRooms = roomManager->getRoomCount();
    roomTable->setRowCount(totalRooms);

    for (int row = 0; row < totalRooms; ++row)
    {
        Room *room = roomManager->getRoomAt(row);
        if (!room)
            continue;

        // Column 0: Room ID
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(room->roomID));
        idItem->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 0, idItem);

        // Column 1: Room Number
        QTableWidgetItem *numItem = new QTableWidgetItem(QString::fromStdString(room->roomNumber));
        numItem->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 1, numItem);

        // Column 2: Patient ID
        QString patientIdText = (room->patientID <= 0) ? "—" : QString::number(room->patientID);
        QTableWidgetItem *pIdItem = new QTableWidgetItem(patientIdText);
        pIdItem->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 2, pIdItem);

        // Column 3: Patient Name
        QString patientNameText = "—";
        if (room->patientID > 0 && patientManager)
        {
            Patient *p = patientManager->findPatientInfo(room->patientID);
            if (p)
            {
                patientNameText = QString::fromStdString(p->getName());
            }
        }
        QTableWidgetItem *nameItem = new QTableWidgetItem(patientNameText);
        nameItem->setTextAlignment(Qt::AlignCenter);
        roomTable->setItem(row, 3, nameItem);

        // Column 4: Status
        bool isAvail = (room->status == AVAILABLE);
        QTableWidgetItem *statusItem = new QTableWidgetItem(isAvail ? "Available" : "Occupied");
        statusItem->setTextAlignment(Qt::AlignCenter);
        QFont statusFont("Gotham", 10, QFont::Bold);
        statusFont.setFamilies({"Gotham", "Montserrat", "Segoe UI", "sans-serif"});
        statusItem->setFont(statusFont);
        statusItem->setForeground(QColor(isAvail ? "#16A34A" : "#DC2626"));
        roomTable->setItem(row, 4, statusItem);
    }

    roomTable->resizeRowsToContents();
}

void RoomPage::refreshRoomSelector()
{
    int currentSelectedRoomId = roomSelector ? roomSelector->currentData().toInt() : -1;

    roomSelector->blockSignals(true);
    roomSelector->clear();

    if (!roomManager || roomManager->getRoomCount() == 0)
    {
        roomSelector->addItem("No rooms created yet", -1);
        roomSelector->blockSignals(false);
        return;
    }

    int matchIndex = 0;
    for (int i = 0; i < roomManager->getRoomCount(); ++i)
    {
        Room *room = roomManager->getRoomAt(i);
        if (!room)
            continue;

        QString statusStr = (room->status == AVAILABLE) ? "Available" : "Occupied";
        QString label = QString("%1 (ID: %2) — %3")
                            .arg(QString::fromStdString(room->roomNumber))
                            .arg(room->roomID)
                            .arg(statusStr);

        roomSelector->addItem(label, room->roomID);

        if (room->roomID == currentSelectedRoomId)
        {
            matchIndex = i;
        }
    }

    roomSelector->setCurrentIndex(matchIndex);
    roomSelector->blockSignals(false);
}

void RoomPage::refreshPatients()
{
    int currentSelectedPatientId = patientSelector ? patientSelector->currentData().toInt() : -1;

    patientSelector->blockSignals(true);
    patientSelector->clear();
    patientSelector->addItem("-- Select a Patient --", -1);

    if (patientManager)
    {
        int matchIndex = 0;
        int count = patientManager->getPatientCount();

        for (int i = 0; i < count; ++i)
        {
            Patient *p = patientManager->getPatientAt(i);
            if (!p)
                continue;

            QString label = QString("ID: %1 — %2")
                                .arg(p->getId())
                                .arg(QString::fromStdString(p->getName()));

            patientSelector->addItem(label, p->getId());

            if (p->getId() == currentSelectedPatientId)
            {
                matchIndex = patientSelector->count() - 1;
            }
        }

        patientSelector->setCurrentIndex(matchIndex);
    }

    patientSelector->blockSignals(false);
}

void RoomPage::updateSummaryCards()
{
    if (!roomManager)
        return;

    int total = roomManager->getRoomCount();
    int available = roomManager->getAvailableCount();
    int occupied = roomManager->getOccupiedCount();

    if (totalValue)
        totalValue->setText(QString::number(total));
    if (availableValue)
        availableValue->setText(QString::number(available));
    if (occupiedValue)
        occupiedValue->setText(QString::number(occupied));
}

void RoomPage::addRoom()
{
    if (!roomManager)
        return;

    if (roomManager->getRoomCount() >= roomManager->getMaxRooms())
    {
        QMessageBox::warning(
            this, "Room Limit Reached",
            QString("Cannot add more rooms. Maximum capacity of %1 rooms reached.")
                .arg(roomManager->getMaxRooms()));
        return;
    }

    bool ok = roomManager->addRoom();
    if (ok)
    {
        refreshAll();
        emit dataChanged();

        if (roomSelector->count() > 0)
        {
            roomSelector->setCurrentIndex(roomSelector->count() - 1);
        }

        Room *latest = roomManager->getRoomAt(roomManager->getRoomCount() - 1);
        QString roomNum = latest ? QString::fromStdString(latest->roomNumber) : "New Room";

        QMessageBox::information(
            this, "Room Added",
            QString("Room %1 has been added successfully to the system.").arg(roomNum));
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to add a new room.");
    }
}

void RoomPage::assignPatient()
{
    if (!roomManager || !patientManager)
        return;

    int roomId = roomSelector->currentData().toInt();
    if (roomId <= 0)
    {
        QMessageBox::warning(this, "Select Room", "Please select a valid room from the Room dropdown.");
        return;
    }

    Room *room = roomManager->findRoom(roomId);
    if (!room)
    {
        QMessageBox::warning(this, "Room Not Found", "The selected room could not be found.");
        return;
    }

    if (room->status == OCCUPIED)
    {
        QMessageBox::warning(
            this, "Room Occupied",
            QString("Room %1 is currently occupied. Please release it before assigning another patient.")
                .arg(QString::fromStdString(room->roomNumber)));
        return;
    }

    int patientId = patientSelector->currentData().toInt();
    if (patientId <= 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a valid patient from the Patient dropdown.");
        return;
    }

    Patient *p = patientManager->findPatientInfo(patientId);
    if (!p)
    {
        QMessageBox::warning(this, "Patient Not Found", "The selected patient record does not exist in the system.");
        return;
    }

    bool success = roomManager->assignRoom(roomId, patientId, *patientManager);
    if (success)
    {
        refreshAll();
        emit dataChanged();

        QMessageBox::information(
            this, "Patient Assigned",
            QString("Patient %1 (ID: %2) was successfully assigned to %3.")
                .arg(QString::fromStdString(p->getName()))
                .arg(patientId)
                .arg(QString::fromStdString(room->roomNumber)));
    }
    else
    {
        QMessageBox::warning(
            this, "Assignment Denied",
            "Could not assign patient to the room. The patient may already be assigned to another emergency room.");
    }
}

void RoomPage::releaseRoom()
{
    if (!roomManager)
        return;

    int roomId = roomSelector->currentData().toInt();
    if (roomId <= 0)
    {
        QMessageBox::warning(this, "Select Room", "Please select a valid room from the Room dropdown.");
        return;
    }

    Room *room = roomManager->findRoom(roomId);
    if (!room)
    {
        QMessageBox::warning(this, "Room Not Found", "The selected room could not be found.");
        return;
    }

    if (room->status == AVAILABLE)
    {
        QMessageBox::information(
            this, "Room Already Available",
            QString("Room %1 is already available (no patient assigned).")
                .arg(QString::fromStdString(room->roomNumber)));
        return;
    }

    bool success = roomManager->releaseRoom(roomId);
    if (success)
    {
        refreshAll();
        emit dataChanged();

        QMessageBox::information(
            this, "Room Released",
            QString("Room %1 has been released and is now available.")
                .arg(QString::fromStdString(room->roomNumber)));
    }
    else
    {
        QMessageBox::warning(this, "Release Failed", "Failed to release the selected room.");
    }
}
