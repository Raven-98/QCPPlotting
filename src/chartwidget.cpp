#include "chartwidget.h"

#include <QSpacerItem>

#include "separatorwidgets.h"

int ChartWidget::SumCharts = 0;
int ChartWidget::ResChartWidgetID = 0;

ChartWidget::ChartWidget(QWidget *parent)
  : QWidget{parent}
{
  SumCharts++;
  ResChartWidgetID++;
}

ChartWidget::~ChartWidget()
{
  SumCharts--;
  if (SumCharts == 0)
    ResChartWidgetID = 0;

  if (customPlot)
    delete customPlot;
  if (gridLayout)
    delete gridLayout;
  if (customContexMenu)
    delete customContexMenu;
}

void ChartWidget::init()
{
  resize(500,400);
  setWindowIcon(QIcon(":/img/graph.png"));
  setWindowTitle(QString("Chart %1").arg(ResChartWidgetID));

  customPlot = new myQCustomPlot;

  customPlot->setInteractions(QCP::iRangeDrag |
                              QCP::iRangeZoom |
                              QCP::iMultiSelect |
                              QCP::iSelectPlottables |
                              QCP::iSelectAxes |
                              QCP::iSelectLegend);
  customPlot->axisRect()->setRangeDragAxes({customPlot->xAxis, customPlot->xAxis2},
                                           {customPlot->yAxis, customPlot->yAxis2});
  customPlot->axisRect()->setRangeZoomAxes({customPlot->xAxis, customPlot->xAxis2},
                                           {customPlot->yAxis, customPlot->yAxis2});

  customPlot->xAxis->setTickLength(0, 5);
  customPlot->xAxis->setSubTickLength(0, 2);

  customPlot->yAxis->setTickLength(0, 5);
  customPlot->yAxis->setSubTickLength(0, 2);

  customPlot->xAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->xAxis2->setSubTicks(false);
  customPlot->xAxis2->setTickLabels(false);

  customPlot->yAxis2->setVisible(true);
  customPlot->yAxis2->setTicks(false);
  customPlot->yAxis2->setSubTicks(false);
  customPlot->yAxis2->setTickLabels(false);

  QPen pen;
  pen.setColor(QColor(200, 200, 200));
  pen.setWidthF(1);
  pen.setStyle(Qt::DotLine);

  customPlot->xAxis->grid()->setPen(pen);
  customPlot->xAxis2->grid()->setPen(pen);
  customPlot->yAxis->grid()->setPen(pen);
  customPlot->yAxis2->grid()->setPen(pen);

  customPlot->xAxis->grid()->setSubGridPen(pen);
  customPlot->xAxis2->grid()->setSubGridPen(pen);
  customPlot->yAxis->grid()->setSubGridPen(pen);
  customPlot->yAxis2->grid()->setSubGridPen(pen);

  customPlot->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(customPlot, &QCustomPlot::axisClick, this, &ChartWidget::axisClick);
  connect(customPlot, &QCustomPlot::axisDoubleClick, this, &ChartWidget::axisDoubleClick);
  connect(customPlot, &QCustomPlot::plottableDoubleClick, this, &ChartWidget::plottableDoubleClick);
  connect(customPlot, &QCustomPlot::customContextMenuRequested, this, &ChartWidget::cContextMenuRequested);
  connect(customPlot, &QCustomPlot::mousePress, this, &ChartWidget::mousePressEventQCP);
  connect(customPlot, &QCustomPlot::mouseRelease, this, &ChartWidget::mouseReleaseEventQCP);
  connect(customPlot, &QCustomPlot::mouseMove, this, &ChartWidget::mouseMoveEventQCP);

  gridLayout = new QGridLayout;
  gridLayout->setContentsMargins(0, 0, 0, 0);
  gridLayout->addWidget(customPlot);
  setLayout(gridLayout);

  auto action_Properties = new QAction;
  action_Properties->setText(tr("Properties"));
  connect(action_Properties, &QAction::triggered, this, &ChartWidget::actionPropertiesTriggered);

  customContexMenu = new QMenu;
  customContexMenu->addAction(action_Properties);
}

/* ****************************************************************************** *
 * Графіки використовуються для відображення однозначних даних.                   *
 *                                                                                *
 * Однозначний означає, що для кожної унікальної ключової координати має бути     *
 * лише одна точка даних.                                                         *
 *                                                                                *
 * Іншими словами, у графі не може бути петель.                                   *
 *                                                                                *
 * Якщо ви хочете побудувати неоднозначні криві, скористайтеся таблицею QCPCurve. *
 * ****************************************************************************** */
void ChartWidget::setGraph(QVector<double> &data_x, QVector<double> &data_y)
{
  customPlot->clearGraphs();
// * >
  customPlot->addGraph();
  customPlot->graph()->setData(data_x, data_y);
  customPlot->graph()->rescaleAxes();
// * <
  customPlot->replot();
  customPlot->xAxis->setLabel("X");
  customPlot->yAxis->setLabel("Y");
}

/* ****************************************************************************** *
 * Таблиця, що представляє гістограму на графіку.                                 *
 * ****************************************************************************** */
void ChartWidget::setBars(QVector<double> &data_x, QVector<double> &data_y)
{
  customPlot->clearGraphs();
// * >
  auto  *newBars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  newBars->setData(data_x, data_y);
  newBars->rescaleAxes();
// * <
  customPlot->replot();
  customPlot->xAxis->setLabel("X");
  customPlot->yAxis->setLabel("Y");
}

/* ****************************************************************************** *
 * Таблиця, що представляє параметричну криву на графіку.                         *
 *                                                                                *
 * На відміну від QCPGraph, таблиці графіків цього типу можуть мати кілька точок  *
 * з однаковою ключовою координатою, тому їх візуальне представлення може мати    *
 * петлі.                                                                         *
 *                                                                                *
 * Це реалізується шляхом введення третьої координати t, яка визначає порядок     *
 * точок, описаних двома іншими координатами x і y.                               *
 * ****************************************************************************** */
void ChartWidget::setCurve(QVector<double> &data_t, QVector<double> &data_x, QVector<double> &data_y)
{
  customPlot->clearGraphs();
// * >
  auto newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
  newCurve->setData(data_t, data_x, data_y);
  newCurve->rescaleAxes();
// * <
  customPlot->replot();
  customPlot->xAxis->setLabel("X");
  customPlot->yAxis->setLabel("Y");
}
/* ****************************************************************************** *
 * Ця таблиця графіків представляє дані часових рядів, згруповані за певними      *
 * інтервалами, які в основному використовуються для біржових діаграм.            *
 * ****************************************************************************** */
