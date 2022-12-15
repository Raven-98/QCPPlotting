#include "tablewidget.h"
#include "qclipboard.h"

#include <QIcon>
#include <QPainter>
#include <QApplication>

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
  if (actionPlotBars)
    delete actionPlotBars;
  if (actionPlotCurve)
    delete actionPlotCurve;
  if (menuPlot)
    delete menuPlot;
  if (actionSaveTable)
    delete actionSaveTable;
  if (actionSaveSelectedTable)
    delete actionSaveSelectedTable;
  if (contextMenu)
    delete contextMenu;

  if (hHeaderContextMenu)
    delete hHeaderContextMenu;
  if (actionAddColumn)
    delete actionAddColumn;

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

  tableWidget = new TableView;

  gridLayout = new QGridLayout;
  gridLayout->addWidget(tableWidget, 0, 0);
  gridLayout->setContentsMargins(0, 0, 0, 0);

  setLayout(gridLayout);

  tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(tableWidget, &QTableView::customContextMenuRequested, this, &TableWidget::checkSelectionModel);
  connect(this, &TableWidget::setEnabledActions, this, &TableWidget::slot_setEnabledActions);
  connect(tableWidget, &TableView::customContextMenuRequested, this, &TableWidget::customMenuRequested);
  connect(tableWidget->horizontalHeader(), &HorizontalHeaderView::customContextMenuRequested, this, &TableWidget::customHeaderMenuRequested);

  actionPlotGraph = new QAction;
  actionPlotGraph->setText(tr("Graph"));
  connect(actionPlotGraph, &QAction::triggered, this, &TableWidget::buildGraphTrigered);

  actionPlotBars = new QAction;
  actionPlotBars->setText(tr("Bars"));
  connect(actionPlotBars, &QAction::triggered, this, &TableWidget::buildBarsTrigered);

  actionPlotCurve = new QAction;
  actionPlotCurve->setText(tr("Curve"));
  connect(actionPlotCurve, &QAction::triggered, this, &TableWidget::buildCurveTrigered);

  menuPlot = new QMenu;
  menuPlot->setTitle(tr("Plot"));
  menuPlot->addAction(actionPlotBars);
  menuPlot->addAction(actionPlotCurve);
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

  actionAddColumn = new QAction;
  actionAddColumn->setText(tr("Add column"));
  actionAddColumn->setIcon(QIcon(":/img/add_column.png"));
  connect(actionAddColumn, &QAction::triggered, this, &TableWidget::addColumnTrigered);

  hHeaderContextMenu = new QMenu;
  hHeaderContextMenu->addMenu(menuPlot);
  hHeaderContextMenu->addSeparator();
  hHeaderContextMenu->addAction(actionAddColumn);
  hHeaderContextMenu->addSeparator();
  hHeaderContextMenu->addAction(actionSaveTable);
}

void TableWidget::setModel(QStandardItemModel *item_model)
{
  tableWidget->setModel(item_model);
}

QAbstractItemModel *TableWidget::model()
{
  return tableWidget->model();
}

void TableWidget::customMenuRequested(const QAbstractItemView *view, const QPoint &pos)
{
  contextMenu->popup(view->viewport()->mapToGlobal(pos));
}

