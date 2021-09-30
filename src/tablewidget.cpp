#include "tablewidget.h"

#include <QDebug>

int TableWidget::SumTables = 0;
int TableWidget::ResTableWidgetID = 0;

TableWidget::TableWidget(QWidget *parent)
    : QWidget(parent)
{
    SumTables++;
    ResTableWidgetID++;

    resize(275,400);
    setWindowIcon(QIcon(":/img/table.png"));
    setWindowTitle(tr("Table ")+QString::number(ResTableWidgetID));

    tableWidget = new QTableView;

    horizontalHeaderView = new HeaderView(Qt::Horizontal,this);
    tableWidget->setHorizontalHeader(horizontalHeaderView);

    gridLayout = new QGridLayout;
    gridLayout->addWidget(tableWidget, 0, 0);
    gridLayout->setContentsMargins(0,0,0,0);

    setLayout(gridLayout);

    actionBuild_Graph = new QAction(tr("Build"));
    actionBuild_Curve = new QAction(tr("Build"));
    actionBuild_Bars = new QAction(tr("Build"));
    actionBuild_Financial = new QAction(tr("Build"));
    actionBuild_StatistacalBox = new QAction(tr("Build"));

    action_AddTo_Graph = new QAction(tr("Add to"));

    menu_Graph = new QMenu(tr("Graph"));
    menu_Graph->addAction(actionBuild_Graph);
    menu_Graph->addAction(action_AddTo_Graph);

    menu_Curve = new QMenu(tr("Curve"));
    menu_Curve->addAction(actionBuild_Curve);
//    menu_Curve->addAction(action_AddTo);
    menu_Curve->setEnabled(false);

    menu_Bars = new QMenu(tr("Bars"));
    menu_Bars->addAction(actionBuild_Bars);
//    menu_Bars->addAction(action_AddTo);
    menu_Bars->setEnabled(false);

    menu_Financial = new QMenu(tr("Financial"));
    menu_Financial->addAction(actionBuild_Financial);
//    menu_Financial->addAction(action_AddTo);
    menu_Financial->setEnabled(false);

    menu_StatistacalBox = new QMenu(tr("Statistacal box"));
    menu_StatistacalBox->addAction(actionBuild_StatistacalBox);
//    menu_StatistacalBox->addAction(action_AddTo);
    menu_StatistacalBox->setEnabled(false);

    tableMenu_Plot = new QMenu;
    tableMenu_Plot->setTitle(tr("Plot"));
    tableMenu_Plot->addMenu(menu_Bars);
    tableMenu_Plot->addMenu(menu_Curve);
    tableMenu_Plot->addMenu(menu_Graph);
    tableMenu_Plot->addMenu(menu_Financial);
    tableMenu_Plot->addMenu(menu_StatistacalBox);

    action_AddColumn = new QAction(tr("Add column"));
    action_AddColumn->setIcon(QIcon(":/img/add_column.png"));
    connect(action_AddColumn,&QAction::triggered,this,&TableWidget::slot_AddColumn);

    action_SaveTable = new QAction(tr("Save Table"));
    action_SaveTable->setIcon(QIcon(":/img/save.png"));

    hHeaderMenu = new QMenu;
    hHeaderMenu->addMenu(tableMenu_Plot);
    hHeaderMenu->addSeparator();
    hHeaderMenu->addAction(action_AddColumn);
    hHeaderMenu->addSeparator();
    hHeaderMenu->addAction(action_SaveTable);

    tableMenu = new QMenu;
    tableMenu->addMenu(tableMenu_Plot);
    tableMenu->addSeparator();
    tableMenu->addAction(action_SaveTable);

    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget,&QTableView::customContextMenuRequested,this,&TableWidget::customHeaderMenuRequested_table);

    tableWidget->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget->horizontalHeader(),&QTableView::customContextMenuRequested,this,&TableWidget::customHeaderMenuRequested_header);
}

TableWidget::~TableWidget()
{
    SumTables--;
    if (SumTables == 0)
        ResTableWidgetID = 0;

    delete tableMenu_Plot;
    delete actionBuild_Graph;
    delete actionBuild_Curve;
    delete actionBuild_Bars;
    delete actionBuild_Financial;
    delete actionBuild_StatistacalBox;
    delete action_AddTo_Graph;
//    delete action_AddTo;
    delete menu_Graph;
    delete menu_Curve;
    delete menu_Bars;
    delete menu_Financial;
    delete menu_StatistacalBox;
    delete hHeaderMenu;
    delete tableMenu;
    delete horizontalHeaderView;
    delete tableWidget->model();
    delete tableWidget;
    delete gridLayout;
}

void TableWidget::setModel(QStandardItemModel *item_model)
{
    tableWidget->setModel(item_model);
    connect(item_model,&QStandardItemModel::itemChanged,this,&TableWidget::slot_NexCell);
}

