#include "chartsettings.h"

#include <QDebug>

ChartSettings::ChartSettings(QCustomPlot &qcp, QWidget *parent)
    : QDialog(parent)
{
    csdt = csDialodType::Global;

    setWindowTitle(tr("Chart settings"));

    init();
// Global
    tab_Global = new QWidget;

// Axis
    axisLeftSettings = new AxisSettings(*(qcp.yAxis));

    gridLayout_AxisLeft = new QGridLayout;
    gridLayout_AxisLeft->setContentsMargins(0, 0, 0, 0);
    gridLayout_AxisLeft->addWidget(axisLeftSettings);

    tab_AxisLeft = new QWidget;
    tab_AxisLeft->setLayout(gridLayout_AxisLeft);

    axisRightSettings = new AxisSettings(*(qcp.yAxis2));

    gridLayout_AxisRight = new QGridLayout;
    gridLayout_AxisRight->setContentsMargins(0, 0, 0, 0);
    gridLayout_AxisRight->addWidget(axisRightSettings);

    tab_AxisRight = new QWidget;
    tab_AxisRight->setLayout(gridLayout_AxisRight);

    axisTopSettings = new AxisSettings(*(qcp.xAxis2));

    gridLayout_AxisTop = new QGridLayout;
    gridLayout_AxisTop->setContentsMargins(0, 0, 0, 0);
    gridLayout_AxisTop->addWidget(axisTopSettings);

    tab_AxisTop = new QWidget;
    tab_AxisTop->setLayout(gridLayout_AxisTop);

    axisBottomSettings = new AxisSettings(*(qcp.xAxis));

    gridLayout_AxisBottom = new QGridLayout;
    gridLayout_AxisBottom->setContentsMargins(0, 0, 0, 0);
    gridLayout_AxisBottom->addWidget(axisBottomSettings);

    tab_AxisBottom = new QWidget;
    tab_AxisBottom->setLayout(gridLayout_AxisBottom);

    tabWidget_Axis = new QTabWidget;
    tabWidget_Axis->setTabPosition(QTabWidget::West);
    tabWidget_Axis->addTab(tab_AxisLeft, tr("Left axis"));
    tabWidget_Axis->addTab(tab_AxisBottom, tr("Bottom axis"));
    tabWidget_Axis->addTab(tab_AxisRight, tr("Right axis"));
    tabWidget_Axis->addTab(tab_AxisTop, tr("Top axis"));

    gridLayout_Axis = new QGridLayout;
    gridLayout_Axis->setContentsMargins(0, 0, 0, 0);
    gridLayout_Axis->addWidget(tabWidget_Axis);

    tab_Axis = new QWidget;
    tab_Axis->setLayout(gridLayout_Axis);
// Chart
    tabWidget_Graph = new QTabWidget;
    tabWidget_Graph->setTabPosition(QTabWidget::West);
    for (int i = 0; i < qcp.graphCount(); i++)
    {
        GraphSettings *graphSettings = new GraphSettings(*(qcp.graph(i)));

        QGridLayout *gridLayout = new QGridLayout;
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->addWidget(graphSettings);

        QWidget *tab = new QWidget;
        tab->setLayout(gridLayout);

        tabWidget_Graph->addTab(tab,qcp.graph(i)->name());
    }

    gridLayout_Graph = new QGridLayout;
    gridLayout_Graph->setContentsMargins(0, 0, 0, 0);
    gridLayout_Graph->addWidget(tabWidget_Graph);

    tab_Graph = new QWidget;
    tab_Graph->setLayout(gridLayout_Graph);
// Grid
    gridSettings = new GridSettings(qcp);

    gridLayout_Grid = new QGridLayout;
    gridLayout_Grid->setContentsMargins(0, 0, 0, 0);
    gridLayout_Grid->addWidget(gridSettings);

    tab_Grid = new QWidget;
    tab_Grid->setLayout(gridLayout_Grid);
// Legend
    legendSettings = new LegendSettings(*(qcp.legend));

    gridLayout_Legend = new QGridLayout;
    gridLayout_Legend->setContentsMargins(0, 0, 0, 0);
    gridLayout_Legend->addWidget(legendSettings);

    tab_Legend = new QWidget;
    tab_Legend->setLayout(gridLayout_Legend);
//
    tabWidget = new QTabWidget;
    tabWidget->addTab(tab_Global, tr("Glibal"));
    tabWidget->addTab(tab_Axis, tr("Axes"));
    tabWidget->addTab(tab_Graph, tr("Graph"));
    tabWidget->addTab(tab_Grid, tr("Grid"));
    tabWidget->addTab(tab_Legend, tr("Legend"));

    vBoxLayout->insertWidget(0, tabWidget);
}

ChartSettings::ChartSettings(QCPAxis &axis, QWidget *parent)
    : QDialog(parent)
{
    csdt = csDialodType::Axis;

    setWindowTitle(tr("Axis settings"));

    init();

    axisSettings = new AxisSettings(axis);
    vBoxLayout->insertWidget(0, axisSettings);
}

ChartSettings::ChartSettings(QCPAbstractPlottable &plottable, QWidget *parent)
    : QDialog(parent)
{
    csdt = csDialodType::Graph;

    setWindowTitle(tr("Graph settings"));

    init();
    graphSettings = new GraphSettings(plottable);
    vBoxLayout->insertWidget(0, graphSettings);
}

ChartSettings::~ChartSettings()
{
    delete dialogButtonBox;
    delete vBoxLayout;
    if (axisSettings)
        delete axisSettings;
    if (graphSettings)
        delete graphSettings;

    if (axisLeftSettings)
        delete axisLeftSettings;
    if (gridLayout_AxisLeft)
        delete gridLayout_AxisLeft;
    if (tab_AxisLeft)
        delete tab_AxisLeft;

    if (axisRightSettings)
        delete axisRightSettings;
    if (gridLayout_AxisRight)
        delete gridLayout_AxisRight;
    if (tab_AxisRight)
        delete tab_AxisRight;

    if (axisTopSettings)
        delete axisTopSettings;
    if (gridLayout_AxisTop)
        delete gridLayout_AxisTop;
    if (tab_AxisTop)
        delete tab_AxisTop;

    if (axisBottomSettings)
        delete axisBottomSettings;
    if (gridLayout_AxisBottom)
        delete gridLayout_AxisBottom;
    if (tab_AxisBottom)
        delete tab_AxisBottom;

    if (gridLayout_Axis)
        delete gridLayout_Axis;
    if (tab_Axis)
        delete tab_Axis;

    if (tabWidget_Graph)
        delete tabWidget_Graph;
    if (gridLayout_Graph)
        delete gridLayout_Graph;
    if (tab_Graph)
        delete tab_Graph;

    if (gridSettings)
        delete gridSettings;
    if (gridLayout_Grid)
        delete gridLayout_Grid;
    if (tab_Grid)
        delete tab_Grid;

    if (legendSettings)
        delete legendSettings;
    if (gridLayout_Legend)
        delete gridLayout_Legend;
    if (tab_Legend)
        delete tab_Legend;

    if (tabWidget)
        delete tabWidget;
}

void ChartSettings::set()
{
    switch (csdt) {
    case csDialodType::Global:
        axisLeftSettings->set();
        axisRightSettings->set();
        axisTopSettings->set();
        axisBottomSettings->set();
        for (auto &graphSettings : tabWidget_Graph->findChildren<GraphSettings *>())
        {
            graphSettings->set();
        }
        gridSettings->set();
        legendSettings->set();
        break;
    case csDialodType::Axis:
        axisSettings->set();
        break;
    case csDialodType::Graph:
        graphSettings->set();
        break;
    default:
        break;
    }
}

