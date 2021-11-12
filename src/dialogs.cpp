#include "dialogs.h"

Dialogs::Dialogs(QWidget *parent)
    : QDialog(parent)
{
    dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(dialogButtonBox,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(dialogButtonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);

    BoxLayout = new QBoxLayout(QBoxLayout::BottomToTop);
    BoxLayout->addWidget(dialogButtonBox);

    setLayout(BoxLayout);
}

Dialogs::~Dialogs()
{
    delete dialogButtonBox;
    delete BoxLayout;
}

FileDialogSetDetails::FileDialogSetDetails(GBS::Settings &sett, GBS::FDSD::Type t, QWidget *parent)
    : Dialogs(parent),
      settings(sett),
      T(t)
{
    setWindowTitle(tr("Set details"));

    if (T == GBS::FDSD::Open)
    {
        checkBox_readZeroRow = new QCheckBox(tr("Read zero row"));
        checkBox_readZeroRow->setChecked(settings.FileDialogSetDetails.readZeroRow);

        checkBox_readZeroColumn = new QCheckBox(tr("Read zero column"));
        checkBox_readZeroColumn->setChecked(settings.FileDialogSetDetails.readZeroColumn);

        hBoxLayout_readZero = new QHBoxLayout;
        hBoxLayout_readZero->addWidget(checkBox_readZeroRow);
        hBoxLayout_readZero->addWidget(checkBox_readZeroColumn);
    }

    label_Delimiter = new QLabel(tr("Delimiter"));

    comboBox_Delimiter = new QComboBox;
    comboBox_Delimiter->addItems({tr("Comma"),
                                  tr("Tab step"),
                                  tr("Semicolon"),
                                  tr("Space")
                                 });
    comboBox_Delimiter->setCurrentIndex(settings.FileDialogSetDetails.delimiter);

    hBoxLayout_Delimiter = new QHBoxLayout;
    hBoxLayout_Delimiter->addWidget(label_Delimiter);
    hBoxLayout_Delimiter->addWidget(comboBox_Delimiter);

    BoxLayout->addLayout(hBoxLayout_Delimiter);
    if (T == GBS::FDSD::Open)
    {
        BoxLayout->addLayout(hBoxLayout_readZero);
    }
}

FileDialogSetDetails::~FileDialogSetDetails()
{
    if (T == GBS::FDSD::Open)
    {
        delete checkBox_readZeroRow;
        delete  checkBox_readZeroColumn;
        delete hBoxLayout_readZero;
    }
    delete label_Delimiter;
    delete comboBox_Delimiter;
    delete hBoxLayout_Delimiter;
}

void FileDialogSetDetails::setData(GBS::FDSD::Data *data)
{
    checkBox_readZeroRow->setChecked(data->readZeroRow);
    checkBox_readZeroColumn->setChecked(data->readZeroColumn);
    comboBox_Delimiter->setCurrentIndex(data->delimiter);
}

GBS::FDSD::Data *FileDialogSetDetails::getData()
{
    GBS::FDSD::Data *data = new GBS::FDSD::Data;
    if (T == GBS::FDSD::Open)
    {
        data->readZeroRow = checkBox_readZeroRow->isChecked();
        data->readZeroColumn = checkBox_readZeroColumn->isChecked();
    }
    data->delimiter = comboBox_Delimiter->currentIndex();

    settings.FileDialogSetDetails << *data;

    return data;
}

//DialodAddPlot::DialodAddPlot(QList<ChartWidget *> &widgetsList, QWidget *parent)
DialodAddPlot::DialodAddPlot(QStringList &widgetsList, QWidget *parent)
    : Dialogs(parent),
      WidgetsList(widgetsList)
{
    setWindowTitle(tr("Add to"));

    vBoxLayout = new QVBoxLayout;
    for (int i = 0; i < WidgetsList.size(); i++)
    {
//        vBoxLayout->addWidget(new QCheckBox(WidgetsList.at(i)->windowTitle()));
        vBoxLayout->addWidget(new QCheckBox(WidgetsList.at(i)));
    }

    groupBox = new QGroupBox;
    groupBox->setFlat(true);
    groupBox->setLayout(vBoxLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(groupBox);

    checkBox_NewCoordinateSystem = new QCheckBox("New coordinate system");
    checkBox_NewCoordinateSystem->setEnabled(false);

    BoxLayout->addWidget(checkBox_NewCoordinateSystem);
    BoxLayout->addWidget(scrollArea);
}

DialodAddPlot::~DialodAddPlot()
{
    for (auto &checkBox : vBoxLayout->findChildren<QCheckBox *>())
    {
        delete checkBox;
    }
    delete vBoxLayout;
    delete groupBox;
    delete scrollArea;
    delete checkBox_NewCoordinateSystem;
}

//QList<ChartWidget *> DialodAddPlot::getData()
QStringList DialodAddPlot::getData()
{
//    QList<ChartWidget *> lst;
    QStringList lst;

    int *st = new int(0);
    for(auto &checkbox : groupBox->findChildren<QCheckBox *>())
    {
        if (checkbox->isChecked())
        {
            lst.append(WidgetsList.at(*st));
        }
        *st = *st + 1;
    }
    delete st;

    return lst;
}

DialogSavePlot::DialogSavePlot(QString format, QWidget *parent)
    : Dialogs(parent)
{
    setWindowTitle(tr("Save"));

    label_Width = new QLabel(tr("Width"));

    spinBox_Width = new QSpinBox;
    spinBox_Width->setRange(0, INT_MAX);

    label_WidthUnit = new QLabel("px");

    label_Height = new QLabel(tr("Height"));

    spinBox_Height = new QSpinBox;
    spinBox_Height->setRange(0, INT_MAX);

    label_HeightUnit = new QLabel("px");

    label_Scale = new QLabel(tr("Scale"));

    doubleSpinBox_Scale = new QDoubleSpinBox;
    doubleSpinBox_Scale->setRange(0, INT_MAX);

    if (format == "png" or format == "jpg")
    {
        label_Quality = new QLabel(tr("Quality"));

        spinBox_Quality = new QSpinBox;
        spinBox_Quality->setRange(0, 100);
    }

    label_Resolution = new QLabel(tr("Resolution"));

    spinBox_Resolution = new QSpinBox;
    spinBox_Resolution->setRange(0, INT_MAX);

    comboBox_ResolutionUnit = new QComboBox;
    comboBox_ResolutionUnit->addItems({"dpm",
                                       "dpcm",
                                       "ppi"
                                      });

    gridLayout = new QGridLayout;
    gridLayout->addWidget(label_Width, 0, 0);
    gridLayout->addWidget(spinBox_Width, 0, 1, 1, 2);
    gridLayout->addWidget(label_WidthUnit, 0, 3);
    gridLayout->addWidget(label_Height, 1, 0);
    gridLayout->addWidget(spinBox_Height, 1, 1, 1, 2);
    gridLayout->addWidget(label_HeightUnit, 1, 3);
    gridLayout->addWidget(label_Scale, 2, 0);
    gridLayout->addWidget(doubleSpinBox_Scale, 2, 1, 1, 3);
    if (format == "png" or format == "jpg")
    {
        gridLayout->addWidget(label_Quality, 3, 0);
        gridLayout->addWidget(spinBox_Quality, 3, 1, 1, 3);
    }
    gridLayout->addWidget(label_Resolution, 4, 0);
    gridLayout->addWidget(spinBox_Resolution, 4, 1);
    gridLayout->addWidget(comboBox_ResolutionUnit, 4, 2, 1, 2);
    gridLayout->addLayout(new Spacer(1, 0), 5, 1);

    BoxLayout->addLayout(gridLayout);
}

DialogSavePlot::~DialogSavePlot()
{
    delete label_Width;
    delete spinBox_Width;
    delete label_WidthUnit;
    delete label_Height;
    delete spinBox_Height;
    delete label_HeightUnit;
    delete label_Scale;
    delete doubleSpinBox_Scale;
    if (label_Quality)
        delete label_Quality;
    if (spinBox_Quality)
        delete spinBox_Quality;
    delete label_Resolution;
    delete spinBox_Resolution;
    delete comboBox_ResolutionUnit;
    delete gridLayout;
}

GBS::DSP::Data DialogSavePlot::getData()
{
    GBS::DSP::Data data;

    data.Width = spinBox_Width->value();
    data.Height = spinBox_Height->value();
    data.Scale = doubleSpinBox_Scale->value();
    if (spinBox_Quality)
        data.Quality = spinBox_Quality->value();
    data.Resolution = spinBox_Resolution->value();
    data.ResolutionUnit = comboBox_ResolutionUnit->currentIndex();

    return data;
}

void DialogSavePlot::setData(GBS::DSP::Data data)
{
    spinBox_Width->setValue(data.Width);
    spinBox_Height->setValue(data.Height);
    doubleSpinBox_Scale->setValue(data.Scale);
    if (spinBox_Quality)
        spinBox_Quality->setValue(data.Quality);
    spinBox_Resolution->setValue(data.Resolution);
    comboBox_ResolutionUnit->setCurrentIndex(data.ResolutionUnit);
}

DialogAnalyzeDiffractionDataDRON2::DialogAnalyzeDiffractionDataDRON2(GBS::Settings &sett, QWidget *parent)
    : Dialogs(parent),
      settings(sett)
{
    setWindowTitle(tr("Analyze diffraction data (DRON-2)"));
    setMinimumSize(400,200);

    labelBoxTitleInfo = new QLabel;
    labelBoxTitleInfo->setText(tr("<html><head/><body><p><b>Info</b></span></p></body></html>"));

    labelInfo = new QLabel;
    labelInfo->setWordWrap(true);
    labelInfo->setText(tr("<html><head/><body><p>"
                            "Analysis of diffraction data from digital multimeter data<br>"
                            "<i>(DRON-2, Department of Solid State Physics, V. N. Karazin Kharkiv National University)</i>"
                            "</span></p></body></html>"));

    vBoxLayoutInfo = new QVBoxLayout;
    vBoxLayoutInfo->addWidget(labelBoxTitleInfo);
    vBoxLayoutInfo->addLayout(new Spacer(0, 1));
    vBoxLayoutInfo->addWidget(labelInfo);
    vBoxLayoutInfo->addLayout(new Spacer(0, 1));

    groupBoxInfo = new QGroupBox;
    groupBoxInfo->setLayout(vBoxLayoutInfo);

    labelFile = new QLabel;
    labelFile->setText(tr("<html><head/><body><p>"
                            "<b>File</b>"
                            "</span></p></body></html>"));

    lineEditFile = new QLineEdit;
    lineEditFile->setText(settings.DialogAnalyzeDiffractionDataDRON2.fileName);

    pushButtonOpen = new QPushButton;
    pushButtonOpen->setText(tr("Open"));
    connect(pushButtonOpen,
            &QPushButton::clicked,
            this,
            &DialogAnalyzeDiffractionDataDRON2::slot_pushButtonOpen);

    hBoxLayoutFile = new QHBoxLayout;
    hBoxLayoutFile->addWidget(labelFile);
    hBoxLayoutFile->addWidget(lineEditFile);
    hBoxLayoutFile->addWidget(pushButtonOpen);

    groupBoxFile = new QGroupBox;
    groupBoxFile->setLayout(hBoxLayoutFile);

    labelBoxTitleShootingAngles = new QLabel;
    labelBoxTitleShootingAngles->setText(tr("<html><head/><body><p>"
                                            "<b>Shooting angles</b>"
                                            "</span></p></body></html>"));

    labelTwoThetaStart = new QLabel;
    labelTwoThetaStart->setText("<html><head/><body><p align=\"right\">"
                                "2θ<span style=\" vertical-align:sub;\">start</span>"
                                "</p></body></html>");

    lineEditTwoThetaStart = new QLineEdit;
    lineEditTwoThetaStart->setText(QString::number(settings.DialogAnalyzeDiffractionDataDRON2.twoThetaStart));

    labelTwoThetaEnd = new QLabel;
    labelTwoThetaEnd->setText("<html><head/><body><p align=\"right\">"
                                "2θ<span style=\" vertical-align:sub;\">end</span>"
                                "</p></body></html>");

    lineEditTwoThetaEnd = new QLineEdit;
    lineEditTwoThetaEnd->setText(QString::number(settings.DialogAnalyzeDiffractionDataDRON2.twoThetaEnd));

    gridLayoutShootingAngles = new QGridLayout;
    gridLayoutShootingAngles->addWidget(labelBoxTitleShootingAngles, 0, 0, 1, 4);
    gridLayoutShootingAngles->addWidget(labelTwoThetaStart, 1, 0);
    gridLayoutShootingAngles->addWidget(lineEditTwoThetaStart, 1, 1);
    gridLayoutShootingAngles->addWidget(labelTwoThetaEnd, 1, 2);
    gridLayoutShootingAngles->addWidget(lineEditTwoThetaEnd, 1, 3);

    groupBoxShootingAngles = new QGroupBox;
    groupBoxShootingAngles->setLayout(gridLayoutShootingAngles);

    labelDelimiter = new QLabel;
    labelDelimiter->setText(tr("<html><head/><body><p>"
                                "<b>Delimiter</b>"
                                "</span></p></body></html>"));

    comboBoxDelimiter = new QComboBox;
    comboBoxDelimiter->addItems({"Comma", "Tab step", "Semicolon", "Space"});
    comboBoxDelimiter->setCurrentIndex(settings.DialogAnalyzeDiffractionDataDRON2.delimiter);

    gridLayotDelimiter = new QGridLayout;
    gridLayotDelimiter->addWidget(labelDelimiter, 0, 0);
    gridLayotDelimiter->addWidget(comboBoxDelimiter, 0, 1);
    gridLayotDelimiter->addLayout(new Spacer(1,0), 1, 1);

    groupBoxDelimiter = new QGroupBox;
    groupBoxDelimiter->setLayout(gridLayotDelimiter);

    BoxLayout->addWidget(groupBoxDelimiter);
    BoxLayout->addWidget(groupBoxShootingAngles);
    BoxLayout->addWidget(groupBoxFile);
    BoxLayout->addWidget(groupBoxInfo);
}

DialogAnalyzeDiffractionDataDRON2::~DialogAnalyzeDiffractionDataDRON2()
{
    delete labelBoxTitleInfo;
    delete labelInfo;
    delete vBoxLayoutInfo;
    delete groupBoxInfo;

    delete labelFile;
    delete lineEditFile;
    delete pushButtonOpen;
    delete hBoxLayoutFile;
    delete groupBoxFile;

    delete labelBoxTitleShootingAngles;
    delete labelTwoThetaStart;
    delete labelTwoThetaEnd;
    delete lineEditTwoThetaStart;
    delete lineEditTwoThetaEnd;
    delete gridLayoutShootingAngles;
    delete groupBoxShootingAngles;

    delete labelDelimiter;
    delete comboBoxDelimiter;
    delete gridLayotDelimiter;
    delete groupBoxDelimiter;
}

GBS::DADDDRON2::Data DialogAnalyzeDiffractionDataDRON2::getData()
{
    GBS::DADDDRON2::Data data;

    data.fileName = lineEditFile->text();
    data.twoThetaStart = lineEditTwoThetaStart->text().toFloat();
    data.twoThetaEnd = lineEditTwoThetaEnd->text().toFloat();
    data.delimiter = comboBoxDelimiter->currentIndex();

    settings.DialogAnalyzeDiffractionDataDRON2 << data;

    return data;
}

void DialogAnalyzeDiffractionDataDRON2::slot_pushButtonOpen()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setDirectory(settings.DialogAnalyzeDiffractionDataDRON2.fileName.section("/",0,-2));
    fileDialog->setMimeTypeFilters({"text/plain", "text/csv", "application/octet-stream"});
    switch (fileDialog->exec())
    {
    case QDialog::Accepted:
        lineEditFile->setText(fileDialog->selectedFiles().at(0));
        break;
    case QDialog::Rejected:
        break;
    }
    delete fileDialog;
}
