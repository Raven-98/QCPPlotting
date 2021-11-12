#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialogs.h"
#include "tablewidget.h"
#include "chartwidget.h"
#include "analyzediffractiondatadron2.h"
#include "aboutprogram.h"

#include "global.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void loadSettings();
    void saveSettings();
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
    void readCSV(QString file, GBS::FDSD::Data *r_data);
    void buildChart(QVector<QVector<double> > data, GBS::CW::ChartStyle chartStyle);
    void addChart(QList<ChartWidget *> widgetsList, QVector<QVector<double> > data, GBS::CW::ChartStyle chartStyle);
    bool checkTableData(QVector<QVector<double> > data);
    void saveTable(QVector<QVector<double> > data);
    void savePlot();

    GBS::Settings settings;
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
    QMenu *menuExtra;
    // Analyze diffraction data (DRON-2, V. N. Karazin Kharkiv National University, Department of Solid State Physics)
    QAction *actionAnalyzeDiffractionData_DRON2;
    QMenu *menuHelp;
    QAction *actionAboutProgram;

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
    void slot_AnalyzeDiffractionDataDRON2();
    void slot_AboutProgram();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
