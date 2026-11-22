#include "../include/room.h"
#include "../../include/PatientManager.h"
#include "../../model/Patient.h"

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

RoomPage::RoomPage(PatientManager *mgr, QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      roomSelector(nullptr),
      patientSelector(nullptr),
      roomTable(nullptr),
      totalValue(nullptr),
      availableValue(nullptr),
      occupiedValue(nullptr)
{
    rooms = {
        {1, "R-1", 1001, "John Doe"},
        {2, "R-2", -1, ""},
        {3, "R-3", 1002, "Mary Smith"},
        {4, "R-4", -1, ""},
        {5, "R-5", 1003, "David Lee"},
        {6, "R-6", -1, ""},
        {7, "R-7", -1, ""},
        {8, "R-8", 1004, "Emily Chen"},
        {9, "R-9", -1, ""},
        {10, "R-10", -1, ""}};

    buildInterface();
    refreshRoomTable();
    refreshRoomSelector();
    refreshPatients();
    updateSummaryCards();
}

QWidget *RoomPage::buildHeader()
{
    QWidget *header = new QWidget;
    header->setMinimumHeight(100);
    header->setStyleSheet("background-color: #eef6ff; border-radius: 12px; margin-bottom: 5px;");

    QHBoxLayout *layout = new QHBoxLayout(header);
    layout->setContentsMargins(25, 15, 25, 15);

    QLabel *icon = new QLabel("🛏");
    icon->setStyleSheet("font-size: 40px; color: #1766a8;");
    layout->addWidget(icon);

    QVBoxLayout *textLayout = new QVBoxLayout;

    QLabel *title = new QLabel("Room Management");
    title->setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "color: #102f50;");

    QLabel *subtitle = new QLabel(
        "Manage emergency rooms and patient assignments");
    subtitle->setStyleSheet("font-size: 14px; color: #294d70;");

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
    card->setMinimumHeight(110);
    card->setStyleSheet(
        QString(
            "QFrame { background-color: %1; border-radius: 14px; }"
            "QLabel { border: none; }")
            .arg(background));

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(22, 15, 22, 15);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #153654;");

    valueLabel = new QLabel("0");
    valueLabel->setStyleSheet(
        QString("font-size: 32px; font-weight: bold; color: %1;")
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
        " background-color: white;"
        " border: 1px solid #d2dfed;"
        " border-radius: 14px;"
        "}");

    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *heading = new QLabel("  " + title);
    heading->setMinimumHeight(52);
    heading->setStyleSheet(
        "QLabel {"
        " background-color: #10589a;"
        " color: white;"
        " font-size: 18px;"
        " font-weight: bold;"
        " border: none;"
        " border-top-left-radius: 13px;"
        " border-top-right-radius: 13px;"
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
    button->setMinimumHeight(48);
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(
        QString(
            "QPushButton {"
            " background-color: %1;"
            " color: white;"
            " font-size: 15px;"
            " font-weight: bold;"
            " border: none;"
            " border-radius: 9px;"
            "}"
            "QPushButton:hover { background-color: #164e82; }"
            "QPushButton:pressed { background-color: #0c3a65; }")
            .arg(background));
    return button;
}

void RoomPage::buildInterface()
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(25, 20, 25, 25);
    rootLayout->setSpacing(16);

    rootLayout->addWidget(buildHeader());

    // Summary cards
    QHBoxLayout *cardsLayout = new QHBoxLayout;
    cardsLayout->setSpacing(15);

    QFrame *totalCard = buildSummaryCard(
        "Total Rooms", totalValue, "#e2f0ff", "#1766b0");

    QFrame *availableCard = buildSummaryCard(
        "Available Rooms", availableValue, "#e3f8ed", "#159653");

    QFrame *occupiedCard = buildSummaryCard(
        "Occupied Rooms", occupiedValue, "#ffebeb", "#d92d2d");

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
    actionsPanel->setMaximumWidth(400);

    QVBoxLayout *actionsLayout =
        qobject_cast<QVBoxLayout *>(actionsPanel->layout());

    QWidget *actionsBody = new QWidget;
    QVBoxLayout *bodyLayout = new QVBoxLayout(actionsBody);
    bodyLayout->setContentsMargins(18, 16, 18, 18);
    bodyLayout->setSpacing(12);

    QLabel *roomLabel = new QLabel("Room ID");
    roomLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #172337;");

    roomSelector = new QComboBox;
    roomSelector->setMinimumHeight(44);
    roomSelector->setStyleSheet(
        "QComboBox {"
        " background: white;"
        " border: 1px solid #c7d6e8;"
        " border-radius: 8px;"
        " padding: 6px;"
        " font-size: 14px;"
        "}");

    QLabel *patientLabel = new QLabel("Patient ID");
    patientLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #172337;");

    patientSelector = new QComboBox;
    patientSelector->setMinimumHeight(44);
    patientSelector->setStyleSheet(
        "QComboBox {"
        " background: white;"
        " border: 1px solid #c7d6e8;"
        " border-radius: 8px;"
        " padding: 6px;"
        " font-size: 14px;"
        "}");

    bodyLayout->addWidget(roomLabel);
    bodyLayout->addWidget(roomSelector);
    bodyLayout->addSpacing(6);
    bodyLayout->addWidget(patientLabel);
    bodyLayout->addWidget(patientSelector);
    bodyLayout->addSpacing(8);

    QPushButton *addButton =
        makeActionButton("＋  Add Room", "#159653");
    QPushButton *assignButton =
        makeActionButton("♟  Assign Patient", "#1673c8");
    QPushButton *releaseButton =
        makeActionButton("↩  Release Room", "#df302b");

    bodyLayout->addWidget(addButton);
    bodyLayout->addWidget(assignButton);
    bodyLayout->addWidget(releaseButton);
    bodyLayout->addStretch();

    actionsLayout->addWidget(actionsBody);

    // Room list panel
    QFrame *tablePanel = buildPanel("☷  Room List");
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
    roomTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    roomTable->setMinimumHeight(400);
    roomTable->setStyleSheet(
        "QTableWidget {"
        " background-color: white;"
        " alternate-background-color: #eef4fb;"
        " gridline-color: #d8e2ef;"
        " border: none;"
        " font-size: 14px;"
        "}"
        "QHeaderView::section {"
        " background-color: #dcecff;"
        " color: #153654;"
        " font-size: 14px;"
        " font-weight: bold;"
        " padding: 10px 5px;"
        " border: none;"
        " border-bottom: 1px solid #c9d8e8;"
        "}"
        "QTableWidget::item { padding: 6px; }"
        "QTableWidget::item:selected {"
        " background-color: #cde5ff;"
        " color: #102f50;"
        "}");

    tablePanelLayout->addWidget(roomTable);
    mainContentLayout->addWidget(actionsPanel, 1);
    mainContentLayout->addWidget(tablePanel, 3);

    rootLayout->addLayout(mainContentLayout);

    // Button actions
    connect(addButton, &QPushButton::clicked,
            this, [this]()
            { addRoom(); });

    connect(assignButton, &QPushButton::clicked,
            this, [this]()
            { assignPatient(); });

    connect(releaseButton, &QPushButton::clicked,
            this, [this]()
            { releaseRoom(); });

    connect(roomTable, &QTableWidget::cellClicked,
            this, [this](int row, int)
            {
        if (row < 0 || row >= rooms.size())
            return;

        roomSelector->setCurrentIndex(row); });

    connect(roomSelector, &QComboBox::currentIndexChanged,
            this, [this](int index)
            {
        if (index >= 0 && index < rooms.size())
            roomTable->selectRow(index); });
}

