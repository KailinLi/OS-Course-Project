#include "explorer.h"
#include "ui_explorer.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

Explorer::Explorer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Explorer),
    getSysMes(false)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth (0, 150);
    ui->tableWidget->setColumnWidth (1, 60);
    ui->tableWidget->setColumnWidth (2, 60);
    ui->tableWidget->setColumnWidth (3, 70);
    ui->tableWidget->setColumnWidth (4, 50);
    ui->killPushButton->hide ();
    connect (&timer, &QTimer::timeout, this, &Explorer::refreshData);
    connect (ui->tabWidget, &QTabWidget::currentChanged, this, &Explorer::changeTab);
    connect (ui->okPushButton, &QPushButton::clicked, this, &Explorer::accept);
    connect (ui->killPushButton, &QPushButton::clicked, this, &Explorer::killProcess);
    ui->cpuProgressBar->setMaximum (100);
    ui->memoryProgressBar->setMaximum (100);
    timer.start (1000);
}

Explorer::~Explorer()
{
    delete ui;
}

void Explorer::refreshData()
{
    int index = ui->tabWidget->currentIndex ();
    if (index == 0)
        updateBase ();
    else if (index == 2)
        updateSys();
}

void Explorer::changeTab(int index)
{
    if (index == 1) {
        ui->killPushButton->show ();
        updateProcess();
    }
    else if (index == 2) {
        ui->killPushButton->hide();
        updateSys ();
    }
    else {
        ui->killPushButton->hide ();
        updateBase();
    }
}

void Explorer::updateBase()
{
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        fputs("Open file error\n", stderr);
        return;
    }
    int user, nice, system, idle;
    char tmpCpu[4];
    fscanf(fp, "%s %d %d %d %d", tmpCpu, &user, &nice, &system, &idle);
    double res = (double)(user + nice + system) / (user + nice + system + idle) * 100;
    fclose(fp);
    ui->cpuProgressBar->setValue ((int)res);
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        fputs("Open file error\n", stderr);
        return;
    }
    int memTotal, memFree;
    fscanf (fp, "MemTotal: %d kB", &memTotal);
    fscanf (fp, "MemFree: %d kB", &memFree);
//    qDebug() << memTotal << " " << memFree;
    res = ((double)(memTotal - memFree) / memTotal) * 100;
//    qDebug() << memTotal << " " << memFree << " " << res;
    fclose(fp);
    ui->memoryProgressBar->setValue ((int)res);
    ui->numberMemLabel->setText (tr("MemTotal: %1\tMemFree: %2").arg (memTotal).arg (memFree));
}

void Explorer::updateProcess()
{
    struct dirent *dirp;
    DIR *dp;
    bool ok;
    FILE *fp;
    char ignore[1024];
    if ((dp = opendir("/proc")) == NULL) {
        fputs("Can not read dir\n", stderr);
        return;
    }
    ui->tableWidget->setRowCount (0);
    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_type == DT_DIR) {
            std::string preString(dirp->d_name, dirp->d_name + strlen(dirp->d_name));
            QString pidString = QString::fromStdString (preString);
            int pid = pidString.toInt (&ok);
            if (ok) {
                pidString = QString("/proc/") + pidString + QString("/status");
                fp = fopen (pidString.toStdString ().c_str (), "r");
                if (fp == NULL) {
                    fputs ("Open file error\n", stderr);
                    continue;
                }
                char name[32];
                int ppid, threads;
                char state[14];
                fscanf(fp, "Name: %s\n", name);
                fgets(ignore, sizeof(ignore), fp);
                fscanf(fp, "State: %*c (%10[^)])\n", state);
                for (int i = 0; i < 3; ++i)
                    fgets(ignore, sizeof(ignore), fp);
                fscanf(fp, "PPid: %d\n", &ppid);
                while (strcmp (ignore, "Threads:")) {
                    fgets(ignore, sizeof(ignore), fp);
                    ignore[8] = '\0';
                }
                ignore[8] = '\t';
                sscanf (ignore, "Threads: %d", &threads);
                fclose(fp);
                ui->tableWidget->insertRow (0);
                QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString (std::string(name, name + strlen(name))));
                QTableWidgetItem *pidItem = new QTableWidgetItem(QString::number (pid));
                QTableWidgetItem *ppidItem = new QTableWidgetItem(QString::number (ppid));
                QTableWidgetItem *stateItem = new QTableWidgetItem(QString::fromStdString (std::string(state, state + strlen(state))));
                QTableWidgetItem *threadItem = new QTableWidgetItem(QString::number (threads));
                ui->tableWidget->setItem (0, 0, nameItem);
                ui->tableWidget->setItem (0, 1, pidItem);
                ui->tableWidget->setItem (0, 2, ppidItem);
                ui->tableWidget->setItem (0, 3, stateItem);
                ui->tableWidget->setItem (0, 4, threadItem);
            }
        }
    }
    if (closedir(dp) < 0) {
        fputs("Can not close\n", stderr);
    }
    return;
}