//void ChartWidget::setFinancial(QVector<double> &keys, QVector<double> &open, QVector<double> &high, QVector<double> &low, QVector<double> &close)
//{
//  customPlot->clearGraphs();
//// * >
//// * <
//  customPlot->replot();
//  customPlot->xAxis->setLabel("X");
//  customPlot->yAxis->setLabel("Y");
//}

/* ****************************************************************************** *
 * Таблиця, що представляє один статистичний блок на графіку.                     *
 * ****************************************************************************** */
//void ChartWidget::setStatisticalBox()
//{
//  customPlot->clearGraphs();
//// * >
//// * <
//  customPlot->replot();
//  customPlot->xAxis->setLabel("X");
//  customPlot->yAxis->setLabel("Y");
//}

void ChartWidget::tst()
{
//  emit plottableDoubleClick(customPlot->plottable(0));

  /*

*/
}

void ChartWidget::axisClick(QCPAxis *axis, QCPAxis::SelectablePart  part)
{
//  if (part == QCPAxis::spAxisLabel) {

//    }
}

void ChartWidget::axisDoubleClick(QCPAxis *axis)
{
  axis->setSelectedParts(QCPAxis::spAxis |
                         QCPAxis::spTickLabels |
                         QCPAxis::spAxisLabel);
  customPlot->replot();

  auto chartSettings = new ChartSettings;
  chartSettings->init(axis);
  switch (chartSettings->exec()) {
    case ChartSettings::Accepted:
      break;
    case ChartSettings::Rejected:
      break;
    default:
      QMessageBox::critical(chartSettings, tr("Error"), tr("Unexpected result"));
    }
  delete chartSettings;
  axis->setSelectedParts(QCPAxis::spNone);
  customPlot->replot();
}

void ChartWidget::plottableDoubleClick(QCPAbstractPlottable *plottable)
{
  auto chartSettings = new ChartSettings;
  chartSettings->init(plottable);
  switch (chartSettings->exec()) {
    case ChartSettings::Accepted:
      break;
    case ChartSettings::Rejected:
      break;
    default:
      QMessageBox::critical(chartSettings, tr("Error"), tr("Unexpected result"));
    }
  plottable->setSelection(QCPDataSelection());
  customPlot->replot();
}

void ChartWidget::cContextMenuRequested(const QPoint &pos)
{
  customContexMenu->popup(mapToGlobal(pos));
}

void ChartWidget::mousePressEventQCP(QMouseEvent *event)
{

}

void ChartWidget::mouseReleaseEventQCP(QMouseEvent *event)
{

}

void ChartWidget::mouseMoveEventQCP(QMouseEvent *event)
{

}

void ChartWidget::actionPropertiesTriggered()
{
  auto chartSettings = new ChartSettings;
  chartSettings->init(customPlot);
  switch (chartSettings->exec()) {
    case ChartSettings::Accepted:
      break;
    case ChartSettings::Rejected:
      break;
    default:
      QMessageBox::critical(chartSettings, tr("Error"), tr("Unexpected result"));
    }
  customPlot->replot();
}

QList<QCPAxis *> myQCustomPlot::axes() const
{
  QList<QCPAxis*> axes;
  for (auto rect : axisRects()) {
      axes << rect->axes();
    }
  return axes;
}

//void myQCustomPlot::mouseReleaseEvent(QMouseEvent *event)
//{
//  if (mMouseSignalLayerableDetails.value<QCPAxis::SelectablePart>() != QCPAxis::spAxisLabel) {
//      emit QCustomPlot::mouseReleaseEvent(event);
//    }
//}

ChartSettings::ChartSettings(QWidget *parent)
  : QDialog{parent}
{}

ChartSettings::~ChartSettings()
{
  if (dialogButtonBox)
    delete dialogButtonBox;
  if (vBoxLayout)
    delete vBoxLayout;
}

//void ChartSettings::init(QCustomPlot *qcp)
void ChartSettings::init(myQCustomPlot *qcp)
{
  baseStartInit();

  resize(800, 600);
  setWindowTitle(tr("Chart settings"));

// Axes
  auto tabWidget_Axes = new QTabWidget;
  tabWidget_Axes->setTabPosition(QTabWidget::West);

  for (auto Axis : qcp->axes()) {
      auto axisSettings = new AxisSettings(Axis);
      axisSettings->init();
       connect(this, &QDialog::accepted, axisSettings, &AxisSettings::setChange);

      auto gridLayout = new QGridLayout;
      gridLayout->setContentsMargins(0, 0, 0, 0);
      gridLayout->addWidget(axisSettings);

      auto tab = new QWidget;
      tab->setLayout(gridLayout);

      switch (Axis->axisType()) {
        case QCPAxis::atLeft:
            tabWidget_Axes->addTab(tab, tr("Left axis"));
            break;
        case QCPAxis::atBottom:
            tabWidget_Axes->addTab(tab, tr("Bottom axis"));
            break;
        case QCPAxis::atRight:
            tabWidget_Axes->addTab(tab, tr("Right axis"));
            break;
        case QCPAxis::atTop:
            tabWidget_Axes->addTab(tab, tr("Top axis"));
            break;
        default:
            break;
        }
    }

  auto gridLayout_Axes = new QGridLayout;
  gridLayout_Axes->setContentsMargins(0, 0, 0, 0);
  gridLayout_Axes->addWidget(tabWidget_Axes);

  auto tab_Axes = new QWidget;
  tab_Axes->setLayout(gridLayout_Axes);

// Plots
  auto tabWidget_Plots = new QTabWidget;
  tabWidget_Plots->setTabPosition(QTabWidget::West);

  for (int i = 0; i < qcp->graphCount(); i++) {
      auto plotSettings = new PlotSettings(qcp->plottable(i));
      plotSettings->init();
      connect(this, &QDialog::accepted, plotSettings, &PlotSettings::setChange);

      auto gridLayout = new QGridLayout;
      gridLayout->setContentsMargins(0, 0, 0, 0);
      gridLayout->addWidget(plotSettings);

      auto tab = new QWidget;
      tab->setLayout(gridLayout);

      tabWidget_Plots->addTab(tab, qcp->plottable(i)->name());
    }

  auto gridLayout_Plots = new QGridLayout;
  gridLayout_Plots->setContentsMargins(0, 0, 0, 0);
  gridLayout_Plots->addWidget(tabWidget_Plots);

  auto tab_Plots = new QWidget;
  tab_Plots->setLayout(gridLayout_Plots);

// Legend
  auto tab_Legend = new QWidget;

//
  auto tabWidget = new QTabWidget;
  tabWidget->addTab(tab_Axes, tr("Axes"));
  tabWidget->addTab(tab_Plots, tr("Graph"));
  tabWidget->addTab(tab_Legend, tr("Legend"));

  vBoxLayout->addWidget(tabWidget);

  baseEndInit();
}

