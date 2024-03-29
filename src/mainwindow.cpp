#include "mainwindow.h"
#include "tablewidget.h"
#include "chartwidget.h"
#include "logicalmethods.h"
#include "dialogs.h"

#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMessageBox>

#ifdef QT_DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
  saveSettings();

  if (separatorMenuFile)
    delete separatorMenuFile;
  if (actionExit)
    delete actionExit;
  if (actionFullScreen)
    delete actionFullScreen;
  if (actionImportCSV)
    delete actionImportCSV;
  if (actionNewTable)
    delete actionNewTable;
  if (actionAboutProgram)
    delete actionAboutProgram;

  if (menuFile)
    delete menuFile;
  if (menuTable)
    delete menuTable;
  if (menuWindow)
    delete menuWindow;
  if (menuExtra)
    delete menuExtra;
  if (menuOptions)
    delete menuOptions;
  if (menuSettings)
    delete menuSettings;
  if (menuReference)
    delete menuReference;

  if (menuBar)
    delete menuBar;
  if (mdiArea)
    delete mdiArea;
}

void MainWindow::init()
{
  setMinimumSize(800,600);

  mdiArea = new QMdiArea;
  mdiArea->cascadeSubWindows();
  setCentralWidget(mdiArea);

  menuBar = new QMenuBar;
  setMenuBar(menuBar);

  menuFile = new QMenu;
  menuFile->setTitle(tr("&File"));
  menuFile->addSeparator();
  menuBar->addMenu(menuFile);

  menuTable = new QMenu;
  menuTable->setTitle(tr("&Table"));
  menuBar->addMenu(menuTable);

  menuWindow = new QMenu;
  menuWindow->setTitle(tr("&Window"));
  menuBar->addMenu(menuWindow);

  menuExtra = new QMenu;
  menuExtra->setTitle(tr("&Extra"));
  menuBar->addMenu(menuExtra);

  menuOptions = new QMenu;
  menuOptions->setTitle(tr("&Options"));
  menuBar->addMenu(menuOptions);

  menuSettings = new QMenu;
  menuSettings->setTitle(tr("&Settings"));
  menuBar->addMenu(menuSettings);

  menuReference = new QMenu;
  menuReference->setTitle(tr("&Reference"));
  menuBar->addMenu(menuReference);

  separatorMenuFile = new QAction;
  separatorMenuFile->setSeparator(true);
  menuFile->addAction(separatorMenuFile);

  actionExit = new QAction;
  actionExit->setText(tr("Exit"));
  actionExit->setIcon(QIcon(":/img/exit.png"));
  actionExit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
  connect(actionExit, &QAction::triggered, this, &MainWindow::close);
  menuFile->addAction(actionExit);

  actionFullScreen = new QAction;
  actionFullScreen->setText(tr("Full screen"));
  actionFullScreen->setIcon(QIcon(":/img/full_screen.png"));
  actionFullScreen->setShortcut(QKeySequence(Qt::Key_F11));
  connect(actionFullScreen, &QAction::triggered, this, &MainWindow::slot_FullScreen);
  menuWindow->addAction(actionFullScreen);

  actionImportCSV = new QAction;
  actionImportCSV->setText(tr("Import CSV"));
  actionImportCSV->setIcon(QIcon(":/img/import_csv.png"));
  connect(actionImportCSV, &QAction::triggered, this, &MainWindow::slot_ImportCSV);
  menuFile->insertAction(separatorMenuFile, actionImportCSV);

  actionNewTable = new QAction;
  actionNewTable->setText(tr("New table"));
  actionNewTable->setIcon(QIcon(":/img/table.png"));
  connect(actionNewTable, &QAction::triggered, this, &MainWindow::slot_newTable);
  menuTable->addAction(actionNewTable);

  actionAboutProgram = new QAction;
  actionAboutProgram->setText(tr("About program"));
  actionAboutProgram->setIcon(QIcon(":/img/about_program.png"));
  connect(actionAboutProgram, &QAction::triggered, this, &MainWindow::slot_AboutProgram);
  menuReference->addAction(actionAboutProgram);
}

void MainWindow::loadSettings()
{
  settings.loadSettings();

  resize(settings.MainWindow.width, settings.MainWindow.heidht);
}

void MainWindow::initTable()
{
  addTable(new QStandardItemModel(15,2));
}

