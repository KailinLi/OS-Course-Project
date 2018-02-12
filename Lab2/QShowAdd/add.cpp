#include "add.h"

Add::Add(QWidget *parent):sum(0), i(0)
{
    setSegmentStyle(Filled);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showRes()));
    timer->start(3000);

    showRes();

    setWindowTitle(tr("Add"));
    resize(150, 60);
}

void Add::showRes()
{
    QString text = QString::number(sum);
    display(text);
    if (i < 100) {
        ++i;
        sum += i;
    }
}
