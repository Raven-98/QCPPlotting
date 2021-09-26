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

    QTableView *tableWidget;
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
    void slot_NexCell();
    void slot_NexCell_Column();
    void slot_NexCell_Row();
    void slot_AddColumn();
    void customHeaderMenuRequested_table(const QPoint &pos);
    void customHeaderMenuRequested_header(const QPoint &pos);
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
