#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

CPU::CPU(QWidget *parent)
{
    setSegmentStyle(Filled);
    setDigitCount(6);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CPU::showCPU);
    timer->start(2000);

    showCPU();

    setWindowTitle(tr("CPU"));
    resize(150, 60);
}

void CPU::showCPU()
{
//    QTime time = QTime::currentTime();
//    QString text = time.toString("hh:mm:ss");
//    if ((time.second() % 2) == 0) {
//        text[2] = ' ';
//        text[5] = ' ';
//    }
//    display(text);
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        fputs("Open file error\n", stderr);
        return;
    }
    int user, nice, system, idle;
    char tmpCpu[4];
    fscanf(fp, "%s %d %d %d %d", tmpCpu, &user, &nice, &system, &idle);
//    printf("%s %d %d %d %d\n", tmpCpu, user, nice, system, idle);
    double res = (double)(user + nice + system) / (user + nice + system + idle) * 100;
    display(res);
    fclose(fp);
}
