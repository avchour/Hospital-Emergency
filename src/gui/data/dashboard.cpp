#include "../include/dashboard.h"

#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #F4F6FA;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 25, 30, 25);
    mainLayout->setSpacing(20);

    // Dashboard heading
    QLabel *title = new QLabel("Dashboard");
    title->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "color: #172B4D;");

    QLabel *subtitle = new QLabel(
        "Hospital Emergency Management Overview");

    subtitle->setStyleSheet(
        "font-size: 14px;"
        "color: #7A869A;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Statistics cards
    QGridLayout *cardLayout = new QGridLayout();
    cardLayout->setSpacing(18);

    QString cardStyle =
        "QFrame {"
        "background-color: white;"
        "border-radius: 10px;"
        "border: 1px solid #E5E9F0;"
        "}";

    auto createCard = [&](QString heading, QString value,
                          QString description)
    {
        QFrame *card = new QFrame();
        card->setStyleSheet(cardStyle);
        card->setMinimumHeight(135);

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setContentsMargins(20, 15, 20, 15);

        QLabel *headingLabel = new QLabel(heading);
        headingLabel->setStyleSheet(
            "font-size: 14px;"
            "color: #667085;"
            "border: none;");

        QLabel *valueLabel = new QLabel(value);
        valueLabel->setStyleSheet(
            "font-size: 30px;"
            "font-weight: bold;"
            "color: #172B4D;"
            "border: none;");

        QLabel *descriptionLabel = new QLabel(description);
        descriptionLabel->setStyleSheet(
            "font-size: 12px;"
            "color: #98A2B3;"
            "border: none;");

        layout->addWidget(headingLabel);
        layout->addWidget(valueLabel);
        layout->addWidget(descriptionLabel);
        layout->addStretch();

        return card;
    };

    cardLayout->addWidget(
        createCard("Total Patients", "128", "Registered patients"),
        0, 0);

    cardLayout->addWidget(
        createCard("Waiting Patients", "12", "Emergency queue"),
        0, 1);

    cardLayout->addWidget(
        createCard("In Treatment", "8", "Currently receiving care"),
        1, 0);

    cardLayout->addWidget(
        createCard("Available Rooms", "15", "Ready for admission"),
        1, 1);

    mainLayout->addLayout(cardLayout);

    // Emergency section
    QLabel *emergencyTitle = new QLabel("Emergency Overview");

    emergencyTitle->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #172B4D;");

    QFrame *emergencyPanel = new QFrame();
    emergencyPanel->setStyleSheet(
        "background-color: white;"
        "border: 1px solid #E5E9F0;"
        "border-radius: 10px;");

    QVBoxLayout *panelLayout = new QVBoxLayout(emergencyPanel);
    panelLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *message = new QLabel(
        "Emergency monitoring will appear here.");

    message->setStyleSheet(
        "font-size: 14px;"
        "color: #667085;"
        "border: none;");

    panelLayout->addWidget(message);

    mainLayout->addWidget(emergencyTitle);
    mainLayout->addWidget(emergencyPanel);
    mainLayout->addStretch();
}
