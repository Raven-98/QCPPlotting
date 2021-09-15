#ifndef CHARTSETTINGS_H
#define CHARTSETTINGS_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSpacerItem>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include "qcustomplot.h"

class AxisSettings;
class GraphSettings;
class GridSettings;
class GridSettingsBox;
class LegendSettings;

enum csDialodType {
    Global,
    Axis,
    Graph,
};

class ChartSettings : public QDialog
{
    Q_OBJECT
public:
    ChartSettings(QCustomPlot &qcp, QWidget *parent = nullptr);
    ChartSettings(QCPAxis &axis, QWidget *parent = nullptr);
    ChartSettings(QCPAbstractPlottable &plottable, QWidget *parent = nullptr);
    ~ChartSettings();
    void set();

private:
    void init();

    QVBoxLayout *vBoxLayout;
    QDialogButtonBox *dialogButtonBox;
    csDialodType csdt;
    AxisSettings *axisSettings = nullptr;
    AxisSettings *axisLeftSettings = nullptr;
    AxisSettings *axisRightSettings = nullptr;
    AxisSettings *axisTopSettings = nullptr;
    AxisSettings *axisBottomSettings = nullptr;
    GraphSettings *graphSettings = nullptr;
    QTabWidget *tabWidget = nullptr;
    QTabWidget *tabWidget_Axis = nullptr;
    QWidget *tab_Axis = nullptr;
    QWidget *tab_AxisLeft = nullptr;
    QWidget *tab_AxisRight = nullptr;
    QWidget *tab_AxisTop = nullptr;
    QWidget *tab_AxisBottom = nullptr;
    QTabWidget *tabWidget_Graph = nullptr;
    QWidget *tab_Graph = nullptr;
    QWidget *tab_Grid = nullptr;
    QWidget *tab_Legend = nullptr;
    QGridLayout *gridLayout_Axis = nullptr;
    QGridLayout *gridLayout_AxisLeft = nullptr;
    QGridLayout *gridLayout_AxisRight = nullptr;
    QGridLayout *gridLayout_AxisTop = nullptr;
    QGridLayout *gridLayout_AxisBottom = nullptr;
    QGridLayout *gridLayout_Graph = nullptr;
    QGridLayout *gridLayout_Grid = nullptr;
    QGridLayout *gridLayout_Legend = nullptr;
    GridSettings *gridSettings = nullptr;
    LegendSettings *legendSettings = nullptr;
    QWidget *tab_Global = nullptr;
};

class AxisSettings : public QWidget
{
    Q_OBJECT

public:
    AxisSettings(QCPAxis &axis, QWidget *parent = nullptr);
    ~AxisSettings();

    void set();

private:
    void setPushButtonStyleSheet(QPushButton *button);
    void setPushButtonStyleSheet(QPushButton *button, QColor *color);

    QCPAxis &Axis;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_Title;
    QGroupBox *groupBox_RangeAxis;
    QGroupBox *groupBox_AxisColor;
    QGroupBox *groupBox_MajorTicks;
    QGroupBox *groupBox_MinorTicks;
    QGroupBox *groupBox_TicksLabel;
    QCheckBox *checkBox_AxisTitle;
    QLabel *label_Title;
    QLabel *label_TitleColor;
    QLabel *label_RangeAxis;
    QLabel *label_RangeAxisFrom;
    QLabel *label_RangeAxisTo;
    QLabel *label_AxisColor;
    QLabel *label_MajorTicksSide;
    QLabel *label_MajorTicksColor;
    QLabel *label_MinorTicksSide;
    QLabel *label_MinorTicksColor;
    QLabel *label_TicksLabelSide;
    QLabel *label_TicksLabelColor;
    QCheckBox *checkBox_MajorTicks;
    QCheckBox *checkBox_MinorTicks;
    QCheckBox *checkBox_TicksLabel;
    QLineEdit *lineEdit_Title;
    QDoubleSpinBox *doubleSpinBox_RangeAxisFrom;
    QDoubleSpinBox *doubleSpinBox_RangeAxisTo;
    QPushButton *pushButton_TitleColor;
    QPushButton *pushButton_AxisColor;
    QPushButton *pushButton_MajorTicksColor;
    QPushButton *pushButton_MinorTicksColor;
    QPushButton *pushButton_TicksLabelColor;
    QComboBox *comboBox_MajorTicksSide;
    QComboBox *comboBox_MinorTicksSide;
    QComboBox *comboBox_TicksLabelSide;
    QGridLayout *gridLayout_Title;
    QGridLayout *gridLayout_RangeAxis;
    QGridLayout *gridLayout_AxisColor;
    QGridLayout *gridLayout_MajorTicks;
    QGridLayout *gridLayout_MinorTicks;
    QGridLayout *gridLayout_TicksLabel;

private slots:
    void slot_TitleColor();
    void slot_AxisColor();
    void slot_MajorTicksColor();
    void slot_MinorTicksColor();
    void slot_TicksLabelColor();
    void slot_AxisWidgetsEnabled(int state);
    void slot_TitleWidgetsEnabled(QString text);
    void slot_MajorTicksWidgetsEnabled(int state);
    void slot_MinorTicksWidgetsEnabled(int state);
    void slot_TicksLabelWidgetsEnabled(int state);
};

