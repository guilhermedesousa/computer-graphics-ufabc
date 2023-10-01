#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void paintEvent(QPaintEvent *);

    public slots:
        void SelectedDraw(int);

    private:
        Ui::MainWindow *ui;

        int typeOfDraw{0}; // CLEAN
};
#endif // MAINWINDOW_H