QVector<QVector<double> > TableWidget::getData()
{
    QVector<QVector<double> > data;

    QModelIndexList *selectedIndexes = new QModelIndexList(tableWidget->selectionModel()->selectedIndexes());

    if (selectedIndexes->size() > 1)
    {
        int *selectedIndexesCount = new int(selectedIndexes->count());

        QList<int> lst;
        for (int i = 0; i < *selectedIndexesCount; i++)
        {
            int c = selectedIndexes->at(i).column();
            if (!lst.contains(c))
            {
                lst.append(c);
            }
        }
        int *columns = new int(lst.count());
        lst.clear();

        for (int column = 0; column < *columns; column++)
        {
            QVector<double> vector;
            if (selectedIndexes->at(0).column() != selectedIndexes->at(1).column())
            {
                for (int row = column; row < *selectedIndexesCount; row += *columns)
                {
                    bool ok;
                    double d = selectedIndexes->at(row).data().toDouble(&ok);
                    if (ok)
                        vector.push_back(d);
                }
            }
            else
            {
                for (int row = (*selectedIndexesCount / *columns * column); row < (*selectedIndexesCount / *columns * (column + 1)); row++)
                {
                    bool ok;
                    double d = selectedIndexes->at(row).data().toDouble(&ok);
                    if (ok)
                        vector.push_back(d);
                }
            }
            data.push_back(vector);
            vector.clear();
        }

        delete selectedIndexesCount;
        delete columns;
    }
    delete selectedIndexes;

    return data;
}

void TableWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->modifiers())
    {
    case Qt::CTRL:
        switch (event->key())
        {
        case Qt::Key_V:
            PasteFromClipboard();
            break;
        case Qt::Key_C:
            CopyToClipboard();
            break;
        }
        break;
    default:
        switch (event->key())
        {
        case Qt::Key_Enter:
            slot_NexCell();
            break;
        case Qt::Key_Return:
            slot_NexCell();
            break;
        case Qt::Key_Tab:
            slot_NexCell();
            break;
        case Qt::Key_Delete:
            DeleteDataKey();
            break;
        }
    }
//    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return || event->key() == Qt::Key_Tab)
//    {
//        slot_NexCell();
//    }
//    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_V)
//    {
//        PasteFromClipboard();
//    }
//    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_C)
//    {
//        CopyToClipboard();
//    }
//    if (event->key() == Qt::Key_Delete)
//    {
//        DeleteDataKey();
//    }
}

void TableWidget::PasteFromClipboard()
{
    int *current_row = new int(tableWidget->selectionModel()->currentIndex().row());
    int *current_column = new int(tableWidget->selectionModel()->currentIndex().column());

    QStringList rows = QApplication::clipboard()->text().split('\n');

    for( int i = 0; i < rows.size(); i++ )
    {
        QStringList cols = rows.at(i).split('\t');
        for( int j = 0; j < cols.size(); j++ )
        {
            QStandardItem *item = new QStandardItem(cols.at(j));
            item->setTextAlignment(Qt::AlignCenter);
            qobject_cast<QStandardItemModel *>(tableWidget->model())->setItem(*current_row + i, *current_column + j, item);
        }
    }

    delete current_row;
    delete current_column;
}

void TableWidget::CopyToClipboard()
{
    QString cbStr;
    QClipboard *cb = QApplication::clipboard();
    QModelIndexList list =  tableWidget->selectionModel()->selectedIndexes();

    if( list.isEmpty() ) return;

    if (list.at(0).column() == list.at(1).column())
    {
        int firstRow = list.first().row();
        int lastRow = list.last().row();
        int rowCount = lastRow - firstRow + 1;

        for(int i = 0; i < rowCount; ++i, cbStr += '\n')
            for(int j = i; j < list.count(); j += rowCount, cbStr += '\t')
                cbStr += tableWidget->model()->data(list[j], Qt::EditRole).toString();
    }
    else
    {
        int firstColumn = list.first().column();
        int lastColumn = list.last().column();
        int columnCount = lastColumn - firstColumn + 1;

        for(int i = 0; i < list.count(); i++)
        {
            cbStr += tableWidget->model()->data(list[i], Qt::EditRole).toString();
            if ((i + 1) % columnCount == 0)
            {
                cbStr += "\n";
            }
            else
                cbStr += "\t";
        }
    }

    cb->setText(cbStr);
}

void TableWidget::DeleteDataKey()
{
    QModelIndexList *selectedIndexes = new QModelIndexList(tableWidget->selectionModel()->selectedIndexes());

    Changed = false;
    for (int i = 0; i < selectedIndexes->size(); i++)
    {
        int r = selectedIndexes->at(i).row();
        int c = selectedIndexes->at(i).column();
        qobject_cast<QStandardItemModel *>(tableWidget->model())->setItem(r, c, new QStandardItem(""));
    }
    Changed = true;

    delete selectedIndexes;
}

