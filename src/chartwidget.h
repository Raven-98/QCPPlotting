#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include "global.h"
#include "chartsettings.h"
#include "dialogs.h"


class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = nullptr);
    ~ChartWidget();
    void setDataGraph(QVector<QVector<double>> &data);
    void addDataGraph(QVector<QVector<double>> &data);

    QAction *action_Save;
    QCustomPlot *customPlot;

    static int SumCharts;
    static int ResChartWidgetID;

private:
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