void RoomPage::refreshRoomTable()
{
    roomTable->setRowCount(rooms.size());

    for (int row = 0; row < rooms.size(); ++row)
    {
        const RoomData &room = rooms[row];

        roomTable->setItem(
            row, 0, new QTableWidgetItem(QString::number(room.id)));

        roomTable->setItem(
            row, 1, new QTableWidgetItem(room.number));

        QString patientIdText =
            room.patientId == -1
                ? "—"
                : QString::number(room.patientId);

        QString patientNameText =
            room.patientId == -1 ? "—" : room.patientName;

        roomTable->setItem(
            row, 2, new QTableWidgetItem(patientIdText));

        roomTable->setItem(
            row, 3, new QTableWidgetItem(patientNameText));

        QTableWidgetItem *statusItem = new QTableWidgetItem(
            room.isAvailable() ? "Available" : "Occupied");

        statusItem->setTextAlignment(Qt::AlignCenter);
        statusItem->setFont(QFont("Arial", 10, QFont::Bold));

        if (room.isAvailable())
        {
            statusItem->setForeground(QColor("#118344"));
        }
        else
        {
            statusItem->setForeground(QColor("#c62828"));
        }

        roomTable->setItem(row, 4, statusItem);

        for (int col = 0; col < roomTable->columnCount(); ++col)
        {
            if (roomTable->item(row, col))
                roomTable->item(row, col)->setTextAlignment(
                    Qt::AlignCenter);
        }
    }

    roomTable->resizeRowsToContents();
}

