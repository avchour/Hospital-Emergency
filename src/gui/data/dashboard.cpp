#include "../include/dashboard.h"
#include "../../include/PatientManager.h"
#include "../../include/PriorityQueue.h"
#include "../../include/Queue.h"

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

    void setCounts(const QVector<int> &counts)
    {
        priorityCounts = counts;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

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

        QRectF chartRect(15, 15, width() - 30, height() - 30);

        if (total == 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#E2E8F0"));
            painter.drawEllipse(chartRect);

            QRectF holeRect(
                chartRect.center().x() - 48,
                chartRect.center().y() - 48,
                96, 96);
            painter.setBrush(Qt::white);
            painter.drawEllipse(holeRect);

            painter.setPen(QColor("#64748B"));
            painter.setFont(QFont("Segoe UI", 10, QFont::Bold));
            painter.drawText(holeRect, Qt::AlignCenter, "0 Patients");
            return;
        }

        int startAngle = 90 * 16;
        for (int i = 0; i < priorityCounts.size(); i++)
        {
            if (priorityCounts[i] == 0) continue;
            int spanAngle = (priorityCounts[i] * 360 * 16) / total;

            painter.setPen(Qt::NoPen);
            painter.setBrush(colors[i % colors.size()]);
            painter.drawPie(chartRect, startAngle, spanAngle);

            startAngle += spanAngle;
        }

        QRectF holeRect(
            chartRect.center().x() - 48,
            chartRect.center().y() - 48,
            96, 96);

        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(holeRect);

        painter.setPen(QColor("#0F172A"));
        painter.setFont(QFont("Segoe UI", 16, QFont::Bold));

        QRectF textRect(holeRect.left(), holeRect.top() - 8, holeRect.width(), holeRect.height() / 2);
        painter.drawText(textRect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(total));

        painter.setPen(QColor("#64748B"));
        painter.setFont(QFont("Segoe UI", 9));
        QRectF subRect(holeRect.left(), holeRect.center().y(), holeRect.width(), holeRect.height() / 2);
        painter.drawText(subRect, Qt::AlignTop | Qt::AlignHCenter, "In Queue");
    }

private:
    QVector<int> priorityCounts;
};

static QFrame *createPanel(const QString &title, QVBoxLayout *&contentLayout)
{
    QFrame *panel = new QFrame();
    panel->setObjectName("contentPanel");

    contentLayout = new QVBoxLayout(panel);
    contentLayout->setContentsMargins(20, 18, 20, 18);
    contentLayout->setSpacing(14);

    QLabel *heading = new QLabel(title);
    heading->setObjectName("sectionTitle");
    heading->setStyleSheet("font-size: 18px; font-weight: bold; color: #172554;");
    contentLayout->addWidget(heading);

    return panel;
}

static QFrame *createStatCard(const QString &title, const QString &description, QLabel *&valRef)
{
    QFrame *card = new QFrame();
    card->setObjectName("dashboardCard");
    card->setMinimumHeight(125);
    card->setStyleSheet(
        "QFrame {"
        "background-color: white;"
        "border: 1px solid #E2E8F0;"
        "border-radius: 8px;"
        "padding: 12px;"
        "}"
    );

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(18, 15, 18, 15);
    layout->setSpacing(7);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 14px; color: #64748B; font-weight: bold;");

    valRef = new QLabel("0");
    valRef->setStyleSheet("font-size: 28px; font-weight: bold; color: #172554;");

    QLabel *descriptionLabel = new QLabel(description);
    descriptionLabel->setStyleSheet("font-size: 12px; color: #94A3B8;");

    layout->addWidget(titleLabel);
    layout->addWidget(valRef);
    layout->addWidget(descriptionLabel);
    layout->addStretch();

    return card;
}