void ChartSettings::init(QCPAxis *axis)
{
  baseStartInit();

  resize(800, 600);
  setWindowTitle(tr("Axis settings"));

  auto axisSettings = new AxisSettings(axis);
  axisSettings->init();
  connect(this, &QDialog::accepted, axisSettings, &AxisSettings::setChange);

  vBoxLayout->addWidget(axisSettings);

  baseEndInit();
}

void ChartSettings::init(QCPAbstractPlottable *plottable)
{
  baseStartInit();

  resize(600, 600);
  setWindowTitle(tr("Graph settings"));

  auto plotSettings = new PlotSettings(plottable);
  plotSettings->init();
  connect(this, &QDialog::accepted, plotSettings, &PlotSettings::setChange);

  vBoxLayout->addWidget(plotSettings);

  baseEndInit();
}

void ChartSettings::baseStartInit()
{
  vBoxLayout = new QVBoxLayout;
  setLayout(vBoxLayout);
}

void ChartSettings::baseEndInit()
{
  dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                         QDialogButtonBox::Cancel);
  connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  vBoxLayout->addWidget(dialogButtonBox);
}

AxisSettings::AxisSettings(QCPAxis *axis, QWidget *parent)
  : QWidget{parent}
{
  Axis = axis;
}

AxisSettings::~AxisSettings()
{
  if (lineEdit_Title)
    delete lineEdit_Title;
  if (fontComboBox_TitleFont)
    delete fontComboBox_TitleFont;
  if (pushButton_TitleColor)
    delete pushButton_TitleColor;

  if (doubleSpinBox_RangeAxisFrom)
    delete doubleSpinBox_RangeAxisFrom;
  if (doubleSpinBox_RangeAxisTo)
    delete doubleSpinBox_RangeAxisTo;
  if (pushButton_AxiColor)
    delete pushButton_AxiColor;

  if (checkBox_MajorTicks)
    delete checkBox_MajorTicks;
  if (comboBox_MajorTicksSide)
    delete comboBox_MajorTicksSide;
  if (pushButton_MajorTicksColor)
    delete pushButton_MajorTicksColor;
  if (checkBox_MinorTicks)
    delete checkBox_MinorTicks;
  if (comboBox_MinorTicksSide)
    delete comboBox_MinorTicksSide;
  if (pushButton_MinorTicksColor)
    delete pushButton_MinorTicksColor;
  if (checkBox_TicksLabel)
    delete checkBox_TicksLabel;
  if (comboBox_TicksLabelSide)
    delete comboBox_TicksLabelSide;
  if (pushButton_TicksLabelColor)
    delete pushButton_TicksLabelColor;

  if (checkBox_MajorGrid)
    delete checkBox_MajorGrid;
  if (comboBox_MajorGrid_LineStyle)
    delete comboBox_MajorGrid_LineStyle;
  if (doubleSpinBox_MajorGrid_LineWidth)
    delete doubleSpinBox_MajorGrid_LineWidth;
  if (pushButton_MajorGrid_LineColor)
    delete pushButton_MajorGrid_LineColor;
  if (checkBox_MinorGrid)
    delete checkBox_MinorGrid;
  if (comboBox_MinorGrid_LineStyle)
    delete comboBox_MinorGrid_LineStyle;
  if (doubleSpinBox_MinorGrid_LineWidth)
    delete doubleSpinBox_MinorGrid_LineWidth;
  if (pushButton_MinorGrid_LineColor)
    delete pushButton_MinorGrid_LineColor;

  if (checkBox_AxisTitle)
    delete checkBox_AxisTitle;
  if (scrollArea)
    delete scrollArea;

  if (vBoxLayout)
    delete vBoxLayout;
}

void AxisSettings::init()
{
  checkBox_AxisTitle = new QCheckBox;
  switch (Axis->axisType()) {
    case QCPAxis::atLeft:
        checkBox_AxisTitle->setText(tr("Left axis"));
        break;
    case QCPAxis::atBottom:
        checkBox_AxisTitle->setText(tr("Bottom axis"));
        break;
    case QCPAxis::atRight:
        checkBox_AxisTitle->setText(tr("Right axis"));
        break;
    case QCPAxis::atTop:
        checkBox_AxisTitle->setText(tr("Top axis"));
        break;
    default:
        break;
    }
  checkBox_AxisTitle->setStyleSheet("font: bold");
  checkBox_AxisTitle->setChecked(Axis->visible());
  connect(checkBox_AxisTitle, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxAxisTitleChecked);

  scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  scrollArea->setBackgroundRole(QPalette::AlternateBase);

  auto gridLayout_ScrollArea = new QGridLayout;
  gridLayout_ScrollArea->setContentsMargins(0, 0, 0, 0);
  gridLayout_ScrollArea->addWidget(titleBlock());
  gridLayout_ScrollArea->addWidget(new HLine);
  gridLayout_ScrollArea->addWidget(axisBlock());
  gridLayout_ScrollArea->addWidget(new HLine);
  gridLayout_ScrollArea->addWidget(ticksBlock());
  gridLayout_ScrollArea->addWidget(new HLine);
  gridLayout_ScrollArea->addWidget(gridBlock());

  auto scrollContainer = new QWidget;
  scrollContainer->setLayout(gridLayout_ScrollArea);

  scrollArea->setWidget(scrollContainer);

  vBoxLayout = new QVBoxLayout;
  vBoxLayout->setContentsMargins(0, 0, 0, 0);
  vBoxLayout->addWidget(checkBox_AxisTitle);
  vBoxLayout->addWidget(scrollArea);

  setLayout(vBoxLayout);

  setWidgetEnabled(checkBox_AxisTitle->isChecked());
  setEnabledTitleBlock(!Axis->label().isEmpty());
  setEnabledMajorTicksBlock(checkBox_MajorTicks->isChecked());
  if (checkBox_MinorTicks->isEnabled())
    setEnabledMinorTicksBlock(checkBox_MinorTicks->isChecked());
  if (checkBox_TicksLabel->isEnabled())
    setEnabledTicksLabelBlock(checkBox_TicksLabel->isChecked());
  setEnabledMajorGridBlock(checkBox_MajorGrid->isChecked());
  if (checkBox_MinorGrid->isEnabled())
    setEnabledMinorGridBlock(checkBox_MinorGrid->isChecked());
}