void ChartSettings::init()
{
    dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                           QDialogButtonBox::Cancel);
    connect(dialogButtonBox,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(dialogButtonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);

    vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(dialogButtonBox);

    setLayout(vBoxLayout);
}

AxisSettings::AxisSettings(QCPAxis &axis, QWidget *parent)
    : QWidget(parent),
      Axis(axis)
{
//    label_AxisTitle = new QLabel;
    checkBox_AxisTitle = new QCheckBox;
    switch (Axis.axisType()) {
    case QCPAxis::atLeft:
//        label_AxisTitle->setText(tr("Left axis"));
        checkBox_AxisTitle->setText(tr("Left axis"));
        break;
    case QCPAxis::atBottom:
//        label_AxisTitle->setText(tr("Bottom axis"));
        checkBox_AxisTitle->setText(tr("Bottom axis"));
        break;
    case QCPAxis::atRight:
//        label_AxisTitle->setText(tr("Right axis"));
        checkBox_AxisTitle->setText(tr("Right axis"));
        break;
    case QCPAxis::atTop:
//        label_AxisTitle->setText(tr("Top axis"));
        checkBox_AxisTitle->setText(tr("Top axis"));
        break;
    default:
        break;
    }
//    label_AxisTitle->setStyleSheet("QLabel{ font: bold;"
//                                        "   qproperty-alignment: AlignCenter;"
//                                        "}");
    checkBox_AxisTitle->setStyleSheet("QCheckBox{ font: bold;}");
    checkBox_AxisTitle->setChecked(Axis.visible());
    connect(checkBox_AxisTitle,&QCheckBox::stateChanged,this,&AxisSettings::slot_AxisWidgetsEnabled);

    label_Title = new QLabel;
    label_Title->setText(tr("Title"));
    label_Title->setStyleSheet("QLabel{ font: bold;}");

    lineEdit_Title = new QLineEdit;
    lineEdit_Title->setText(Axis.label());
    connect(lineEdit_Title,&QLineEdit::textEdited,this,&AxisSettings::slot_TitleWidgetsEnabled);

    label_TitleColor = new QLabel;
    label_TitleColor->setText(tr("Color"));

    QColor *color = new QColor(Axis.labelColor());
    pushButton_TitleColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_TitleColor, color);
    connect(pushButton_TitleColor,&QPushButton::clicked,this,&AxisSettings::slot_TitleColor);

    gridLayout_Title = new QGridLayout;
    gridLayout_Title->addWidget(label_Title, 0, 0, 1, 2);
    gridLayout_Title->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout_Title->addWidget(lineEdit_Title,2,0, 1, 2);
    gridLayout_Title->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);
    gridLayout_Title->addWidget(label_TitleColor,4,0);
    gridLayout_Title->addWidget(pushButton_TitleColor,4,1);
    gridLayout_Title->addLayout(new Spacer(),5,1);

    groupBox_Title = new QGroupBox;
    groupBox_Title->setLayout(gridLayout_Title);

    label_RangeAxis = new QLabel;
    label_RangeAxis->setText(tr("Range axis"));
    label_RangeAxis->setStyleSheet("QLabel{ font: bold;}");

    label_RangeAxisFrom = new QLabel;
    label_RangeAxisFrom->setText(tr("From"));

    doubleSpinBox_RangeAxisFrom = new QDoubleSpinBox;
    doubleSpinBox_RangeAxisFrom->setRange(-Axis.range().maxRange, Axis.range().maxRange);
    doubleSpinBox_RangeAxisFrom->setValue(Axis.range().lower);

    label_RangeAxisTo = new QLabel;
    label_RangeAxisTo->setText(tr("To"));

    doubleSpinBox_RangeAxisTo = new QDoubleSpinBox;
    doubleSpinBox_RangeAxisTo->setRange(-Axis.range().maxRange, Axis.range().maxRange);
    doubleSpinBox_RangeAxisTo->setValue(Axis.range().upper);

    gridLayout_RangeAxis = new QGridLayout;
    gridLayout_RangeAxis->addWidget(label_RangeAxis,0,0,1,2);
    gridLayout_RangeAxis->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout_RangeAxis->addWidget(label_RangeAxisFrom,2,0);
    gridLayout_RangeAxis->addWidget(doubleSpinBox_RangeAxisFrom,2,1);
    gridLayout_RangeAxis->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);
    gridLayout_RangeAxis->addWidget(label_RangeAxisTo,4,0);
    gridLayout_RangeAxis->addWidget(doubleSpinBox_RangeAxisTo,4,1);
    gridLayout_RangeAxis->addLayout(new Spacer(), 5, 1);

    groupBox_RangeAxis = new QGroupBox;
    groupBox_RangeAxis->setLayout(gridLayout_RangeAxis);

    label_AxisColor = new QLabel;
    label_AxisColor->setText(tr("Axis color"));
    label_AxisColor->setStyleSheet("QLabel{ font: bold;}");

    *color = Axis.basePen().color();
    pushButton_AxisColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_AxisColor, color);
    connect(pushButton_AxisColor,&QPushButton::clicked,this,&AxisSettings::slot_AxisColor);

    gridLayout_AxisColor = new QGridLayout;
    gridLayout_AxisColor->addWidget(label_AxisColor,0,0);
    gridLayout_AxisColor->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout_AxisColor->addWidget(pushButton_AxisColor,2,0);
    gridLayout_AxisColor->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);

    groupBox_AxisColor = new QGroupBox;
    groupBox_AxisColor->setLayout(gridLayout_AxisColor);

    checkBox_MajorTicks = new QCheckBox;
    checkBox_MajorTicks->setText(tr("Major ticks"));
    checkBox_MajorTicks->setChecked(Axis.ticks());
    connect(checkBox_MajorTicks,&QCheckBox::stateChanged,this,&AxisSettings::slot_MajorTicksWidgetsEnabled);

    label_MajorTicksSide = new QLabel;
    label_MajorTicksSide->setText(tr("Side"));

    comboBox_MajorTicksSide = new QComboBox;
    comboBox_MajorTicksSide->addItems({tr("Inside"),
                                       tr("Outside"),
                                       tr("On both sides")
                                      });
    if (Axis.tickLengthIn() != 0 & Axis.tickLengthOut() == 0)
        comboBox_MajorTicksSide->setCurrentIndex(0);
    else if (Axis.tickLengthIn() == 0 & Axis.tickLengthOut() != 0)
        comboBox_MajorTicksSide->setCurrentIndex(1);
    else
        comboBox_MajorTicksSide->setCurrentIndex(2);

    label_MajorTicksColor = new QLabel;
    label_MajorTicksColor->setText(tr("Color"));

    *color = Axis.tickPen().color();
    pushButton_MajorTicksColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_MajorTicksColor, color);
    connect(pushButton_MajorTicksColor,&QPushButton::clicked,this,&AxisSettings::slot_MajorTicksColor);

    gridLayout_MajorTicks = new QGridLayout;
    gridLayout_MajorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 0, 0);
    gridLayout_MajorTicks->addWidget(checkBox_MajorTicks,1,0,1,2);
    gridLayout_MajorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 2, 0);
    gridLayout_MajorTicks->addWidget(label_MajorTicksSide,3,0);
    gridLayout_MajorTicks->addWidget(comboBox_MajorTicksSide,3,1);
    gridLayout_MajorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 4, 0);
    gridLayout_MajorTicks->addWidget(label_MajorTicksColor,5,0);
    gridLayout_MajorTicks->addWidget(pushButton_MajorTicksColor,5,1);
    gridLayout_MajorTicks->addLayout(new Spacer(),6,1);

    groupBox_MajorTicks = new QGroupBox;
    groupBox_MajorTicks->setLayout(gridLayout_MajorTicks);

    checkBox_MinorTicks = new QCheckBox;
    checkBox_MinorTicks->setText(tr("Minor ticks"));
    checkBox_MinorTicks->setChecked(Axis.subTicks());
    connect(checkBox_MinorTicks,&QCheckBox::stateChanged,this,&AxisSettings::slot_MinorTicksWidgetsEnabled);

    label_MinorTicksSide = new QLabel;
    label_MinorTicksSide->setText(tr("Side"));

    comboBox_MinorTicksSide = new QComboBox;
    comboBox_MinorTicksSide->addItems({tr("Inside"),
                                       tr("Outside"),
                                       tr("On both sides")
                                      });
    if (Axis.subTickLengthIn() != 0 & Axis.subTickLengthOut() == 0)
        comboBox_MinorTicksSide->setCurrentIndex(0);
    else if (Axis.subTickLengthIn() == 0 & Axis.subTickLengthOut() != 0)
        comboBox_MinorTicksSide->setCurrentIndex(1);
    else
        comboBox_MinorTicksSide->setCurrentIndex(2);

    label_MinorTicksColor = new QLabel;
    label_MinorTicksColor->setText(tr("Color"));

    *color = Axis.subTickPen().color();
    pushButton_MinorTicksColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_MinorTicksColor, color);
    connect(pushButton_MinorTicksColor,&QPushButton::clicked,this,&AxisSettings::slot_MinorTicksColor);

    gridLayout_MinorTicks = new QGridLayout;
    gridLayout_MinorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 0, 0);
    gridLayout_MinorTicks->addWidget(checkBox_MinorTicks,1,0,1,2);
    gridLayout_MinorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 2, 0);
    gridLayout_MinorTicks->addWidget(label_MinorTicksSide,3,0);
    gridLayout_MinorTicks->addWidget(comboBox_MinorTicksSide,3,1);
    gridLayout_MinorTicks->addLayout(new Spacer(QSizePolicy::Minimum), 4, 0);
    gridLayout_MinorTicks->addWidget(label_MinorTicksColor,5,0);
    gridLayout_MinorTicks->addWidget(pushButton_MinorTicksColor,5,1);
    gridLayout_MinorTicks->addLayout(new Spacer(),6,1);

    groupBox_MinorTicks = new QGroupBox;
    groupBox_MinorTicks->setLayout(gridLayout_MinorTicks);

    checkBox_TicksLabel = new QCheckBox;
    checkBox_TicksLabel->setText(tr("Ticks label"));
    checkBox_TicksLabel->setChecked(Axis.tickLabels());
    connect(checkBox_TicksLabel,&QCheckBox::stateChanged,this,&AxisSettings::slot_TicksLabelWidgetsEnabled);

    label_TicksLabelSide = new QLabel;
    label_TicksLabelSide->setText(tr("Side"));

    comboBox_TicksLabelSide = new QComboBox;
    comboBox_TicksLabelSide->addItems({tr("Inside"),
                                       tr("Outside")
                                      });
    comboBox_TicksLabelSide->setCurrentIndex(Axis.tickLabelSide());

    label_TicksLabelColor = new QLabel;
    label_TicksLabelColor->setText(tr("Color"));

    *color = Axis.tickLabelColor();
    pushButton_TicksLabelColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_TicksLabelColor, color);
    connect(pushButton_TicksLabelColor,&QPushButton::clicked,this,&AxisSettings::slot_TicksLabelColor);

    gridLayout_TicksLabel = new QGridLayout;
    gridLayout_TicksLabel->addLayout(new Spacer(QSizePolicy::Minimum), 0, 0);
    gridLayout_TicksLabel->addWidget(checkBox_TicksLabel,1,0,1,2);
    gridLayout_TicksLabel->addLayout(new Spacer(QSizePolicy::Minimum), 2, 0);
    gridLayout_TicksLabel->addWidget(label_TicksLabelSide,3,0);
    gridLayout_TicksLabel->addWidget(comboBox_TicksLabelSide,3,1);
    gridLayout_TicksLabel->addLayout(new Spacer(QSizePolicy::Minimum), 4, 0);
    gridLayout_TicksLabel->addWidget(label_TicksLabelColor,5,0);
    gridLayout_TicksLabel->addWidget(pushButton_TicksLabelColor,5,1);
    gridLayout_TicksLabel->addLayout(new Spacer(),6,1);

    groupBox_TicksLabel = new QGroupBox;
    groupBox_TicksLabel->setLayout(gridLayout_TicksLabel);

    delete color;

    gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
