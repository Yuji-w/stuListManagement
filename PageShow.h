#ifndef PAGESHOW_H
#define PAGESHOW_H

#include <QMainWindow>

class pageShow : public QMainWindow
{
    Q_OBJECT
public:
    explicit pageShow()
    {

    }
    ~pageShow();

private:


private slots:
    int getFirstPage();
    int getPrePage();
    int getNextPage();
    int getLastPage();
    int getThisPage();
    int getALtogePage(int value);

};







#endif // PAGESHOW_H