void AxisSettings::setChange()
{
  Axis->setVisible(checkBox_AxisTitle->isChecked());

  // title block
  Axis->setLabel(lineEdit_Title->text());
  Axis->setLabelFont(fontComboBox_TitleFont->currentFont());
  Axis->setLabelColor(pushButton_TitleColor->getColor());

  // axis block
  Axis->setRange(doubleSpinBox_RangeAxisFrom->value(), doubleSpinBox_RangeAxisTo->value());
  Axis->setBasePen(QPen(pushButton_AxiColor->getColor()));

  // ticks block
  Axis->setTicks(checkBox_MajorTicks->isChecked());
  switch (comboBox_MajorTicksSide->currentIndex()) {
    case 0:
      Axis->setTickLength(5, 0);
      break;
    case 1:
      Axis->setTickLength(0, 5);
      break;
    default:
      Axis->setTickLength(5, 5);
      break;
    }
  Axis->setTickPen(QPen(pushButton_MajorTicksColor->getColor()));

  Axis->setTicks(checkBox_MinorTicks->isChecked());
  switch (comboBox_MinorTicksSide->currentIndex()) {
    case 0:
      Axis->setSubTickLength(2, 0);
      break;
    case 1:
      Axis->setSubTickLength(0, 2);
      break;
    default:
      Axis->setSubTickLength(2, 2);
      break;
    }
  Axis->setSubTickPen(QPen(pushButton_MinorTicksColor->getColor()));

  Axis->setTickLabels(checkBox_TicksLabel->isChecked());
  Axis->setTickLabelSide(static_cast<QCPAxis::LabelSide>(comboBox_TicksLabelSide->currentIndex()));
  Axis->setTickLabelColor(pushButton_TicksLabelColor->getColor());

  // grid block
  Axis->grid()->setVisible(checkBox_MajorGrid->isChecked());
  QPen majorGridPen;
  majorGridPen.setStyle(static_cast<Qt::PenStyle>(comboBox_MajorGrid_LineStyle->currentIndex() + 1));
  majorGridPen.setWidthF(doubleSpinBox_MajorGrid_LineWidth->value());
  majorGridPen.setColor(pushButton_MajorGrid_LineColor->getColor());
  Axis->grid()->setPen(majorGridPen);

  Axis->grid()->setSubGridVisible(checkBox_MinorGrid->isChecked());
  QPen minorGridPen;
  minorGridPen.setStyle(static_cast<Qt::PenStyle>(comboBox_MinorGrid_LineStyle->currentIndex() + 1));
  minorGridPen.setWidthF(doubleSpinBox_MinorGrid_LineWidth->value());
  minorGridPen.setColor(pushButton_MinorGrid_LineColor->getColor());
  Axis->grid()->setSubGridPen(minorGridPen);
}

QWidget *AxisSettings::titleBlock()
{
  auto label = new QLabel;
  label->setText(tr("Title"));
  label->setStyleSheet("font: bold");

  lineEdit_Title = new QLineEdit;
  lineEdit_Title->setText(Axis->label());
  connect(lineEdit_Title, &QLineEdit::textEdited, this, &AxisSettings::lineEditTitleTextEdited);

  auto label_Font = new QLabel;
  label_Font->setText(tr("Font"));

  fontComboBox_TitleFont = new QFontComboBox;
  fontComboBox_TitleFont->setFont(Axis->labelFont());

  auto label_Color = new QLabel;
  label_Color->setText(tr("Color"));

  pushButton_TitleColor = new PushButtonColorPicker;
  pushButton_TitleColor->setColor(Axis->labelColor());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 2);
  gridLayout->addItem(new VSpacer, 1, 0);
  gridLayout->addWidget(lineEdit_Title, 2, 0, 1, 2);
  gridLayout->addItem(new VSpacer, 3, 0);
  gridLayout->addWidget(label_Font, 4, 0);
  gridLayout->addWidget(fontComboBox_TitleFont, 4, 1);
  gridLayout->addItem(new Spacer, 5, 1);
  gridLayout->addWidget(label_Color, 6, 0);
  gridLayout->addWidget(pushButton_TitleColor, 6, 1);
  gridLayout->addItem(new Spacer, 7, 1);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

QWidget *AxisSettings::axisBlock()
{
  auto label = new QLabel;
  label->setText(tr("Axis"));
  label->setStyleSheet("font: bold");

  auto label_Range = new QLabel;
  label_Range->setText(tr("Range"));

  auto label_RangeFrom = new QLabel;
  label_RangeFrom->setText(tr("From"));

  doubleSpinBox_RangeAxisFrom = new QDoubleSpinBox;
  doubleSpinBox_RangeAxisFrom->setRange(-Axis->range().maxRange, Axis->range().maxRange);
  doubleSpinBox_RangeAxisFrom->setValue(Axis->range().lower);

  auto label_RangeTo = new QLabel;
  label_RangeTo->setText(tr("To"));

  doubleSpinBox_RangeAxisTo = new QDoubleSpinBox;
  doubleSpinBox_RangeAxisTo->setRange(-Axis->range().maxRange, Axis->range().maxRange);
  doubleSpinBox_RangeAxisTo->setValue(Axis->range().upper);

  auto label_Color = new QLabel;
  label_Color->setText(tr("Color"));

  pushButton_AxiColor = new PushButtonColorPicker;
  pushButton_AxiColor->setColor(Axis->basePen().color());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 3);
  gridLayout->addItem(new VSpacer, 1, 0);
  gridLayout->addWidget(label_Range, 2, 0, 2, 1);
  gridLayout->addWidget(label_RangeFrom, 2, 1);
  gridLayout->addWidget(doubleSpinBox_RangeAxisFrom, 2, 2);
  gridLayout->addWidget(label_RangeTo, 3, 1);
  gridLayout->addWidget(doubleSpinBox_RangeAxisTo, 3, 2);
  gridLayout->addItem(new VSpacer, 4, 0);
  gridLayout->addWidget(label_Color, 5, 0);
  gridLayout->addWidget(pushButton_AxiColor, 5, 1, 1, 2);
  gridLayout->addItem(new Spacer, 6, 2);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

