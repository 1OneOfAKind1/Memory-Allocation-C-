#include "memoryallocation.h"
#include "ui_memoryallocation.h"
#include <QMessageBox>


#include <iostream>
#include <vector>
#include <string>


using namespace std;


struct Block
{
    int Type;               // 0 for holes , 1 for process
    int base;
    int limit;
    string Name;           // name all holes as "Hole"
};

int memsize;
vector<Block> Memory;
int Hash=0;


void Add_Hole(Block B)
{
    Memory.push_back(B);
}


void Sorting(void)
{
    Block tt;
    for(uint i=0; i<Memory.size() ;i++)
    {
        for (uint j=i+1 ; j<Memory.size() ; j++)
        {
            if(Memory[j].base<Memory[i].base)
            {
                tt=Memory[i];
                Memory[i]=Memory[j];
                Memory[j]=tt;
            }
        }
    }
}


void finish_fun(void)
{
    if((Memory.empty()!=1) && Memory[0].base != 0)
    {
        Block tt;
        tt.Type=1;
        char text[20];
        sprintf(text,"#%d",Hash);
        tt.Name=string(text);
        tt.base=0;
        tt.limit = Memory[0].base;
        Memory.insert(Memory.begin(),tt);
        Hash++;
    }
    vector<Block> temp;
    for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
    {
        if((*i).base+(*i).limit < (*(i+1)).base)
        {
            Block tt;
            tt.Type=1;
            char text[20];
            sprintf(text,"#%d",Hash);
            tt.Name=string(text);
            tt.base=(*i).base + (*i).limit;
            tt.limit = (*(i+1)).base-tt.base;
            temp.push_back((*i));
            temp.push_back(tt);

         //   Memory.insert(i+1,tt);

            Hash++;
        }
        else
        {
            temp.push_back((*i));
        }


    }

    Memory = temp;

    if((Memory.empty()!=1) && Memory.back().base + Memory.back().limit < memsize)
    {
        Block tt;
        tt.Type=1;
        char text[20];
        sprintf(text,"#%d",Hash);
        tt.Name=string(text);
        tt.base=Memory.back().base + Memory.back().limit;
        tt.limit = memsize-tt.base;
        Memory.push_back(tt);
        Hash++;
    }



}


bool Add_Block(Block B , int method)
{
    if(method == 0)                     //Frst fit
    {
        for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
        {
            if((*i).Type==0 && (*i).limit>=B.limit)
            {
                Block tt;
                tt.Type=0;
                tt.Name="Hole";
                tt.limit = (*i).limit - B.limit;
                tt.base = (*i).base + B.limit;
                B.base = (*i).base;
                (*i) = B;
                if(tt.limit != 0)
                    Memory.insert(i+1,tt);

                return true;
            }
        }
    }
    else if(method == 1)                //Best fit
    {
        vector<Block>::iterator j=Memory.begin();
        int t=0;
        for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
        {
            if(t==0)
            {
                if((*i).Type==0 && (*i).limit>=B.limit)
                {
                    t=(*i).limit;
                    j=i;
                }
            }
            else
            {
                if((*i).Type==0 && (*i).limit>=B.limit && (*i).limit<t)
                {
                    t=(*i).limit;
                    j=i;
                }
            }
        }


        if(t!=0)
        {
            Block tt;
            tt.Type=0;
            tt.Name="Hole";
            tt.limit = (*j).limit - B.limit;
            tt.base = (*j).base + B.limit;
            B.base = (*j).base;
            (*j) = B;
            if(tt.limit != 0)
                Memory.insert(j+1,tt);
            return true;
        }
    }
    else if(method == 2)                //Worst fit
    {
        vector<Block>::iterator j=Memory.begin();
        int t=0;
        for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
        {
            if(t==0)
            {
                if((*i).Type==0 && (*i).limit>=B.limit)
                {
                    t=(*i).limit;
                    j=i;
                }
            }
            else
            {
                if((*i).Type==0 && (*i).limit>=B.limit && (*i).limit>t)
                {
                    t=(*i).limit;
                    j=i;
                }
            }
        }


        if(t!=0)
        {
            Block tt;
            tt.Type=0;
            tt.Name="Hole";
            tt.limit = (*j).limit - B.limit;
            tt.base = (*j).base + B.limit;
            B.base = (*j).base;
            (*j) = B;
            if(tt.limit != 0)
                Memory.insert(j+1,tt);
            return true;
        }
    }



    return false;
}


void Remove_Block(string s)
{
    for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
    {
        if((*i).Name==s)
        {
            (*i).Type=0;
            (*i).Name="Hole";
        }
    }
}


void compaction(void)
{
    vector<Block> tempmem;
    int t=0;
    for(vector<Block>::iterator i=Memory.begin() ; i != Memory.end() ;i++)
    {
        if((*i).Type == 1)
        {
            (*i).base = t;
            t+=(*i).limit;
            tempmem.push_back((*i));
        }
    }
    Block tt;
    tt.Type=0;
    tt.Name="Hole";
    tt.base = t;
    tt.limit = memsize-t;
    tempmem.push_back(tt);

    Memory = tempmem;

}





Memoryallocation::Memoryallocation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Memoryallocation)
{
    ui->setupUi(this);
}

Memoryallocation::~Memoryallocation()
{
    delete ui;
}




