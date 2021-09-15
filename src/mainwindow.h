#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QMdiSubWindow>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QMessageBox>

#include "dialogs.h"
#include "tablewidget.h"
#include "chartwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initTable();

private:
    void initActions();
    void deleteActions();
    void initMenus();
    void deleteMenus();
    void initMenuBar();
    void deleteMenuBar();
    void initToolBar();
    void deleteToolBar();
    void loadSubWindow(QWidget *widget);
    void addTable(QStandardItemModel *item_model);
    void readCSV(QString file, FDSD::Data *r_data);
    void buildChart(QVector<QVector<double> > data, ChartWidget::ChartStyle chartStyle);
    void addChart(QList<ChartWidget *> widgetsList, QVector<QVector<double> > data, ChartWidget::ChartStyle chartStyle);
    bool checkTableData(QVector<QVector<double> > data);
    void saveTable(QVector<QVector<double> > data);
    void savePlot();

    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QToolBar *toolBar;
    QMenu *menuFile;
    QMenu *menuWindow;
    QMenu *menuTable;
    QAction *actionExit;
    QAction *actionFullScreen;
    QAction *actionImportCSV;
    QAction *actionNewTable;

private slots:
    void slot_FullScreen();
    void slot_ImportCSV();
    void slot_PlotGraph();
    void slot_PlotCurve();
    void slot_PlotBars();
    void slot_PlotFinancial();
    void slot_PlotStatistacalBox();
    void slot_addGraphs();
    void slot_saveTable();
    void slot_newTable();
    void slot_savePlot();
};
#endif // MAINWINDOW_H
