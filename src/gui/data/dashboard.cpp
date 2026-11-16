#include "../include/dashboard.h"
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStringList>
#include <QColor>
#include <QFont>

// ==========================================================
// 📊 CUSTOM DONUT CHART - FIVE TRIAGE PRIORITIES
// ==========================================================

class PriorityDonutChart : public QWidget
{
public:
    explicit PriorityDonutChart(
        const QVector<int> &counts,
        QWidget *parent = nullptr)
        : QWidget(parent), priorityCounts(counts)
    {
        setMinimumSize(190, 190);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // ✏️ MANUAL CHANGE: Priority colors
        const QVector<QColor> colors = {
            QColor("#EF4444"), // Resuscitation
            QColor("#F97316"), // Emergent
            QColor("#EAB308"), // Urgent
            QColor("#22C55E"), // Less Urgent
            QColor("#3B82F6")  // Non-Urgent
        };

        int total = 0;

        for (int count : priorityCounts)
            total += count;

        // Chart dimensions
        QRectF chartRect(
            (width() - 160) / 2.0,
            (height() - 160) / 2.0,
            160,
            160);

        const int penWidth = 28;

        if (total > 0)
        {
            int startAngle = 90 * 16;

            for (int i = 0; i < priorityCounts.size(); i++)
            {
                if (priorityCounts[i] <= 0)
                    continue;

                // Calculate each segment's angle.
                int spanAngle = qRound(
                    360.0 * 16 * priorityCounts[i] / total);

                painter.setPen(Qt::NoPen);
                painter.setBrush(colors[i]);

                painter.drawPie(
                    chartRect,
                    startAngle,
                    spanAngle);

                startAngle += spanAngle;
            }
        }
        else
        {
            // Empty chart when there are no patients.
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#EAECF0"));
            painter.drawEllipse(chartRect);
        }

        // Create the donut hole.
        QRectF innerRect = chartRect.adjusted(
            penWidth, penWidth, -penWidth, -penWidth);

        painter.setBrush(Qt::white);
        painter.drawEllipse(innerRect);

        // Center text
        painter.setPen(QColor("#172337"));

        QFont totalFont = painter.font();
        totalFont.setPointSize(24);
        totalFont.setBold(true);
        painter.setFont(totalFont);

        painter.drawText(
            innerRect.adjusted(0, -8, 0, -8),
            Qt::AlignCenter,
            QString::number(total));

        QFont labelFont = painter.font();
        labelFont.setPointSize(9);
        labelFont.setBold(false);
        painter.setFont(labelFont);
        painter.setPen(QColor("#667085"));

        painter.drawText(
            innerRect.adjusted(0, 23, 0, 23),
            Qt::AlignCenter,
            "Patients");
    }

private:
    QVector<int> priorityCounts;
};

// ==========================================================
// 🧩 REUSABLE COMPONENT: CONTENT PANEL
// ==========================================================

static QFrame *createPanel(
    const QString &title,
    QVBoxLayout *&contentLayout)
{
    QFrame *panel = new QFrame();
    panel->setObjectName("contentPanel");

    contentLayout = new QVBoxLayout(panel);
    contentLayout->setContentsMargins(20, 18, 20, 18);
    contentLayout->setSpacing(14);

    QLabel *heading = new QLabel(title);
    heading->setObjectName("sectionTitle");

    contentLayout->addWidget(heading);

    return panel;
}

// ==========================================================
// 🧩 REUSABLE COMPONENT: STATISTICS CARD
// ==========================================================

static QFrame *createStatCard(
    const QString &title,
    const QString &value,
    const QString &description)
{
    QFrame *card = new QFrame();
    card->setObjectName("dashboardCard");
    card->setMinimumHeight(125);

    QVBoxLayout *layout = new QVBoxLayout(card);

    // ✏️ MANUAL CHANGE:
    // Adjust internal card spacing here.
    layout->setContentsMargins(18, 15, 18, 15);
    layout->setSpacing(7);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("cardTitle");

    QLabel *valueLabel = new QLabel(value);
    valueLabel->setObjectName("cardValue");

    QLabel *descriptionLabel = new QLabel(description);
    descriptionLabel->setObjectName("cardDescription");

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    layout->addWidget(descriptionLabel);
    layout->addStretch();

    return card;
}

// ==========================================================
// 🏠 DASHBOARD CONSTRUCTOR
// ==========================================================

