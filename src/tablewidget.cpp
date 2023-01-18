#include "tablewidget.h"
#include "qclipboard.h"

#include <QIcon>
#include <QPainter>
#include <QApplication>

#ifdef QT_DEBUG
#include <QDebug>
#endif

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
//  if (actionSaveTable)
//    delete actionSaveTable;
//  if (actionSaveSelectedTable)
//    delete actionSaveSelectedTable;

  if (actionAddColumn)
    delete actionAddColumn;
  if (actionRemoveColumn)
    delete actionRemoveColumn;

//  if (actionCut)
//    delete actionCut;
//  if (actionCopy)
//    delete actionCopy;
//  if (actionPaste)
//    delete actionPaste;
//  if (actionDelete)
//    delete actionDelete;

//  if (contextMenu)
//    delete contextMenu;
//  if (hHeaderContextMenu)
//    delete hHeaderContextMenu;

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

  connect(tableWidget, &TableView::customContextMenuRequested, this, &TableWidget::customMenuRequested);
  connect(tableWidget->horizontalHeader(), &HorizontalHeaderView::customContextMenuRequested, this, &TableWidget::customHeaderMenuRequested);
  connect(tableWidget, &TableView::saveTable, this, &TableWidget::saveTableTrigered);

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

//  actionSaveTable = new QAction;
//  actionSaveTable->setText(tr("Save table"));
//  actionSaveTable->setIcon(QIcon(":/img/save.png"));
//  connect(actionSaveTable, &QAction::triggered, this, &TableWidget::saveTrigered);

//  actionSaveSelectedTable = new QAction;
//  actionSaveSelectedTable->setText(tr("Save selected table"));
//  actionSaveSelectedTable->setIcon(QIcon(":/img/save.png"));
//  connect(actionSaveSelectedTable, &QAction::triggered, this, &TableWidget::saveTableTrigered);

//  actionCut = new QAction;
//  actionCut->setText(tr("Cut"));
//  actionCut->setIcon(QIcon(":/img/cut.png"));
//  connect(actionCut, &QAction::triggered, this, &TableWidget::cutTrigered);

//  actionCopy = new QAction;
//  actionCopy->setText(tr("Copy"));
//  actionCopy->setIcon(QIcon(":/img/copy.png"));
//  connect(actionCopy, &QAction::triggered, this, &TableWidget::copyTrigered);

//  actionPaste = new QAction;
//  actionPaste->setText(tr("Paste"));
//  actionPaste->setIcon(QIcon(":/img/paste.png"));
//  connect(actionPaste, &QAction::triggered, this, &TableWidget::pasteTrigered);

//  actionDelete = new QAction;
//  actionDelete->setText(tr("Delete"));
//  actionDelete->setIcon(QIcon(":/img/remove.png"));
//  connect(actionDelete, &QAction::triggered, this, &TableWidget::deleteTrigered);

//  contextMenu = new QMenu;
//  contextMenu->addMenu(menuPlot);
//  contextMenu->addSeparator();
//  contextMenu->addAction(actionCut);
//  contextMenu->addAction(actionCopy);
//  contextMenu->addAction(actionPaste);
//  contextMenu->addAction(actionDelete);
//  contextMenu->addSeparator();
//  contextMenu->addAction(actionSaveSelectedTable);
//  contextMenu->addAction(actionSaveTable);

  auto action{tableWidget->tableContextMenu()->actions().at(0)};
  tableWidget->tableContextMenu()->insertMenu(action, menuPlot);
  tableWidget->tableContextMenu()->insertSeparator(action);

  actionAddColumn = new QAction;
  actionAddColumn->setText(tr("Add column"));
  actionAddColumn->setIcon(QIcon(":/img/add_column.png"));
  connect(actionAddColumn, &QAction::triggered, this, &TableWidget::addColumnTrigered);

  actionRemoveColumn = new QAction;
  actionRemoveColumn->setText(tr("Remove column"));
  actionRemoveColumn->setIcon(QIcon(":/img/remove_column.png"));
  connect(actionRemoveColumn, &QAction::triggered, this, &TableWidget::removeColumnTrigered);

