#include "copyfile.h"
#include "ui_copyfile.h"
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <QDebug>
#include <QFileDialog>

CopyFile::CopyFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyFile)
{
    ui->setupUi(this);
    connect(ui->chooseBtn, &QPushButton::clicked, this, &CopyFile::chooseFile);
    connect(ui->chooseDirBtn, &QPushButton::clicked, this, &CopyFile::chooseDir);
    connect(ui->cpFileBtn, &QPushButton::clicked, this, &CopyFile::cpFile);
    ui->chooseDirBtn->setEnabled(false);
    ui->cpFileBtn->setEnabled(false);
}

CopyFile::~CopyFile()
{
    delete ui;
}

void CopyFile::chooseFile()
{
    QString fileName = QFileDialog::getOpenFileName (this);
    std::string origin = fileName.toStdString();
    strcpy(this->fileName, origin.c_str());
//    printf("%s\n", this->fileName);
    ui->chooseBtn->setEnabled(false);
    ui->chooseDirBtn->setEnabled(true);
    ui->cpFileBtn->setEnabled(false);
}

void CopyFile::cpFile()
{
    FILE *fp;
    unsigned int size;
    char *buffer;
    fp = fopen(this->fileName, "rb");
    if (fp == NULL) {
        fputs("Read file error\n", stderr);
        exit (1);
    }
    fseek(fp, 0, SEEK_END);
    size = (unsigned int)ftell(fp);
    rewind(fp);
    buffer = (char *)malloc(sizeof(char) * size);
    if (buffer == NULL) {
        fputs("Malloc error\n", stderr);
        exit (1);
    }
    if (size != fread(buffer, sizeof(char), size, fp)) {
        fputs("Read error\n", stderr);
        exit (1);
    }
    fclose(fp);
    fp = fopen(this->toFileName, "wb");
    if (fp == NULL) {
        fputs("File error\n", stderr);
        exit (1);
    }
    if (size != fwrite(buffer, sizeof(char), size, fp)) {
        fputs("Write error\n", stderr);
        exit (1);
    }
    fclose(fp);
    free(buffer);
    ui->chooseBtn->setEnabled(true);
    ui->chooseDirBtn->setEnabled(false);
    ui->cpFileBtn->setEnabled(false);
}

void CopyFile::chooseDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                "/home",
                                                                QFileDialog::ShowDirsOnly
                                                                | QFileDialog::DontResolveSymlinks);
    std::string origin = (dir.append(tr("/output"))).toStdString();
    strcpy(this->toFileName, origin.c_str());
    ui->chooseBtn->setEnabled(false);
    ui->chooseDirBtn->setEnabled(false);
    ui->cpFileBtn->setEnabled(true);
}
