#ifndef mainwindow_H
#define mainwindow_H

#include <QMainWindow>
#include <QList>

class QStackedWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
public:
    // 🏗️ CONSTRUCTOR
    explicit MainWindow(QWidget *parent = nullptr);

private:
    // 🧭 NAVIGATION
    QStackedWidget *pages;

    // ✏️ MANUAL CHANGE:
    // Add or remove navigation buttons here if you change the menu.
    QList<QPushButton *> navButtons;

    // 🎨 GUI SETUP
    void setupUI();

    // 🔄 PAGE NAVIGATION
    void changePage(int index);

    // ⭐ SIDEBAR DESIGN
    void setActiveButton(int activeIndex);
};

#endif
