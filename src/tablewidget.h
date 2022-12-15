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
  void buildGraph(QVector<QVector<double>>);
  void buildBars(QVector<QVector<double>>);
  void buildCurve(QVector<QVector<double>>);
  void saveTable();
  void setEnabledActions(bool);

private slots:
  void customMenuRequested(const QAbstractItemView *view, const QPoint &pos);
  void customHeaderMenuRequested(const QAbstractItemView *view, const QPoint &pos);
  void checkSelectionModel();
  void slot_setEnabledActions(bool e);
  void buildGraphTrigered();
  void buildBarsTrigered();
  void buildCurveTrigered();
  void saveTableTrigered();
  void addColumnTrigered();

  QVector<QVector<double>> builderData(QModelIndexList &selectedIndexes);
  QVector<QVector<double>> builderNumberedData(QModelIndexList &selectedIndexes);

private:
  TableView *tableWidget = nullptr;
  QGridLayout *gridLayout = nullptr;
  QMenu *contextMenu = nullptr;
  QMenu *menuPlot= nullptr;
  QAction *actionPlotGraph = nullptr;
  QAction *actionPlotBars = nullptr;
  QAction *actionPlotCurve = nullptr;
  QAction *actionSaveTable = nullptr;
  QAction *actionSaveSelectedTable = nullptr;
  QMenu *hHeaderContextMenu = nullptr;
  QAction *actionAddColumn = nullptr;
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
  QVector<QVector<double>> builderData(QModelIndexList &selectedIndexes);
  QVector<QVector<double>> builderNumberedData(QModelIndexList &selectedIndexes);

  HorizontalHeaderView *horizontalHeader();

signals:
//    void keyEnterReleased();
  void error(QString);
  void warning(QString);
  void message(QString);
  void setEnabledActions(bool);

  void customContextMenuRequested(const QAbstractItemView *view, const QPoint &pos);

private slots:
  void customHeaderMenuRequested(const QPoint &pos);

private:
  void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);

private:
  HorizontalHeaderView *horizontalHeaderView = nullptr;
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
  bool pressMiddleButton = 0;
  int firstPos;
  int lastPos;
  QLabel *sectionIndicator = nullptr;
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
