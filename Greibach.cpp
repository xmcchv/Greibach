

#include <iostream>
#include <fstream>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QFile>
#define LINE_LENGTH 100
using namespace std;

class Greibach{

public:
    //终结符
    QList<QByteArray> Vt={};
    //非终结符
    QList<QByteArray> Vn={};
    QMap<QByteArray,QList<QByteArray> > Production;

//S->aSBb|abC
//A->aBAb|Cab
//B->bBb|C|$
//C->cCab|a|$


void readtxt(){
    QFile file("D:/QtWorkSpace/Greibach/text.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray tmp;
    while(!file.atEnd()){
        tmp=file.readLine();
        QList<QByteArray> qtmp=tmp.simplified().split('-');
        Vn.append(qtmp[0]);
        QList<QByteArray> b = qtmp[1].split('>')[1].split('|');
        Production.insert(qtmp[0],b);
    }

    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end())
    {
       int idx=0;
       foreach(QString pt,iter.value()){
            for(int i=0;i<pt.size();i++){
                if(pt[i].isLower()){
                    QByteArray ptmp=QString(""+pt[i]).toLatin1();
                    if(!Vt.contains(ptmp)){
                        Vt.append(ptmp);
                    }
                }else if(pt[i].isUpper()){
                    QByteArray ptmp=QString(""+pt[i]).toLatin1();
                    if(!Vn.contains(ptmp)){
                        iter.value().removeAt(idx);
                    }
                }
            }
            idx++;
       }
       iter++;
    }
    qDebug()<<"读取完成后。终结符："<<Vt<<"非终结符："<<Vn;
    file.close();
}

void removeUselessProduct(){
    int idx=0;
    foreach(QByteArray pt,Vn){
        if(pt!="S"){
            QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
            bool flag=true;
            while (iter != Production.end()){
                foreach(QByteArray pro,iter.value()){
                    if(pro.contains(pt)){
                        flag=false;
                    }
                }
                iter++;
            }
            if(flag){
                Production.remove(pt);
                Vn.removeAt(idx);
            }
        }
        idx++;
    }
}

void removeSingleProduct(){
    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end()){
        int idx=0;
        foreach(QByteArray pt,iter.value()){
            if(pt.size()==1&&pt.isUpper()){
                qDebug()<<pt;
                iter.value().removeAt(idx);
                foreach(QByteArray tmp,Production.find(pt).value()){
                    if(!iter.value().contains(tmp)){
                        iter.value().append(tmp);
                    }
                }
            }
            idx++;
        }
        iter++;
    }
}

void removeEpsilonProduct(){
    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end()){
        int idx=0;
        foreach(QByteArray pt,iter.value()){
            if(pt=="$"){
                iter.value().removeAt(idx);//删掉epsilon产生式
                QMap<QByteArray,QList<QByteArray> >::iterator iterpro = Production.begin();
                while (iterpro != Production.end()){
                    foreach(QByteArray ptpt,iterpro.value()){
                        if(ptpt.contains(iter.key())){ //对每个含有key的产生式替换当前key的产生式
                            iterpro.value().append(ptpt.replace(iter.key(),""));
                        }
                    }
                    iterpro++;
                }
            }
            idx++;
        }
        iter++;
    }
}

void removeLeftRecursion(){
    QMap<QByteArray,QList<QByteArray> >::iterator iter1 = Production.begin();
    while (iter1 != Production.end()){
        int idx=0;
        foreach(QByteArray qt,iter1.value()){
            if(iter1.key()==QString(qt.at(0)).toLatin1()){
                Vn.append(iter1.key()+"*");
                QList<QByteArray> a,b;
                foreach(QByteArray qttmp,iter1.value()){
                    if(iter1.key()==QString(qttmp.at(0)).toLatin1()){
                        b.append(qttmp.replace(0,1,"").append(iter1.key()+"*"));
                    }else{
                        a.append(qttmp.append(iter1.key()+"*"));
                    }
                }
                b.append("$");
                iter1.value()=a;
                Production.insert(iter1.key()+"*",b);
                break;
            }
            idx++;
        }
        iter1++;
    }
}

void removeFirstNoTerminal(){
    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end()){
        foreach(QByteArray ptpt,iter.value()){
            if(QString(ptpt.at(0)).toLatin1().isUpper()){
                iter.value().removeOne(ptpt);
                QList<QByteArray> tmplist=Production.find(QString(ptpt.at(0)).toLatin1()).value();
                foreach(QByteArray ptptmp,tmplist){
                    QByteArray ptptpttmp=ptpt.replace(0,1,"");
                    if(!iter.value().contains(ptptmp.append(ptptpttmp)))
                    iter.value().append(ptptmp.append(ptptpttmp));
                }
            }
        }
        iter++;
    }
}

void getGreibachPorduct(){
    QList<QByteArray> Vntmp={};
    QMap<QByteArray,QList<QByteArray> > producttmp;
    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end()){
        int idx=0;
        foreach(QString pt,iter.value()){
             for(int i=1;i<pt.size();i++){
                 if(pt[i].isLower()){
                     QByteArray tmp=(QString(pt[i].toUpper())+"1").toLatin1();
                     if(Vntmp.contains(tmp)){
                         iter.value().removeOne(pt.toLatin1());
                         iter.value().append(pt.replace(i,1,QString(pt[i].toUpper())+"1").toLatin1());
                     }else{
                         Vntmp.append(tmp);
                         QList<QByteArray> b;
                         b.append((QString(pt[i]+"").toLatin1()));
                         producttmp.insert(tmp,b);
                         iter.value().removeOne(pt.toLatin1());
                         iter.value().append(pt.replace(i,1,QString(pt[i].toUpper())+"1").toLatin1());
                     }
                 }
             }
             idx++;
        }
        iter++;
    }
    Vn.append(Vntmp);
    QMap<QByteArray,QList<QByteArray> >::iterator iterpro = producttmp.begin();
    while (iterpro != producttmp.end()){
        Production.insert(iterpro.key(),iterpro.value());
        iterpro++;
    }
}

QMap<QByteArray,QList<QByteArray> > initilize(){
    readtxt();
    removeUselessProduct();
    qDebug()<<"消除无用产生式后。终结符："<<Vt<<"非终结符："<<Vn;
    printmap();
    removeSingleProduct();
    qDebug()<<"消除单一产生式后。终结符："<<Vt<<"非终结符："<<Vn;
    printmap();

    removeEpsilonProduct();
    qDebug()<<"消除epsilon产生式后";
    printmap();

    removeLeftRecursion();
    qDebug()<<"消除左递归后";
    printmap();

    removeEpsilonProduct();
    qDebug()<<"消除epsilon产生式后";
    printmap();

    removeFirstNoTerminal();
    qDebug()<<"消除不以非终结符开头的产生式后";
    printmap();

    getGreibachPorduct();
    qDebug()<<"得到Greibach产生式";
    printmap();

    return Production;
}

void printmap(){
    qDebug() << "当前产生式";
    QMap<QByteArray,QList<QByteArray> >::iterator iter = Production.begin();
    while (iter != Production.end()){
        qDebug() << "Iterator " << iter.key() << ":" << iter.value(); // 迭代器
        iter++;
    }

}


};