QWidget *AxisSettings::ticksBlock()
{
  auto label = new QLabel;
  label->setText(tr("Ticks"));
  label->setStyleSheet("QLabel{ font: bold;}");

  auto label_Side = new QLabel;
  label_Side->setText(tr("Side"));

  auto label_Color = new QLabel;
  label_Color->setText(tr("Color"));

  checkBox_MajorTicks = new QCheckBox;
  checkBox_MajorTicks->setText(tr("Major ticks"));
  checkBox_MajorTicks->setChecked(Axis->ticks());
  connect(checkBox_MajorTicks, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxMajorTicksChecked);

  comboBox_MajorTicksSide = new QComboBox;
  comboBox_MajorTicksSide->addItems({tr("Inside"),
                                     tr("Outside"),
                                     tr("On both sides")
                                    });
  if (Axis->tickLengthIn() != 0 & Axis->tickLengthOut() == 0)
    comboBox_MajorTicksSide->setCurrentIndex(0);
  else if (Axis->tickLengthIn() == 0 & Axis->tickLengthOut() != 0)
    comboBox_MajorTicksSide->setCurrentIndex(1);
  else
    comboBox_MajorTicksSide->setCurrentIndex(2);

  pushButton_MajorTicksColor = new PushButtonColorPicker;
  pushButton_MajorTicksColor->setColor(Axis->tickPen().color());

  checkBox_MinorTicks = new QCheckBox;
  checkBox_MinorTicks->setText(tr("Minor ticks"));
  checkBox_MinorTicks->setChecked(Axis->subTicks());
  connect(checkBox_MinorTicks, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxMinorTicksChecked);

  comboBox_MinorTicksSide = new QComboBox;
  comboBox_MinorTicksSide->addItems({tr("Inside"),
                                     tr("Outside"),
                                     tr("On both sides")
                                    });
  if (Axis->subTickLengthIn() != 0 & Axis->subTickLengthOut() == 0)
    comboBox_MinorTicksSide->setCurrentIndex(0);
  else if (Axis->subTickLengthIn() == 0 & Axis->subTickLengthOut() != 0)
    comboBox_MinorTicksSide->setCurrentIndex(1);
  else
    comboBox_MinorTicksSide->setCurrentIndex(2);

  pushButton_MinorTicksColor = new PushButtonColorPicker;
  pushButton_MinorTicksColor->setColor(Axis->subTickPen().color());

  checkBox_TicksLabel = new QCheckBox;
  checkBox_TicksLabel->setText(tr("Ticks label"));
  checkBox_TicksLabel->setChecked(Axis->tickLabels());
  connect(checkBox_TicksLabel, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxTicksLabelChecked);

  comboBox_TicksLabelSide = new QComboBox;
  comboBox_TicksLabelSide->addItems({tr("Inside"),
                                     tr("Outside")
                                    });
  comboBox_TicksLabelSide->setCurrentIndex(Axis->tickLabelSide());

  pushButton_TicksLabelColor = new PushButtonColorPicker;
  pushButton_TicksLabelColor->setColor(Axis->tickLabelColor());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 6);
  // column 0
  gridLayout->addItem(new VSpacer, 1, 0);
  gridLayout->addItem(new VSpacer, 3, 0);
  gridLayout->addWidget(label_Side, 4, 0);
  gridLayout->addItem(new VSpacer, 5, 0);
  gridLayout->addWidget(label_Color, 6, 0);
  // column 1
  gridLayout->addWidget(checkBox_MajorTicks, 2, 1);
  gridLayout->addWidget(comboBox_MajorTicksSide, 4, 1);
  gridLayout->addWidget(pushButton_MajorTicksColor, 6, 1);
  gridLayout->addItem(new Spacer, 7, 1);
  // column 2
  gridLayout->addWidget(new VLine(0), 1, 2, 6, 1);
  // column 3
  gridLayout->addWidget(checkBox_MinorTicks, 2, 3);
  gridLayout->addWidget(comboBox_MinorTicksSide, 4, 3);
  gridLayout->addWidget(pushButton_MinorTicksColor, 6, 3);
  gridLayout->addItem(new Spacer, 7, 3);
  // column 4
  gridLayout->addWidget(new VLine(0), 1, 4, 6, 1);
  // column 5
  gridLayout->addWidget(checkBox_TicksLabel, 2, 5);
  gridLayout->addWidget(comboBox_TicksLabelSide, 4, 5);
  gridLayout->addWidget(pushButton_TicksLabelColor, 6, 5);
  gridLayout->addItem(new Spacer, 7, 5);


  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

