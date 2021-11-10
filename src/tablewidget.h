#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QApplication>
#include <QClipboard>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QGridLayout>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

#include <QLineEdit>
#include <QStyledItemDelegate>

class TableWidget;
class TableView;
class TableLineEdit;
class TableStyledItemDelegate;
class HeaderView;

class TableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableWidget(QWidget *parent = nullptr);
    ~TableWidget();
    void setModel(QStandardItemModel *item_model);
    QVector<QVector<double> > getData();

    static int SumTables;
    static int ResTableWidgetID;

//    TableView *tableWidget;
    QAction *actionBuild_Graph;
    QAction *actionBuild_Curve;
    QAction *actionBuild_Bars;
    QAction *actionBuild_Financial;
    QAction *actionBuild_StatistacalBox;
    QAction *action_AddTo_Graph;
    QAction *action_SaveTable;

private:
    void keyPressEvent(QKeyEvent *event);
    void PasteFromClipboard();
    void CopyToClipboard();
    void DeleteDataKey();

    TableView *tableWidget;
    QGridLayout *gridLayout;
    QMenu *hHeaderMenu;
    QMenu *tableMenu;
    QMenu *tableMenu_Plot;
    QMenu *menu_Graph;
    QMenu *menu_Curve;
    QMenu *menu_Bars;
    QMenu *menu_Financial;
    QMenu *menu_StatistacalBox;
    QHeaderView *horizontalHeaderView;
    QAction *action_AddColumn;
    bool Changed = true;

private slots:
    void slot_Changed();
    void slot_NexCellInColumn();
    void slot_NexCellInRow();
    void slot_AddColumn();
    void customHeaderMenuRequested_table(const QPoint &pos);
    void customHeaderMenuRequested_header(const QPoint &pos);
};

class TableView : public QTableView
{
    Q_OBJECT
public:
    using QTableView::QTableView;

signals:
    void keyEnterReleased();
    void keyTabReleased();

protected:
    void keyReleaseEvent(QKeyEvent *event);
};

class TableLineEdit: public QLineEdit
{
    Q_OBJECT
public:
    using QLineEdit::QLineEdit;
signals:
    void keyTabReleased();
    void keyEnterReleased();
protected:
    void keyReleaseEvent(QKeyEvent *event);
};

class TableStyledItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const;
signals:
    void keyTabReleased();
    void keyEnterReleased();
};

class HeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HeaderView(Qt::Orientation orientation,QWidget *parent = nullptr);
    ~HeaderView();

private:
//    Qt::Orientation ort;
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

#endif // TABLEWIDGET_H
