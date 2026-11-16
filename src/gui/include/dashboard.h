#ifndef dashboard_H
#define dashboard_H

#include <QWidget>
#include <functional>

class Dashboard : public QWidget
{
public:
    // 🧭 Navigation callback provided by MainWindow.
    explicit Dashboard(
        std::function<void(int)> navigate = {},
        QWidget *parent = nullptr);
};

#endif
