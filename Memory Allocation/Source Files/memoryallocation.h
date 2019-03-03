#ifndef MEMORYALLOCATION_H
#define MEMORYALLOCATION_H

#include <QMainWindow>

namespace Ui {
class Memoryallocation;
}

class Memoryallocation : public QMainWindow
{
    Q_OBJECT

public:
    explicit Memoryallocation(QWidget *parent = 0);
    ~Memoryallocation();

private slots:
    void on_add_holes_clicked();

    void on_finish_clicked();

    void on_add_process_clicked();

    void on_compact_clicked();

    void on_reset_clicked();

    void on_delet_process_clicked();

private:
    Ui::Memoryallocation *ui;
};

#endif // MEMORYALLOCATION_H