class GraphSettings : public QWidget
{
    Q_OBJECT

public:
    GraphSettings(QCPAbstractPlottable &plottable, QWidget *parent = nullptr);
    ~GraphSettings();
    void set();

private:
    void setPushButtonStyleSheet(QPushButton *button);
    void setPushButtonStyleSheet(QPushButton *button, QColor *color);

    QCPAbstractPlottable &Plottable;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_Line;
    QGroupBox *groupBox_Scatter;
    QGroupBox *groupBox_Global;
    QGroupBox *groupBox_GraphName;
    QLabel *label_GraphName;
    QLineEdit *lineEdit_GraphName;
    QGridLayout *gridLayout_GraphName;
    QLabel *label_Axes;
    QLabel *label_Line;
    QLabel *label_Scatter;
    QLabel *label_LineConnect;
    QLabel *label_LineStyle;
    QLabel *label_LineWidth;
    QLabel *label_LineColor;
    QLabel *label_ScatterShape;
    QLabel *label_ScatterSize;
    QLabel *label_ScatterEdgeColor;
    QLabel *label_ScatterFillColor;
    QComboBox *comboBox_xAxis;
    QComboBox *comboBox_yAxis;
    QComboBox *comboBox_LineConnect;
    QComboBox *comboBox_LineStyle;
    QComboBox *comboBox_ScatterShape;
    QDoubleSpinBox *doubleSpinBox_LineWidth;
    QDoubleSpinBox *doubleSpinBox_ScatterSize;
    QPushButton *pushButton_LineColor;
    QPushButton *pushButton_ScatterEdgeColor;
    QPushButton *pushButton_ScatterFillColor;
    QGridLayout *gridLayout_Global;
    QGridLayout *gridLayout_Line;
    QGridLayout *gridLayout_Scatter;

private slots:
    void slot_LineColor();
    void slot_ScatterEdgeColor();
    void slot_ScatterFillColor();
    void slot_ScatterWidgetsEnabled(int index);
    void slot_LineWidgetsEnabled(int index);
};

class GridSettings : public QWidget
{
    Q_OBJECT

public:
    GridSettings(QCustomPlot &qcp, QWidget *parent = nullptr);
    ~GridSettings();
    void set();

private:
    QGridLayout *gridLayout;
    GridSettingsBox *gridSettingsBox_Horizontal;
    GridSettingsBox *gridSettingsBox_Vertical;
};

class GridSettingsBox : public QGroupBox
{
    Q_OBJECT

public:
    enum Grid {
        Horizontal,
        Vertical,
    };

    GridSettingsBox(QCustomPlot &qcp_, Grid g, QWidget *parent = nullptr);
    ~GridSettingsBox();
    void set();

private:
    void setPushButtonStyleSheet(QPushButton *button);
    void setPushButtonStyleSheet(QPushButton *button, QColor *color);

    Grid G;
    QCustomPlot &qcp;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_LineStyle;
    QLabel *label_LineWidth;
    QLabel *label_LineColor;
    QLabel *label_Axis;
    QCheckBox *checkBoxMajor;
    QCheckBox *checkBoxMinor;
    QComboBox *comboBoxMajor_LineStyle;
    QComboBox *comboBoxMinor_LineStyle;
    QComboBox *comboBox_Axis;
    QDoubleSpinBox *doubleSpinBoxMajor_LineWidth;
    QDoubleSpinBox *doubleSpinBoxMinor_LineWidth;
    QPushButton *pushButtonMajor_LineColor;
    QPushButton *pushButtonMinor_LineColor;

private slots:
    void slot_MajorLineColor();
    void slot_MinorLineColor();
    void slot_MajorWidgetsEnabled(int state);
    void slot_MinorWidgetsEnabled(int state);
};

class LegendSettings : public QWidget
{
    Q_OBJECT

public:
    LegendSettings(QCPLegend &legend, QWidget *parent = nullptr);
    ~LegendSettings();
    void set();

private:
    QCPLegend &Legend;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_Visible;
};

class Spacer : public QVBoxLayout
{
    Q_OBJECT
public:
    Spacer(QSizePolicy::Policy hPolicy = QSizePolicy::Expanding, QSizePolicy::Policy vPolicy = QSizePolicy::Expanding, QWidget *parent = nullptr);
    ~Spacer();
};

#endif // CHARTSETTINGS_H
