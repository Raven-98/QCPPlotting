#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QGridLayout>
#include <QStandardItemModel>
#include <QLabel>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include "structs.h"

class TableWidget;
class TableView;
class HorizontalHeaderView;
class TableStyledItemDelegate;
class TableLineEdit;

class TableWidget : public QWidget
{
  Q_OBJECT
public:
  explicit TableWidget(QWidget *parent = nullptr);
  ~TableWidget();

  void init();
  void setModel(QStandardItemModel *item_model);
  QAbstractItemModel *model();

public:
  static int SumTables;
  static int ResTableWidgetID;

signals:
  void error(QString);
  void warning(QString);
  void message(QString);
//  void buildGraph(QVector<QVector<double>>);
  void buildGraph(QCPPlotting::NumSheet);
//  void buildBars(QVector<QVector<double>>);
  void buildBars(QCPPlotting::NumSheet);
//  void buildCurve(QVector<QVector<double>>);
  void buildCurve(QCPPlotting::NumSheet);
  void saveTable();

private slots:
  void customMenuRequested(const QAbstractItemView *view, const QPoint &pos);
  void customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos);
  void buildGraphTrigered();
  void buildBarsTrigered();
  void buildCurveTrigered();
//  void saveTableTrigered();
//  void saveTrigered();
  void addColumnTrigered();
  void removeColumnTrigered();
//  void cutTrigered();
//  void copyTrigered();
//  void pasteTrigered();
//  void deleteTrigered();

//  QVector<QVector<double>> builderData(QModelIndexList &selectedIndexes);
  QCPPlotting::NumSheet builderData(QModelIndexList &selectedIndexes);
//  QVector<QVector<double>> builderNumberedData(QModelIndexList &selectedIndexes);
  QCPPlotting::NumSheet builderNumberedData(QModelIndexList &selectedIndexes);

private:
  TableView *tableWidget{nullptr};
  QGridLayout *gridLayout{nullptr};
//  QMenu *contextMenu{nullptr};
  QMenu *menuPlot{nullptr};
  QAction *actionPlotGraph{nullptr};
  QAction *actionPlotBars{nullptr};
  QAction *actionPlotCurve{nullptr};
//  QAction *actionSaveTable{nullptr};
//  QAction *actionSaveSelectedTable{nullptr};
//  QMenu *hHeaderContextMenu{nullptr};
  QAction *actionAddColumn{nullptr};
  QAction *actionRemoveColumn{nullptr};
//  QAction *actionCut{nullptr};
//  QAction *actionCopy{nullptr};
//  QAction *actionPaste{nullptr};
//  QAction *actionDelete{nullptr};
};

class TableView : public QTableView
{
    Q_OBJECT
public:
  TableView(QWidget *parent = nullptr);
  ~TableView();

  void PasteFromClipboard();
  void CopyToClipboard();
  void CutToClipboard();
  void DeleteDataKey();
//  QVector<QVector<double>> builderData(QModelIndexList &selectedIndexes);
  QCPPlotting::NumSheet builderData(QModelIndexList &selectedIndexes);
//  QVector<QVector<double>> builderNumberedData(QModelIndexList &selectedIndexes);
  QCPPlotting::NumSheet builderNumberedData(QModelIndexList &selectedIndexes);

  HorizontalHeaderView *horizontalHeader();

  QList<int> selectedRanges();
  bool hasSelection();
  bool hasSelectedColumn();

  QMenu *tableContextMenu();
  QMenu *headerContextMenu();

signals:
//    void keyEnterReleased();
  void error(QString);
  void warning(QString);
  void message(QString);
  void setEnabledActions(bool);
  void customContextMenuRequested(const QAbstractItemView *view, const QPoint &pos);

private slots:
  void customMenuRequested(const QPoint &pos);
  void customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos);
  void saveTableTrigered();
  void saveTrigered();
  void cutTrigered();
  void copyTrigered();
  void pasteTrigered();
  void deleteTrigered();

private:
  void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);

private:
  HorizontalHeaderView *horizontalHeaderView{nullptr};
  QMenu *contextMenu{nullptr};
  QAction *actionSaveTable{nullptr};
  QAction *actionSaveSelectedTable{nullptr};
  QMenu *hHeaderContextMenu{nullptr};
  QAction *actionCut{nullptr};
  QAction *actionCopy{nullptr};
  QAction *actionPaste{nullptr};
  QAction *actionDelete{nullptr};
};

class HorizontalHeaderView : public QHeaderView
{
  Q_OBJECT
public:
  explicit HorizontalHeaderView(QWidget *parent = nullptr);
  ~HorizontalHeaderView();


signals:
  void customContextMenuRequested(const QAbstractItemView *view, const QPoint &pos);

private slots:
  void customHeaderMenuRequested(const QPoint &pos);

private:
  bool pressMiddleButton{false};
  int firstPos;
  int lastPos;
  QLabel *sectionIndicator{nullptr};
  int sectionIndicatorOffset;

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
};

class TableStyledItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;

signals:
    void keyTabReleased();
};

class TableLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    using QLineEdit::QLineEdit;

signals:
    void keyTabReleased();

protected:
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // TABLEWIDGET_H