QWidget *AxisSettings::gridBlock()
{
  auto label = new QLabel;
  label->setText(tr("Grid"));
  label->setStyleSheet("QLabel{ font: bold;}");

  auto label_LineStyle = new QLabel;
  label_LineStyle->setText(tr("Line style"));

  auto label_LineWidth = new QLabel;
  label_LineWidth->setText(tr("Line width"));

  auto label_LineColor = new QLabel;
  label_LineColor->setText(tr("Line color"));

  checkBox_MajorGrid = new QCheckBox;
  checkBox_MajorGrid->setText(tr("Major grid"));
  checkBox_MajorGrid->setChecked(Axis->grid()->visible());
  connect(checkBox_MajorGrid, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxMajorGridChecked);

  comboBox_MajorGrid_LineStyle = new QComboBox;
  comboBox_MajorGrid_LineStyle->addItems({tr("Solid"),
                                     tr("Dash"),
                                     tr("Dot"),
                                     tr("Dash dot"),
                                     tr("Dash dot dot")
                                    });
  comboBox_MajorGrid_LineStyle->setCurrentIndex(Axis->grid()->pen().style() - 1);

  doubleSpinBox_MajorGrid_LineWidth = new QDoubleSpinBox;
  doubleSpinBox_MajorGrid_LineWidth->setValue(Axis->grid()->pen().widthF());

  pushButton_MajorGrid_LineColor = new PushButtonColorPicker(Axis->grid()->pen().color());

  checkBox_MinorGrid = new QCheckBox;
  checkBox_MinorGrid->setText(tr("Minor grid"));
  checkBox_MinorGrid->setChecked(Axis->grid()->subGridVisible());
  connect(checkBox_MinorGrid, &QCheckBox::stateChanged, this, &AxisSettings::checkBoxMinorGridChecked);

  comboBox_MinorGrid_LineStyle = new QComboBox;
  comboBox_MinorGrid_LineStyle->addItems({tr("Solid"),
                                     tr("Dash"),
                                     tr("Dot"),
                                     tr("Dash dot"),
                                     tr("Dash dot dot")
                                    });
  comboBox_MinorGrid_LineStyle->setCurrentIndex(Axis->grid()->subGridPen().style() - 1);

  doubleSpinBox_MinorGrid_LineWidth = new QDoubleSpinBox;
  doubleSpinBox_MinorGrid_LineWidth->setValue(Axis->grid()->subGridPen().widthF());

  pushButton_MinorGrid_LineColor = new PushButtonColorPicker(Axis->grid()->pen().color());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 2);
  // column 0
  gridLayout->addItem(new VSpacer, 1, 0);
  gridLayout->addItem(new VSpacer, 3, 0);
  gridLayout->addWidget(label_LineStyle, 4, 0);
  gridLayout->addItem(new VSpacer, 5, 0);
  gridLayout->addWidget(label_LineWidth, 6, 0);
  gridLayout->addItem(new VSpacer, 7, 0);
  gridLayout->addWidget(label_LineColor, 8, 0);
  // column 1
  gridLayout->addWidget(checkBox_MajorGrid, 2, 1);
  gridLayout->addWidget(comboBox_MajorGrid_LineStyle, 4, 1);
  gridLayout->addWidget(doubleSpinBox_MajorGrid_LineWidth, 6, 1);
  gridLayout->addWidget(pushButton_MajorGrid_LineColor, 8, 1);
  gridLayout->addItem(new Spacer, 9, 1);
  // column 2
  gridLayout->addWidget(new VLine(0), 1, 2, 8, 1);
  // column 3
  gridLayout->addWidget(checkBox_MinorGrid, 2, 3);
  gridLayout->addWidget(comboBox_MinorGrid_LineStyle, 4, 3);
  gridLayout->addWidget(doubleSpinBox_MinorGrid_LineWidth, 6, 3);
  gridLayout->addWidget(pushButton_MinorGrid_LineColor, 8, 3);
  gridLayout->addItem(new Spacer, 9, 3);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

void AxisSettings::setWidgetEnabled(bool enabled)
{
  scrollArea->setEnabled(enabled);
}

void AxisSettings::setEnabledTitleBlock(bool enabled)
{
  fontComboBox_TitleFont->setEnabled(enabled);
  pushButton_TitleColor->setEnabled(enabled);
}

void AxisSettings::setEnabledMajorTicksBlock(bool enabled)
{
  comboBox_MajorTicksSide->setEnabled(enabled);
  pushButton_MajorTicksColor->setEnabled(enabled);

  checkBox_MinorTicks->setEnabled(enabled);
  setEnabledMinorTicksBlock(enabled);
//  comboBox_MinorTicksSide->setEnabled(enabled);
//  pushButton_MinorTicksColor->setEnabled(enabled);

  checkBox_TicksLabel->setEnabled(enabled);
  setEnabledTicksLabelBlock(enabled);
//  comboBox_TicksLabelSide->setEnabled(enabled);
//  pushButton_TicksLabelColor->setEnabled(enabled);
}

void AxisSettings::setEnabledMinorTicksBlock(bool enabled)
{
//  if (checkBox_MinorTicks->isEnabled()) {
      comboBox_MinorTicksSide->setEnabled(enabled);
      pushButton_MinorTicksColor->setEnabled(enabled);
//    }
}

void AxisSettings::setEnabledTicksLabelBlock(bool enabled)
{
//  if (checkBox_TicksLabel->isEnabled()) {
      comboBox_TicksLabelSide->setEnabled(enabled);
      pushButton_TicksLabelColor->setEnabled(enabled);
//    }
}

void AxisSettings::setEnabledMajorGridBlock(bool enabled)
{
  comboBox_MajorGrid_LineStyle->setEnabled(enabled);
  doubleSpinBox_MajorGrid_LineWidth->setEnabled(enabled);
  pushButton_MajorGrid_LineColor->setEnabled(enabled);

  checkBox_MinorGrid->setEnabled(enabled);
  setEnabledMinorGridBlock(enabled);
}

void AxisSettings::setEnabledMinorGridBlock(bool enabled)
{
  comboBox_MinorGrid_LineStyle->setEnabled(enabled);
  doubleSpinBox_MinorGrid_LineWidth->setEnabled(enabled);
  pushButton_MinorGrid_LineColor->setEnabled(enabled);
}

void AxisSettings::checkBoxAxisTitleChecked(int state)
{
  setWidgetEnabled(state == Qt::Checked);
}

void AxisSettings::lineEditTitleTextEdited(QString text)
{
  setEnabledTitleBlock(!text.isEmpty());
}

void AxisSettings::checkBoxMajorTicksChecked(int state)
{
  setEnabledMajorTicksBlock(state == Qt::Checked);
}

void AxisSettings::checkBoxMinorTicksChecked(int state)
{
  setEnabledMinorTicksBlock(state == Qt::Checked);
}

void AxisSettings::checkBoxTicksLabelChecked(int state)
{
  setEnabledTicksLabelBlock(state == Qt::Checked);
}

void AxisSettings::checkBoxMajorGridChecked(int state)
{
  setEnabledMajorGridBlock(state == Qt::Checked);
}

void AxisSettings::checkBoxMinorGridChecked(int state)
{
  setEnabledMinorGridBlock(state == Qt::Checked);
}

PlotSettings::PlotSettings(QCPAbstractPlottable *plottable, QWidget *parent)
  : QWidget(parent)
{
  Plottable = plottable;
}

PlotSettings::~PlotSettings()
{
  if (lineEdit_PlotName)
    delete lineEdit_PlotName;

  if (comboBox_LineStyle)
    delete comboBox_LineStyle;
  if (comboBox_PenStyle)
    delete comboBox_PenStyle;
  if (doubleSpinBox_LineWidth)
    delete doubleSpinBox_LineWidth;
  if (pushButton_LineColor)
    delete pushButton_LineColor;

  if (comboBox_ScatterShape)
    delete comboBox_ScatterShape;
  if (doubleSpinBox_ScatterSize)
    delete doubleSpinBox_ScatterSize;
  if (pushButton_ScatterEdgeColor)
    delete pushButton_ScatterEdgeColor;
  if (pushButton_ScatterFillColor)
    delete pushButton_ScatterFillColor;

  if (comboBox_xAxis)
    delete comboBox_xAxis;
  if (comboBox_yAxis)
    delete comboBox_yAxis;
}