//    gridLayout->addWidget(label_AxisTitle, 0, 0, 1, 6);
    gridLayout->addWidget(checkBox_AxisTitle, 0, 0, 1, 6);
    gridLayout->addWidget(groupBox_Title, 1, 0, 1, 6);
    gridLayout->addWidget(groupBox_RangeAxis, 2, 0, 1, 3);
    gridLayout->addWidget(groupBox_AxisColor, 2, 3, 1, 3);
    gridLayout->addWidget(groupBox_MajorTicks, 3, 0, 1, 2);
    gridLayout->addWidget(groupBox_MinorTicks, 3, 2, 1, 2);
    gridLayout->addWidget(groupBox_TicksLabel, 3, 4, 1, 2);

    setLayout(gridLayout);

    slot_AxisWidgetsEnabled(checkBox_AxisTitle->checkState());
    slot_TitleWidgetsEnabled(lineEdit_Title->text());
    slot_MajorTicksWidgetsEnabled(checkBox_MajorTicks->checkState());
    slot_MinorTicksWidgetsEnabled(checkBox_MinorTicks->checkState());
    slot_TicksLabelWidgetsEnabled(checkBox_TicksLabel->checkState());
}

AxisSettings::~AxisSettings()
{
//    delete label_AxisTitle;
    delete checkBox_AxisTitle;
    delete label_Title;
    delete label_TitleColor;
    delete label_RangeAxis;
    delete label_RangeAxisFrom;
    delete label_RangeAxisTo;
    delete label_AxisColor;
    delete label_MajorTicksSide;
    delete label_MajorTicksColor;
    delete label_MinorTicksSide;
    delete label_MinorTicksColor;
    delete label_TicksLabelSide;
    delete label_TicksLabelColor;
    delete checkBox_MajorTicks;
    delete checkBox_MinorTicks;
    delete checkBox_TicksLabel;
    delete lineEdit_Title;
    delete doubleSpinBox_RangeAxisFrom;
    delete doubleSpinBox_RangeAxisTo;
    delete pushButton_TitleColor;
    delete pushButton_AxisColor;
    delete pushButton_MajorTicksColor;
    delete pushButton_MinorTicksColor;
    delete pushButton_TicksLabelColor;
    delete comboBox_MajorTicksSide;
    delete comboBox_MinorTicksSide;
    delete comboBox_TicksLabelSide;
    delete gridLayout_Title;
    delete gridLayout_RangeAxis;
    delete gridLayout_AxisColor;
    delete gridLayout_MajorTicks;
    delete gridLayout_MinorTicks;
    delete gridLayout_TicksLabel;
    delete groupBox_Title;
    delete groupBox_RangeAxis;
    delete groupBox_AxisColor;
    delete groupBox_MajorTicks;
    delete groupBox_MinorTicks;
    delete groupBox_TicksLabel;
    delete gridLayout;
}

