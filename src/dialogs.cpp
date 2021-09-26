#include "dialogs.h"

#include <QDebug>

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

FileDialogSetDetails::FileDialogSetDetails(FDSD::Type t, QWidget *parent)
    : Dialogs(parent),
      T(t)
{
    setWindowTitle(tr("Set details"));

    if (T == FDSD::Open)
    {
        checkBox_readZeroRow = new QCheckBox(tr("Read zero row"));

        checkBox_readZeroColumn = new QCheckBox(tr("Read zero column"));

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

    hBoxLayout_Delimiter = new QHBoxLayout;
    hBoxLayout_Delimiter->addWidget(label_Delimiter);
    hBoxLayout_Delimiter->addWidget(comboBox_Delimiter);

    BoxLayout->addLayout(hBoxLayout_Delimiter);
    if (T == FDSD::Open)
    {
        BoxLayout->addLayout(hBoxLayout_readZero);
    }
}

FileDialogSetDetails::~FileDialogSetDetails()
{
    if (T == FDSD::Open)
    {
        delete checkBox_readZeroRow;
        delete  checkBox_readZeroColumn;
        delete hBoxLayout_readZero;
    }
    delete label_Delimiter;
    delete comboBox_Delimiter;
    delete hBoxLayout_Delimiter;
}

void FileDialogSetDetails::setData(FDSD::Data *data)
{
    checkBox_readZeroRow->setChecked(data->readZeroRow);
    checkBox_readZeroColumn->setChecked(data->readZeroColumn);
    comboBox_Delimiter->setCurrentIndex(data->delimiter);
}

FDSD::Data *FileDialogSetDetails::getData()
{
    FDSD::Data *data = new FDSD::Data;
    if (T == FDSD::Open)
    {
        data->readZeroRow = checkBox_readZeroRow->isChecked();
        data->readZeroColumn = checkBox_readZeroColumn->isChecked();
    }
    data->delimiter = comboBox_Delimiter->currentIndex();
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

DialogSavePlot::DialogSavePlot(QWidget *parent)
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

    label_Quality = new QLabel(tr("Quality"));

    spinBox_Quality = new QSpinBox;
    spinBox_Quality->setRange(0, 100);

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
    gridLayout->addWidget(label_Quality, 3, 0);
    gridLayout->addWidget(spinBox_Quality, 3, 1, 1, 3);
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
    delete label_Quality;
    delete spinBox_Quality;
    delete label_Resolution;
    delete spinBox_Resolution;
    delete comboBox_ResolutionUnit;
    delete gridLayout;
}

DSP::Data DialogSavePlot::getData()
{
    DSP::Data data;

    data.Width = spinBox_Width->value();
    data.Height = spinBox_Height->value();
    data.Scale = doubleSpinBox_Scale->value();
    data.Quality = spinBox_Quality->value();
    data.Resolution = spinBox_Resolution->value();
    data.ResolutionUnit = comboBox_ResolutionUnit->currentIndex();

    return data;
}

void DialogSavePlot::setData(DSP::Data data)
{
    spinBox_Width->setValue(data.Width);
    spinBox_Height->setValue(data.Height);
    doubleSpinBox_Scale->setValue(data.Scale);
    spinBox_Quality->setValue(data.Quality);
    spinBox_Resolution->setValue(data.Resolution);
    comboBox_ResolutionUnit->setCurrentIndex(data.ResolutionUnit);
}
