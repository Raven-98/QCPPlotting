#include "chartwidget.h"

#include <QDebug>

int ChartWidget::SumCharts = 0;
int ChartWidget::ResChartWidgetID = 0;

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    SumCharts++;
    ResChartWidgetID++;

    resize(500,400);
    setWindowTitle("Chart "+QString::number(ResChartWidgetID));
    setWindowIcon(QIcon(":/img/graph.png"));

    customPlot = new QCustomPlot;

    customPlot->setInteractions(QCP::iRangeDrag |
                                QCP::iRangeZoom |
                                QCP::iMultiSelect |
                                QCP::iSelectPlottables |
                                QCP::iSelectAxes |
                                QCP::iSelectLegend);
    customPlot->axisRect()->setRangeDragAxes({customPlot->xAxis, customPlot->xAxis2},
                                             {customPlot->yAxis, customPlot->yAxis2});
    customPlot->axisRect()->setRangeZoomAxes({customPlot->xAxis, customPlot->xAxis2},
                                             {customPlot->yAxis, customPlot->yAxis2});

    customPlot->setContextMenuPolicy(Qt::CustomContextMenu);

    customPlot->xAxis->setTickLength(0,5);
    customPlot->xAxis->setSubTickLength(0,2);

    customPlot->yAxis->setTickLength(0,5);
    customPlot->yAxis->setSubTickLength(0,2);

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->xAxis2->setSubTicks(false);
    customPlot->xAxis2->setTickLabels(false);

    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTicks(false);
    customPlot->yAxis2->setSubTicks(false);
    customPlot->yAxis2->setTickLabels(false);

    QPen *pen = new QPen;
    pen->setColor(QColor(200, 200, 200));
    pen->setWidthF(1);
    pen->setStyle(Qt::DotLine);

    customPlot->xAxis->grid()->setPen(*pen);
    customPlot->xAxis2->grid()->setPen(*pen);
    customPlot->yAxis->grid()->setPen(*pen);
    customPlot->yAxis2->grid()->setPen(*pen);

    customPlot->xAxis->grid()->setSubGridPen(*pen);
    customPlot->xAxis2->grid()->setSubGridPen(*pen);
    customPlot->yAxis->grid()->setSubGridPen(*pen);
    customPlot->yAxis2->grid()->setSubGridPen(*pen);

    delete pen;

    connect(customPlot,&QCustomPlot::axisDoubleClick,this,&ChartWidget::slot_axisDoubleClick);
    connect(customPlot,&QCustomPlot::plottableDoubleClick,this,&ChartWidget::slot_plottableDoubleClick);
    connect(customPlot,&QCustomPlot::customContextMenuRequested,this,&ChartWidget::slot_customContextMenuRequest);
    connect(customPlot,&QCustomPlot::mousePress,this,&ChartWidget::mousePressEventQCP);
    connect(customPlot,&QCustomPlot::mouseRelease,this,&ChartWidget::mouseReleaseEventQCP);
    connect(customPlot,&QCustomPlot::mouseMove,this,&ChartWidget::mouseMoveEventQCP);
    connect(customPlot,&QCustomPlot::beforeReplot,this,&ChartWidget::beforeReplot);

    gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->addWidget(customPlot);
    setLayout(gridLayout);

    action_Properties = new QAction;
    action_Properties->setText(tr("Properties"));
    connect(action_Properties,&QAction::triggered,this,&ChartWidget::slot_Properties);

    action_Save = new QAction;
    action_Save->setText(tr("Save"));
//    action_Save->setEnabled(false);

    customContexMenu = new QMenu;
    customContexMenu->addAction(action_Properties);
    customContexMenu->addAction(action_Save);

    draggingLegend = false;
    customPlot->axisRect()->insetLayout()->setInsetPlacement(0, QCPLayoutInset::ipFree);
}

ChartWidget::~ChartWidget()
{
    SumCharts--;
    if (SumCharts == 0)
        ResChartWidgetID = 0;

    delete customPlot;
    delete gridLayout;
    delete action_Properties;
    delete customContexMenu;
}

void ChartWidget::setDataGraph(QVector<QVector<double> > &data)
{
    customPlot->clearGraphs();

    addDataGraph(data);

    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");
}