void AxisSettings::set()
{
    Axis.setVisible(checkBox_AxisTitle->isChecked());
    if (checkBox_AxisTitle->isChecked())
    {
        Axis.setLabel(lineEdit_Title->text());
        Axis.setLabelColor(pushButton_TitleColor->text());
        Axis.setRange(doubleSpinBox_RangeAxisFrom->value(),
                      doubleSpinBox_RangeAxisTo->value());
        QPen *pen = new QPen;
        pen->setColor(pushButton_AxisColor->text());
        Axis.setBasePen(*pen);
        Axis.setTicks(checkBox_MajorTicks->isChecked());
        switch (comboBox_MajorTicksSide->currentIndex()) {
        case 0:
            Axis.setTickLength(5, 0);
            break;
        case 1:
            Axis.setTickLength(0, 5);
            break;
        case 2:
            Axis.setTickLength(5, 5);
            break;
        default:
            break;
        }
        pen->setColor(pushButton_MajorTicksColor->text());
        Axis.setTickPen(*pen);
        Axis.setSubTicks(checkBox_MinorTicks->isChecked());
        switch (comboBox_MinorTicksSide->currentIndex()) {
        case 0:
            Axis.setSubTickLength(2, 0);
            break;
        case 1:
            Axis.setSubTickLength(0, 2);
            break;
        case 2:
            Axis.setSubTickLength(2, 2);
            break;
        default:
            break;
        }
        pen->setColor(pushButton_MinorTicksColor->text());
        Axis.setSubTickPen(*pen);
        delete pen;
        Axis.setTickLabels(checkBox_TicksLabel->isChecked());
        switch (comboBox_TicksLabelSide->currentIndex()) {
        case 0:
            Axis.setTickLabelSide(QCPAxis::lsInside);
            break;
        case 1:
            Axis.setTickLabelSide(QCPAxis::lsOutside);
            break;
        default:
            break;
        }
        Axis.setTickLabelColor(pushButton_TicksLabelColor->text());
    }
}

void AxisSettings::setPushButtonStyleSheet(QPushButton *button)
{
    QColor *color = new QColor(button->text());
    QColorDialog *colorDialog = new QColorDialog(*color, this);
    switch (colorDialog->exec()) {
    case QDialog::Accepted:
        *color = colorDialog->currentColor();
        break;
    case QDialog::Rejected:
        break;
    default:
        break;
    }
    delete colorDialog;
    setPushButtonStyleSheet(button, color);
    delete color;
}

void AxisSettings::setPushButtonStyleSheet(QPushButton *button, QColor *color)
{
    button->setText(color->name(QColor::HexRgb));
    button->setStyleSheet("QPushButton:disabled {color:#BEBEBE;}"
                         "QPushButton:enabled {font-weight: bold; color: "
                         + color->name(QColor::HexRgb) + ";}");
}

void AxisSettings::slot_TitleColor()
{
    setPushButtonStyleSheet(pushButton_TitleColor);
}

void AxisSettings::slot_AxisColor()
{
    setPushButtonStyleSheet(pushButton_AxisColor);
}

void AxisSettings::slot_MajorTicksColor()
{
    setPushButtonStyleSheet(pushButton_MajorTicksColor);
}

void AxisSettings::slot_MinorTicksColor()
{
    setPushButtonStyleSheet(pushButton_MinorTicksColor);
}

void AxisSettings::slot_TicksLabelColor()
{
    setPushButtonStyleSheet(pushButton_TicksLabelColor);
}

void AxisSettings::slot_AxisWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        groupBox_Title->setEnabled(true);
        groupBox_RangeAxis->setEnabled(true);
        groupBox_AxisColor->setEnabled(true);
        groupBox_MajorTicks->setEnabled(true);
        groupBox_MinorTicks->setEnabled(true);
        groupBox_TicksLabel->setEnabled(true);
    }
    else
    {
        groupBox_Title->setEnabled(false);
        groupBox_RangeAxis->setEnabled(false);
        groupBox_AxisColor->setEnabled(false);
        groupBox_MajorTicks->setEnabled(false);
        groupBox_MinorTicks->setEnabled(false);
        groupBox_TicksLabel->setEnabled(false);
    }
}

void AxisSettings::slot_TitleWidgetsEnabled(QString text)
{
    if (text.isEmpty())
    {
        label_TitleColor->setEnabled(false);
        pushButton_TitleColor->setEnabled(false);
    }
    else
    {
        label_TitleColor->setEnabled(true);
        pushButton_TitleColor->setEnabled(true);
    }
}

void AxisSettings::slot_MajorTicksWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        label_MajorTicksSide->setEnabled(true);
        comboBox_MajorTicksSide->setEnabled(true);
        label_MajorTicksColor->setEnabled(true);
        pushButton_MajorTicksColor->setEnabled(true);
    }
    else
    {
        label_MajorTicksSide->setEnabled(false);
        comboBox_MajorTicksSide->setEnabled(false);
        label_MajorTicksColor->setEnabled(false);
        pushButton_MajorTicksColor->setEnabled(false);
    }
}

void AxisSettings::slot_MinorTicksWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        label_MinorTicksSide->setEnabled(true);
        comboBox_MinorTicksSide->setEnabled(true);
        label_MinorTicksColor->setEnabled(true);
        pushButton_MinorTicksColor->setEnabled(true);
    }
    else
    {
        label_MinorTicksSide->setEnabled(false);
        comboBox_MinorTicksSide->setEnabled(false);
        label_MinorTicksColor->setEnabled(false);
        pushButton_MinorTicksColor->setEnabled(false);
    }
}

void AxisSettings::slot_TicksLabelWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        label_TicksLabelSide->setEnabled(true);
        comboBox_TicksLabelSide->setEnabled(true);
        label_TicksLabelColor->setEnabled(true);
        pushButton_TicksLabelColor->setEnabled(true);
    }
    else
    {
        label_TicksLabelSide->setEnabled(false);
        comboBox_TicksLabelSide->setEnabled(false);
        label_TicksLabelColor->setEnabled(false);
        pushButton_TicksLabelColor->setEnabled(false);
    }
}