Dashboard::Dashboard(
    PatientManager *mgr,
    PriorityQueue *pq,
    Queue *normQueue,
    std::function<void(int)> navigate,
    QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      priorityQueue(pq),
      normalQueue(normQueue),
      navigateFn(navigate),
      totalPatientsVal(nullptr),
      emergencyWaitingVal(nullptr),
      normalWaitingVal(nullptr),
      treatedCountVal(nullptr),
      donutChart(nullptr),
      queueTable(nullptr)
{
    for (int i = 0; i < 5; i++)
        priorityCountLabels[i] = nullptr;

    setupUI();
    refreshStats();
}

void Dashboard::setupUI()
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *container = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(24, 20, 24, 24);
    mainLayout->setSpacing(20);

    // Header
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QVBoxLayout *headingLayout = new QVBoxLayout();

    QLabel *welcome = new QLabel("Welcome, Hospital Administrator");
    welcome->setStyleSheet("font-size: 24px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("Emergency Management System  |  Real-Time Overview");
    subtitle->setStyleSheet("font-size: 14px; color: #64748B;");

    headingLayout->addWidget(welcome);
    headingLayout->addWidget(subtitle);
    headerLayout->addLayout(headingLayout);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // 4 Stat Cards
    QGridLayout *cardLayout = new QGridLayout();
    cardLayout->setSpacing(14);

    cardLayout->addWidget(createStatCard("Registered Patients", "Total registered in system", totalPatientsVal), 0, 0);
    cardLayout->addWidget(createStatCard("Emergency Queue", "Waiting for emergency triage", emergencyWaitingVal), 0, 1);
    cardLayout->addWidget(createStatCard("Normal Queue", "Outpatient FIFO queue", normalWaitingVal), 0, 2);
    cardLayout->addWidget(createStatCard("System Status", "All backend queues active", treatedCountVal), 0, 3);

    for (int col = 0; col < 4; col++)
        cardLayout->setColumnStretch(col, 1);

    mainLayout->addLayout(cardLayout);

    // Middle Section: Donut chart & legend
    QHBoxLayout *middleLayout = new QHBoxLayout();
    middleLayout->setSpacing(16);

    QVBoxLayout *emergencyLayout = nullptr;
    QFrame *emergencyPanel = createPanel("Emergency Priority Distribution", emergencyLayout);

    QHBoxLayout *chartLayout = new QHBoxLayout();
    chartLayout->setSpacing(16);

    donutChart = new PriorityDonutChart({0, 0, 0, 0, 0});
    chartLayout->addWidget(donutChart, 0, Qt::AlignCenter);

    QVBoxLayout *legendLayout = new QVBoxLayout();
    legendLayout->setSpacing(10);

    QStringList priorityNames = {
        "Resuscitation (Level 1)",
        "Emergent (Level 2)",
        "Urgent (Level 3)",
        "Less Urgent (Level 4)",
        "Non-Urgent (Level 5)"
    };

    QVector<QColor> priorityColors = {
        QColor("#EF4444"),
        QColor("#F97316"),
        QColor("#EAB308"),
        QColor("#22C55E"),
        QColor("#3B82F6")
    };

    for (int i = 0; i < 5; i++)
    {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *dot = new QLabel();
        dot->setFixedSize(12, 12);
        dot->setStyleSheet(QString("background-color: %1; border-radius: 6px;").arg(priorityColors[i].name()));

        QLabel *lbl = new QLabel(priorityNames[i]);
        lbl->setStyleSheet("font-size: 13px; color: #334155; font-weight: 500;");

        priorityCountLabels[i] = new QLabel("0");
        priorityCountLabels[i]->setStyleSheet("font-size: 14px; font-weight: bold; color: #0F172A;");

        row->addWidget(dot);
        row->addWidget(lbl, 1);
        row->addWidget(priorityCountLabels[i]);
        legendLayout->addLayout(row);
    }

    chartLayout->addLayout(legendLayout);
    emergencyLayout->addLayout(chartLayout);
    middleLayout->addWidget(emergencyPanel, 3);

    // Quick Actions Panel
    QVBoxLayout *actionLayout = nullptr;
    QFrame *actionPanel = createPanel("Quick Navigation", actionLayout);

    QStringList actionNames = {
        "♙ Patient Management",
        "♥ Emergency Triage",
        "☷ Emergency Queue",
        "✚ Normal Queue & Treatment"
    };

    int pageIndices[] = {1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
    {
        QPushButton *btn = new QPushButton(actionNames[i]);
        btn->setMinimumHeight(44);
        btn->setStyleSheet(
            "QPushButton {"
            "background-color: #F8FAFC;"
            "color: #1E293B;"
            "border: 1px solid #CBD5E1;"
            "border-radius: 6px;"
            "font-weight: bold;"
            "text-align: left;"
            "padding-left: 16px;"
            "}"
            "QPushButton:hover { background-color: #E2E8F0; }"
        );

        int target = pageIndices[i];
        if (navigateFn)
        {
            connect(btn, &QPushButton::clicked, [this, target]() {
                navigateFn(target);
            });
        }
        actionLayout->addWidget(btn);
    }
    actionLayout->addStretch();
    middleLayout->addWidget(actionPanel, 2);

    mainLayout->addLayout(middleLayout);

    // Bottom Section: Priority Emergency Queue preview table
    QVBoxLayout *queueLayout = nullptr;
    QFrame *queuePanel = createPanel("High-Priority Emergency Queue (Live)", queueLayout);

    queueTable = new QTableWidget();
    queueTable->setColumnCount(5);
    queueTable->setHorizontalHeaderLabels({
        "Priority", "Patient ID", "Name", "Age", "Symptoms"
    });
    queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    queueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    queueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    queueTable->verticalHeader()->setVisible(false);
    queueTable->setMaximumHeight(180);
    queueTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white; border: 1px solid #E2E8F0; border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC; color: #475569; font-weight: bold; padding: 6px;"
        "border: none; border-bottom: 1px solid #E2E8F0;"
        "}"
    );

    queueLayout->addWidget(queueTable);
    mainLayout->addWidget(queuePanel);

    scrollArea->setWidget(container);
    rootLayout->addWidget(scrollArea);
}

void Dashboard::refreshStats()
{
    int totalRegistered = patientManager ? patientManager->getPatientCount() : 0;
    int emergencyWaiting = priorityQueue ? priorityQueue->getQueueCount() : 0;

    int normalWaiting = 0;
    if (normalQueue)
    {
        for (Node *cur = normalQueue->head; cur != nullptr; cur = cur->next)
            normalWaiting++;
    }

    if (totalPatientsVal)
        totalPatientsVal->setText(QString::number(totalRegistered));

    if (emergencyWaitingVal)
        emergencyWaitingVal->setText(QString::number(emergencyWaiting));

    if (normalWaitingVal)
        normalWaitingVal->setText(QString::number(normalWaiting));

    if (treatedCountVal)
        treatedCountVal->setText("Online");

    // Donut chart & legend
    std::array<int, 5> pCounts = {0, 0, 0, 0, 0};
    if (priorityQueue)
        pCounts = priorityQueue->getPriorityCounts();

    QVector<int> countsVec = {pCounts[0], pCounts[1], pCounts[2], pCounts[3], pCounts[4]};

    if (donutChart)
        donutChart->setCounts(countsVec);

    for (int i = 0; i < 5; i++)
    {
        if (priorityCountLabels[i])
            priorityCountLabels[i]->setText(QString::number(pCounts[i]));
    }

    // Queue preview table
    if (queueTable && priorityQueue && patientManager)
    {
        queueTable->setRowCount(0);
        int previewCount = std::min(4, priorityQueue->getQueueCount());

        for (int i = 0; i < previewCount; i++)
        {
            TriageRecord *tr = priorityQueue->getQueueRecordAt(i);
            if (!tr) continue;

            Patient *p = patientManager->findPatientInfo(tr->patientID);
            int row = queueTable->rowCount();
            queueTable->insertRow(row);

            QString pName;
            switch (tr->priority)
            {
            case RESUSCITATION: pName = "Resuscitation (1)"; break;
            case EMERGENT:      pName = "Emergent (2)"; break;
            case URGENT:        pName = "Urgent (3)"; break;
            case LESS_URGENT:   pName = "Less Urgent (4)"; break;
            case NON_URGENT:    pName = "Non Urgent (5)"; break;
            }

            QTableWidgetItem *pItem = new QTableWidgetItem(pName);
            QFont f = pItem->font();
            f.setBold(true);
            pItem->setFont(f);

            if (tr->priority == RESUSCITATION) pItem->setForeground(QColor("#EF4444"));
            else if (tr->priority == EMERGENT) pItem->setForeground(QColor("#F97316"));
            else if (tr->priority == URGENT)   pItem->setForeground(QColor("#EAB308"));
            else if (tr->priority == LESS_URGENT) pItem->setForeground(QColor("#22C55E"));
            else pItem->setForeground(QColor("#3B82F6"));

            queueTable->setItem(row, 0, pItem);
            queueTable->setItem(row, 1, new QTableWidgetItem(QString::number(tr->patientID)));
            queueTable->setItem(row, 2, new QTableWidgetItem(p ? QString::fromStdString(p->name) : "Unknown"));
            queueTable->setItem(row, 3, new QTableWidgetItem(p ? QString::number(p->age) : "-"));
            queueTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(tr->symptoms)));
        }
    }
}
