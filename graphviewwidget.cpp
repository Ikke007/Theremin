#include "graphviewwidget.h"

#include <QDebug>

GraphViewWidget::GraphViewWidget(QWidget *parent)
    : QWidget(parent)
{
    pixmap = new QPixmap(this->width(), this->height());
    painter = new QPainter(pixmap);
    pen.setWidth(1);

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer.setInterval(50);
    timer.start();
}

GraphViewWidget::~GraphViewWidget()
{
    delete painter;
    delete pixmap;
}

void GraphViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.drawPixmap(0,0,this->width(),this->height(), *pixmap);
}

void GraphViewWidget::onTimeout()
{
    for (int index = 0; index < current.size(); index++) {
        pen.setColor(color.at(index));
        painter->setPen(pen);
        painter->drawLine(row, pixmap->height() - last.at(index),
                          row+1, pixmap->height() - current.at(index));
        last.replace(index, current.at(index));
    }

    row += 2;
    if (row == pixmap->width()) {
        pixmap->fill(Qt::black);
        row = 0;
    }
    update();
}

int GraphViewWidget::addGraph(QColor color)
{
    this->color.append(color);
    this->current.append(0);
    this->last.append(0);
    return current.size() - 1;
}

void GraphViewWidget::setValue(int index, double value)
{
    if (index > current.size()){
        qDebug() << "GraphViewWidget: invalid graph index: " << index;
        return;
    }

    current.replace(index, value * pixmap->height());
}

void GraphViewWidget::resizeEvent(QResizeEvent *)
{
    delete painter;
    delete pixmap;
    row = 0;
    pixmap = new QPixmap(this->width(), this->height());
    painter = new QPainter(pixmap);
}
