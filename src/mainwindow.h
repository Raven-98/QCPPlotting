#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>

#include "structs.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void init();
  void loadSettings();
  void initTable();

  void tst();

private slots:
  void slot_FullScreen();
  void slot_ImportCSV();
  void slot_newTable();
  void slot_AboutProgram();
  void buildGraphTrigered(QVector<QVector<double> > data);

private:
  void saveSettings();
  void loadSubWindow(QWidget *widget);
  void showError(QString err);
  void showWarning(QString war);
  void showMessage(QString mess);
  void addTable(QStandardItemModel *item_model, QString *title = nullptr);
  void addTables(QCPPlotting::SpreadSheet &spreadSheet);
  void addChart(QVector<QVector<double>> &data, QCPPlotting::ChartType chartType);
  void readCSV(const QStringList files, QCPPlotting::FileDialogSetDetails::Data *data);

private:
  QMdiArea *mdiArea = nullptr;
  QMenuBar *menuBar = nullptr;
  QMenu *menuFile = nullptr;
  QMenu *menuWindow = nullptr;
  QMenu *menuTable = nullptr;
  QMenu *menuExtra = nullptr;
  QMenu *menuOptions = nullptr;
  QMenu *menuSettings = nullptr;
  QMenu *menuReference = nullptr;
  QAction *separatorMenuFile = nullptr;
  QAction *actionExit = nullptr;
  QAction *actionFullScreen = nullptr;
  QAction *actionImportCSV = nullptr;
  QAction *actionNewTable = nullptr;
  QAction *actionAboutProgram = nullptr;

  QCPPlotting::Settings settings;
};
#endif // MAINWINDOW_H