#ifdef QT_DEBUG
void MainWindow::tst()
{
  auto rs{6}, cs{2};
  auto model = new QStandardItemModel(rs, cs);
  QCPPlotting::NumSheet data;
  data.HorizontalHeader = QStringList({"X", "Y"});
  for (int i = 0; i < cs; i++) {
      QCPPlotting::NumColumn vector;
      for (int j = 0; j < rs; j++) {
          auto num{(j + i) * (i + 1)};
          auto item = new QStandardItem(QString::number(num));
          model->setItem(j, i, item);
          vector.push_back(num);
        }
      data.push_back(vector);
    }

  mdiArea->currentSubWindow()->findChild<TableWidget *>()->setModel(model);

  addChart(data, QCPPlotting::GRAPH);

//  mdiArea->currentSubWindow()->findChild<ChartWidget *>()->tst();
}
#endif

void MainWindow::saveSettings()
{
  settings.MainWindow.width = width();
  settings.MainWindow.heidht = height();

  settings.saveSettings();
}

void MainWindow::slot_FullScreen()
{
  if (isFullScreen())
    showNormal();
  else
    showFullScreen();
}

void MainWindow::slot_ImportCSV()
{
  QFileDialog *fileDialog = new QFileDialog(this);
  fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog->setFileMode(QFileDialog::ExistingFiles);
  fileDialog->setMimeTypeFilters({"text/csv", "application/octet-stream"});
  switch (fileDialog->exec()) {
    case QDialog::Accepted:
      {
        FileDialogSetDetails *fpd = new FileDialogSetDetails(QCPPlotting::FileDialogSetDetails::Open, this);
        fpd->setSettings(settings);
        switch(fpd->exec()) {
          case QDialog::Accepted:
            {
              fpd->getSettings(settings);
              readCSV(fileDialog->selectedFiles(), fpd->getData());
              break;
            }
          case QDialog::Rejected:
            break;
          default:
            showError(tr("QDialog: Unexpected result"));
          }
        delete fpd;
        break;
      }
    case QDialog::Rejected:
      break;
    default:
      showError(tr("QDialog: Unexpected result"));
    }
  delete fileDialog;
}

void MainWindow::exportCSVTrigered(QCPPlotting::SpreadSheet spreadSheet)
{
  FileDialogSetDetails *fpd = new FileDialogSetDetails(QCPPlotting::FileDialogSetDetails::Save, this);
  fpd->setSettings(settings);
  switch(fpd->exec()) {
    case QDialog::Accepted:
      {
        fpd->getSettings(settings);
        auto key{spreadSheet.keys().at(0)};
        QFileDialog *fileDialog = new QFileDialog(this);
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setMimeTypeFilters({"text/csv", "application/octet-stream"});
//        fileDialog->selectFile(QString("%1/%2").arg(QDir::homePath(), key));
        fileDialog->selectFile(key);
        switch (fileDialog->exec()) {
          case QDialog::Accepted:
            writeCSV(spreadSheet.value(key), fileDialog->selectedFiles().at(0), fpd->getData());
            break;
          case QDialog::Rejected:
            break;
          default:
            showError(tr("QDialog: Unexpected result"));
          }
        delete fileDialog;
        break;
      }
    case QDialog::Rejected:
      break;
    default:
      showError(tr("QDialog: Unexpected result"));
    }
  delete fpd;
}

void MainWindow::slot_newTable()
{
  addTable(new QStandardItemModel(15,2));
}

void MainWindow::slot_AboutProgram()
{

}

//void MainWindow::buildGraphTrigered(QVector<QVector<double>> data)
void MainWindow::buildGraphTrigered(QCPPlotting::NumSheet data)
{
  addChart(data, QCPPlotting::GRAPH);
}

//void MainWindow::buildBarsTrigered(QVector<QVector<double>> data)
void MainWindow::buildBarsTrigered(QCPPlotting::NumSheet data)
{
  addChart(data, QCPPlotting::BARS);
}

//void MainWindow::buildCurveTrigered(QVector<QVector<double> > data)
void MainWindow::buildCurveTrigered(QCPPlotting::NumSheet data)
{
  addChart(data, QCPPlotting::CURVE);
}

void MainWindow::loadSubWindow(QWidget *widget)
{
  QMdiSubWindow *window = new QMdiSubWindow;
  window->setWidget(widget);
  window->setAttribute(Qt::WA_DeleteOnClose);
  mdiArea->addSubWindow(window);
  window->setWindowTitle(widget->windowTitle());
  window->setWindowIcon(widget->windowIcon());
  window->resize(widget->size());
  window->show();
}

void MainWindow::showError(QString err)
{
  QMessageBox::critical(this, tr("Error"), err);
}

void MainWindow::showWarning(QString war)
{
  QMessageBox::warning(this, tr("Warning"), war);
}

void MainWindow::showMessage(QString mess)
{
  QMessageBox::information(this, tr("Message"), mess);
}