void PlotSettings::init()
{
  auto gridLayout_ScrollArea = new QGridLayout;
  gridLayout_ScrollArea->setContentsMargins(0, 0, 0, 0);

  gridLayout_ScrollArea->addWidget(nameBlock(), 0, 0);
  gridLayout_ScrollArea->addWidget(new HLine, 1, 0);
  gridLayout_ScrollArea->addWidget(lineBlock(), 2, 0);
  gridLayout_ScrollArea->addWidget(new HLine(), 3, 0);

  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0) {
      gridLayout_ScrollArea->addWidget(scatterBlock(), 4, 0);
      gridLayout_ScrollArea->addWidget(new HLine, 5, 0);
    }

  gridLayout_ScrollArea->addWidget(axesBlock(), 6, 0);

//  gridLayout_ScrollArea->addWidget(nameBlock(), 0, 0, 1, 3);
//  gridLayout_ScrollArea->addWidget(new HLine, 1, 0, 1, 3);
//  gridLayout_ScrollArea->addWidget(lineBlock(), 2, 0);
//  gridLayout_ScrollArea->addWidget(new VLine(0), 2, 1);
//  gridLayout_ScrollArea->addWidget(scatterBlock(), 2, 2);
//  gridLayout_ScrollArea->addWidget(new HLine, 3, 0, 1, 3);
//  gridLayout_ScrollArea->addWidget(axesBlock(), 4, 0, 1, 3);

  auto scrollContainer = new QWidget;
  scrollContainer->setLayout(gridLayout_ScrollArea);

  auto scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);
  scrollArea->setBackgroundRole(QPalette::AlternateBase);
  scrollArea->setWidget(scrollContainer);

  auto vBoxLayout = new QVBoxLayout;
  vBoxLayout->setContentsMargins(0, 0, 0, 0);
  vBoxLayout->addWidget(scrollArea);

  setLayout(vBoxLayout);

  if (comboBox_LineStyle)
    comboBoxLineStyleCheckIndex(comboBox_LineStyle->currentIndex());
  if (comboBox_ScatterShape)
    comboBoxScatterShapeCheckIndex(comboBox_ScatterShape->currentIndex());
}

void PlotSettings::setChange()
{
  Plottable->setName(lineEdit_PlotName->text());

  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0) {
      qobject_cast<QCPGraph *>(Plottable)->setLineStyle(static_cast<QCPGraph::LineStyle>(comboBox_LineStyle->currentIndex()));
    }
  QPen linePen;
  linePen.setStyle(static_cast<Qt::PenStyle>(comboBox_PenStyle->currentIndex() + 1));
  linePen.setWidthF(doubleSpinBox_LineWidth->value());
  linePen.setColor(pushButton_LineColor->getColor());
  Plottable->setPen(linePen);

  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0) {
      QCPScatterStyle myScatter;
      myScatter.setShape(static_cast<QCPScatterStyle::ScatterShape>(comboBox_ScatterShape->currentIndex()));
      myScatter.setSize(doubleSpinBox_ScatterSize->value());
      myScatter.setPen(pushButton_ScatterEdgeColor->getColor());
      myScatter.setBrush(pushButton_ScatterFillColor->getColor());
      qobject_cast<QCPGraph *>(Plottable)->setScatterStyle(myScatter);
    }

  switch (comboBox_xAxis->currentIndex()) {
    case 0:
      Plottable->setKeyAxis(qobject_cast<QCustomPlot *>(Plottable->parent())->xAxis);
        break;
    case 1:
      Plottable->setKeyAxis(qobject_cast<QCustomPlot *>(Plottable->parent())->xAxis2);
        break;
    default:
        break;
    }

  switch (comboBox_yAxis->currentIndex()) {
    case 0:
      Plottable->setValueAxis(qobject_cast<QCustomPlot *>(Plottable->parent())->yAxis);
      break;
    case 1:
      Plottable->setValueAxis(qobject_cast<QCustomPlot *>(Plottable->parent())->yAxis2);
      break;
    default:
      break;
    }
}

QWidget *PlotSettings::nameBlock()
{
  auto label = new QLabel;
  label->setText(tr("Name"));
  label->setStyleSheet("font: bold;");

  lineEdit_PlotName = new QLineEdit;
  lineEdit_PlotName->setText(Plottable->name());

  auto hLayout = new QHBoxLayout;
  hLayout->addWidget(label);
  hLayout->addWidget(lineEdit_PlotName);

  auto widget = new QWidget;
  widget->setLayout(hLayout);

  return widget;
}

QWidget *PlotSettings::lineBlock()
{
  auto label = new QLabel;
  label->setText(tr("Line"));
  label->setStyleSheet("font: bold");

  auto label_PenStyle = new QLabel;
  label_PenStyle->setText(tr("Pen style"));

  comboBox_PenStyle = new QComboBox;
  comboBox_PenStyle->addItems({tr("Solid"),
                               tr("Dash"),
                               tr("Dot"),
                               tr("Dash dot"),
                               tr("Dash dot dot")
                              });
  comboBox_PenStyle->setCurrentIndex(Plottable->pen().style() - 1);

  auto label_LineWidth = new QLabel;
  label_LineWidth->setText(tr("Width"));

  doubleSpinBox_LineWidth = new QDoubleSpinBox;
  doubleSpinBox_LineWidth->setValue(Plottable->pen().widthF());

  auto label_LineColor = new QLabel;
  label_LineColor->setText(tr("Color"));

  pushButton_LineColor = new PushButtonColorPicker;
  pushButton_LineColor->setColor(Plottable->pen().color());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 2);
  gridLayout->addItem(new VSpacer, 1, 0);

  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0) {
      auto label_LineStyle = new QLabel;
      label_LineStyle->setText(tr("Line style"));

      comboBox_LineStyle = new QComboBox;
      comboBox_LineStyle->addItems({tr("None"),
                                    tr("Line"),
                                    tr("Step left"),
                                    tr("Step right"),
                                    tr("Step center"),
                                    tr("Impulse")
                                   });
      comboBox_LineStyle->setCurrentIndex(qobject_cast<QCPGraph *>(Plottable)->lineStyle());
      connect(comboBox_LineStyle, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PlotSettings::comboBoxLineStyleCurrentIndexChanged);

      gridLayout->addWidget(label_LineStyle, 2, 0);
      gridLayout->addWidget(comboBox_LineStyle, 2, 1);
      gridLayout->addItem(new VSpacer, 3, 0);
    }

  gridLayout->addWidget(label_PenStyle, 4, 0);
  gridLayout->addWidget(comboBox_PenStyle, 4, 1);
  gridLayout->addItem(new VSpacer, 5, 0);
  gridLayout->addWidget(label_LineWidth, 6, 0);
  gridLayout->addWidget(doubleSpinBox_LineWidth, 6, 1);
  gridLayout->addItem(new VSpacer, 7, 0);
  gridLayout->addWidget(label_LineColor, 8, 0);
  gridLayout->addWidget(pushButton_LineColor, 8, 1);
  gridLayout->addItem(new Spacer, 9, 1);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

