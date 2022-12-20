#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QDoubleSpinBox>

#include "pushbuttoncolorpicker.h"
#include "tablewidget.h"
#include "structs.h"

#include "qcustomplot.h"

class ChartWidget;
class myQCustomPlot;
class ChartSettings;
class AxisSettings;
class PlotSettings;
class LayerSettings;
class LegendSettings;

class ChartWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ChartWidget(QWidget *parent = nullptr);
  ~ChartWidget();

  void init();
//  void setGraph(QVector<double> &data_x, QVector<double> &data_y);
//  void setBars(QVector<double> &data_x, QVector<double> &data_y);
//  void setCurve(QVector<double> &data_t, QVector<double> &data_x, QVector<double> &data_y);
//  void setFinancial(QVector<double> &keys, QVector<double> &open, QVector<double> &high, QVector<double> &low, QVector<double> &close);
//  void setStatisticalBox();
  void setPlot(QCPPlotting::NumSheet &data, QCPPlotting::ChartType &chartType);

private:
  void setGraph(QCPPlotting::NumSheet &data);
  void setBars(QCPPlotting::NumSheet &data);
  void setCurve(QCPPlotting::NumSheet &data);

#ifdef QT_DEBUG
  void tst();
#endif

private slots:
  void axisClick(QCPAxis *axis, QCPAxis::SelectablePart part);
  void axisDoubleClick(QCPAxis *axis);
  void plottableDoubleClick(QCPAbstractPlottable *plottable);
  void cContextMenuRequested(const QPoint &pos);
  void mousePressEventQCP(QMouseEvent *event);
  void mouseReleaseEventQCP(QMouseEvent *event);
  void mouseMoveEventQCP(QMouseEvent *event);
  void actionPropertiesTriggered();

private:
  static int SumCharts;
  static int ResChartWidgetID;
  myQCustomPlot *customPlot = nullptr;
  QGridLayout *gridLayout = nullptr;
  QMenu *customContexMenu = nullptr;
};

class myQCustomPlot : public QCustomPlot
{
  Q_OBJECT
public:
  using QCustomPlot::QCustomPlot;

  QList<QCPAxis *> axes() const;

//protected:
//  void mouseReleaseEvent(QMouseEvent *event);
};

class ChartSettings : public QDialog
{
  Q_OBJECT
public:
  ChartSettings(QWidget *parent = nullptr);
  ~ChartSettings();

//  void init(QCustomPlot *qcp);
  void init(myQCustomPlot *qcp);
  void init(QCPAxis *axis);
  void init(QCPAbstractPlottable *plottable);

private:
  void baseStartInit();
  void baseEndInit();

private:
  QVBoxLayout *vBoxLayout = nullptr;
  QDialogButtonBox *dialogButtonBox = nullptr;
};

class AxisSettings : public QWidget
{
  Q_OBJECT
public:
  AxisSettings(QCPAxis *axis, QWidget *parent = nullptr);
  ~AxisSettings();

  void init();

public slots:
  void setChange();

private:
  QWidget *titleBlock();
  QWidget *axisBlock();
  QWidget *ticksBlock();
  QWidget *gridBlock();
  void setWidgetEnabled(bool enabled);
  void setEnabledTitleBlock(bool enabled);
  void setEnabledMajorTicksBlock(bool enabled);
  void setEnabledMinorTicksBlock(bool enabled);
  void setEnabledTicksLabelBlock(bool enabled);
  void setEnabledMajorGridBlock(bool enabled);
  void setEnabledMinorGridBlock(bool enabled);

private slots:
  void checkBoxAxisTitleChecked(int state);
  void lineEditTitleTextEdited(QString text);
  void checkBoxMajorTicksChecked(int state);
  void checkBoxMinorTicksChecked(int state);
  void checkBoxTicksLabelChecked(int state);
  void checkBoxMajorGridChecked(int state);
  void checkBoxMinorGridChecked(int state);

private:
  QCPAxis *Axis = nullptr;
  QVBoxLayout *vBoxLayout = nullptr;
  QCheckBox *checkBox_AxisTitle = nullptr;
  QScrollArea *scrollArea = nullptr;

  QLineEdit *lineEdit_Title = nullptr;
  QFontComboBox *fontComboBox_TitleFont = nullptr;
  PushButtonColorPicker *pushButton_TitleColor = nullptr;

  QDoubleSpinBox *doubleSpinBox_RangeAxisFrom = nullptr;
  QDoubleSpinBox *doubleSpinBox_RangeAxisTo = nullptr;
  PushButtonColorPicker *pushButton_AxiColor = nullptr;

