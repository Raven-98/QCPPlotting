#ifndef ABOUTPROGRAM_H
#define ABOUTPROGRAM_H

#include "global.h"

class AboutProgram : public QDialog
{
    Q_OBJECT
public:
    AboutProgram(QWidget *parent);
    ~AboutProgram();

private:
    QGridLayout *gridLayout;
    QVBoxLayout *boxLayoutAbout,
                *boxLayoutLibraries,
                *boxLayoutAuthors;
    QTabWidget *tabsWidget;
    QWidget *tabAbout,
            *tabLibraries,
            *tabAuthors;
    QLabel  *iconLabel,
            *nameLabel,
            *versionLabel,
            *labelAbout,
            *labelAboutOwner,
            *labelAboutWEB,
            *labelAboutLicense,
            *labelLibraries_Qt,
            *labelLibraries_QCP,
            *labelAuthors_Raven98;
    QDialogButtonBox *buttonBox;
};

#endif // ABOUTPROGRAM_H