//  hHeaderContextMenu = new QMenu;
//  hHeaderContextMenu->addMenu(menuPlot);
//  hHeaderContextMenu->addSeparator();
//  hHeaderContextMenu->addAction(actionCut);
//  hHeaderContextMenu->addAction(actionCopy);
//  hHeaderContextMenu->addAction(actionPaste);
//  hHeaderContextMenu->addAction(actionDelete);
//  hHeaderContextMenu->addSeparator();
//  hHeaderContextMenu->addAction(actionAddColumn);
//  hHeaderContextMenu->addAction(actionRemoveColumn);
//  hHeaderContextMenu->addSeparator();
//  hHeaderContextMenu->addAction(actionSaveTable);

  action = tableWidget->headerContextMenu()->actions().at(0);
  tableWidget->headerContextMenu()->insertMenu(action, menuPlot);
  tableWidget->headerContextMenu()->insertSeparator(action);

  action = tableWidget->headerContextMenu()->actions().at(6);
  tableWidget->headerContextMenu()->insertSeparator(action);
  tableWidget->headerContextMenu()->insertActions(action, {actionAddColumn,
                                                           actionRemoveColumn});
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
  if (tableWidget->hasSelection()) {
      menuPlot->setEnabled(true);
//      actionCut->setEnabled(true);
//      actionCopy->setEnabled(true);
//      actionPaste->setEnabled(true);
//      actionDelete->setEnabled(true);
    }
  else {
      menuPlot->setEnabled(false);
//      actionCut->setEnabled(false);
//      actionCopy->setEnabled(false);
//      actionPaste->setEnabled(false);
//      actionDelete->setEnabled(false);
    }

//  contextMenu->popup(view->viewport()->mapToGlobal(pos));
//  tableWidget->tableContextMenu()->popup(view->viewport()->mapToGlobal(pos));
}

void TableWidget::customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos)
{
  if (tableWidget->hasSelection()) {
      menuPlot->setEnabled(true);
    }
  else {
      menuPlot->setEnabled(false);
    }

  if (tableWidget->hasSelectedColumn()) {
//      actionCut->setEnabled(true);
//      actionCopy->setEnabled(true);
//      actionPaste->setEnabled(true);
//      actionDelete->setEnabled(true);
      actionRemoveColumn->setEnabled(true);
    }
  else {
//      actionCut->setEnabled(false);
//      actionCopy->setEnabled(false);
//      actionPaste->setEnabled(false);
//      actionDelete->setEnabled(false);
      actionRemoveColumn->setEnabled(false);
    }

//  hHeaderContextMenu->popup(view->viewport()->mapToGlobal(pos));
//  tableWidget->headerContextMenu()->popup(view->viewport()->mapToGlobal(pos));
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

void TableWidget::saveTableTrigered(QCPPlotting::Sheet sheet)
{

  QCPPlotting::SpreadSheet spreadSheet;
  spreadSheet.insert(windowTitle(), sheet);

  emit saveTable(spreadSheet);
}

void TableWidget::addColumnTrigered()
{
  tableWidget->model()->insertColumn(tableWidget->model()->columnCount());
}

void TableWidget::removeColumnTrigered()
{
  //  tableWidget->model()->removeColumn();
}

//void TableWidget::cutTrigered()
//{
//  tableWidget->CutToClipboard();
//}

//void TableWidget::copyTrigered()
//{
//  tableWidget->CopyToClipboard();
//}

//void TableWidget::pasteTrigered()
//{
//  tableWidget->PasteFromClipboard();
//}

//void TableWidget::deleteTrigered()
//{
//  tableWidget->DeleteDataKey();
//}

//QVector<QVector<double>> TableWidget::builderData(QModelIndexList &selectedIndexes)
QCPPlotting::NumSheet TableWidget::builderData(QModelIndexList &selectedIndexes)
{
  return tableWidget->builderData(selectedIndexes);
}

//QVector<QVector<double>> TableWidget::builderNumberedData(QModelIndexList &selectedIndexes)
QCPPlotting::NumSheet TableWidget::builderNumberedData(QModelIndexList &selectedIndexes)
{
  return tableWidget->builderNumberedData(selectedIndexes);
}


TableView::TableView(QWidget *parent)
  : QTableView{parent}
{
  horizontalHeaderView = new HorizontalHeaderView;
  setHorizontalHeader(horizontalHeaderView);
  connect(horizontalHeaderView, &HorizontalHeaderView::customContextMenuRequested, this, &TableView::customHeaderMenuRequested);
//  setEditTriggers(editTriggers() | QAbstractItemView::AnyKeyPressed);

  setEditTriggers(QAbstractItemView::DoubleClicked |
                  QAbstractItemView::SelectedClicked |
                  QAbstractItemView::EditKeyPressed |
                  QAbstractItemView::AnyKeyPressed);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QTableView::customContextMenuRequested, this, &TableView::customMenuRequested);

  actionSaveTable = new QAction;
  actionSaveTable->setText(tr("Save table"));
  actionSaveTable->setIcon(QIcon(":/img/save.png"));
  connect(actionSaveTable, &QAction::triggered, this, &TableView::saveTableTrigered);

  actionSaveSelectedTable = new QAction;
  actionSaveSelectedTable->setText(tr("Save selected table"));
  actionSaveSelectedTable->setIcon(QIcon(":/img/save.png"));
  connect(actionSaveSelectedTable, &QAction::triggered, this, &TableView::saveTrigered);

  actionCut = new QAction;
  actionCut->setText(tr("Cut"));
  actionCut->setIcon(QIcon(":/img/cut.png"));
  connect(actionCut, &QAction::triggered, this, &TableView::cutTrigered);

  actionCopy = new QAction;
  actionCopy->setText(tr("Copy"));
  actionCopy->setIcon(QIcon(":/img/copy.png"));
  connect(actionCopy, &QAction::triggered, this, &TableView::copyTrigered);

  actionPaste = new QAction;
  actionPaste->setText(tr("Paste"));
  actionPaste->setIcon(QIcon(":/img/paste.png"));
  connect(actionPaste, &QAction::triggered, this, &TableView::pasteTrigered);

  actionDelete = new QAction;
  actionDelete->setText(tr("Delete"));
  actionDelete->setIcon(QIcon(":/img/remove.png"));
  connect(actionDelete, &QAction::triggered, this, &TableView::deleteTrigered);

  contextMenu = new QMenu;
  contextMenu->addAction(actionCut);
  contextMenu->addAction(actionCopy);
  contextMenu->addAction(actionPaste);
  contextMenu->addAction(actionDelete);
  contextMenu->addSeparator();
  contextMenu->addAction(actionSaveSelectedTable);
  contextMenu->addAction(actionSaveTable);

  hHeaderContextMenu = new QMenu;
  hHeaderContextMenu->addAction(actionCut);
  hHeaderContextMenu->addAction(actionCopy);
  hHeaderContextMenu->addAction(actionPaste);
  hHeaderContextMenu->addAction(actionDelete);
  hHeaderContextMenu->addSeparator();
  hHeaderContextMenu->addAction(actionSaveTable);
}

