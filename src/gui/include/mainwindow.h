
#ifndef mainwindow_H
#define mainwindow_H

#include <QMainWindow>

class QStackedWidget;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *pages;

    void setupUI();
    void changePage(int index);
};

#endif