void TableWidget::customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos)
{
  hHeaderContextMenu->popup(view->viewport()->mapToGlobal(pos));
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

void TableWidget::buildBarsTrigered()
{
  auto selectedIndexes = tableWidget->selectionModel()->selectedIndexes();
  if (selectedIndexes.size() < 2) {
      emit error("selectedIndexes.size() < 2");
      return;
    }

  auto data = builderData(selectedIndexes);

  emit buildBars(data);
}

void TableWidget::buildCurveTrigered()
{
  auto selectedIndexes = tableWidget->selectionModel()->selectedIndexes();
  if (selectedIndexes.size() < 2) {
      emit error("selectedIndexes.size() < 2");
      return;
    }

  auto data = builderNumberedData(selectedIndexes);

  emit buildCurve(data);
}

void TableWidget::saveTableTrigered()
{
}

void TableWidget::addColumnTrigered()
{
  tableWidget->model()->insertColumn(tableWidget->model()->columnCount());
}

/* ******************************************************************************** *
 * !!!
 * Елементи заповнюються по черзі вибору.                                           *
 * !!!
 *                                                                                  *
 * Тобто якщо вибирати елементи у таблиці з парва на ліво то вони так і занесуться  *
 * до вектора.                                                                      *
 *                                                                                  *
 * Причиною є використання QModelIndexList.                                         *
 * ******************************************************************************** */
QVector<QVector<double> > TableWidget::builderData(QModelIndexList &selectedIndexes)
{
  return tableWidget->builderData(selectedIndexes);
}

QVector<QVector<double>> TableWidget::builderNumberedData(QModelIndexList &selectedIndexes)
{
  return tableWidget->builderNumberedData(selectedIndexes);
}


TableView::TableView(QWidget *parent)
  : QTableView(parent)
{
  horizontalHeaderView = new HorizontalHeaderView;
  setHorizontalHeader(horizontalHeaderView);
  connect(this, &QTableView::customContextMenuRequested, this, &TableView::customHeaderMenuRequested);
}

TableView::~TableView()
{
  if (horizontalHeaderView)
    delete horizontalHeaderView;
}

void TableView::PasteFromClipboard()
{
  auto current_row = selectionModel()->currentIndex().row();
  auto current_column = selectionModel()->currentIndex().column();

  auto rows = QApplication::clipboard()->text().split('\n');

   for( int i = 0; i < rows.size(); i++ ) {
       auto cols = rows.at(i).split('\t');
       for( int j = 0; j < cols.size(); j++ ) {
           auto item = new QStandardItem(cols.at(j));
           qobject_cast<QStandardItemModel *>(model())->setItem(current_row + i, current_column + j, item);
         }
     }
}

void TableView::CopyToClipboard()
{
  QString cbStr;
  auto cb = QApplication::clipboard();
  auto list =  selectionModel()->selectedIndexes();

   if(list.isEmpty())
     return;

   if (list.at(0).column() == list.at(1).column()) {
       auto firstRow = list.first().row();
       auto lastRow = list.last().row();
       auto rowCount = lastRow - firstRow + 1;

       for(int i = 0; i < rowCount; ++i, cbStr += '\n') {
           for(int j = i; j < list.count(); j += rowCount, cbStr += '\t') {
               cbStr += model()->data(list[j], Qt::EditRole).toString();
             }
         }
     }
   else {
       auto firstColumn = list.first().column();
       auto lastColumn = list.last().column();
       auto columnCount = lastColumn - firstColumn + 1;

       for(int i = 0; i < list.count(); i++) {
           cbStr += model()->data(list[i], Qt::EditRole).toString();
           cbStr += ((i + 1) % columnCount == 0) ? "\n" : "\t";
         }
     }

   cb->setText(cbStr);
}

void TableView::CutToClipboard()
{
  CopyToClipboard();
  DeleteDataKey();
}

void TableView::DeleteDataKey()
{
  auto selectedIndexes = selectionModel()->selectedIndexes();

  for (int i = 0; i < selectedIndexes.size(); i++) {
      auto r = selectedIndexes.at(i).row();
      auto c = selectedIndexes.at(i).column();
      qobject_cast<QStandardItemModel *>(model())->setItem(r, c, new QStandardItem(""));
    }
}

/* ******************************************************************************** *
 * !!!
 * Елементи заповнюються по черзі вибору.                                           *
 * !!!
 *                                                                                  *
 * Тобто якщо вибирати елементи у таблиці з парва на ліво то вони так і занесуться  *
 * до вектора.                                                                      *
 *                                                                                  *
 * Причиною є використання QModelIndexList.                                         *
 * ******************************************************************************** */
QVector<QVector<double>> TableView::builderData(QModelIndexList &selectedIndexes)
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

QVector<QVector<double>> TableView::builderNumberedData(QModelIndexList &selectedIndexes)
{
  auto data = builderData(selectedIndexes);

  QVector<double> vector;
  for (int i = 0; i < data.at(0).count(); i++) {
      vector.push_back(i);
    }
  data.push_front(vector);

  return data;
}

HorizontalHeaderView *TableView::horizontalHeader()
{
  return horizontalHeaderView;
}

void TableView::customHeaderMenuRequested(const QPoint &pos) {
  emit customContextMenuRequested(this, pos);
}

void TableView::keyPressEvent(QKeyEvent *event)
{
  switch (event->modifiers()) {
    case Qt::CTRL:
      switch (event->key()) {
        case Qt::Key_V:
          PasteFromClipboard();
          break;
        case Qt::Key_C:
          CopyToClipboard();
          break;
        case Qt::Key_X:
          CutToClipboard();
          break;
        }
      break;
    default:
      switch (event->key()) {
        case Qt::Key_Delete:
          DeleteDataKey();
          break;
        }
    }
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
}

HorizontalHeaderView::~HorizontalHeaderView()
{
  if (sectionIndicator)
    delete sectionIndicator;
}

void HorizontalHeaderView::customHeaderMenuRequested(const QPoint &pos)
{
  emit customContextMenuRequested(this, pos);
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