Dashboard::Dashboard(
    std::function<void(int)> navigate,
    QWidget *parent)
    : QWidget(parent)
{
    setObjectName("dashboardPage");

    // 📜 Scrollable dashboard
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setObjectName("dashboardScroll");

    QWidget *container = new QWidget();
    container->setObjectName("dashboardContainer");

    QVBoxLayout *mainLayout = new QVBoxLayout(container);

    // ✏️ MANUAL CHANGE: Dashboard outer spacing.
    mainLayout->setContentsMargins(28, 25, 28, 25);
    mainLayout->setSpacing(20);

    // ======================================================
    // 01. WELCOME HEADER
    // ======================================================

    QHBoxLayout *headerLayout = new QHBoxLayout();

    QVBoxLayout *headingLayout = new QVBoxLayout();
    headingLayout->setSpacing(5);

    QLabel *welcome =
        new QLabel("Welcome, Administrator");

    welcome->setObjectName("pageTitle");

    QLabel *subtitle = new QLabel(
        "Emergency Management Dashboard  |  Overview");

    subtitle->setObjectName("pageSubtitle");

    headingLayout->addWidget(welcome);
    headingLayout->addWidget(subtitle);

    QLabel *demoLabel = new QLabel("DEMO DATA");
    demoLabel->setObjectName("demoBadge");
    demoLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addLayout(headingLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(demoLabel);

    mainLayout->addLayout(headerLayout);

    // ======================================================
    // 02. STATISTICS CARDS
    // ======================================================

    QGridLayout *cardLayout = new QGridLayout();
    cardLayout->setSpacing(14);

    // 📊 DEMO DATA
    // ✏️ MANUAL CHANGE:
    // Replace these values with PatientManager,
    // PriorityQueue, and other backend calculations.
    //
    // Do not treat these sample numbers as real records.

    cardLayout->addWidget(
        createStatCard(
            "Total Patients",
            "128",
            "Registered patients"),
        0, 0);

    cardLayout->addWidget(
        createStatCard(
            "Waiting Patients",
            "12",
            "Emergency waiting queue"),
        0, 1);

    cardLayout->addWidget(
        createStatCard(
            "In Treatment",
            "8",
            "Currently receiving care"),
        0, 2);

    cardLayout->addWidget(
        createStatCard(
            "Available Rooms",
            "15",
            "Ready for admission"),
        0, 3);

    for (int col = 0; col < 4; col++)
        cardLayout->setColumnStretch(col, 1);

    mainLayout->addLayout(cardLayout);

    // ======================================================
    // 03. EMERGENCY OVERVIEW - FIVE PRIORITY DONUT CHART
    // ======================================================

    QHBoxLayout *middleLayout = new QHBoxLayout();
    middleLayout->setSpacing(16);

    QVBoxLayout *emergencyLayout = nullptr;

    QFrame *emergencyPanel =
        createPanel("Emergency Overview", emergencyLayout);

    QLabel *emergencySubtitle = new QLabel(
        "Distribution by triage priority");

    emergencySubtitle->setObjectName("pageSubtitle");
    emergencyLayout->addWidget(emergencySubtitle);

    // ======================================================
    // 🚨 DEMO PRIORITY DATA
    //
    // Order must match your TriagePriority enum:
    // 1. RESUSCITATION
    // 2. EMERGENT
    // 3. URGENT
    // 4. LESS_URGENT
    // 5. NON_URGENT
    //
    // ✏️ MANUAL CHANGE:
    // Replace these sample counts with actual backend data.
    // These values are for demonstration only.
    // ======================================================

    QStringList priorityNames = {
        "Resuscitation",
        "Emergent",
        "Urgent",
        "Less Urgent",
        "Non-Urgent"};

    QVector<int> priorityCounts = {
        2, // RESUSCITATION
        3, // EMERGENT
        4, // URGENT
        3, // LESS_URGENT
        3  // NON_URGENT
    };

    QVector<QColor> priorityColors = {
        QColor("#EF4444"), // Red
        QColor("#F97316"), // Orange
        QColor("#EAB308"), // Yellow
        QColor("#22C55E"), // Green
        QColor("#3B82F6")  // Blue
    };

    // Chart and legend layout
    QHBoxLayout *chartLayout = new QHBoxLayout();
    chartLayout->setSpacing(12);

    // Donut chart
    PriorityDonutChart *donutChart =
        new PriorityDonutChart(priorityCounts);

    chartLayout->addWidget(
        donutChart, 0, Qt::AlignCenter);

    // Priority legend
    QVBoxLayout *legendLayout = new QVBoxLayout();
    legendLayout->setSpacing(13);

    for (int i = 0; i < priorityNames.size(); i++)
    {
        QHBoxLayout *legendRow = new QHBoxLayout();
        legendRow->setSpacing(7);

        // Colored indicator
        QLabel *colorIndicator = new QLabel();
        colorIndicator->setFixedSize(10, 10);

        colorIndicator->setStyleSheet(
            QString(
                "background-color: %1;"
                "border-radius: 5px;")
                .arg(priorityColors[i].name()));

        // Priority name
        QLabel *priorityLabel =
            new QLabel(priorityNames[i]);

        priorityLabel->setObjectName("severityLabel");

        // Patient count
        QLabel *countLabel =
            new QLabel(QString::number(priorityCounts[i]));

        countLabel->setObjectName("severityCount");
        countLabel->setAlignment(Qt::AlignRight);

        legendRow->addWidget(colorIndicator);
        legendRow->addWidget(priorityLabel, 1);
        legendRow->addWidget(countLabel);

        legendLayout->addLayout(legendRow);
    }

    legendLayout->addStretch();

    chartLayout->addLayout(legendLayout, 1);

    emergencyLayout->addLayout(chartLayout);
    emergencyLayout->addStretch();

    // ======================================================
    // 04. RECENT ACTIVITIES
    // ======================================================

    QVBoxLayout *activityLayout = nullptr;

    QFrame *activityPanel =
        createPanel("Recent Activities", activityLayout);

    // 📝 DEMO DATA
    // ✏️ MANUAL CHANGE:
    // Later replace these with actual application events.

    QStringList activities = {
        "Patient registration completed",
        "Emergency triage assigned",
        "Patient moved to treatment",
        "Treatment record updated"};

    QStringList activityTimes = {
        "09:15",
        "09:10",
        "08:55",
        "08:40"};

    for (int i = 0; i < activities.size(); i++)
    {
        QHBoxLayout *row = new QHBoxLayout();

        QLabel *activity =
            new QLabel("•  " + activities[i]);

        activity->setObjectName("activityText");

        QLabel *time = new QLabel(activityTimes[i]);
        time->setObjectName("activityTime");

        row->addWidget(activity);
        row->addStretch();
        row->addWidget(time);

        activityLayout->addLayout(row);
    }

    activityLayout->addStretch();

    middleLayout->addWidget(emergencyPanel, 1);
    middleLayout->addWidget(activityPanel, 1);

    mainLayout->addLayout(middleLayout);

    // ======================================================
    // 05. EMERGENCY QUEUE PREVIEW
    // ======================================================

    QVBoxLayout *queueLayout = nullptr;

    QFrame *queuePanel =
        createPanel("Emergency Queue - Preview", queueLayout);

    QTableWidget *queueTable = new QTableWidget(3, 4);

    queueTable->setHorizontalHeaderLabels({"Patient ID",
                                           "Patient Name",
                                           "Priority",
                                           "Status"});

    queueTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    queueTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    queueTable->setAlternatingRowColors(true);

    queueTable->verticalHeader()->setVisible(false);

    queueTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    queueTable->setMinimumHeight(160);
    queueTable->setMaximumHeight(185);

    // 🧪 SAMPLE RECORDS - NOT REAL PATIENTS
    // ✏️ MANUAL CHANGE:
    // Later load records from PriorityQueue.

    QString queueData[3][4] = {
        {"DEMO-001", "Sample Patient A", "Critical", "Waiting"},
        {"DEMO-002", "Sample Patient B", "Major", "Waiting"},
        {"DEMO-003", "Sample Patient C", "Moderate", "In Triage"}};

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            QTableWidgetItem *item =
                new QTableWidgetItem(queueData[row][col]);

            if (col == 2)
            {
                const QString priority = queueData[row][col];

                if (priority == "Critical")
                    item->setForeground(QColor("#EF4444"));
                else if (priority == "Major")
                    item->setForeground(QColor("#F97316"));
                else if (priority == "Moderate")
                    item->setForeground(QColor("#EAB308"));
                else
                    item->setForeground(QColor("#22C55E"));

                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
            }

            queueTable->setItem(row, col, item);
        }
    }

    queueLayout->addWidget(queueTable);
    mainLayout->addWidget(queuePanel);

    // ======================================================
    // 06. QUICK ACTIONS
    // ======================================================

    QVBoxLayout *actionLayout = nullptr;

    QFrame *actionPanel =
        createPanel("Quick Actions", actionLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);

    QStringList actionNames = {
        "+ Register Patient",
        "Emergency Triage",
        "View Queue",
        "Treatment Records"};

    // 🧭 Match MainWindow's QStackedWidget indices.
    // ✏️ MANUAL CHANGE:
    // Update indices if you rearrange pages.

    int pageIndices[] = {1, 2, 3, 4};

    for (int i = 0; i < actionNames.size(); i++)
    {
        QPushButton *button =
            new QPushButton(actionNames[i]);

        button->setObjectName("quickActionButton");
        button->setMinimumHeight(45);
        button->setCursor(Qt::PointingHandCursor);

        const int targetPage = pageIndices[i];

        // 🔗 Navigate using the callback from MainWindow.
        // The target pages may still be placeholders.
        if (navigate)
        {
            connect(button, &QPushButton::clicked,
                    this, [navigate, targetPage]()
                    { navigate(targetPage); });
        }
        else
        {
            button->setEnabled(false);
        }

        buttonLayout->addWidget(button);
    }

    actionLayout->addLayout(buttonLayout);
    mainLayout->addWidget(actionPanel);

    mainLayout->addStretch();

    // ======================================================
    // 07. FINAL SCROLLABLE LAYOUT
    // ======================================================

    scrollArea->setWidget(container);
    rootLayout->addWidget(scrollArea);
}
