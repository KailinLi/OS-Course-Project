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
    ui(new Ui::Explorer)
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
}

void Explorer::changeTab(int index)
{
    if (index == 1) {
        ui->killPushButton->show ();
        updateProcess();
    }
    else if (index == 0) {
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
