#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include "qcustomplot.h"

#include "chartsettings.h"
#include "dialogs.h"

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    enum ChartStyle{
        Graph,
        Bars,
        Curve,
        Financial,
        StatisticalBox,
    };

    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();
    void setDataGraph(QVector<QVector<double>> &data);
    void addDataGraph(QVector<QVector<double>> &data);

    QAction *action_Save;

    static int SumCharts;
    static int ResChartWidgetID;

private:
    QCustomPlot *customPlot;
    QGridLayout *gridLayout;
    QMenu *customContexMenu;
    QAction *action_Properties;
    bool draggingLegend;
    QPointF dragLegendOrigin;

private slots:
    void slot_axisDoubleClick(QCPAxis *axis);
    void slot_plottableDoubleClick(QCPAbstractPlottable *plottable);
    void slot_customContextMenuRequest(const QPoint &pos);
    void slot_Properties();
    void mousePressEventQCP(QMouseEvent *event);
    void mouseReleaseEventQCP(QMouseEvent *event);
    void mouseMoveEventQCP(QMouseEvent *event);
    void beforeReplot();
};

#endif // CHARTWIDGET_H
