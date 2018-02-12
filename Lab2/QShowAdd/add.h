#ifndef ADD_H
#define ADD_H

#include <QObject>
#include <QLCDNumber>
#include <QTime>
#include <QTimer>

class Add : public QLCDNumber
{
    Q_OBJECT
public:
    Add(QWidget *parent = 0);
private:
    int sum;
    int i;
private slots:
    void showRes();
};

#endif // ADD_H