void RoomPage::refreshRoomSelector()
{
    roomSelector->clear();

    for (const RoomData &room : rooms)
    {
        roomSelector->addItem(
            QString("%1 (ID: %2)").arg(room.number).arg(room.id),
            room.id);
    }
}

void RoomPage::refreshPatients()
{
    patientSelector->clear();
    patientSelector->addItem("Select a patient", -1);

    if (patientManager)
    {
        for (int i = 0; i < patientManager->getPatientCount(); ++i)
        {
            Patient *p = patientManager->getPatientAt(i);
            if (p)
            {
                patientSelector->addItem(
                    QString("%1 – %2").arg(p->getId()).arg(QString::fromStdString(p->getName())),
                    p->getId());
            }
        }
    }
}

void RoomPage::updateSummaryCards()
{
    int available = 0;

    for (const RoomData &room : rooms)
    {
        if (room.isAvailable())
            ++available;
    }

    int occupied = rooms.size() - available;

    totalValue->setText(QString::number(rooms.size()));
    availableValue->setText(QString::number(available));
    occupiedValue->setText(QString::number(occupied));
}

int RoomPage::selectedRoomIndex() const
{
    int roomId = roomSelector->currentData().toInt();

    for (int i = 0; i < rooms.size(); ++i)
    {
        if (rooms[i].id == roomId)
            return i;
    }

    return -1;
}

void RoomPage::addRoom()
{
    if (rooms.size() >= 20)
    {
        QMessageBox::warning(
            this, "Room Limit", "The maximum number of rooms is 20.");
        return;
    }

    int newId = 1;

    for (const RoomData &room : rooms)
        newId = qMax(newId, room.id + 1);

    RoomData newRoom;
    newRoom.id = newId;
    newRoom.number = QString("R-%1").arg(newId);
    newRoom.patientId = -1;
    newRoom.patientName = "";

    rooms.append(newRoom);

    refreshRoomTable();
    refreshRoomSelector();
    updateSummaryCards();

    roomSelector->setCurrentIndex(roomSelector->count() - 1);
    emit dataChanged();

    QMessageBox::information(
        this, "Room Added",
        QString("Room %1 was added.").arg(newRoom.number));
}

void RoomPage::assignPatient()
{
    int index = selectedRoomIndex();

    if (index == -1)
    {
        QMessageBox::warning(this, "Select Room", "Please select a room.");
        return;
    }

    if (!rooms[index].isAvailable())
    {
        QMessageBox::warning(
            this, "Room Occupied",
            "This room already has a patient. Release it first.");
        return;
    }

    int patientId = patientSelector->currentData().toInt();

    if (patientId == -1)
    {
        QMessageBox::warning(
            this, "Select Patient", "Please select a patient.");
        return;
    }

    for (const RoomData &room : rooms)
    {
        if (room.patientId == patientId)
        {
            QMessageBox::warning(
                this, "Patient Already Assigned",
                "This patient is already assigned to a room.");
            return;
        }
    }

    QString patientName = "";
    if (patientManager)
    {
        Patient *p = patientManager->findPatientInfo(patientId);
        if (p)
            patientName = QString::fromStdString(p->getName());
    }

    if (patientName.isEmpty())
    {
        QString text = patientSelector->currentText();
        int dash = text.indexOf("–");
        if (dash == -1) dash = text.indexOf("-");
        if (dash != -1)
            patientName = text.mid(dash + 1).trimmed();
        else
            patientName = text;
    }

    rooms[index].patientId = patientId;
    rooms[index].patientName = patientName;

    refreshRoomTable();
    updateSummaryCards();
    emit dataChanged();

    QMessageBox::information(
        this, "Patient Assigned",
        "The patient has been assigned to the selected room.");
}

void RoomPage::releaseRoom()
{
    int index = selectedRoomIndex();

    if (index == -1)
    {
        QMessageBox::warning(this, "Select Room", "Please select a room.");
        return;
    }

    if (rooms[index].isAvailable())
    {
        QMessageBox::information(
            this, "Room Available",
            "This room is already available.");
        return;
    }

    rooms[index].patientId = -1;
    rooms[index].patientName = "";

    refreshRoomTable();
    updateSummaryCards();
    emit dataChanged();

    QMessageBox::information(
        this, "Room Released",
        "The patient was released from the room.");
}
