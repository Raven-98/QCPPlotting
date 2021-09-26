#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(800,600);

    mdiArea = new QMdiArea;
    mdiArea->cascadeSubWindows();
    setCentralWidget(mdiArea);

    initActions();
    initMenus();
    initMenuBar();
}

MainWindow::~MainWindow()
{
    deleteActions();
    deleteMenus();
    deleteMenuBar();
    delete mdiArea;
}

void MainWindow::initActions()
{
    actionExit = new QAction;
    actionExit->setText(tr("Exit"));
    actionExit->setIcon(QIcon(":/img/exit.png"));
    actionExit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(actionExit,&QAction::triggered,this,&MainWindow::close);

    actionFullScreen = new QAction;
    actionFullScreen->setText(tr("Full screen"));
    actionFullScreen->setIcon(QIcon(":/img/full_screen.png"));
    actionFullScreen->setShortcut(QKeySequence(Qt::Key_F11));
    connect(actionFullScreen,&QAction::triggered,this,&MainWindow::slot_FullScreen);

    actionImportCSV = new QAction;
    actionImportCSV->setText(tr("Import CSV"));
    actionImportCSV->setIcon(QIcon(":/img/import_csv.png"));
    connect(actionImportCSV,&QAction::triggered,this,&MainWindow::slot_ImportCSV);

    actionNewTable = new QAction;
    actionNewTable->setText(tr("New table"));
    actionNewTable->setIcon(QIcon(":/img/table.png"));
    connect(actionNewTable,&QAction::triggered,this,&MainWindow::slot_newTable);
}

void MainWindow::deleteActions()
{
    delete actionExit;
    delete actionFullScreen;
    delete actionImportCSV;
    delete actionNewTable;
}

void MainWindow::initMenus()
{
    menuFile = new QMenu;
    menuFile->setTitle(tr("&File"));
    menuFile->addAction(actionImportCSV);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuTable = new QMenu;
    menuTable->setTitle(tr("Table"));
    menuTable->addAction(actionNewTable);

    menuWindow = new QMenu;
    menuWindow->setTitle(tr("&Window"));
    menuWindow->addAction(actionFullScreen);
}

void MainWindow::deleteMenus()
{
    delete menuFile;
    delete menuTable;
    delete menuWindow;
}

void MainWindow::initMenuBar()
{
    menuBar = new QMenuBar();

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuTable);
    menuBar->addMenu(menuWindow);

    setMenuBar(menuBar);
}

void MainWindow::deleteMenuBar()
{
    delete menuBar;
}

void MainWindow::initToolBar()
{

}