void MainWindow::addTable(QStandardItemModel *item_model, QString *title)
{
  auto tableWidget = new TableWidget;
  if (!tableWidget)
    return;

  tableWidget->init();
  if (title)
    tableWidget->setWindowTitle(*title);
  tableWidget->setModel(item_model);

  connect(tableWidget, &TableWidget::error, this, &MainWindow::showError);
  connect(tableWidget, &TableWidget::warning, this, &MainWindow::showWarning);
  connect(tableWidget, &TableWidget::message, this, &MainWindow::showMessage);
  connect(tableWidget, &TableWidget::buildGraph, this, &MainWindow::buildGraphTrigered);
  connect(tableWidget, &TableWidget::buildBars, this, &MainWindow::buildBarsTrigered);
  connect(tableWidget, &TableWidget::buildCurve, this, &MainWindow::buildCurveTrigered);
  connect(tableWidget, &TableWidget::saveTable, this, &MainWindow::exportCSVTrigered);

  loadSubWindow(tableWidget);
}

void MainWindow::addTables(QCPPlotting::SpreadSheet &spreadSheet)
{
  for (auto key : spreadSheet.keys()) {
      auto sheet = spreadSheet.value(key);
      auto item_model = new QStandardItemModel;
      if (!sheet.HorizontalHeader.isEmpty()) {
          item_model->setHorizontalHeaderLabels(sheet.HorizontalHeader);
        }
      for (auto row : sheet) {
          QList<QStandardItem *> standardItemsList;
          for (auto item : row) {
               standardItemsList.append(new QStandardItem(item));
            }
          item_model->insertRow(item_model->rowCount(), standardItemsList);
          standardItemsList.clear();
        }
      addTable(item_model, &key);
    }
}

//void MainWindow::addChart(QVector<QVector<double>> &data, QCPPlotting::ChartType chartType)
void MainWindow::addChart(QCPPlotting::NumSheet &data, QCPPlotting::ChartType chartType)
{
  auto chartWidget = new ChartWidget;
  if (!chartWidget)
    return;

  chartWidget->init();
  chartWidget->setPlot(data, chartType);

//  switch (chartType) {
//    case QCPPlotting::GRAPH:
////      chartWidget->setGraph(data[0], data[1]);
//      chartWidget->setGraph(data);
//      break;
//    case QCPPlotting::BARS:
////      chartWidget->setBars(data[0], data[1]);
//      chartWidget->setBars(data);
//      break;
//    case QCPPlotting::CURVE:
////      chartWidget->setCurve(data[0], data[1], data[2]);
//      chartWidget->setCurve(data);
//    default:
//      break;
//    }

  loadSubWindow(chartWidget);
}

void MainWindow::readCSV(const QStringList files, QCPPlotting::FileDialogSetDetails::Data *data)
{
  QCPPlotting::SpreadSheet spreadSheet;

  auto delimiter{charDelimiter(data->delimiter)};

  unsigned char parameters{0};
  if (data->readZeroRow)
    parameters |= QCPPlotting::READ_ZERO_ROW;
  if (data->readZeroColumn)
    parameters |= QCPPlotting::READ_ZERO_COLUMN;
  if (data->useZeroRow)
    parameters |= QCPPlotting::USE_ZERO_ROW;

  delete data;

  LogicalMethods logicalMethods;
  connect(&logicalMethods, &LogicalMethods::error, this, &MainWindow::showError);
  connect(&logicalMethods, &LogicalMethods::warning, this, &MainWindow::showWarning);
  connect(&logicalMethods, &LogicalMethods::message, this, &MainWindow::showMessage);

  for (auto file : files) {
      if (!logicalMethods.readCSV(spreadSheet, file, delimiter, parameters))
        return;
    }

  addTables(spreadSheet);
}

void MainWindow::writeCSV(QCPPlotting::Sheet sheet, const QString file, QCPPlotting::FileDialogSetDetails::Data *data)
{
  auto delimiter{charDelimiter(data->delimiter)};

  unsigned char parameters{0};
  if (data->writeHHeader)
    parameters |= QCPPlotting::USE_ZERO_ROW;

  delete data;

  LogicalMethods logicalMethods;
  connect(&logicalMethods, &LogicalMethods::error, this, &MainWindow::showError);
  connect(&logicalMethods, &LogicalMethods::warning, this, &MainWindow::showWarning);
  connect(&logicalMethods, &LogicalMethods::message, this, &MainWindow::showMessage);

  if (!logicalMethods.writeCSV(sheet, file, delimiter, parameters))
    return;
  else
    emit showMessage(tr("'%1' is saved").arg(file));
}

char MainWindow::charDelimiter(short &delimiter)
{
  switch (delimiter) {
    case QCPPlotting::COMMA:
      return ',';
    case QCPPlotting::TAB_STEP:
      return'\t';
    case QCPPlotting::SEMICOLON:
      return ';';
    case QCPPlotting::SPACE:
      return' ';
    default:
      return ',';
    }
}