void TableWidget::slot_NexCell()
{
    if (Changed)
    {
        int *current_row = new int(tableWidget->selectionModel()->currentIndex().row());
        int *current_column = new int(tableWidget->selectionModel()->currentIndex().column());
        QModelIndex *index = new QModelIndex(tableWidget->model()->index(*current_row,*current_column));


        tableWidget->model()->setData(*index,tableWidget->model()->data(*index).toString().replace(",","."));

        bool ok;
        tableWidget->model()->data(*index).toDouble(&ok);
        if (!ok)
        {
            tableWidget->model()->setData(*index,QColor(Qt::red),Qt::BackgroundRole);
            tableWidget->model()->setData(*index,QColor(Qt::white),Qt::ForegroundRole);
        }
        else
        {
            tableWidget->model()->setData(*index,QColor(Qt::white),Qt::BackgroundRole);
            tableWidget->model()->setData(*index,QColor(Qt::black),Qt::ForegroundRole);
        }

        delete index;

        *current_column = tableWidget->horizontalHeader()->visualIndex(*current_column);

        if (*current_column == tableWidget->model()->columnCount()-1)
        {
            *current_column = 0;
            if (++*current_row == tableWidget->model()->rowCount())
            {
                tableWidget->model()->insertRow(*current_row);
            }
        }
        else
        {
            ++*current_column;
        }
        tableWidget->selectionModel()->setCurrentIndex(tableWidget->model()->index(*current_row,tableWidget->horizontalHeader()->logicalIndex(*current_column)),QItemSelectionModel::Deselect | QItemSelectionModel::SelectCurrent);

        delete current_row;
        delete current_column;
    }
}

void TableWidget::slot_AddColumn()
{
    tableWidget->model()->insertColumn(tableWidget->model()->columnCount());
}

void TableWidget::customHeaderMenuRequested_table(const QPoint &pos)
{
    tableMenu->popup(tableWidget->viewport()->mapToGlobal(pos));
}

void TableWidget::customHeaderMenuRequested_header(const QPoint &pos)
{
    hHeaderMenu->popup(tableWidget->horizontalHeader()->viewport()->mapToGlobal(pos));
}

HeaderView::HeaderView(Qt::Orientation orientation,QWidget *parent) :
    QHeaderView(orientation,parent)
//  , ort (orientation)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSectionsClickable(true);
    this->setSectionResizeMode(QHeaderView::Interactive);
}

HeaderView::~HeaderView()
{

}

void HeaderView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        pressMiddleButton = 1;
        firstPos = event->pos().x();
        this->sectionPressed(this->logicalIndexAt(event->pos()));
//
        if (!sectionIndicator)
            sectionIndicator = new QLabel(this->viewport());

        int section = logicalIndexAt(firstPos);
        int w = sectionSize(section);
        int h = viewport()->height();
        int p = sectionViewportPosition(section);

        sectionIndicator->resize(w,h);

        const qreal pixmapDevicePixelRatio = this->devicePixelRatio();
        QPixmap pm(QSize(w,h) * pixmapDevicePixelRatio);
        pm.setDevicePixelRatio(pixmapDevicePixelRatio);
        pm.fill(QColor(0,0,0,45));
        QRect rect(0,0,w,h);
        QPainter painter(&pm);
        const QVariant variant = this->model()->headerData(section,Qt::Horizontal,Qt::FontRole);
        if (variant.isValid() && variant.canConvert<QFont>())
        {
            const QFont sectionFont = qvariant_cast<QFont>(variant);
            painter.setFont(sectionFont);
        } else
        {
            painter.setFont(this->font());
        }
        painter.setOpacity(0.8);
        this->paintSection(&painter,rect,section);
        painter.end();

        sectionIndicator->setPixmap(pm);
        sectionIndicatorOffset = firstPos - qMax(p,0);
//
    }

    QHeaderView::mousePressEvent(event);
}

void HeaderView::mouseMoveEvent(QMouseEvent *event)
{
    if (pressMiddleButton)
    {
        int pos =  event->pos().x();
        if (pos < 0)
            return;
        int visual = visualIndexAt(pos);
        if (visual == -1)
            return;

        if (!sectionIndicator)
                return;
        sectionIndicator->move(pos - sectionIndicatorOffset,0);
        sectionIndicator->show();

        lastPos = pos;
    }

    QHeaderView::mouseMoveEvent(event);
}

void HeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        int from = visualIndexAt(firstPos);
        int to = visualIndexAt(lastPos);
        if (from != -1 && to != -1)
            this->moveSection(from,to);

        if (!sectionIndicator)
                return;
        sectionIndicator->hide();
        pressMiddleButton = 0;
    }

    QHeaderView::mouseReleaseEvent(event);
}