  QCheckBox *checkBox_MajorTicks = nullptr;
  QComboBox *comboBox_MajorTicksSide = nullptr;
  PushButtonColorPicker *pushButton_MajorTicksColor = nullptr;
  QCheckBox *checkBox_MinorTicks = nullptr;
  QComboBox *comboBox_MinorTicksSide = nullptr;
  PushButtonColorPicker *pushButton_MinorTicksColor = nullptr;
  QCheckBox *checkBox_TicksLabel = nullptr;
  QComboBox *comboBox_TicksLabelSide = nullptr;
  PushButtonColorPicker *pushButton_TicksLabelColor = nullptr;

  QCheckBox *checkBox_MajorGrid = nullptr;
  QComboBox *comboBox_MajorGrid_LineStyle = nullptr;
  QDoubleSpinBox *doubleSpinBox_MajorGrid_LineWidth = nullptr;
  PushButtonColorPicker *pushButton_MajorGrid_LineColor = nullptr;
  QCheckBox *checkBox_MinorGrid = nullptr;
  QComboBox *comboBox_MinorGrid_LineStyle = nullptr;
  QDoubleSpinBox *doubleSpinBox_MinorGrid_LineWidth;
  PushButtonColorPicker *pushButton_MinorGrid_LineColor = nullptr;
};

class PlotSettings : public QWidget
{
  Q_OBJECT
public:
  PlotSettings(QCPAbstractPlottable *plottable, QWidget *parent = nullptr);
  ~PlotSettings();

  void init();

public slots:
  void setChange();

private:
  QWidget *nameBlock();
  QWidget *propertiesBlock();
  QWidget *dataBlock();
  QWidget *lineBlock();
  QWidget *fillBlock();
  QWidget *scatterBlock();
  QWidget *axesBlock();
  void comboBoxLineStyleCheckIndex(int index);
  void setEnabledLineBlock(bool enabled);
  void comboBoxScatterShapeCheckIndex(int index);
  void setEnabledScatterBlock(bool enabled);

  QStandardItemModel *tableModel(QCPGraphDataContainer *data, QCPAxis *keyAxis, QCPAxis *valueAxis);
  QStandardItemModel *tableModel(QCPCurveDataContainer *data, QCPAxis *keyAxis, QCPAxis *valueAxis);
  QStandardItemModel *tableModel(QCPBarsDataContainer *data, QCPAxis *keyAxis, QCPAxis *valueAxis);
  void setChangeDataContainers(QCPGraphDataContainer *data);
  void setChangeDataContainers(QCPCurveDataContainer *data);
  void setChangeDataContainers(QCPBarsDataContainer *data);
  template <class DT>
  QStandardItemModel *_tableModel_(QCPDataContainer<DT> *data, QCPAxis *keyAxis, QCPAxis *valueAxis);
  template <class DT>
  void _setChangeDataContainers_(QCPDataContainer<DT> *data);

private slots:
  void comboBoxLineStyleCurrentIndexChanged(int index);
  void comboBoxScatterShapeCurrentIndexChanged(int index);

private:
  QCPAbstractPlottable *Plottable = nullptr;

  QLineEdit *lineEdit_PlotName = nullptr;

  QComboBox *comboBox_LineStyle = nullptr;
  QComboBox *comboBox_PenStyle = nullptr;
  QDoubleSpinBox *doubleSpinBox_LineWidth = nullptr;
  PushButtonColorPicker *pushButton_LineColor = nullptr;

  QComboBox *comboBox_FillStyle = nullptr;
  PushButtonColorPicker *pushButton_FillColor = nullptr;

  QComboBox *comboBox_ScatterShape = nullptr;
  QDoubleSpinBox *doubleSpinBox_ScatterSize = nullptr;
  PushButtonColorPicker *pushButton_ScatterEdgeColor = nullptr;
  PushButtonColorPicker *pushButton_ScatterFillColor = nullptr;

  QComboBox *comboBox_xAxis = nullptr;
  QComboBox *comboBox_yAxis = nullptr;

  TableView *table = nullptr;
};


class LayerSettings : public QWidget
{
  Q_OBJECT
public:
  LayerSettings(QWidget *parent = nullptr);
  ~LayerSettings();
};

class LegendSettings : public QWidget
{
  Q_OBJECT
public:
  LegendSettings(QWidget *parent = nullptr);
  ~LegendSettings();
};

#endif // CHARTWIDGET_H
