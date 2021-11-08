#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Greibach.cpp>
#include <QList>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Greibach* g = new Greibach();
    Production=g->initilize();
    print(Production);
}

void MainWindow::NPDA(QMap<QByteArray,QList<QByteArray> > map,QByteArray tape){
    QList<QByteArray> stack;
    qDebug()<<"当前纸带:"<<tape;
    stack.push_back("S");
    if(!stack.isEmpty()){
        QByteArray topstack=stack.back();
        QList<QByteArray> tmplist=map.find(topstack).value();
        foreach(QByteArray tmp,tmplist){
            QByteArray tapetmp=tape;
            QList<QByteArray> stacktmp=stack;
            if(tape.front()==tmp.front()){
                tapetmp.remove(0,1);
                stacktmp.pop_back();
                for(int i=tmp.length()-1;i>0;i--){
                    QByteArray stmp=QString(tmp.at(i)).toLatin1();
                    if(stmp=="1"||stmp=="*"){
                        i--;
                        stmp=QString(tmp.at(i)).toLatin1().append(stmp);
                    }
//                    qDebug()<<stmp;
                    stacktmp.push_back(stmp);
                }
                bool flag=NPDARec(map,tapetmp,stacktmp);
                if(flag){
                    qDebug()<<"接受该语句";

                    QList<QByteArray> tmpres1;
                    tmpres1.append(QString(tmp.at(0)).toLatin1());
                    tmpres1.append(tapetmp);
                    tmpres1.append(stacktmp);
                    result.append(tmpres1);

                    QList<QByteArray> tmpres;
                    tmpres.append(topstack+"->"+tmp);
                    tmpres.append(tape);
                    tmpres.append(stack);
                    result.append(tmpres);

                    QByteArray NPDAResult;
                    for(int j=result.length()-1;j>=0;j--){
                        QList<QByteArray> tmp=result[j];
                        NPDAResult.append("产生式:").append(tmp[0]).append("\t纸带:").append(tmp[1]).append("\t栈:");
                        int len=tmp.size();
                        for(int i=2;i<len;i++){
                            NPDAResult.append(tmp[i]).append(" ");
                        }
                        NPDAResult.append("\n");
                    }
                    NPDAResult.append("接受该语句\n");
                    ui->NPDAEdit->setText(NPDAResult);
                    return;
                }else{
                    QByteArray NPDAResult;
                    NPDAResult.append("不接受该语句\n");
                    ui->NPDAEdit->setText(NPDAResult);
                }
            }
        }
    }
}
bool MainWindow::NPDARec(QMap<QByteArray,QList<QByteArray> > map,QByteArray tape, QList<QByteArray> stack){
    if(!stack.isEmpty()){
        QByteArray topstack=stack.back();
        if(topstack.isLower()){
            if(tape.front()==topstack.front()){
                tape.remove(0,1);
                stack.pop_back();
                if(NPDARec(map,tape,stack)){
                    // 产生式 纸带 栈
                    QList<QByteArray> tmpres;
                    tmpres.append(topstack);
                    tmpres.append(tape);
                    tmpres.append(stack);
                    result.append(tmpres);
                    return true;
                }else{
                    return false;
                }
            }
        }else{
            QList<QByteArray> tmplist=map.find(topstack).value();
            foreach(QByteArray tmp,tmplist){
                QList<QByteArray> stacktmp=stack;
                if(tape.front()==tmp.front()){
                    QByteArray tapetmp=tape;
                    tapetmp.remove(0,1);
                    stacktmp.pop_back();
                    for(int i=tmp.size()-1;i>0;i--){
                        QByteArray stmp=QString(tmp.at(i)).toLatin1();
                        if(stmp=="1"||stmp=="*"){
                            i--;
                            stmp=QString(tmp.at(i)).toLatin1().append(stmp);
                        }
                        stacktmp.push_back(stmp);
                    }
                    if(NPDARec(map,tapetmp,stacktmp)){

                        QList<QByteArray> tmpres1;
                        tmpres1.append(QString(tmp.at(0)).toLatin1());
                        tmpres1.append(tapetmp);
                        tmpres1.append(stacktmp);
                        result.append(tmpres1);
                        // 产生式 纸带 栈
                        QList<QByteArray> tmpres;
                        tmpres.append(topstack+"->"+tmp);
                        tmpres.append(tape);
                        tmpres.append(stack);
                        result.append(tmpres);

                        return true;
                    }
                }
            }
        }
    }
    if(stack.isEmpty()&&tape.isEmpty()){
        return true;
    }else{
        return false;
    }
}
void MainWindow::print(QMap<QByteArray,QList<QByteArray> > map){
    QMap<QByteArray,QList<QByteArray> >::iterator iter = map.begin();
    QString showGreibach;
    while (iter != map.end()){
        QString value="";
        foreach(QString a,iter.value()){
            value+=a+"|";
        }
        value.remove(value.size()-1,1);
        qDebug() << iter.key().data() << "->" << qPrintable(value); // 迭代器
        showGreibach+=qPrintable(iter.key()+"->"+value+"\n");
        iter++;
    }
    ui->showGreibach->setText(showGreibach);
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray tape = ui->tapeEdit->text().toLatin1();
    NPDA(Production,tape);

}