GraphSettings::GraphSettings(QCPAbstractPlottable &plottable, QWidget *parent)
    : QWidget(parent),
      Plottable(plottable)
{
    label_GraphName = new QLabel;
    label_GraphName->setText(tr("Name"));
    label_GraphName->setStyleSheet("QLabel{ font: bold;}");

    lineEdit_GraphName = new QLineEdit;
    lineEdit_GraphName->setText(qobject_cast<QCPGraph *>(&Plottable)->name());

    gridLayout_GraphName = new QGridLayout;
    gridLayout_GraphName->addWidget(label_GraphName, 0, 0);
    gridLayout_GraphName->addWidget(lineEdit_GraphName, 0, 1);

    groupBox_GraphName = new QGroupBox;
    groupBox_GraphName->setLayout(gridLayout_GraphName);

    label_Line = new QLabel;
    label_Line->setText(tr("Line"));
    label_Line->setStyleSheet("QLabel{ font: bold;}");

    label_LineConnect = new QLabel;
    label_LineConnect->setText(tr("Connect"));

    comboBox_LineConnect = new QComboBox;
    comboBox_LineConnect->addItems({tr("None"),
                                    tr("Line"),
                                    tr("Step left"),
                                    tr("Step right"),
                                    tr("Step center"),
                                    tr("Impulse")
                                   });
    comboBox_LineConnect->setCurrentIndex(qobject_cast<QCPGraph *>(&Plottable)->lineStyle());
    connect(comboBox_LineConnect,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&GraphSettings::slot_LineWidgetsEnabled);

    label_LineStyle = new QLabel;
    label_LineStyle->setText(tr("Style"));

    comboBox_LineStyle = new QComboBox;
    comboBox_LineStyle->addItems({tr("Solid"),
                                  tr("Dash"),
                                  tr("Dot"),
                                  tr("Dash dot"),
                                  tr("Dash dot dot")
                                 });
    comboBox_LineStyle->setCurrentIndex(Plottable.pen().style() - 1);

    label_LineWidth = new QLabel;
    label_LineWidth->setText(tr("Width"));

    doubleSpinBox_LineWidth = new QDoubleSpinBox;
    doubleSpinBox_LineWidth->setValue(Plottable.pen().widthF());

    label_LineColor = new QLabel;
    label_LineColor->setText(tr("Color"));

    QColor *color = new QColor(Plottable.pen().color());
    pushButton_LineColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_LineColor, color);
    connect(pushButton_LineColor,&QPushButton::clicked,this,&GraphSettings::slot_LineColor);

    gridLayout_Line = new QGridLayout;
    gridLayout_Line->addWidget(label_Line, 0, 0, 1, 2);
    gridLayout_Line->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout_Line->addWidget(label_LineConnect, 2, 0);
    gridLayout_Line->addWidget(comboBox_LineConnect, 2, 1);
    gridLayout_Line->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);
    gridLayout_Line->addWidget(label_LineStyle, 4, 0);
    gridLayout_Line->addWidget(comboBox_LineStyle, 4, 1);
    gridLayout_Line->addLayout(new Spacer(QSizePolicy::Minimum), 5, 0);
    gridLayout_Line->addWidget(label_LineWidth, 6, 0);
    gridLayout_Line->addWidget(doubleSpinBox_LineWidth, 6, 1);
    gridLayout_Line->addLayout(new Spacer(QSizePolicy::Minimum), 7, 0);
    gridLayout_Line->addWidget(label_LineColor, 8, 0);
    gridLayout_Line->addWidget(pushButton_LineColor, 8, 1);
    gridLayout_Line->addLayout(new Spacer(), 9, 1);

    groupBox_Line = new QGroupBox;
    groupBox_Line->setLayout(gridLayout_Line);

    label_Scatter = new QLabel;
    label_Scatter->setText(tr("Scatter"));
    label_Scatter->setStyleSheet("QLabel{ font: bold;}");

    label_ScatterShape = new QLabel;
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
    comboBox_ScatterShape->setCurrentIndex(qobject_cast<QCPGraph *>(&Plottable)->scatterStyle().shape());
    connect(comboBox_ScatterShape,static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&GraphSettings::slot_ScatterWidgetsEnabled);

    label_ScatterSize = new QLabel;
    label_ScatterSize->setText(tr("Size"));

    doubleSpinBox_ScatterSize = new QDoubleSpinBox;
    doubleSpinBox_ScatterSize->setValue(qobject_cast<QCPGraph *>(&Plottable)->scatterStyle().size());

    label_ScatterEdgeColor = new QLabel;
    label_ScatterEdgeColor->setText(tr("Edge color"));

    *color = qobject_cast<QCPGraph *>(&Plottable)->scatterStyle().pen().color();
    pushButton_ScatterEdgeColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_ScatterEdgeColor, color);
    connect(pushButton_ScatterEdgeColor,&QPushButton::clicked,this,&GraphSettings::slot_ScatterEdgeColor);

    label_ScatterFillColor = new QLabel;
    label_ScatterFillColor->setText(tr("Fill color"));

    *color = qobject_cast<QCPGraph *>(&Plottable)->scatterStyle().brush().color();
    pushButton_ScatterFillColor = new QPushButton;
    setPushButtonStyleSheet(pushButton_ScatterFillColor, color);
    connect(pushButton_ScatterFillColor,&QPushButton::clicked,this,&GraphSettings::slot_ScatterFillColor);

    delete color;

    gridLayout_Scatter = new QGridLayout;
    gridLayout_Scatter->addWidget(label_Scatter, 0, 0, 1, 2);
    gridLayout_Scatter->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout_Scatter->addWidget(label_ScatterShape, 2, 0);
    gridLayout_Scatter->addWidget(comboBox_ScatterShape, 2, 1);
    gridLayout_Scatter->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);
    gridLayout_Scatter->addWidget(label_ScatterSize, 4, 0);
    gridLayout_Scatter->addWidget(doubleSpinBox_ScatterSize, 4, 1);
    gridLayout_Scatter->addLayout(new Spacer(QSizePolicy::Minimum), 5, 0);
    gridLayout_Scatter->addWidget(label_ScatterEdgeColor, 6, 0);
    gridLayout_Scatter->addWidget(pushButton_ScatterEdgeColor, 6, 1);
    gridLayout_Scatter->addLayout(new Spacer(QSizePolicy::Minimum), 7, 0);
    gridLayout_Scatter->addWidget(label_ScatterFillColor, 8, 0);
    gridLayout_Scatter->addWidget(pushButton_ScatterFillColor, 8, 1);
    gridLayout_Scatter->addLayout(new Spacer(), 9, 1);

    groupBox_Scatter = new QGroupBox;
    groupBox_Scatter->setLayout(gridLayout_Scatter);

    label_Axes = new QLabel;
    label_Axes->setText(tr("Axes"));

    comboBox_xAxis = new QComboBox;
    comboBox_xAxis->addItems({tr("Bottom"),
                              tr("Top")
                             });
    switch (Plottable.keyAxis()->axisType()) {
    case QCPAxis::atBottom:
        comboBox_xAxis->setCurrentIndex(0);
        break;
    case QCPAxis::atTop:
        comboBox_xAxis->setCurrentIndex(1);
        break;
    default:
        break;
    }

    comboBox_yAxis = new QComboBox;
    comboBox_yAxis->addItems({tr("Left"),
                              tr("Right")
                             });
    switch (Plottable.valueAxis()->axisType()) {
    case QCPAxis::atLeft:
        comboBox_yAxis->setCurrentIndex(0);
        break;
    case QCPAxis::atRight:
        comboBox_yAxis->setCurrentIndex(1);
        break;
    default:
        break;
    }

    gridLayout_Global = new QGridLayout;
    gridLayout_Global->addLayout(new Spacer(QSizePolicy::Minimum), 0, 0);
    gridLayout_Global->addWidget(label_Axes, 1, 0);
    gridLayout_Global->addWidget(comboBox_xAxis, 1, 1);
    gridLayout_Global->addWidget(comboBox_yAxis, 1, 2);
    gridLayout_Global->addLayout(new Spacer(), 2, 1);
    gridLayout_Global->addLayout(new Spacer(), 2, 2);

    groupBox_Global = new QGroupBox;
    groupBox_Global->setLayout(gridLayout_Global);

    gridLayout = new QGridLayout;
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(groupBox_GraphName, 0, 0, 1, 2);
    gridLayout->addWidget(groupBox_Line, 1, 0);
    gridLayout->addWidget(groupBox_Scatter, 1, 1);
    gridLayout->addWidget(groupBox_Global, 2, 0, 1, 2);

    setLayout(gridLayout);

    slot_LineWidgetsEnabled(comboBox_LineConnect->currentIndex());
    slot_ScatterWidgetsEnabled(comboBox_ScatterShape->currentIndex());
}