QWidget *PlotSettings::scatterBlock()
{
  auto label = new QLabel;
  label->setText(tr("Scatter"));
  label->setStyleSheet("font: bold");

  auto label_ScatterShape = new QLabel;
  label_ScatterShape->setText(tr("Shape"));

  comboBox_ScatterShape = new QComboBox;
  comboBox_ScatterShape->addItems({tr("None"),
                                   tr("Dot"),
                                   tr("Cross"),
                                   tr("Plus"),
                                   tr("Circle"),
                                   tr("Disc"),
                                   tr("Square"),
                                   tr("Diamond"),
                                   tr("Star"),
                                   tr("Triangle"),
                                   tr("Triangle inverted"),
                                   tr("Cross square"),
                                   tr("Plus square"),
                                   tr("Cross circle"),
                                   tr("Plus circle"),
                                   tr("Peace")
                                  });
  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0)
    comboBox_ScatterShape->setCurrentIndex(qobject_cast<QCPGraph *>(Plottable)->scatterStyle().shape());
  connect(comboBox_ScatterShape, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PlotSettings::comboBoxScatterShapeCurrentIndexChanged);

  auto label_ScatterSize = new QLabel;
  label_ScatterSize->setText(tr("Size"));

  doubleSpinBox_ScatterSize = new QDoubleSpinBox;
  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0)
    doubleSpinBox_ScatterSize->setValue(qobject_cast<QCPGraph *>(Plottable)->scatterStyle().size());

  auto label_ScatterEdgeColor = new QLabel;
  label_ScatterEdgeColor->setText(tr("Edge color"));

  pushButton_ScatterEdgeColor = new PushButtonColorPicker;
  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0)
    pushButton_ScatterEdgeColor->setColor(qobject_cast<QCPGraph *>(Plottable)->scatterStyle().pen().color());

  auto label_ScatterFillColor = new QLabel;
  label_ScatterFillColor->setText(tr("Fill color"));

  pushButton_ScatterFillColor = new PushButtonColorPicker;
  if (strcmp(Plottable->metaObject()->className(), "QCPGraph") == 0)
    pushButton_ScatterFillColor->setColor(qobject_cast<QCPGraph *>(Plottable)->scatterStyle().brush().color());

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0, 1, 2);
  gridLayout->addItem(new VSpacer, 1, 0);
  gridLayout->addWidget(label_ScatterShape, 2, 0);
  gridLayout->addWidget(comboBox_ScatterShape, 2, 1);
  gridLayout->addItem(new VSpacer, 3, 0);
  gridLayout->addWidget(label_ScatterSize, 4, 0);
  gridLayout->addWidget(doubleSpinBox_ScatterSize, 4, 1);
  gridLayout->addItem(new VSpacer, 5, 0);
  gridLayout->addWidget(label_ScatterEdgeColor, 6, 0);
  gridLayout->addWidget(pushButton_ScatterEdgeColor, 6, 1);
  gridLayout->addItem(new VSpacer, 7, 0);
  gridLayout->addWidget(label_ScatterFillColor, 8, 0);
  gridLayout->addWidget(pushButton_ScatterFillColor, 8, 1);
  gridLayout->addItem(new Spacer, 9, 1);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

QWidget *PlotSettings::axesBlock()
{
  auto label = new QLabel;
  label->setText(tr("Axes"));
  label->setStyleSheet("font: bold;");

  comboBox_xAxis = new QComboBox;
  comboBox_xAxis->addItems({tr("Bottom"),
                            tr("Top")
                           });
  switch (Plottable->keyAxis()->axisType()) {
    case QCPAxis::atTop:
      comboBox_xAxis->setCurrentIndex(1);
      break;
    case QCPAxis::atBottom:
      comboBox_xAxis->setCurrentIndex(0);
      break;
    default:
        break;
    }

  comboBox_yAxis = new QComboBox;
  comboBox_yAxis->addItems({tr("Left"),
                            tr("Right")
                           });
  switch (Plottable->keyAxis()->axisType()) {
    case QCPAxis::atLeft:
      comboBox_yAxis->setCurrentIndex(0);
      break;
    case QCPAxis::atRight:
      comboBox_yAxis->setCurrentIndex(1);
      break;
    default:
      break;
    }

  auto gridLayout = new QGridLayout;
  gridLayout->addWidget(label, 0, 0);
  gridLayout->addWidget(comboBox_xAxis, 0, 1);
  gridLayout->addWidget(comboBox_yAxis, 0, 2);
  gridLayout->addItem(new HSpacer, 1, 1);
  gridLayout->addItem(new HSpacer, 1, 2);

  auto widget = new QWidget;
  widget->setLayout(gridLayout);

  return widget;
}

void PlotSettings::comboBoxLineStyleCheckIndex(int index)
{
  if (index == 0)
    setEnabledLineBlock(false);
  else
    setEnabledLineBlock(true);
}

void PlotSettings::setEnabledLineBlock(bool enabled)
{
  comboBox_PenStyle->setEnabled(enabled);
  doubleSpinBox_LineWidth->setEnabled(enabled);
  pushButton_LineColor->setEnabled(enabled);
}

void PlotSettings::comboBoxScatterShapeCheckIndex(int index)
{
  if (index == 0)
    setEnabledScatterBlock(false);
  else
    setEnabledScatterBlock(true);
}

void PlotSettings::setEnabledScatterBlock(bool enabled)
{
  doubleSpinBox_ScatterSize->setEnabled(enabled);
  pushButton_ScatterEdgeColor->setEnabled(enabled);
  pushButton_ScatterFillColor->setEnabled(enabled);
}

void PlotSettings::comboBoxLineStyleCurrentIndexChanged(int index)
{
  comboBoxLineStyleCheckIndex(index);
}

void PlotSettings::comboBoxScatterShapeCurrentIndexChanged(int index)
{
  comboBoxScatterShapeCheckIndex(index);
}