void MainWindow::deleteToolBar()
{

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

void MainWindow::addTable(QStandardItemModel *item_model)
{
    TableWidget *tableWidget = new TableWidget;
    if (!tableWidget)
        return;
    tableWidget->setModel(item_model);
    connect(tableWidget->actionBuild_Graph,&QAction::triggered,this,&MainWindow::slot_PlotGraph);
    connect(tableWidget->actionBuild_Curve,&QAction::triggered,this,&MainWindow::slot_PlotCurve);
    connect(tableWidget->actionBuild_Bars,&QAction::triggered,this,&MainWindow::slot_PlotBars);
    connect(tableWidget->actionBuild_Financial,&QAction::triggered,this,&MainWindow::slot_PlotFinancial);
    connect(tableWidget->actionBuild_StatistacalBox,&QAction::triggered,this,&MainWindow::slot_PlotStatistacalBox);
    connect(tableWidget->action_AddTo_Graph,&QAction::triggered,this,&MainWindow::slot_addGraphs);
    connect(tableWidget->action_SaveTable,&QAction::triggered,this,&MainWindow::slot_saveTable);

    loadSubWindow(tableWidget);
}

void MainWindow::readCSV(QString file, FDSD::Data *r_data)
{
    if (file.data()->isNull())
    {
        QMessageBox::critical(this,tr("Error"),tr("No file name specified"));
        return;
    }
    QFile *file_ = new QFile(file);
    if ( !file_->open(QFile::ReadOnly | QFile::Text) )
    {
        QMessageBox::critical(this,tr("Error"),tr("File not exists"));
        delete file_;
        return;
    }
    QTextStream *in = new QTextStream(file_);
    QString *line = new QString;
    QStandardItemModel *item_model = new QStandardItemModel;
    QList<QStandardItem *> standardItemsList;
    char delimiter;
    switch (r_data->delimiter) {
    case FDSD::Delimiter::Comma:
        delimiter = ',';
        break;
    case FDSD::Delimiter::Tab_step:
        delimiter = '\t';
        break;
    case FDSD::Delimiter::Semicolon:
        delimiter = ';';
        break;
    case FDSD::Delimiter::Space:
        delimiter = ' ';
        break;
    default:
        delimiter = ',';
        break;
    }
    while (!in->atEnd())
    {
        *line = in->readLine();
        for (QString item : line->split(delimiter))
        {
            standardItemsList.append(new QStandardItem(item));
        }
        item_model->insertRow(item_model->rowCount(), standardItemsList);
        standardItemsList.clear();
    }
    delete line;
    delete in;
    file_->close();
    delete file_;
    if (!r_data->readZeroRow)
        item_model->removeRow(0);
    if (!r_data->readZeroColumn)
        item_model->removeColumn(0);
    addTable(item_model);
}

void MainWindow::buildChart(QVector<QVector<double> > data, ChartWidget::ChartStyle chartStyle)
{
    if (!checkTableData(data))
        return;

    ChartWidget *chartWidget = new ChartWidget;
    if (!chartWidget)
        return;
    if (chartStyle == ChartWidget::Graph)
        chartWidget->setDataGraph(data);

    connect(chartWidget->action_Save,&QAction::triggered,this,&MainWindow::slot_savePlot);
    loadSubWindow(chartWidget);
}

void MainWindow::addChart(QList<ChartWidget *> widgetsList, QVector<QVector<double> > data, ChartWidget::ChartStyle chartStyle)
{
    if (!checkTableData(data))
        return;

    for (auto &widget : widgetsList)
    {
        if (chartStyle == ChartWidget::Graph)
            widget->addDataGraph(data);
    }
}

bool MainWindow::checkTableData(QVector<QVector<double> > data)
{
    int *s = new int(data.size());
    if (*s < 2)
    {
        QMessageBox::critical(this,tr("Error"),tr("The selected table is empty"));
        delete s;
        return false;
    }
    for (int i = 0;i < *s;i++)
    {
        if (!data[i].size())
        {
            QMessageBox::critical(this,tr("Error"),tr("Zero column size"));
            delete s;
            return false;
        }
    }
    for (int i = 1;i < *s;i++)
    {
        if (data[i-1].size() != data[i].size())
        {
            QMessageBox::critical(this,tr("Error"),tr("Table columns have different sizes"));
            delete s;
            return false;
        }
    }
    delete s;

    return true;
}

void MainWindow::saveTable(QVector<QVector<double> > data)
{
    if (!checkTableData(data))
        return;

    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setMimeTypeFilters({"text/csv", "application/octet-stream"});
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
    {
        FileDialogSetDetails *fpd = new FileDialogSetDetails(FDSD::Save, this);
        switch(fpd->exec())
        {
        case QDialog::Accepted:
        {
            QFile *fil = new QFile(fileDialog->selectedFiles().at(0));
            if (!fil->open(QIODevice::WriteOnly))
            {
                fil->close();
                QMessageBox::critical(this,tr("Error"),tr("Error")+": "+fil->errorString());
            }
            else
            {
                QTextStream *ts = new QTextStream(fil);
                char delimiter;
                switch (fpd->getData()->delimiter) {
                case FDSD::Delimiter::Comma:
                    delimiter = ',';
                    break;
                case FDSD::Delimiter::Tab_step:
                    delimiter = '\t';
                    break;
                case FDSD::Delimiter::Semicolon:
                    delimiter = ';';
                    break;
                case FDSD::Delimiter::Space:
                    delimiter = ' ';
                    break;
                default:
                    delimiter = ',';
                    break;
                }
                int c = data.size();
                for (int i = 0; i < data[0].size(); i++)
                {
                    for (int j = 0; j < c-1; j++)
                    {
                        *ts << data[j][i] << delimiter;
                    }
                    *ts << data[c-1][i] << "\n";
                }
                fil->close();
                delete ts;
                QMessageBox::information(this,"",tr("Saving was successful"));
            }
            delete fil;
            break;
        }
        case QDialog::Rejected:
            break;
        default:
            QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
            break;
        }
        delete fpd;
        break;
    }
    case QDialog::Rejected:
        break;
    default:
        QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
        break;
    }
    delete fileDialog;
}