void Memoryallocation::on_add_holes_clicked()
{
    QString memorysize = ui->mem_size->text();
    memsize = memorysize.toInt();
    Block newhole;
    QString holelimit = ui->limit->text();
    QString holebase = ui->base->text();
    newhole.base=holebase.toInt();
    newhole.limit=holelimit.toInt();
    newhole.Type=0;
    newhole.Name="Hole";
    Add_Hole(newhole);
    ui->limit->setText("");
    ui->base->setText("");
}

void Memoryallocation::on_finish_clicked()
{
    Sorting();
    finish_fun();
    ui->memtable->setColumnCount(1);
    ui->memtable->setRowCount(Memory.size());
    ui->memtable->horizontalHeader()->hide();
    ui->memtable->verticalHeader()->hide();

    for(uint i=0; i<Memory.size();i++)
    {
        if(Memory[i].Type == 1)
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Memory[i].Name)+"  "+QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->setBackgroundColor(QColor::fromHsv(Memory[i].limit%255,245,255));
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
        else
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
    }
}

void Memoryallocation::on_add_process_clicked()
{
    int method_num;
    Block Process;
    QString processname = ui->name->text();
    QString processsize = ui->size->text();
    QString methodtype = ui->method->currentText();
    Process.Type=1;
    Process.base=0;
    Process.Name=processname.toStdString();
    Process.limit=processsize.toInt();

    if(methodtype == "First Fit")
    {
        method_num=0;
    }
    else if(methodtype == "Best Fit")
    {
        method_num=1;
    }
    else if(methodtype == "Worst Fit")
    {
        method_num=2;
    }

    ui->name->setText("");
    ui->size->setText("");

    if(Add_Block(Process , method_num))
    {
        ui->memtable->setColumnCount(1);
        ui->memtable->setRowCount(Memory.size());
        ui->memtable->horizontalHeader()->hide();
        ui->memtable->verticalHeader()->hide();

        for(uint i=0; i<Memory.size();i++)
        {
            if(Memory[i].Type == 1)
            {
                ui->memtable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Memory[i].Name)+"  "+QString::number(Memory[i].limit)));
                ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
                ui->memtable->setRowHeight(i,Memory[i].limit/2);
                ui->memtable->item(i,0)->setBackgroundColor(QColor::fromHsv(Memory[i].limit%255,245,255));
                ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
            }
            else
            {
                ui->memtable->setItem(i,0,new QTableWidgetItem(QString::number(Memory[i].limit)));
                ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
                ui->memtable->setRowHeight(i,Memory[i].limit/2);
                ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
            }
        }
    }
    else
    {
        QMessageBox::information( this, tr("Error"), tr("No Available space for the New process deallocate process or press compact") );
    }
}

void Memoryallocation::on_compact_clicked()
{
    compaction();
    ui->memtable->setColumnCount(1);
    ui->memtable->setRowCount(Memory.size());
    ui->memtable->horizontalHeader()->hide();
    ui->memtable->verticalHeader()->hide();
    for(uint i=0; i<Memory.size();i++)
    {
        if(Memory[i].Type == 1)
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Memory[i].Name)+"  "+QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->setBackgroundColor(QColor::fromHsv(Memory[i].limit%255,245,255));
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
        else
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
    }
}

void Memoryallocation::on_reset_clicked()
{
    ui->memtable->setColumnCount(0);
    ui->memtable->setRowCount(0);
    ui->memtable->horizontalHeader()->hide();
    ui->memtable->verticalHeader()->hide();

    ui->limit->setText("");
    ui->name->setText("");
    ui->size->setText("");
    ui->base->setText("");
    ui->mem_size->setText("");
    Memory.clear();
    memsize=0;
    Hash=0;
}

void Memoryallocation::on_delet_process_clicked()
{
    QString deleteditem = ui->memtable->currentItem()->text();
    ui->memtable->removeCellWidget(ui->memtable->currentRow(),ui->memtable->currentRow());
//    QTableWidgetItem *it = ui->memtable->deletItem(ui->memtable->currentRow(),ui->memtable->currentRow());
//    delete it;
//    delete ui->memtable->currentItem();
//    string mystring=deleted_item.toStdString();
//    string arr[] = mystring.split(" ", 2);
//    string Proc_Name = arr[0];
//    string Proc_Size = arr[1];
    string deleted_item=deleteditem.toStdString();
    string s="";
    for(uint i=0;i<deleted_item.length();i++)
    {
        if(deleted_item[i] == ' ')
        {
            break;
        }
        s=s+deleted_item[i];
    }
    Remove_Block(s);
    ui->memtable->setColumnCount(1);
    ui->memtable->setRowCount(Memory.size());
    ui->memtable->horizontalHeader()->hide();
    ui->memtable->verticalHeader()->hide();

    for(uint i=0; i<Memory.size();i++)
    {
        if(Memory[i].Type == 1)
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::fromStdString(Memory[i].Name)+"  "+QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->setBackgroundColor(QColor::fromHsv(Memory[i].limit%255,245,255));
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
        else
        {
            ui->memtable->setItem(i,0,new QTableWidgetItem(QString::number(Memory[i].limit)));
            ui->memtable->item(i,0)->setToolTip("from " + QString::number(Memory[i].base) + " to " + QString::number(Memory[i].base+Memory[i].limit));
            ui->memtable->setRowHeight(i,Memory[i].limit/2);
            ui->memtable->item(i,0)->QTableWidgetItem::setTextAlignment(Qt:: AlignCenter);
        }
    }

}
