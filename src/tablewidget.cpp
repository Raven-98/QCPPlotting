#include "tablewidget.h"

#include <QIcon>
#include <QPainter>

int TableWidget::SumTables = 0;
int TableWidget::ResTableWidgetID = 0;

TableWidget::TableWidget(QWidget *parent)
  : QWidget{parent}
{
  SumTables++;
  ResTableWidgetID++;
}

TableWidget::~TableWidget()
{
  SumTables--;
  if (SumTables == 0)
    ResTableWidgetID = 0;

  if (actionPlotGraph)
    delete actionPlotGraph;
  if (menuPlot)
    delete menuPlot;
  if (actionSaveTable)
    delete actionSaveTable;
  if (actionSaveSelectedTable)
    delete actionSaveSelectedTable;
  if (contextMenu)
    delete contextMenu;

  if (tableWidget) {
      delete tableWidget->model();
      delete tableWidget;
    }
  if (gridLayout)
    delete gridLayout;
}

void TableWidget::init()
{
  resize(300, 400);
  setWindowIcon(QIcon(":/img/table.png"));
  setWindowTitle(tr("Table %1").arg(ResTableWidgetID));

  tableWidget = new QTableView;

  horizontalHeaderView = new HorizontalHeaderView;
  tableWidget->setHorizontalHeader(horizontalHeaderView);  
  connect(horizontalHeaderView, &HorizontalHeaderView::customContextMenuRequested, this, &TableWidget::checkSelectionModel);
  connect(this, &TableWidget::setEnabledActions, horizontalHeaderView, &HorizontalHeaderView::slot_setEnabledActions);
  connect(horizontalHeaderView, &HorizontalHeaderView::buildGraph, this, &TableWidget::buildGraphTrigered);
  connect(horizontalHeaderView, &HorizontalHeaderView::saveTable, this, &TableWidget::saveTableTrigered);
  connect(horizontalHeaderView, &HorizontalHeaderView::addColumn, this, &TableWidget::addColumnTrigered);

  gridLayout = new QGridLayout;
  gridLayout->addWidget(tableWidget, 0, 0);
  gridLayout->setContentsMargins(0, 0, 0, 0);

  setLayout(gridLayout);

  tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(tableWidget, &QTableView::customContextMenuRequested, this, &TableWidget::checkSelectionModel);
  connect(this, &TableWidget::setEnabledActions, this, &TableWidget::slot_setEnabledActions);
  connect(tableWidget, &QTableView::customContextMenuRequested, this, &TableWidget::customHeaderMenuRequested);  

  actionPlotGraph = new QAction;
  actionPlotGraph->setText(tr("Graph"));
  connect(actionPlotGraph, &QAction::triggered, this, &TableWidget::buildGraphTrigered);

  menuPlot = new QMenu;
  menuPlot->setTitle(tr("Plot"));
  menuPlot->addAction(actionPlotGraph);

  actionSaveTable = new QAction;
  actionSaveTable->setText(tr("Save table"));
  actionSaveTable->setIcon(QIcon(":/img/save.png"));

  actionSaveSelectedTable = new QAction;
  actionSaveSelectedTable->setText(tr("Save selected table"));
  actionSaveSelectedTable->setIcon(QIcon(":/img/save.png"));

  contextMenu = new QMenu;
  contextMenu->addMenu(menuPlot);
  contextMenu->addSeparator();
  contextMenu->addAction(actionSaveSelectedTable);
  contextMenu->addAction(actionSaveTable);
}

void TableWidget::setModel(QStandardItemModel *item_model)
{
  tableWidget->setModel(item_model);
}

void TableWidget::customHeaderMenuRequested(const QPoint &pos)
{
  contextMenu->popup(tableWidget->viewport()->mapToGlobal(pos));
}

void TableWidget::checkSelectionModel()
{
  emit setEnabledActions(tableWidget->selectionModel()->hasSelection());
}

void TableWidget::slot_setEnabledActions(bool e)
{
  menuPlot->setEnabled(e);
  actionSaveSelectedTable->setEnabled(e);
}

void TableWidget::buildGraphTrigered()
{
  auto selectedIndexes = tableWidget->selectionModel()->selectedIndexes();
  if (selectedIndexes.size() < 2) {
      emit error("selectedIndexes.size() < 2");
      return;
    }

  auto data = builderData(selectedIndexes);

  emit buildGraph(data);
}

void TableWidget::saveTableTrigered()
{
}

void TableWidget::addColumnTrigered()
{
  tableWidget->model()->insertColumn(tableWidget->model()->columnCount());
}

// !!!
// Елементи заповнюються по черзі вибору
// !!!
QVector<QVector<double> > TableWidget::builderData(QModelIndexList &selectedIndexes)
{
  int selectedIndexesCount = selectedIndexes.count();
  QList<int> lst;
  for (int i = 0; i < selectedIndexesCount; i++) {
      int c = selectedIndexes.at(i).column();
      if (!lst.contains(c))
        lst.append(c);
    }
  int columns = lst.count();
  lst.clear();

  QVector<QVector<double>> data;

  for (int column = 0; column < columns; column++) {
      QVector<double> vector;
      if (selectedIndexes.at(0).column() != selectedIndexes.at(1).column()) {
          for (int row = column; row < selectedIndexesCount; row += columns) {
              bool ok;
              auto d = selectedIndexes.at(row).data().toDouble(&ok);
              if (ok)
                vector.push_back(d);
            }
        }
      else {
          for (int row = (selectedIndexesCount / columns * column); row < (selectedIndexesCount / columns * (column + 1)); row++) {
              bool ok;
              auto d = selectedIndexes.at(row).data().toDouble(&ok);
              if (ok)
                vector.push_back(d);
            }
        }

      data.push_back(vector);
      vector.clear();
    }

  return data;
}

