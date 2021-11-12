#ifndef DIALOGS_H
#define DIALOGS_H

#include "global.h"
//#include "chartwidget.h"
#include "spacer.h"


class Dialogs;
class FileDialogSetDetails;
class DialodAddPlot;
class DialogSavePlot;
class DialogAnalyzeDiffractionDataDRON2;

class Dialogs : public QDialog
{
    Q_OBJECT

public:
    Dialogs(QWidget *parent = nullptr);
    ~Dialogs();

    QBoxLayout *BoxLayout;

private:
    QDialogButtonBox *dialogButtonBox;
};

class FileDialogSetDetails : public Dialogs
{
    Q_OBJECT
public:
    FileDialogSetDetails(GBS::Settings &sett, GBS::FDSD::Type t, QWidget *parent = nullptr);
    ~FileDialogSetDetails();

    void setData(GBS::FDSD::Data *data);
    GBS::FDSD::Data *getData();

private:
    GBS::Settings &settings;
    GBS::FDSD::Type T;
    QHBoxLayout *hBoxLayout_readZero = nullptr;
    QHBoxLayout *hBoxLayout_Delimiter;
    QCheckBox *checkBox_readZeroRow = nullptr;
    QCheckBox *checkBox_readZeroColumn = nullptr;
    QLabel *label_Delimiter;
    QComboBox *comboBox_Delimiter;
};

class DialodAddPlot : public Dialogs
{
    Q_OBJECT
public:
//    DialodAddPlot(QList<ChartWidget *> &widgetsList, QWidget *parent = nullptr);
    DialodAddPlot(QStringList &widgetsList, QWidget *parent = nullptr);
    ~DialodAddPlot();
//    QList<ChartWidget *> getData();
    QStringList getData();

private:
    QGroupBox *groupBox;
    QScrollArea *scrollArea;
    QVBoxLayout *vBoxLayout;
//    QList<ChartWidget *> &WidgetsList;
    QStringList &WidgetsList;
    QCheckBox *checkBox_NewCoordinateSystem;
};

class DialogSavePlot : public Dialogs
{
    Q_OBJECT
public:
    DialogSavePlot(QString format, QWidget *parent = nullptr);
    ~DialogSavePlot();

    GBS::DSP::Data getData();
    void setData(GBS::DSP::Data data);

private:
    QSpinBox *spinBox_Width;
    QSpinBox *spinBox_Height;
    QLabel *label_Width;
    QLabel *label_Height;
    QLabel *label_WidthUnit;
    QLabel *label_HeightUnit;
    QGridLayout *gridLayout;
    QLabel *label_Scale;
    QDoubleSpinBox *doubleSpinBox_Scale;
    QLabel *label_Quality = nullptr;
    QSpinBox *spinBox_Quality = nullptr;
    QLabel *label_Resolution;
    QSpinBox *spinBox_Resolution;
    QComboBox *comboBox_ResolutionUnit;
};

class DialogAnalyzeDiffractionDataDRON2 : public Dialogs
{
    Q_OBJECT

public:
    DialogAnalyzeDiffractionDataDRON2(GBS::Settings &sett, QWidget *parent = nullptr);
    ~DialogAnalyzeDiffractionDataDRON2();

    GBS::DADDDRON2::Data getData();

private:
    GBS::Settings &settings;
    QLabel *labelInfo;
    QLabel *labelBoxTitleInfo;
    QLabel *labelFile;
    QLabel *labelBoxTitleShootingAngles;
    QLabel *labelTwoThetaStart;
    QLabel *labelTwoThetaEnd;
    QLabel *labelDelimiter;
    QLineEdit *lineEditFile;
    QLineEdit *lineEditTwoThetaStart;
    QLineEdit *lineEditTwoThetaEnd;
    QPushButton *pushButtonOpen;
    QComboBox *comboBoxDelimiter;
    QGroupBox *groupBoxInfo;
    QGroupBox *groupBoxFile;
    QGroupBox *groupBoxShootingAngles;
    QGroupBox *groupBoxDelimiter;
    QVBoxLayout *vBoxLayoutInfo;
    QHBoxLayout *hBoxLayoutFile;
    QGridLayout *gridLayoutShootingAngles;
    QGridLayout *gridLayotDelimiter;

private slots:
    void slot_pushButtonOpen();
};

#endif // DIALOGS_H
