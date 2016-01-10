#ifndef GRAPHVIEWWIDGET_H
#define GRAPHVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QVector>

class GraphViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphViewWidget(QWidget *parent = 0);
    virtual ~GraphViewWidget();
    void setValue(int index, double value);
    int addGraph(QColor color);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);

private:
    QPixmap* pixmap;
    QPainter* painter;
    QPen pen;
    QTimer timer;
    QVector<int> last;
    QVector<int> current;
    QVector<QColor> color;
    int row;

private slots:
    void onTimeout();

signals:

public slots:
};

#endif // GRAPHVIEWWIDGET_H
