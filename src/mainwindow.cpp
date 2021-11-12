#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{
    saveSettings();

    deleteActions();
    deleteMenus();
    deleteMenuBar();
    delete mdiArea;
}

void MainWindow::init()
{
    setMinimumSize(800,600);

    mdiArea = new QMdiArea;
    mdiArea->cascadeSubWindows();
    setCentralWidget(mdiArea);

    initActions();
    initMenus();
    initMenuBar();
}

void MainWindow::loadSettings()
{
    settings.loadSettings(new QString("/sett.ini"));

    resize(settings.MainWindow.width, settings.MainWindow.heidht);
}

void MainWindow::saveSettings()
{
    settings.MainWindow.width = width();
    settings.MainWindow.heidht = height();

    settings.saveSettings(new QString("/sett.ini"));
}

void MainWindow::initActions()
{
    actionExit = new QAction;
    actionExit->setText(tr("Exit"));
    actionExit->setIcon(QIcon(":/img/exit.png"));
    actionExit->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
    connect(actionExit,
            &QAction::triggered,
            this,
            &MainWindow::close);

    actionFullScreen = new QAction;
    actionFullScreen->setText(tr("Full screen"));
    actionFullScreen->setIcon(QIcon(":/img/full_screen.png"));
    actionFullScreen->setShortcut(QKeySequence(Qt::Key_F11));
    connect(actionFullScreen,
            &QAction::triggered,
            this,
            &MainWindow::slot_FullScreen);

    actionImportCSV = new QAction;
    actionImportCSV->setText(tr("Import CSV"));
    actionImportCSV->setIcon(QIcon(":/img/import_csv.png"));
    connect(actionImportCSV,
            &QAction::triggered,
            this,
            &MainWindow::slot_ImportCSV);

    actionNewTable = new QAction;
    actionNewTable->setText(tr("New table"));
    actionNewTable->setIcon(QIcon(":/img/table.png"));
    connect(actionNewTable,
            &QAction::triggered,
            this,
            &MainWindow::slot_newTable);

    actionAnalyzeDiffractionData_DRON2 = new QAction;
    actionAnalyzeDiffractionData_DRON2->setText((tr("Analyze diffraction data (DRON-2)")));
    connect(actionAnalyzeDiffractionData_DRON2,
            &QAction::triggered,
            this,
            &MainWindow::slot_AnalyzeDiffractionDataDRON2);

    actionAboutProgram = new QAction;
    actionAboutProgram->setText(tr("About program"));
    connect(actionAboutProgram,
            &QAction::triggered,
            this,
            &MainWindow::slot_AboutProgram);
}

void MainWindow::deleteActions()
{
    delete actionExit;
    delete actionFullScreen;
    delete actionImportCSV;
    delete actionNewTable;
    delete actionAnalyzeDiffractionData_DRON2;
    delete actionAboutProgram;
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

    menuExtra = new QMenu;
    menuExtra->setTitle(tr("&Extra"));
    menuExtra->addAction(actionAnalyzeDiffractionData_DRON2);

    menuHelp = new QMenu;
    menuHelp->setTitle(tr("&Help"));
    menuHelp->addAction(actionAboutProgram);
}

void MainWindow::deleteMenus()
{
    delete menuFile;
    delete menuTable;
    delete menuWindow;
    delete menuExtra;
    delete menuHelp;
}

