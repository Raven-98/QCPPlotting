#include "analyzediffractiondatadron2.h"

AnalyzeDiffractionDataDRON2::AnalyzeDiffractionDataDRON2(GBS::DADDDRON2::Data data, QObject *parent)
    : QObject(parent),
      data(data)
{}

AnalyzeDiffractionDataDRON2::~AnalyzeDiffractionDataDRON2()
{}

void AnalyzeDiffractionDataDRON2::run()
{
    if (data.fileName.data()->isNull())
    {
        emit error(tr("No file name specified"));
        emit quit();
        return;
    }


    QFile *file = new QFile(data.fileName);
    if ( !file->open(QFile::ReadOnly | QFile::Text) )
    {
        delete file;
        emit error(tr("File not exists"));
        emit quit();
        return;
    }

    int *count_data = new int(QTextStream(file).readAll().split('\n').count() - 2);
    file->seek(0);

    QTextStream *in = new QTextStream(file);
    QString *line = new QString(in->readLine());

    char *delimiter;
    switch (data.delimiter){
    case GBS::FDSD::Delimiter::Comma:
        delimiter = new char(',');
        break;
    case GBS::FDSD::Delimiter::Tab_step:
        delimiter = new char('\t');
        break;
    case GBS::FDSD::Delimiter::Semicolon:
        delimiter = new char(';');
        break;
    case GBS::FDSD::Delimiter::Space:
        delimiter = new char(' ');
        break;
    default:
        delimiter = new char(',');
        break;
    }

    QStringList *stringItemsList = new QStringList;
    for (auto &item : line->split(*delimiter))
        stringItemsList->append(item);
    int *index_data = new int(-1);
    for (int i = 0; i < stringItemsList->count(); i++)
        if (stringItemsList->at(i) == "Value")
            *index_data = i;
    if (*index_data == -1)
    {
        delete count_data;
        delete delimiter;
        delete index_data;
        delete line;
        delete in;
        file->close();
        delete file;
        emit error(tr("Column \"Value\" not found"));
        emit quit();
        return;
    }
    stringItemsList->clear();

    QStandardItemModel *item_model = new QStandardItemModel;
    item_model->setHorizontalHeaderItem(0,new QStandardItem("2θ"));
    item_model->setHorizontalHeaderItem(1,new QStandardItem(tr("Intensity")));
    int *current = new int(0);
    while (!in->atEnd())
    {
        QList<QStandardItem *> standardItemsList;
        *line = in->readLine();
        for (auto &item : line->split(*delimiter))
        {
            stringItemsList->append(item);
        }
        float two_theta = data.twoThetaStart + *current * ((data.twoThetaEnd - data.twoThetaStart) / (*count_data));
        two_theta = two_theta - (0.544 + 0.000599591 * two_theta);
        standardItemsList.append(new QStandardItem(QString::number(two_theta)));
        standardItemsList.append(new QStandardItem(stringItemsList->at(*index_data)));
        item_model->insertRow(item_model->rowCount(), standardItemsList);
        standardItemsList.clear();
        stringItemsList->clear();
        *current += 1;
    }

    delete delimiter;
    delete stringItemsList;
    delete current;
    delete index_data;
    delete count_data;
    delete line;
    delete in;

    file->close();
    delete file;

    emit finished(item_model);
    emit quit();
}