GraphSettings::~GraphSettings()
{
    delete label_GraphName;
    delete lineEdit_GraphName;
    delete gridLayout_GraphName;
    delete groupBox_GraphName;
    delete label_Line;
    delete label_LineConnect;
    delete comboBox_LineConnect;
    delete label_LineStyle;
    delete comboBox_LineStyle;
    delete label_LineWidth;
    delete doubleSpinBox_LineWidth;
    delete label_LineColor;
    delete pushButton_LineColor;
    delete gridLayout_Line;
    delete groupBox_Line;
    delete label_Scatter;
    delete label_ScatterShape;
    delete comboBox_ScatterShape;
    delete label_ScatterSize;
    delete doubleSpinBox_ScatterSize;
    delete label_ScatterEdgeColor;
    delete pushButton_ScatterEdgeColor;
    delete label_ScatterFillColor;
    delete pushButton_ScatterFillColor;
    delete gridLayout_Scatter;
    delete groupBox_Scatter;
    delete gridLayout;
}

void GraphSettings::set()
{
    qobject_cast<QCPGraph *>(&Plottable)->setName(lineEdit_GraphName->text());

    switch (comboBox_LineConnect->currentIndex()) { //пал
    case 0:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsNone);
        break;
    case 1:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsLine);
        break;
    case 2:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsStepLeft);
        break;
    case 3:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsStepRight);
        break;
    case 4:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsStepCenter);
        break;
    case 5:
        qobject_cast<QCPGraph *>(&Plottable)->setLineStyle(QCPGraph::lsImpulse);
        break;
    default:
        break;
    }

    QPen *pen = new QPen;

    switch (comboBox_LineStyle->currentIndex()) {
    case 0:
        pen->setStyle(Qt::SolidLine);
        break;
    case 1:
        pen->setStyle(Qt::DashLine);
        break;
    case 2:
        pen->setStyle(Qt::DotLine);
        break;
    case 3:
        pen->setStyle(Qt::DashDotLine);
        break;
    case 4:
        pen->setStyle(Qt::DashDotDotLine);
        break;
    default:
        break;
    }

    pen->setWidthF(doubleSpinBox_LineWidth->value());
    pen->setColor(pushButton_LineColor->text());

    Plottable.setPen(*pen);

    delete pen;

    switch (comboBox_ScatterShape->currentIndex()) {
    case 0:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 1:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 2:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 3:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 4:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 5:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 6:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 7:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 8:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 9:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 10:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 11:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossSquare, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 12:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 13:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossCircle, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 14:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    case 15:
        qobject_cast<QCPGraph *>(&Plottable)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPeace, QColor(pushButton_ScatterEdgeColor->text()), QColor(pushButton_ScatterFillColor->text()), doubleSpinBox_ScatterSize->value()));
        break;
    default:
        break;
    }

    switch (comboBox_xAxis->currentIndex()) {
    case 0:
        Plottable.setKeyAxis(qobject_cast<QCustomPlot *>(Plottable.parent())->xAxis);
        break;
    case 1:
        Plottable.setKeyAxis(qobject_cast<QCustomPlot *>(Plottable.parent())->xAxis2);
        break;
    default:
        break;
    }
    switch (comboBox_yAxis->currentIndex()) {
    case 0:
        Plottable.setValueAxis(qobject_cast<QCustomPlot *>(Plottable.parent())->yAxis);
        break;
    case 1:
        Plottable.setValueAxis(qobject_cast<QCustomPlot *>(Plottable.parent())->yAxis2);
        break;
    default:
        break;
    }
}

void GraphSettings::setPushButtonStyleSheet(QPushButton *button)
{
    QColor *color = new QColor(button->text());
    QColorDialog *colorDialog = new QColorDialog(*color, this);
    switch (colorDialog->exec()) {
    case QDialog::Accepted:
        *color = colorDialog->currentColor();
        break;
    case QDialog::Rejected:
        break;
    default:
        break;
    }
    delete colorDialog;
    setPushButtonStyleSheet(button, color);
    delete color;
}

void GraphSettings::setPushButtonStyleSheet(QPushButton *button, QColor *color)
{
    button->setText(color->name(QColor::HexRgb));
    button->setStyleSheet("QPushButton:disabled {color:#BEBEBE;}"
                         "QPushButton:enabled {font-weight: bold; color: "
                         + color->name(QColor::HexRgb) + ";}");
}

void GraphSettings::slot_LineColor()
{

    setPushButtonStyleSheet(pushButton_LineColor);
}

void GraphSettings::slot_ScatterEdgeColor()
{
    setPushButtonStyleSheet(pushButton_ScatterEdgeColor);
}

void GraphSettings::slot_ScatterFillColor()
{
    setPushButtonStyleSheet(pushButton_ScatterFillColor);
}

void GraphSettings::slot_ScatterWidgetsEnabled(int index)
{
    if (index == 0)
    {
        label_ScatterSize->setEnabled(false);
        doubleSpinBox_ScatterSize->setEnabled(false);
        label_ScatterEdgeColor->setEnabled(false);
        pushButton_ScatterEdgeColor->setEnabled(false);
        label_ScatterFillColor->setEnabled(false);
        pushButton_ScatterFillColor->setEnabled(false);
    }
    else if (index == 1 || index == 2 || index == 3 || index == 5 || index == 8)
    {
        label_ScatterSize->setEnabled(true);
        doubleSpinBox_ScatterSize->setEnabled(true);
        label_ScatterEdgeColor->setEnabled(true);
        pushButton_ScatterEdgeColor->setEnabled(true);
        label_ScatterFillColor->setEnabled(false);
        pushButton_ScatterFillColor->setEnabled(false);
    }
    else
    {
        label_ScatterSize->setEnabled(true);
        doubleSpinBox_ScatterSize->setEnabled(true);
        label_ScatterEdgeColor->setEnabled(true);
        pushButton_ScatterEdgeColor->setEnabled(true);
        label_ScatterFillColor->setEnabled(true);
        pushButton_ScatterFillColor->setEnabled(true);
    }
}

void GraphSettings::slot_LineWidgetsEnabled(int index)
{
    if (index == 0)
    {
        label_LineStyle->setEnabled(false);
        comboBox_LineStyle->setEnabled(false);
        label_LineWidth->setEnabled(false);
        doubleSpinBox_LineWidth->setEnabled(false);
        label_LineColor->setEnabled(false);
        pushButton_LineColor->setEnabled(false);
    }
    else
    {
        label_LineStyle->setEnabled(true);
        comboBox_LineStyle->setEnabled(true);
        label_LineWidth->setEnabled(true);
        doubleSpinBox_LineWidth->setEnabled(true);
        label_LineColor->setEnabled(true);
        pushButton_LineColor->setEnabled(true);
    }
}

GridSettings::GridSettings(QCustomPlot &qcp, QWidget *parent)
    : QWidget(parent)
{
    gridSettingsBox_Horizontal = new GridSettingsBox(qcp, GridSettingsBox::Horizontal);

    gridSettingsBox_Vertical = new GridSettingsBox(qcp, GridSettingsBox::Vertical);

    gridLayout = new QGridLayout;
    gridLayout->addWidget(gridSettingsBox_Horizontal);
    gridLayout->addWidget(gridSettingsBox_Vertical);

    setLayout(gridLayout);
}

GridSettings::~GridSettings()
{
    delete gridSettingsBox_Horizontal;
    delete gridSettingsBox_Vertical;
    delete gridLayout;
}

void GridSettings::set()
{
    gridSettingsBox_Horizontal->set();
    gridSettingsBox_Vertical->set();
}

LegendSettings::LegendSettings(QCPLegend &legend, QWidget *parent)
    : QWidget(parent),
      Legend(legend)
{
    checkBox_Visible = new QCheckBox;
    checkBox_Visible->setText(tr("Visible"));
    checkBox_Visible->setChecked(Legend.visible());

    gridLayout = new QGridLayout;
    gridLayout->addWidget(checkBox_Visible);

    setLayout(gridLayout);
}

