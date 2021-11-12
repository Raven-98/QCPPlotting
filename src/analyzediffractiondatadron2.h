#ifndef ANALYZEDIFFRACTIONDATADRON2_H
#define ANALYZEDIFFRACTIONDATADRON2_H

#include "global.h"
#include "dialogs.h"


class AnalyzeDiffractionDataDRON2 : public QObject
{
    Q_OBJECT
public:
    AnalyzeDiffractionDataDRON2(GBS::DADDDRON2::Data data, QObject *parent = nullptr);
    ~ AnalyzeDiffractionDataDRON2();

public slots:
    void run();

signals:
    void finished(QStandardItemModel *item_model);
    void error(QString err);
    void quit();

private:
    GBS::DADDDRON2::Data data;

signals:

};

#endif // ANALYZEDIFFRACTIONDATADRON2_H
