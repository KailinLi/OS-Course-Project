#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QLCDNumber>
#include <QTime>
#include <QTimer>

class CPU : public QLCDNumber
{
    Q_OBJECT
public:
    CPU(QWidget *parent = 0);
private slots:
    void showCPU();
};

#endif // CPU_H