void MainWindow::initMenuBar()
{
    menuBar = new QMenuBar();

    menuBar->addMenu(menuFile);
    menuBar->addMenu(menuTable);
    menuBar->addMenu(menuWindow);
    menuBar->addMenu(menuExtra);
    menuBar->addMenu(menuHelp);

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

void MainWindow::readCSV(QString file, GBS::FDSD::Data *r_data)
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
    case GBS::FDSD::Delimiter::Comma:
        delimiter = ',';
        break;
    case GBS::FDSD::Delimiter::Tab_step:
        delimiter = '\t';
        break;
    case GBS::FDSD::Delimiter::Semicolon:
        delimiter = ';';
        break;
    case GBS::FDSD::Delimiter::Space:
        delimiter = ' ';
        break;
    default:
        delimiter = ',';
        break;
    }
    while (!in->atEnd())
    {
        *line = in->readLine();
        for (const auto &item : line->split(delimiter))
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

void MainWindow::buildChart(QVector<QVector<double> > data, GBS::CW::ChartStyle chartStyle)
{
    if (!checkTableData(data))
        return;

    ChartWidget *chartWidget = new ChartWidget;
    if (!chartWidget)
        return;
    if (chartStyle == GBS::CW::Graph)
        chartWidget->setDataGraph(data);

    connect(chartWidget->action_Save,&QAction::triggered,this,&MainWindow::slot_savePlot);
    loadSubWindow(chartWidget);
}

void MainWindow::addChart(QList<ChartWidget *> widgetsList, QVector<QVector<double> > data, GBS::CW::ChartStyle chartStyle)
{
    if (!checkTableData(data))
        return;

    for (auto &widget : widgetsList)
    {
        if (chartStyle == GBS::CW::Graph)
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
        FileDialogSetDetails *fpd = new FileDialogSetDetails(settings, GBS::FDSD::Save, this);
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
                case GBS::FDSD::Delimiter::Comma:
                    delimiter = ',';
                    break;
                case GBS::FDSD::Delimiter::Tab_step:
                    delimiter = '\t';
                    break;
                case GBS::FDSD::Delimiter::Semicolon:
                    delimiter = ';';
                    break;
                case GBS::FDSD::Delimiter::Space:
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
    fileDialog->setDirectory(settings.DialogSavePlot.filePaht);
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
    {
        QString format =  fileDialog->selectedFiles().at(0).split(".").takeLast();
        DialogSavePlot *dialogSavePlot = new DialogSavePlot(format, this);
        GBS::DSP::Data data;
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

            settings.DialogSavePlot.filePaht = fileDialog->selectedFiles().at(0).section("/",0,-2);
            QMessageBox::information(dialogSavePlot, "", tr("Saving was successful"));
            break;
        }
        case DialogSavePlot::Rejected:
            break;
        default:
            QMessageBox::critical(dialogSavePlot,tr("Error"),tr("Unexpected result"));
            break;
        }
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
    fileDialog->setDirectory(settings.FileDialogSetDetails.filePaht);
    fileDialog->setMimeTypeFilters({"text/csv", "application/octet-stream"});
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
    {
        FileDialogSetDetails *fpd = new FileDialogSetDetails(settings, GBS::FDSD::Open, this);
//        fpd->setData(data);
        switch(fpd->exec())
        {
        case QDialog::Accepted:
            readCSV(fileDialog->selectedFiles().at(0), fpd->getData());
            settings.FileDialogSetDetails.filePaht = fileDialog->selectedFiles().at(0).section("/",0,-2);
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
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::Graph);
}

void MainWindow::slot_PlotCurve()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::Curve);
}

void MainWindow::slot_PlotBars()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::Bars);
}

void MainWindow::slot_PlotFinancial()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::Financial);
}

void MainWindow::slot_PlotStatistacalBox()
{
    buildChart(qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::StatisticalBox);
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
        addChart(wList, qobject_cast<TableWidget *>(mdiArea->activeSubWindow()->widget())->getData(), GBS::CW::Graph);
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

void MainWindow::slot_AnalyzeDiffractionDataDRON2()
{
    DialogAnalyzeDiffractionDataDRON2 *daddDRON = new DialogAnalyzeDiffractionDataDRON2(settings, this);
    switch (daddDRON->exec())
    {
    case QDialog::Accepted:
    {
        GBS::DADDDRON2::Data data;
        data = daddDRON->getData();
        QThread* thread = new QThread;
        AnalyzeDiffractionDataDRON2 *worker = new AnalyzeDiffractionDataDRON2(data);
        worker->moveToThread(thread);
        connect(thread,
                &QThread::started,
                worker,
                &AnalyzeDiffractionDataDRON2::run);
        connect(worker,
                &AnalyzeDiffractionDataDRON2::quit,
                thread,
                &QThread::quit);
        connect(worker,
                &AnalyzeDiffractionDataDRON2::finished,
                this,
                [=](QStandardItemModel *item_model){addTable(item_model);});
        connect(worker,
                &AnalyzeDiffractionDataDRON2::error,
                this,
                [=](QString err){QMessageBox::critical(this, tr("Error"), err);});
        thread->start();
        break;
    }
    case QDialog::Rejected:
        break;
    default:
        QMessageBox::critical(this,tr("Error"),tr("QDialog: Unexpected result"));
        break;
    }
    delete daddDRON;
}

void MainWindow::slot_AboutProgram()
{
    AboutProgram *aboutProgram = new AboutProgram(this);
    aboutProgram->exec();
    delete aboutProgram;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
//    if (QMessageBox::Yes == QMessageBox::question(this,
//                                                  tr("Close Confirmation"),
//                                                  tr("Exit?"),
//                                                  QMessageBox::Yes | QMessageBox::No))
//    {
        mdiArea->closeAllSubWindows();
        event->accept();
//    }
}