LegendSettings::~LegendSettings()
{
    delete checkBox_Visible;
    delete gridLayout;
}

void LegendSettings::set()
{
    Legend.setVisible(checkBox_Visible->isChecked());
}

GridSettingsBox::GridSettingsBox(QCustomPlot &qcp_, Grid g, QWidget *parent)
    : QGroupBox(parent),
      G(g),
      qcp(qcp_)
{
    label = new QLabel;
    label->setStyleSheet("QLabel{ font: bold;}");

    checkBoxMajor = new QCheckBox;
    checkBoxMajor->setText(tr("Major grid"));
    connect(checkBoxMajor,&QCheckBox::stateChanged,this,&GridSettingsBox::slot_MajorWidgetsEnabled);

    checkBoxMinor = new QCheckBox;
    checkBoxMinor->setText(tr("Minor grid"));
    connect(checkBoxMinor,&QCheckBox::stateChanged,this,&GridSettingsBox::slot_MinorWidgetsEnabled);

    label_LineStyle = new QLabel;
    label_LineStyle->setText(tr("Line style"));

    comboBoxMajor_LineStyle = new QComboBox;
    comboBoxMajor_LineStyle->addItems({tr("Solid"),
                                       tr("Dash"),
                                       tr("Dot"),
                                       tr("Dash dot"),
                                       tr("Dash dot dot")
                                      });

    comboBoxMinor_LineStyle = new QComboBox;
    comboBoxMinor_LineStyle->addItems({tr("Solid"),
                                       tr("Dash"),
                                       tr("Dot"),
                                       tr("Dash dot"),
                                       tr("Dash dot dot")
                                      });

    label_LineWidth = new QLabel;
    label_LineWidth->setText(tr("Line width"));

    doubleSpinBoxMajor_LineWidth = new QDoubleSpinBox;

    doubleSpinBoxMinor_LineWidth = new QDoubleSpinBox;

    label_LineColor = new QLabel;
    label_LineColor->setText(tr("Line color"));

    pushButtonMajor_LineColor = new QPushButton;
    connect(pushButtonMajor_LineColor,&QPushButton::clicked,this,&GridSettingsBox::slot_MajorLineColor);

    pushButtonMinor_LineColor = new QPushButton;
    connect(pushButtonMinor_LineColor,&QPushButton::clicked,this,&GridSettingsBox::slot_MinorLineColor);

    label_Axis = new QLabel;
    label_Axis->setText(tr("Axis"));

    comboBox_Axis = new QComboBox;

    gridLayout = new QGridLayout;
    gridLayout->addWidget(label, 0, 0, 1, 3);
    gridLayout->addLayout(new Spacer(QSizePolicy::Minimum), 1, 0);
    gridLayout->addWidget(checkBoxMajor, 2, 1);
    gridLayout->addWidget(checkBoxMinor, 2, 2);
    gridLayout->addLayout(new Spacer(QSizePolicy::Minimum), 3, 0);
    gridLayout->addWidget(label_LineStyle, 4, 0);
    gridLayout->addWidget(comboBoxMajor_LineStyle, 4, 1);
    gridLayout->addWidget(comboBoxMinor_LineStyle, 4, 2);
    gridLayout->addLayout(new Spacer(QSizePolicy::Minimum), 5, 0);
    gridLayout->addWidget(label_LineWidth, 6, 0);
    gridLayout->addWidget(doubleSpinBoxMajor_LineWidth, 6, 1);
    gridLayout->addWidget(doubleSpinBoxMinor_LineWidth, 6, 2);
    gridLayout->addLayout(new Spacer(QSizePolicy::Minimum), 7, 0);
    gridLayout->addWidget(label_LineColor, 8, 0);
    gridLayout->addWidget(pushButtonMajor_LineColor, 8, 1);
    gridLayout->addWidget(pushButtonMinor_LineColor, 8, 2);
    gridLayout->addLayout(new Spacer(), 9, 1);
    gridLayout->addLayout(new Spacer(), 9, 2);
    gridLayout->addWidget(label_Axis, 10, 0);
    gridLayout->addWidget(comboBox_Axis, 10, 1, 1, 2);

    setLayout(gridLayout);

    QColor *color = new QColor;
    switch (G) {
    case Horizontal:
        label->setText(tr("Horizontal"));
        if (qcp.yAxis->grid()->visible())
        {
            checkBoxMajor->setChecked(true);
            comboBoxMajor_LineStyle->setCurrentIndex(qcp.yAxis->grid()->pen().style() - 1);
            doubleSpinBoxMajor_LineWidth->setValue(qcp.yAxis->grid()->pen().widthF());
            *color = qcp.yAxis->grid()->pen().color();
        }
        else if (qcp.yAxis2->grid()->visible())
        {
            checkBoxMajor->setChecked(true);
            comboBoxMajor_LineStyle->setCurrentIndex(qcp.yAxis2->grid()->pen().style() - 1);
            doubleSpinBoxMajor_LineWidth->setValue(qcp.yAxis2->grid()->pen().widthF());
            *color = qcp.yAxis2->grid()->pen().color();
        }
        setPushButtonStyleSheet(pushButtonMajor_LineColor, color);
        if (qcp.yAxis->grid()->subGridVisible())
        {
            checkBoxMinor->setChecked(true);
            comboBoxMinor_LineStyle->setCurrentIndex(qcp.yAxis->grid()->subGridPen().style() - 1);
            doubleSpinBoxMinor_LineWidth->setValue(qcp.yAxis->grid()->subGridPen().widthF());
            *color = qcp.yAxis->grid()->subGridPen().color();
        }
        else if (qcp.yAxis2->grid()->subGridVisible())
        {
            checkBoxMinor->setChecked(true);
            comboBoxMinor_LineStyle->setCurrentIndex(qcp.yAxis2->grid()->subGridPen().style() - 1);
            doubleSpinBoxMinor_LineWidth->setValue(qcp.yAxis2->grid()->subGridPen().widthF());
            *color = qcp.yAxis2->grid()->subGridPen().color();
        }
        setPushButtonStyleSheet(pushButtonMinor_LineColor, color);
        comboBox_Axis->addItems({tr("Left"),
                                 tr("Right")
                                });
        if (qcp.yAxis->grid()->visible() || qcp.yAxis->grid()->subGridVisible())
            comboBox_Axis->setCurrentIndex(0);
        else if (qcp.yAxis2->grid()->visible() || qcp.yAxis2->grid()->subGridVisible())
            comboBox_Axis->setCurrentIndex(1);
        break;
    case Vertical:
        label->setText(tr("Vertical"));
        if (qcp.xAxis->grid()->visible())
        {
            checkBoxMajor->setChecked(true);
            comboBoxMajor_LineStyle->setCurrentIndex(qcp.xAxis->grid()->pen().style() - 1);
            doubleSpinBoxMajor_LineWidth->setValue(qcp.xAxis->grid()->pen().widthF());
            *color = qcp.xAxis->grid()->pen().color();
        }
        else if (qcp.xAxis2->grid()->visible())
        {
            checkBoxMajor->setChecked(true);
            comboBoxMajor_LineStyle->setCurrentIndex(qcp.xAxis2->grid()->pen().style() - 1);
            doubleSpinBoxMajor_LineWidth->setValue(qcp.xAxis2->grid()->pen().widthF());
            *color = qcp.xAxis2->grid()->pen().color();
        }
        setPushButtonStyleSheet(pushButtonMajor_LineColor, color);

        if (qcp.xAxis->grid()->subGridVisible())
        {
            checkBoxMinor->setChecked(true);
            comboBoxMinor_LineStyle->setCurrentIndex(qcp.xAxis->grid()->subGridPen().style() - 1);
            doubleSpinBoxMinor_LineWidth->setValue(qcp.xAxis->grid()->subGridPen().widthF());
            *color = qcp.xAxis->grid()->subGridPen().color();
        }
        else if (qcp.xAxis2->grid()->subGridVisible())
        {
            checkBoxMinor->setChecked(true);
            comboBoxMinor_LineStyle->setCurrentIndex(qcp.xAxis2->grid()->subGridPen().style() - 1);
            doubleSpinBoxMinor_LineWidth->setValue(qcp.xAxis2->grid()->subGridPen().widthF());
            *color = qcp.xAxis2->grid()->subGridPen().color();
        }
        setPushButtonStyleSheet(pushButtonMinor_LineColor, color);

        comboBox_Axis->addItems({tr("Bottom"),
                                 tr("Top")
                                });
        if (qcp.xAxis->grid()->visible() || qcp.xAxis->grid()->subGridVisible())
            comboBox_Axis->setCurrentIndex(0);
        else if (qcp.xAxis2->grid()->visible() || qcp.xAxis2->grid()->subGridVisible())
            comboBox_Axis->setCurrentIndex(1);
        break;
    default:
        break;
    }
    delete color;

    slot_MajorWidgetsEnabled(checkBoxMajor->checkState());
    slot_MinorWidgetsEnabled(checkBoxMinor->checkState());
}

