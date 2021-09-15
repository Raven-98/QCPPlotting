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
}

DialogSavePlot::~DialogSavePlot()
{

}