TableView::~TableView()
{
  if (actionSaveTable)
    delete actionSaveTable;
  if (actionSaveSelectedTable)
    delete actionSaveSelectedTable;

  if (actionCut)
    delete actionCut;
  if (actionCopy)
    delete actionCopy;
  if (actionPaste)
    delete actionPaste;
  if (actionDelete)
    delete actionDelete;

  if (contextMenu)
    delete contextMenu;
  if (hHeaderContextMenu)
    delete hHeaderContextMenu;

  if (horizontalHeaderView)
    delete horizontalHeaderView;
}

void TableView::PasteFromClipboard()
{
  auto current_row{selectionModel()->currentIndex().row()};
  auto current_column{selectionModel()->currentIndex().column()};

  auto rows{QApplication::clipboard()->text().split('\n')};

   for( int i{0}; i < rows.size(); i++) {
       auto cols{rows.at(i).split('\t')};
       for( int j{0}; j < cols.size(); j++ ) {
           auto item = new QStandardItem(cols.at(j));
           qobject_cast<QStandardItemModel *>(model())->setItem(current_row + i, current_column + j, item);
         }
     }
}

void TableView::CopyToClipboard()
{
  QString cbStr;
  auto cb{QApplication::clipboard()};
  auto list{selectionModel()->selectedIndexes()};

   if(list.isEmpty())
     return;

   if (list.at(0).column() == list.at(1).column()) {
       auto firstRow{list.first().row()};
       auto lastRow{list.last().row()};
       auto rowCount{lastRow - firstRow + 1};

       for(int i{0}; i < rowCount; ++i, cbStr += '\n') {
           for(int j{i}; j < list.count(); j += rowCount, cbStr += '\t') {
               cbStr += model()->data(list[j], Qt::EditRole).toString();
             }
         }
     }
   else {
       auto firstColumn{list.first().column()};
       auto lastColumn{list.last().column()};
       auto columnCount{lastColumn - firstColumn + 1};

       for(int i{0}; i < list.count(); i++) {
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

  for (int i{0}; i < selectedIndexes.size(); i++) {
      auto r{selectedIndexes.at(i).row()};
      auto c{selectedIndexes.at(i).column()};
      qobject_cast<QStandardItemModel *>(model())->setItem(r, c, new QStandardItem(""));
    }
}

//QVector<QVector<double>> TableView::builderData(QModelIndexList &selectedIndexes)
QCPPlotting::NumSheet TableView::builderData(QModelIndexList &selectedIndexes)
{
  auto selectedIndexesCount{selectedIndexes.count()};
  QList<int> lst;
  for (int i{0}; i < selectedIndexesCount; i++) {
      auto c{selectedIndexes.at(i).column()};
      if (!lst.contains(c))
        lst.append(c);
    }
  auto columns{lst.count()};
//  lst.clear();

//  QVector<QVector<double>> data;
  QCPPlotting::NumSheet sheet;

  QStringList headerList;
  for (auto i : lst) {
      headerList << model()->headerData(i, Qt::Horizontal).toString();
    }
  lst.clear();
  sheet.HorizontalHeader = headerList;

  for (int c = 0; c < columns; c++) {
//      QVector<double> vector;
      QCPPlotting::NumColumn column;
      if (selectedIndexes.at(0).column() != selectedIndexes.at(1).column()) {
          for (int r{c}; r < selectedIndexesCount; r += columns) {
              bool ok;
              auto d{selectedIndexes.at(r).data().toDouble(&ok)};
              if (ok) {
//                vector.push_back(d);
                  column.append(d);
                }
            }
        }
      else {
          for (int r{selectedIndexesCount / columns * c}; r < (selectedIndexesCount / columns * (c + 1)); r++) {
              bool ok;
              auto d{selectedIndexes.at(r).data().toDouble(&ok)};
              if (ok) {
//                vector.push_back(d);
                  column.append(d);
                }
            }
        }
//      data.push_back(vector);
      sheet.append(column);
    }

//  return data;
  return sheet;
}

//QVector<QVector<double>> TableView::builderNumberedData(QModelIndexList &selectedIndexes)
QCPPlotting::NumSheet TableView::builderNumberedData(QModelIndexList &selectedIndexes)
{
//  auto data = builderData(selectedIndexes);
  auto sheet{builderData(selectedIndexes)};

//  QVector<double> vector;
  QCPPlotting::NumColumn row;
//  for (int i = 0; i < data.at(0).count(); i++) {
  for (int i{0}; i < sheet.at(0).count(); i++) {
//      vector.push_back(i);
      row.append(i);
    }
//  data.push_front(vector);
  sheet.prepend(row);

//  return data;
  return sheet;
}

HorizontalHeaderView *TableView::horizontalHeader()
{
  return horizontalHeaderView;
}

QList<int> TableView::selectedRanges()
{
  auto ranges{selectionModel()->selection()};
  auto rangesCount{ranges.size()};
  QList<int> result;
  for (int i{0}; i < rangesCount; ++i) {
      result.append({ranges.at(i).top(),
                     ranges.at(i).left(),
                     ranges.at(i).bottom(),
                     ranges.at(i).right()
                    });
    }
  return result;
}

bool TableView::hasSelection()
{
  return selectionModel()->hasSelection();
}

bool TableView::hasSelectedColumn()
{
  auto rc{model()->rowCount()};
  auto ranges{selectedRanges()};

  if (ranges.isEmpty())
    return false;

  auto s{ranges.size() / 4};

  if (ranges.at(2) - ranges.at(0) != rc - 1)
    return false;

  return true;
}

QMenu *TableView::tableContextMenu()
{
  return contextMenu;
}

QMenu *TableView::headerContextMenu()
{
  return hHeaderContextMenu;
}

void TableView::customMenuRequested(const QPoint &pos) {
  if (hasSelection()) {
      actionCut->setEnabled(true);
      actionCopy->setEnabled(true);
      actionPaste->setEnabled(true);
      actionDelete->setEnabled(true);
    }
  else {
      actionCut->setEnabled(false);
      actionCopy->setEnabled(false);
      actionPaste->setEnabled(false);
      actionDelete->setEnabled(false);
    }

  emit customContextMenuRequested(this, pos);

  contextMenu->popup(viewport()->mapToGlobal(pos));
}

void TableView::customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos)
{
  if (hasSelectedColumn()) {
      actionCut->setEnabled(true);
      actionCopy->setEnabled(true);
      actionPaste->setEnabled(true);
      actionDelete->setEnabled(true);
    }
  else {
      actionCut->setEnabled(false);
      actionCopy->setEnabled(false);
      actionPaste->setEnabled(false);
      actionDelete->setEnabled(false);
    }

  hHeaderContextMenu->popup(view->viewport()->mapToGlobal(pos));
}

void TableView::saveTableTrigered()
{
  QCPPlotting::Sheet sheet;

  QStringList headerList;
  for (int c = 0; c < model()->columnCount(); c++) {
      headerList << model()->headerData(c, Qt::Horizontal).toString();
    }
  sheet.HorizontalHeader = headerList;

  for (int r = 0; r < model()->rowCount(); r++) {
      QCPPlotting::Row row;
      for (int c = 0; c < model()->columnCount(); c++) {
          row.append(model()->data(model()->index(r, c)).toString());
        }
      sheet.append(row);
    }

  emit saveTable(sheet);
}

void TableView::saveTrigered()
{
  auto selectedIndexesCount{selectionModel()->selectedIndexes().count()};
  QList<int> lst;
  for (int i{0}; i < selectedIndexesCount; i++) {
      auto c{selectionModel()->selectedIndexes().at(i).column()};
      if (!lst.contains(c))
        lst.append(c);
    }
  auto columns{lst.count()};

  QCPPlotting::Sheet sheet;

  QStringList headerList;
  for (auto i : lst) {
      headerList << model()->headerData(i, Qt::Horizontal).toString();
    }
  lst.clear();
  sheet.HorizontalHeader = headerList;

  auto rows = selectedIndexesCount / columns;

  if (selectionModel()->selectedIndexes().at(0).column() != selectionModel()->selectedIndexes().at(1).column()) {
      for (int r = 0; r < rows; r++) {
          QCPPlotting::Row row;
          for (int c{columns * r}; c < (columns * (r + 1)); c++) {
              row.append(selectionModel()->selectedIndexes().at(c).data().toString());
            }
          sheet.append(row);
        }
    }
  else {
      for (int r = 0; r < rows; r++) {
          QCPPlotting::Row row;
          for (int c{r}; c < selectedIndexesCount; c += rows) {
              row.append(selectionModel()->selectedIndexes().at(c).data().toString());
            }
          sheet.append(row);
        }
    }

  emit saveTable(sheet);
}

void TableView::cutTrigered()
{
  CutToClipboard();
}

void TableView::copyTrigered()
{
  CopyToClipboard();
}

void TableView::pasteTrigered()
{
  PasteFromClipboard();
}

void TableView::deleteTrigered()
{
  DeleteDataKey();
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
  : QHeaderView{Qt::Horizontal, parent}
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

      int section{logicalIndexAt(firstPos)};
      int w{sectionSize(section)};
      int h{viewport()->height()};
      int p{sectionViewportPosition(section)};

      sectionIndicator->resize(w, h);

      const qreal pixmapDevicePixelRatio = static_cast<qreal>(devicePixelRatio());
      QPixmap pm{QSize(w, h) * pixmapDevicePixelRatio};
      pm.setDevicePixelRatio(pixmapDevicePixelRatio);
      pm.fill(QColor(0, 0, 0, 45));
      QRect rect{0, 0, w, h};
      QPainter painter{&pm};
      const QVariant variant{model()->headerData(section, Qt::Horizontal, Qt::FontRole)};
      if (variant.isValid() && variant.canConvert<QFont>()) {
          const QFont sectionFont = qvariant_cast<QFont>(variant);
          painter.setFont(sectionFont);
        }
      else {
          painter.setFont(font());
        }
      painter.setOpacity(0.8);
      paintSection(&painter, rect, section);
      painter.end();

      sectionIndicator->setPixmap(pm);
      sectionIndicatorOffset = firstPos - qMax(p, 0);
    }

  QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent *event)
{
  if (pressMiddleButton) {
      int pos{event->pos().x()};
      if (pos < 0)
        return;

      int visual{visualIndexAt(pos)};
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
      int from{visualIndexAt(firstPos)};
      int to{visualIndexAt(lastPos)};
      if (from != -1 && to != -1)
        moveSection(from, to);

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