void Explorer::killProcess()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel ();
    if (!select->hasSelection ()) {
        QMessageBox msgBox;
        msgBox.setText("Please select process");
        msgBox.exec();
        return;
    }
//    qDebug() << ui->tableWidget->currentRow ();
    QTableWidgetItem *item = ui->tableWidget->item (ui->tableWidget->currentRow (), 1);
//    qDebug() << item->text ();
    int pid = item->text ().toInt ();
    if (kill(pid,SIGKILL) == 0) {
        updateProcess ();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Cannot kill this process");
        msgBox.exec();
    }
}

void Explorer::updateSys()
{
    char buffer[100];
    FILE *fp;
    if (!getSysMes) {
        fp = fopen("/proc/sys/kernel/hostname", "r");
        if (fp == NULL) {
            fputs("Open file error\n", stderr);
            return;
        }
        fscanf (fp, "%s", buffer);
        fclose(fp);
        ui->sHostNameLabel->setText (QString::fromStdString (std::string(buffer, buffer + strlen(buffer))));

        fp = fopen("/proc/version", "r");
        if (fp == NULL) {
            fputs("Open file error\n", stderr);
            return;
        }
        fscanf(fp, "%*s %*s %s", buffer);
        fclose(fp);
        ui->sVersionLabel->setText (QString::fromStdString (std::string(buffer, buffer + strlen(buffer))));

        fp = fopen("/proc/cpuinfo", "r");
        if (fp == NULL) {
            fputs("Open file error\n", stderr);
            return;
        }
        fgets(buffer, sizeof(buffer), fp);
        fscanf (fp, "vendor_id : %s\n", buffer);
        ui->sClassCpuLabe->setText ((QString::fromStdString (std::string(buffer, buffer + strlen(buffer)))));
        for (int i = 0; i < 2; ++i) fgets(buffer, sizeof(buffer), fp);
        fscanf (fp, "model name : %99[^\n]\n", buffer);
        ui->sCpuLabel->setText ((QString::fromStdString (std::string(buffer, buffer + strlen(buffer)))));
        fclose(fp);
    }

    fp = fopen("/proc/uptime", "r");
    if (fp == NULL) {
        fputs("Open file error\n", stderr);
        return;
    }
    float f_totalTime, f_runTime;
    int totalTime, runTime;
    fscanf(fp, "%f %f", &f_runTime, &f_totalTime);
//    f_runTime *= 100;
//    f_totalTime *= 100;
    totalTime = (int)f_totalTime;
    runTime = (int)f_runTime;
    ui->sRunLabel->setText (tr("%1天%2时%3分%4秒").arg(runTime/86400).
                            arg((runTime%86400)/3600).arg((runTime%3600)/60).arg (runTime%60));
    ui->sBeginLabel->setText (tr("%1天%2时%3分%4秒").arg(totalTime/86400)
                              .arg((totalTime%86400)/3600).arg((totalTime%3600)/60).arg (totalTime%60));
    fclose(fp);
    getSysMes = true;
}