void ChartWidget::addDataGraph(QVector<QVector<double> > &data)
{
    customPlot->addGraph();

    int *count = new int(customPlot->graphCount());

    customPlot->graph(*count - 1)->setData(data[0],data[1]);

    customPlot->graph(*count - 1)->rescaleAxes();

    QPen *pen = new QPen;

    int *col = new int(*count + 6);
    while (*col > 19)
    {
        *col = *col - 13;
    }

    pen->setColor(Qt::GlobalColor(*col));
    pen->setWidthF(1);

    customPlot->graph(*count - 1)->setPen(*pen);

    delete count;
    delete col;
    delete pen;

    customPlot->replot();
}

void ChartWidget::slot_axisDoubleClick(QCPAxis *axis)
{
    axis->setSelectedParts(QCPAxis::spAxis |
                           QCPAxis::spTickLabels |
                           QCPAxis::spAxisLabel);
    customPlot->replot();

    ChartSettings *chartSettings = new ChartSettings(*axis, this);
    switch (chartSettings->exec())
    {
    case ChartSettings::Accepted:
        chartSettings->set();
        customPlot->replot();
        break;
    case ChartSettings::Rejected:
        break;
    default:
        QMessageBox::critical(chartSettings,tr("Error"),tr("Unexpected result"));
        break;
    }
    delete chartSettings;
    axis->setSelectedParts(QCPAxis::spNone);
    customPlot->replot();
}

void ChartWidget::slot_plottableDoubleClick(QCPAbstractPlottable *plottable)
{
    ChartSettings *chartSettings = new ChartSettings(*plottable, this);
    switch (chartSettings->exec())
    {
    case ChartSettings::Accepted:
        chartSettings->set();
        customPlot->replot();
        break;
    case ChartSettings::Rejected:
        break;
    default:
        QMessageBox::critical(chartSettings,tr("Error"),tr("Unexpected result"));
        break;
    }
    delete chartSettings;
    plottable->setSelection(QCPDataSelection());
    customPlot->replot();
}

void ChartWidget::slot_customContextMenuRequest(const QPoint &pos)
{
    customContexMenu->popup(mapToGlobal(pos));
}

void ChartWidget::slot_Properties()
{
    ChartSettings *chartSettings = new ChartSettings(*customPlot, this);
    switch (chartSettings->exec())
    {
    case ChartSettings::Accepted:
        chartSettings->set();
        customPlot->replot();
        break;
    case ChartSettings::Rejected:
        break;
    default:
        QMessageBox::critical(chartSettings,tr("Error"),tr("Unexpected result"));
        break;
    }
    delete chartSettings;
}

void ChartWidget::mousePressEventQCP(QMouseEvent *event)
{
    if (customPlot->legend->selectTest(event->pos(), false) > 0)
    {
        customPlot->setInteractions(QCP::iSelectLegend);
        draggingLegend = true;
        QPointF mousePoint((event->pos().x() - customPlot->axisRect()->left()) / (double)customPlot->axisRect()->width(),
                           (event->pos().y() - customPlot->axisRect()->top()) / (double)customPlot->axisRect()->height());
        dragLegendOrigin = mousePoint - customPlot->axisRect()->insetLayout()->insetRect(0).topLeft();
    }
}

void ChartWidget::mouseReleaseEventQCP(QMouseEvent *event)
{
    Q_UNUSED(event)
    draggingLegend = false;
    customPlot->setInteractions(QCP::iRangeDrag |
                                QCP::iRangeZoom |
                                QCP::iSelectPlottables |
                                QCP::iSelectAxes |
                                QCP::iSelectLegend);
}

void ChartWidget::mouseMoveEventQCP(QMouseEvent *event)
{
    if (draggingLegend)
    {
        QRectF rect = customPlot->axisRect()->insetLayout()->insetRect(0);
        QPointF mousePoint((event->pos().x() - customPlot->axisRect()->left()) / (double)customPlot->axisRect()->width(),
                           (event->pos().y() - customPlot->axisRect()->top()) / (double)customPlot->axisRect()->height());
        rect.moveTopLeft(mousePoint - dragLegendOrigin);
        customPlot->axisRect()->insetLayout()->setInsetRect(0, rect);
        customPlot->replot();
    }
}

void ChartWidget::beforeReplot()
{
    customPlot->legend->setMaximumSize(customPlot->legend->minimumOuterSizeHint());
}
