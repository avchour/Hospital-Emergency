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
#include <QScrollBar>
#include <QSizePolicy>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <QMessageBox>
#include <QLocale>

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
      queueTable(nullptr),
      revenuePriceVal(nullptr),
      revenueTotalVal(nullptr),
      revenueCaseCountVal(nullptr),
      revenueTable(nullptr),
      viewRevenueReportBtn(nullptr)
{
    for (int i = 0; i < 5; i++)
        priorityCountLabels[i] = nullptr;

    setupUI();
    refreshStats();
}

void Dashboard::setupUI()
{
    setObjectName("dashboardPage");

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("dashboardScroll");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setLayoutDirection(Qt::LeftToRight);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->viewport()->setAutoFillBackground(false);

    // Style the vertical scrollbar docked on the right side
    if (scrollArea->verticalScrollBar())
    {
        scrollArea->verticalScrollBar()->setSingleStep(20);
        scrollArea->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical {"
            "background-color: #F1F5F9;"
            "width: 12px;"
            "margin: 0px;"
            "border-radius: 6px;"
            "}"
            "QScrollBar::handle:vertical {"
            "background-color: #94A3B8;"
            "min-height: 36px;"
            "border-radius: 6px;"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "background-color: #64748B;"
            "}"
            "QScrollBar::handle:vertical:pressed {"
            "background-color: #475569;"
            "}"
            "QScrollBar::sub-line:vertical, QScrollBar::add-line:vertical {"
            "height: 0px;"
            "background: none;"
            "border: none;"
            "}"
            "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
            "background: none;"
            "}"
        );
    }

    QWidget *container = new QWidget();
    container->setObjectName("dashboardContainer");
    container->setMinimumHeight(1020);
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

    // Emergency Revenue Panel
    QVBoxLayout *revenueLayout = nullptr;
    QFrame *revenuePanel = createPanel("Emergency Revenue", revenueLayout);

    QHBoxLayout *revMetricsLayout = new QHBoxLayout();
    revMetricsLayout->setSpacing(10);

    // Fixed Service Price Card
    QFrame *priceCard = new QFrame();
    priceCard->setStyleSheet(
        "QFrame {"
        "background-color: #F8FAFC;"
        "border: 1px solid #E2E8F0;"
        "border-radius: 6px;"
        "padding: 6px;"
        "}"
    );
    QVBoxLayout *priceCardLayout = new QVBoxLayout(priceCard);
    priceCardLayout->setContentsMargins(10, 8, 10, 8);
    priceCardLayout->setSpacing(2);

    QLabel *priceTitle = new QLabel("Service Price");
    priceTitle->setStyleSheet("font-size: 11px; font-weight: bold; color: #64748B; border: none;");

    revenuePriceVal = new QLabel("$1,000");
    revenuePriceVal->setStyleSheet("font-size: 18px; font-weight: bold; color: #172554; border: none;");

    QLabel *priceSub = new QLabel("Fixed per emergency case");
    priceSub->setStyleSheet("font-size: 10px; color: #94A3B8; border: none;");

    priceCardLayout->addWidget(priceTitle);
    priceCardLayout->addWidget(revenuePriceVal);
    priceCardLayout->addWidget(priceSub);

    // Total Emergency Revenue Card
    QFrame *totalCard = new QFrame();
    totalCard->setStyleSheet(
        "QFrame {"
        "background-color: #F0FDF4;"
        "border: 1px solid #BBF7D0;"
        "border-radius: 6px;"
        "padding: 6px;"
        "}"
    );
    QVBoxLayout *totalCardLayout = new QVBoxLayout(totalCard);
    totalCardLayout->setContentsMargins(10, 8, 10, 8);
    totalCardLayout->setSpacing(2);

    QLabel *totalTitle = new QLabel("Total Emergency Revenue");
    totalTitle->setStyleSheet("font-size: 11px; font-weight: bold; color: #15803D; border: none;");

    revenueTotalVal = new QLabel("$0");
    revenueTotalVal->setStyleSheet("font-size: 18px; font-weight: bold; color: #16A34A; border: none;");

    revenueCaseCountVal = new QLabel("0 cases at $1,000");
    revenueCaseCountVal->setStyleSheet("font-size: 10px; color: #15803D; border: none;");

    totalCardLayout->addWidget(totalTitle);
    totalCardLayout->addWidget(revenueTotalVal);
    totalCardLayout->addWidget(revenueCaseCountVal);

    revMetricsLayout->addWidget(priceCard);
    revMetricsLayout->addWidget(totalCard);
    revenueLayout->addLayout(revMetricsLayout);

    // Emergency Cases & Revenue Table
    revenueTable = new QTableWidget();
    revenueTable->setColumnCount(3);
    revenueTable->setHorizontalHeaderLabels({"Case ID", "Patient Name", "Price"});
    revenueTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    revenueTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    revenueTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    revenueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    revenueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    revenueTable->verticalHeader()->setVisible(false);
    revenueTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    revenueTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    revenueTable->setMinimumHeight(170);
    revenueTable->setMaximumHeight(220);
    revenueTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white; border: 1px solid #E2E8F0; border-radius: 6px;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC; color: #475569; font-weight: bold; padding: 5px;"
        "border: none; border-bottom: 1px solid #E2E8F0;"
        "}"
    );
    revenueLayout->addWidget(revenueTable);

    // View Revenue Report Button
    viewRevenueReportBtn = new QPushButton("📊  View Revenue Report");
    viewRevenueReportBtn->setMinimumHeight(38);
    viewRevenueReportBtn->setCursor(Qt::PointingHandCursor);
    viewRevenueReportBtn->setStyleSheet(
        "QPushButton {"
        "background-color: #2563EB;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "font-weight: bold;"
        "font-size: 13px;"
        "padding: 6px 14px;"
        "}"
        "QPushButton:hover { background-color: #1D4ED8; }"
        "QPushButton:pressed { background-color: #1E40AF; }"
    );

    connect(viewRevenueReportBtn, &QPushButton::clicked, this, [this]() {
        const int EMERGENCY_PRICE = 1000;
        int cases = priorityQueue ? priorityQueue->getQueueCount() : 0;
        long long total = static_cast<long long>(cases) * EMERGENCY_PRICE;
        QLocale loc(QLocale::English);

        QString detail = QString(
            "<h3>Emergency Revenue Report</h3>"
            "<hr>"
            "<p><b>Emergency Service Price:</b> $%1 per case (Fixed standard rate)</p>"
            "<p><b>Active Emergency Cases:</b> %2</p>"
            "<p><b>Total Emergency Revenue:</b> <span style='color: #16A34A; font-weight: bold;'>$%3</span></p>"
            "<br>"
            "<p><i>Data Source: PriorityQueue & PatientManager records.<br>"
            "Note: Case IDs are not stored in TriageRecord; each active emergency case is billed at the standard rate of $1,000.</i></p>"
        ).arg(loc.toString(EMERGENCY_PRICE))
         .arg(cases)
         .arg(loc.toString(total));

        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Emergency Revenue Report");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(detail);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    });

    revenueLayout->addWidget(viewRevenueReportBtn);
    revenueLayout->addStretch();
    middleLayout->addWidget(revenuePanel, 2);

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
    queueTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    queueTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    queueTable->setMinimumHeight(220);
    queueTable->setMaximumHeight(320);
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
    mainLayout->addStretch();

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
        int previewCount = priorityQueue->getQueueCount();

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

    // Emergency Revenue updates
    const int EMERGENCY_PRICE_PER_CASE = 1000;
    int emergencyCases = priorityQueue ? priorityQueue->getQueueCount() : 0;
    long long totalRevenue = static_cast<long long>(emergencyCases) * EMERGENCY_PRICE_PER_CASE;

    QLocale locale(QLocale::English);
    if (revenuePriceVal)
    {
        revenuePriceVal->setText(QString("$%1").arg(locale.toString(EMERGENCY_PRICE_PER_CASE)));
    }
    if (revenueTotalVal)
    {
        revenueTotalVal->setText(QString("$%1").arg(locale.toString(totalRevenue)));
    }
    if (revenueCaseCountVal)
    {
        revenueCaseCountVal->setText(QString("%1 %2 at $%3 fixed")
            .arg(emergencyCases)
            .arg(emergencyCases == 1 ? "case" : "cases")
            .arg(locale.toString(EMERGENCY_PRICE_PER_CASE)));
    }

    if (revenueTable)
    {
        revenueTable->setRowCount(0);
        for (int i = 0; i < emergencyCases; ++i)
        {
            TriageRecord *tr = priorityQueue ? priorityQueue->getQueueRecordAt(i) : nullptr;
            if (!tr) continue;

            Patient *p = patientManager ? patientManager->findPatientInfo(tr->patientID) : nullptr;
            int row = revenueTable->rowCount();
            revenueTable->insertRow(row);

            // Case ID: Not stored in TriageRecord, clearly indicate missing info
            QTableWidgetItem *caseItem = new QTableWidgetItem(QString("N/A (PID: %1)").arg(tr->patientID));
            caseItem->setToolTip(QString("Case ID is not tracked in TriageRecord. Patient ID: %1").arg(tr->patientID));
            caseItem->setTextAlignment(Qt::AlignCenter);

            // Patient Name
            QString patName = p ? QString::fromStdString(p->name) : QString("Patient #%1").arg(tr->patientID);
            QTableWidgetItem *nameItem = new QTableWidgetItem(patName);

            // Price: Fixed $1,000 per case
            QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$%1").arg(locale.toString(EMERGENCY_PRICE_PER_CASE)));
            priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            priceItem->setForeground(QColor("#16A34A"));
            QFont priceFont = priceItem->font();
            priceFont.setBold(true);
            priceItem->setFont(priceFont);

            revenueTable->setItem(row, 0, caseItem);
            revenueTable->setItem(row, 1, nameItem);
            revenueTable->setItem(row, 2, priceItem);
        }
    }
}