void MainWindow::savePlot()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setMimeTypeFilters({"image/png",
                                    "image/jpeg",
                                    "image/bmp",
//                                    "application/pdf"
                                   });
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
    {
        QString format =  fileDialog->selectedFiles().at(0).split(".").takeLast();
        DialogSavePlot *dialogSavePlot = new DialogSavePlot(format, this);
        DSP::Data data;
        data.Width = qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->width();
        data.Height = qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->height();
        data.Scale = 1.0;
        data.Quality = 90;
        data.Resolution = 96;
        data.ResolutionUnit = 2;
        dialogSavePlot->setData(data);
        switch (dialogSavePlot->exec())
        {
        case DialogSavePlot::Accepted:
        {
            data = dialogSavePlot->getData();
            if (format == "png")
                qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->savePng(fileDialog->selectedFiles().at(0),
                                                                                                       data.Width,
                                                                                                       data.Height,
                                                                                                       data.Scale,
                                                                                                       data.Quality,
                                                                                                       data.Resolution,
                                                                                                       static_cast<QCP::ResolutionUnit>(data.ResolutionUnit));
            else if (format == "jpg")
                qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->saveJpg(fileDialog->selectedFiles().at(0),
                                                                                                       data.Width,
                                                                                                       data.Height,
                                                                                                       data.Scale,
                                                                                                       data.Quality,
                                                                                                       data.Resolution,
                                                                                                       static_cast<QCP::ResolutionUnit>(data.ResolutionUnit));
            else if (format == "bmp")
                qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->saveBmp(fileDialog->selectedFiles().at(0),
                                                                                                       data.Width,
                                                                                                       data.Height,
                                                                                                       data.Scale,
                                                                                                       data.Resolution,
                                                                                                       static_cast<QCP::ResolutionUnit>(data.ResolutionUnit));
//            else if (format == "pdf")
//                qobject_cast<ChartWidget *>(mdiArea->activeSubWindow()->widget())->customPlot->savePdf(fileDialog->selectedFiles().at(0), data.Width, data.Height, data.Scale);
            break;
        }
        case DialogSavePlot::Rejected:
            break;
        default:
            QMessageBox::critical(dialogSavePlot,tr("Error"),tr("Unexpected result"));
            break;
        }
        QMessageBox::information(dialogSavePlot, "", tr("Saving was successful"));
        delete dialogSavePlot;
        break;
    }
    case QDialog::Rejected:
        break;
    default:
        QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
        break;
    }
    delete fileDialog;
}

void MainWindow::initTable()
{
    addTable(new QStandardItemModel(15,2));
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
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setMimeTypeFilters({"text/csv", "application/octet-stream"});
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
    {
        FileDialogSetDetails *fpd = new FileDialogSetDetails(FDSD::Open, this);
//        fpd->setData(data);
        switch(fpd->exec())
        {
        case QDialog::Accepted:
            readCSV(fileDialog->selectedFiles().at(0), fpd->getData());
            break;
        case QDialog::Rejected:
            break;
        default:
            QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
            break;
        }
        delete fpd;
        break;
    }
    case QDialog::Rejected:
        break;
    default:
        QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
        break;
    }
    delete fileDialog;
}

void MainWindow::slot_PlotGraph()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::Graph);
}

void MainWindow::slot_PlotCurve()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::Curve);
}

void MainWindow::slot_PlotBars()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::Bars);
}

void MainWindow::slot_PlotFinancial()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::Financial);
}

void MainWindow::slot_PlotStatistacalBox()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::StatisticalBox);
}

void MainWindow::slot_addGraphs()
{
//    QList<ChartWidget *> widgetsList;
    QStringList widgetsList;

    for (auto &it_lst : mdiArea->subWindowList())
    {
        if (QString(it_lst->widget()->metaObject()->className()) == "ChartWidget")
//            widgetsList.append(qobject_cast<ChartWidget *>(it_lst->widget()));
            widgetsList.append(qobject_cast<ChartWidget *>(it_lst->widget())->windowTitle());
    }

    DialodAddPlot *dialogAddPlot = new DialodAddPlot(widgetsList, this);
    switch (dialogAddPlot->exec())
    {
    case QDialog::Accepted:
    {
        QList<ChartWidget *> wList;
        QStringList list = dialogAddPlot->getData();
//        for (auto item : dialogAddPlot->getData())
        for (auto &it_lst : mdiArea->subWindowList())
        {
            if (list.contains(it_lst->widget()->windowTitle()))
                wList.append(qobject_cast<ChartWidget *>(it_lst->widget()));
        }
        list.clear();
        addChart(wList, qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), ChartWidget::Graph);
        wList.clear();
        break;
    }
    case QDialog::Rejected:
        break;
    default:
        QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
        break;
    }

    delete dialogAddPlot;
}

void MainWindow::slot_saveTable()
{
    saveTable(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData());
}

void MainWindow::slot_newTable()
{
    addTable(new QStandardItemModel(15,2));
}

void MainWindow::slot_savePlot()
{
    savePlot();
}
