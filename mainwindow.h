#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<QList<QByteArray> >result;
    QMap<QByteArray,QList<QByteArray> > Production;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print(QMap<QByteArray,QList<QByteArray> > map);
    void NPDA(QMap<QByteArray,QList<QByteArray> > map,QByteArray tape);
    bool NPDARec(QMap<QByteArray,QList<QByteArray> > map,QByteArray tape, QList<QByteArray> stack);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