//void TableView::keyReleaseEvent(QKeyEvent *event)
//{
//  QTableView::keyReleaseEvent(event);
//}

HorizontalHeaderView::HorizontalHeaderView(QWidget *parent)
  : QHeaderView(Qt::Horizontal, parent)
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QHeaderView::customContextMenuRequested, this, &HorizontalHeaderView::customHeaderMenuRequested);
  setSectionsClickable(true);
  setSectionResizeMode(QHeaderView::Interactive);
  setItemDelegate(new TableStyledItemDelegate(this));
  setHighlightSections(true);

  actionPlotGraph = new QAction;
  actionPlotGraph->setText(tr("Graph"));
  connect(actionPlotGraph, &QAction::triggered, this, &HorizontalHeaderView::buildGraphTrigered);

  menuPlot = new QMenu;
  menuPlot->setTitle(tr("Plot"));
  menuPlot->addAction(actionPlotGraph);

  actionSaveTable = new QAction;
  actionSaveTable->setText(tr("Save table"));
  actionSaveTable->setIcon(QIcon(":/img/save.png"));
  connect(actionSaveTable, &QAction::triggered, this, &HorizontalHeaderView::saveTableTrigered);

  actionAddColumn = new QAction;
  actionAddColumn->setText(tr("Add column"));
  actionAddColumn->setIcon(QIcon(":/img/add_column.png"));
  connect(actionAddColumn, &QAction::triggered, this, &HorizontalHeaderView::addColumnTrigered);

  hHeaderMenu = new QMenu;
  hHeaderMenu->addMenu(menuPlot);
  hHeaderMenu->addSeparator();
  hHeaderMenu->addAction(actionAddColumn);
  hHeaderMenu->addSeparator();
  hHeaderMenu->addAction(actionSaveTable);
}

HorizontalHeaderView::~HorizontalHeaderView()
{
  if (sectionIndicator)
    delete sectionIndicator;

  if (actionPlotGraph)
    delete actionPlotGraph;
  if (menuPlot)
    delete menuPlot;
  if (actionSaveTable)
    delete actionSaveTable;
  if (actionAddColumn)
    delete actionAddColumn;
  if (hHeaderMenu)
    delete hHeaderMenu;
}

void HorizontalHeaderView::customHeaderMenuRequested(const QPoint &pos)
{
  hHeaderMenu->popup(viewport()->mapToGlobal(pos));
}

void HorizontalHeaderView::slot_setEnabledActions(bool e)
{
  menuPlot->setEnabled(e);
}

void HorizontalHeaderView::buildGraphTrigered()
{
  emit buildGraph();
}

void HorizontalHeaderView::saveTableTrigered()
{
  emit saveTable();
}

void HorizontalHeaderView::addColumnTrigered()
{
  emit addColumn();
}

void HorizontalHeaderView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MiddleButton) {
      pressMiddleButton = 1;
      firstPos = event->pos().x();
      emit sectionPressed(logicalIndexAt(event->pos()));

      if (!sectionIndicator)
        sectionIndicator = new QLabel(this->viewport());

      int section = logicalIndexAt(firstPos);
      int w = sectionSize(section);
      int h = viewport()->height();
      int p = sectionViewportPosition(section);

      sectionIndicator->resize(w, h);

      const qreal pixmapDevicePixelRatio = this->devicePixelRatio();
      QPixmap pm(QSize(w, h) * pixmapDevicePixelRatio);
      pm.setDevicePixelRatio(pixmapDevicePixelRatio);
      pm.fill(QColor(0, 0, 0, 45));
      QRect rect(0, 0, w, h);
      QPainter painter(&pm);
      const QVariant variant = this->model()->headerData(section, Qt::Horizontal, Qt::FontRole);
      if (variant.isValid() && variant.canConvert<QFont>()) {
          const QFont sectionFont = qvariant_cast<QFont>(variant);
          painter.setFont(sectionFont);
        }
      else {
          painter.setFont(this->font());
        }
      painter.setOpacity(0.8);
      this->paintSection(&painter, rect, section);
      painter.end();

      sectionIndicator->setPixmap(pm);
      sectionIndicatorOffset = firstPos - qMax(p, 0);
    }

  QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent *event)
{
  if (pressMiddleButton) {
      int pos =  event->pos().x();
      if (pos < 0)
        return;

      int visual = visualIndexAt(pos);
      if (visual == -1)
        return;

      if (!sectionIndicator)
        return;
      sectionIndicator->move(pos - sectionIndicatorOffset, 0);
      sectionIndicator->show();

      lastPos = pos;
    }

    QHeaderView::mouseMoveEvent(event);
}

void HorizontalHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MiddleButton) {
      int from = visualIndexAt(firstPos);
      int to = visualIndexAt(lastPos);
      if (from != -1 && to != -1)
        this->moveSection(from, to);

      if (!sectionIndicator)
        return;
      sectionIndicator->hide();
      pressMiddleButton = 0;
    }

  QHeaderView::mouseReleaseEvent(event);
}

QWidget *TableStyledItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
  TableLineEdit *editor = new TableLineEdit(parent);
  connect(editor, SIGNAL(keyTabReleased()), this, SIGNAL(keyTabReleased()));
  return editor;
}

void TableLineEdit::keyReleaseEvent(QKeyEvent *event)
{
  switch (event->key()) {
    case Qt::Key_Tab:
      emit keyTabReleased();
      break;
    }
}
