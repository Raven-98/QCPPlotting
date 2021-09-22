#ifndef SPACER_H
#define SPACER_H

#include <QVBoxLayout>

class Spacer : public QVBoxLayout
{
    Q_OBJECT
public:
    Spacer(QSizePolicy::Policy hPolicy = QSizePolicy::Expanding, QSizePolicy::Policy vPolicy = QSizePolicy::Expanding, QWidget *parent = nullptr);
    Spacer(short hPolicy, short vPolicy, QWidget *parent = nullptr);
    ~Spacer();
};

#endif // SPACER_H