GridSettingsBox::~GridSettingsBox()
{
    delete label;
    delete label_LineStyle;
    delete label_LineWidth;
    delete label_LineColor;
    delete label_Axis;
    delete checkBoxMajor;
    delete checkBoxMinor;
    delete comboBoxMajor_LineStyle;
    delete comboBoxMinor_LineStyle;
    delete doubleSpinBoxMajor_LineWidth;
    delete doubleSpinBoxMinor_LineWidth;
    delete pushButtonMajor_LineColor;
    delete pushButtonMinor_LineColor;
    delete comboBox_Axis;
    delete gridLayout;
}

void GridSettingsBox::set()
{
    QCPGrid *grid = nullptr;
    if (G == Horizontal && comboBox_Axis->currentIndex() == 0)
    {
        qcp.yAxis->grid()->setVisible(true);
        qcp.yAxis2->grid()->setVisible(false);
        qcp.yAxis->grid()->setSubGridVisible(true);
        qcp.yAxis2->grid()->setSubGridVisible(false);
        grid = qcp.yAxis->grid();
    }
    else if (G == Horizontal && comboBox_Axis->currentIndex() == 1)
    {
        qcp.yAxis->grid()->setVisible(false);
        qcp.yAxis2->grid()->setVisible(true);
        qcp.yAxis->grid()->setSubGridVisible(false);
        qcp.yAxis2->grid()->setSubGridVisible(true);
        grid = qcp.yAxis2->grid();
    }
    else if (G == Vertical && comboBox_Axis->currentIndex() == 0)
    {
        qcp.xAxis->grid()->setVisible(true);
        qcp.xAxis2->grid()->setVisible(false);
        qcp.xAxis->grid()->setSubGridVisible(true);
        qcp.xAxis2->grid()->setSubGridVisible(false);
        grid = qcp.xAxis->grid();
    }
    else if (G == Vertical && comboBox_Axis->currentIndex() == 1)
    {
        qcp.xAxis->grid()->setVisible(false);
        qcp.xAxis2->grid()->setVisible(true);
        qcp.xAxis->grid()->setSubGridVisible(false);
        qcp.xAxis2->grid()->setSubGridVisible(true);
        grid = qcp.xAxis2->grid();
    }
    if (!grid)
        return;

    QPen *pen = new QPen;
    if (checkBoxMajor->checkState() == Qt::Checked)
    {
        switch (comboBoxMajor_LineStyle->currentIndex()) {
        case 0:
            pen->setStyle(Qt::SolidLine);
            break;
        case 1:
            pen->setStyle(Qt::DashLine);
            break;
        case 2:
            pen->setStyle(Qt::DotLine);
            break;
        case 3:
            pen->setStyle(Qt::DashDotLine);
            break;
        case 4:
            pen->setStyle(Qt::DashDotDotLine);
            break;
        default:
            break;
        }
        pen->setWidthF(doubleSpinBoxMajor_LineWidth->value());
        pen->setColor(pushButtonMajor_LineColor->text());
        grid->setPen(*pen);
    }
    else
    {
        grid->setVisible(false);
    }

    if (checkBoxMinor->checkState() == Qt::Checked)
    {
        switch (comboBoxMinor_LineStyle->currentIndex()) {
        case 0:
            pen->setStyle(Qt::SolidLine);
            break;
        case 1:
            pen->setStyle(Qt::DashLine);
            break;
        case 2:
            pen->setStyle(Qt::DotLine);
            break;
        case 3:
            pen->setStyle(Qt::DashDotLine);
            break;
        case 4:
            pen->setStyle(Qt::DashDotDotLine);
            break;
        default:
            break;
        }
        pen->setWidthF(doubleSpinBoxMinor_LineWidth->value());
        pen->setColor(pushButtonMinor_LineColor->text());
        grid->setSubGridPen(*pen);
    }
    else
    {
        grid->setSubGridVisible(false);
    }
    delete pen;
}

void GridSettingsBox::setPushButtonStyleSheet(QPushButton *button)
{
    QColor *color = new QColor(button->text());
    QColorDialog *colorDialog = new QColorDialog(*color, this);
    switch (colorDialog->exec()) {
    case QDialog::Accepted:
        *color = colorDialog->currentColor();
        break;
    case QDialog::Rejected:
        break;
    default:
        break;
    }
    delete colorDialog;
    setPushButtonStyleSheet(button, color);
    delete color;
}

void GridSettingsBox::setPushButtonStyleSheet(QPushButton *button, QColor *color)
{
    button->setText(color->name(QColor::HexRgb));
    button->setStyleSheet("QPushButton:disabled {color:#BEBEBE;}"
                         "QPushButton:enabled {font-weight: bold; color: "
                         + color->name(QColor::HexRgb) + ";}");
}

void GridSettingsBox::slot_MajorLineColor()
{
    setPushButtonStyleSheet(pushButtonMajor_LineColor);
}

void GridSettingsBox::slot_MinorLineColor()
{
    setPushButtonStyleSheet(pushButtonMinor_LineColor);
}

void GridSettingsBox::slot_MajorWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        comboBoxMajor_LineStyle->setEnabled(true);
        doubleSpinBoxMajor_LineWidth->setEnabled(true);
        pushButtonMajor_LineColor->setEnabled(true);
    }
    else
    {
        comboBoxMajor_LineStyle->setEnabled(false);
        doubleSpinBoxMajor_LineWidth->setEnabled(false);
        pushButtonMajor_LineColor->setEnabled(false);
    }
}

void GridSettingsBox::slot_MinorWidgetsEnabled(int state)
{
    if (state == Qt::Checked)
    {
        comboBoxMinor_LineStyle->setEnabled(true);
        doubleSpinBoxMinor_LineWidth->setEnabled(true);
        pushButtonMinor_LineColor->setEnabled(true);
    }
    else
    {
        comboBoxMinor_LineStyle->setEnabled(false);
        doubleSpinBoxMinor_LineWidth->setEnabled(false);
        pushButtonMinor_LineColor->setEnabled(false);
    }
}
