#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "show_dialog.h"
#include <QMessageBox>
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QHorizontalStackedBarSeries>
#include "carasa.h"
QT_CHARTS_USE_NAMESPACE

#include <QProcess>

float wait;
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


class proces
{
public:
    static int count;
    int name;
    float arriv_time;
    float req_time;
    float real_req_time;
    int priority;
    static int pnum;
    proces(int id,float arriv_t,float req_t,int pr=1)
    {

        count++;
        name=id;
        arriv_time = arriv_t;
        req_time = req_t;
        real_req_time = req_t;
        priority = pr;
    }
};
int proces::count = 0;
class point{
public:
    float x;
    int y;//changed
    point(float d1,int name)
    {
        x=d1;
        y=name;
    }
};

bool arrive_first (proces p1,proces p2) { return (p1.arriv_time<p2.arriv_time); }
bool shorter (proces p1,proces p2) { return (p1.req_time<p2.req_time); }
bool shorter_original (proces p1,proces p2) { return (p1.real_req_time<p2.real_req_time); }
bool more_important(proces p1,proces p2) { return (p1.priority<p2.priority); }

class my_list
{
public:
    vector<proces> p_list;
    bool isElementExist(int element){//add by girgis for find function for button add
        for(int i =0;i<p_list.size();i++){
            if(p_list[i].name==element)
                return true;
        }
        return false;
    }
    void add_process(int id,float arriv_t,float req_t,int pr=1)
{
    proces nnew = proces(id,arriv_t,req_t,pr);
    p_list.push_back(nnew);
}
    float total_time(vector<proces> pros_list)
{
    float t=0;
    for (int i = 0; i < pros_list.size(); i++)
        t += pros_list[i].req_time;
    return t;
}
    float next_small(float time,vector<proces> pros_list)
{
    for(int i=0;i < pros_list.size();i++)
    {
        if(time>pros_list[i].req_time)
            return pros_list[i].arriv_time;
    }
    return -1;
}
    float next_important(int priority,vector<proces> pros_list)
{
    for(int i=0;i < pros_list.size();i++)
    {
        if(priority>pros_list[i].priority)
            return pros_list[i].arriv_time;
    }
    return -1;
}
    float next_arrivetime(vector<proces> pros_list)
{
    float min=pros_list[0].arriv_time;
    for(int i=0;i < pros_list.size();i++)
    {
        if(min>pros_list[i].arriv_time)
            min = pros_list[i].arriv_time;
    }
    return min;
}

    void fcfs(vector<point> &points_list)
{
        wait=0;////////////////////////////////////////////////////////////////////////////////////
        int size=p_list.size();/////////////////////////////////////////////////////////////////////
    points_list.clear();
    float t_current=0;
    points_list.push_back(point(t_current,0));
    sort(p_list.begin(),p_list.end(),arrive_first);
    t_current += p_list[0].arriv_time;
    points_list.push_back(point(t_current,0));
    for (int i = 0; i < p_list.size(); i++)
    {
        if(p_list[i].arriv_time<=t_current)
        {
             wait += (t_current-p_list[i].arriv_time);///////////////////////////////////////////////////////////////////////////
            points_list.push_back(point(t_current,p_list[i].name));
            t_current += p_list[i].req_time;
            points_list.push_back(point(t_current,p_list[i].name));
        }
        else
        {
            points_list.push_back(point(t_current,0));
            t_current = p_list[i].arriv_time;
            points_list.push_back(point(t_current,0));
            points_list.push_back(point(t_current,p_list[i].name));
            t_current += p_list[i].req_time;
            points_list.push_back(point(t_current,p_list[i].name));

        }
    }
    wait=wait/size;///////////////////////////////////////////////////////////////////////////////////////////////
}


    void sjf(vector<point> &points_list)
{
    wait=0;////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int size=p_list.size();//////////////////////////////////////////////////////////////////////////////////////////
    points_list.clear();
    float t_current=0,t_wait=0,total=total_time(p_list);
    bool flag;
    sort(p_list.begin(),p_list.end(),shorter);
    //cout<<total_time(p_list)<<"\n";
    points_list.push_back(point(t_current,0));
    t_current=next_arrivetime(p_list);
    points_list.push_back(point(t_current,0));
    for(;total>0;)
    {
        flag=1;
        for (int j = 0; j < p_list.size(); j++)
        {
            if(p_list[j].arriv_time<=t_current)
            {
                wait += (t_current-p_list[j].arriv_time);////////////////////////////////////////////////////////////////////////////////
                points_list.push_back(point(t_current,p_list[j].name));
                t_current += p_list[j].req_time;
                points_list.push_back(point(t_current,p_list[j].name));
                total -= p_list[j].req_time;
                cout<<p_list[j].name<<" "<<t_current<<"\n";
                p_list.erase(p_list.begin()+j);
                j--;
                flag=0;
            }
        }
        if(flag==1)
        {
            points_list.push_back(point(t_current,0));
            t_current=next_arrivetime(p_list);
            points_list.push_back(point(t_current,0));
        }
    }
    wait=wait/size;//////////////////////////////////////////////////////////////////////////////////////////////////////
}
//    void round_robin(float step,vector<point> &points_list)
//{
//    wait=0;///////////////////////////////////////////////////////////////////////////////////////////////////////////
//    int size=p_list.size();///////////////////////////////////////////////////////////////////////////////////////////
//    points_list.clear();
//    float t_current=0,t_wait=0,total=total_time(p_list);
//    vector<proces> working;
//    bool flag;
//    sort(p_list.begin(),p_list.end(),arrive_first);
//    points_list.push_back(point(t_current,0));
//    t_current=next_arrivetime(p_list);
//    points_list.push_back(point(t_current,0));
//    for(;total>0.000001;)
//    {
//        flag = 1;
//        if(p_list.size()>0)
//        if(p_list[0].arriv_time<=t_current)
//        {
//            wait += (t_current-p_list[0].arriv_time);////////////////////////////////////////////////////////////////////////////////////
//            working.insert(working.begin(),p_list[0]);//////////////////////////////////////////////////////////////////////////////////
//            p_list.erase(p_list.begin());
//        }
//        for(int i=0;i<working.size();i++)
//        {   wait += (step*(working.size()-1));/////////////////////////////////////////////////////////////////////////////////////////////
//            flag=0;
//            points_list.push_back(point(t_current,working[i].name));
//            t_current += step;
//            points_list.push_back(point(t_current,working[i].name));
//            total -= step;
//            //cout<<working[i].name<<" "<<t_current<<"\n";
//            working[i].req_time -=step;
//            if(working[i].req_time<=0.00001)
//                       {working.erase(working.begin()+i);i--;}//////////////////////////////////////////////////////////////////////////////////////////////
//        }
//        if(flag==1)
//        {
//            points_list.push_back(point(t_current,0));
//            t_current=next_arrivetime(p_list);
//            points_list.push_back(point(t_current,0));
//        }
//    }
//    wait=wait/size;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//}
void round_robin(float step,vector<point> &points_list)
{
    wait=0;///////////////////////////////////////////////////////////////////////////////////////////////////////////
    int size=p_list.size();///////////////////////////////////////////////////////////////////////////////////////////
    points_list.clear();
    float t_current=0,t_wait=0,total=total_time(p_list);
    vector<proces> working;
    bool flag;
    sort(p_list.begin(),p_list.end(),arrive_first);
    points_list.push_back(point(t_current,0));
    t_current=next_arrivetime(p_list);
    points_list.push_back(point(t_current,0));
    for(;working.size()>0||p_list.size()>0;)//////////////////////////////////////////////////////////////////////////////
    {
        flag = 1;
        if(p_list.size()>0)
        if(p_list[0].arriv_time<=t_current)
        {
            working.insert(working.begin(),p_list[0]);//////////////////////////////////////////////////////////////////////////////////////////
            p_list.erase(p_list.begin());
        }
        for(int i=0;i<working.size();i++)
        {
            flag=0;
            points_list.push_back(point(t_current,working[i].name));
            t_current += step;
            points_list.push_back(point(t_current,working[i].name));
            total -= step;
            //cout<<working[i].name<<" "<<t_current<<"\n";
            working[i].req_time -=step;
            if(working[i].req_time<=0.00001)
            {
                wait += t_current-working[i].arriv_time-working[i].real_req_time+working[i].req_time;////////////////////////////////////////////////////////////////////
                working.erase(working.begin()+i);
                i--;////////////////////////////////////////////////////////////////////////////////////
            }
        }
        if(flag==1)
        {
            points_list.push_back(point(t_current,0));
            t_current=next_arrivetime(p_list);
            points_list.push_back(point(t_current,0));
        }
    }
    wait=wait/size;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
    void priority(vector<point> &points_list)
{
    wait=0;/////////////////////////////////////////////////////////////////////////////////////////////////////////
    int size=p_list.size();////////////////////////////////////////////////////////////////////////////////////////
    points_list.clear();
    float t_current=0,t_wait=0,total=total_time(p_list);
    bool flag;
    sort(p_list.begin(),p_list.end(),more_important);
    points_list.push_back(point(t_current,0));
    t_current=next_arrivetime(p_list);
    points_list.push_back(point(t_current,0));
    for(;total>0.000001;)
    {
        flag=1;
        for (int j = 0; j < p_list.size(); j++)
        {
            if(p_list[j].arriv_time<=t_current)
            {
                wait += (t_current-p_list[j].arriv_time);//////////////////////////////////////////////////////////////////////////////////////
                points_list.push_back(point(t_current,p_list[j].name));
                t_current += p_list[j].req_time;
                points_list.push_back(point(t_current,p_list[j].name));
                total -= p_list[j].req_time;
                cout<<p_list[j].name<<" "<<t_current<<"\n";
                p_list.erase(p_list.begin()+j);
                j=-1;
                flag=0;
            }
        }
        if(flag==1)
        {
            points_list.push_back(point(t_current,0));
            t_current=next_arrivetime(p_list);
            points_list.push_back(point(t_current,0));
        }
    }
    wait=wait/size;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
    void sjf_preemptive(vector<point> &points_list)
{
    wait=0;///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int size=p_list.size();//////////////////////////////////////////////////////////////////////////////////////////////////////
    points_list.clear();
    float t_current=0,t_wait=0,total=total_time(p_list),n_s;
    bool flag;
    vector<proces> working;
    sort(p_list.begin(),p_list.end(),arrive_first);
    cout<<total_time(p_list)<<"\n";
    for(;total>0.000001;)
    {
        flag = 1;
        for(int i=0;i<p_list.size();i++)
        {
        if(p_list[i].arriv_time<=t_current)
        {
                wait += (t_current-p_list[i].arriv_time);//////////////////////////////////////////////////////////////////////////////////////////
                working.push_back(p_list[i]);
                sort(working.begin(),working.end(),shorter);
                p_list.erase(p_list.begin()+i);
                i--;
        }
        }
        if(working.size()!=0)
        {
            n_s = next_small(working[0].real_req_time,p_list);
            flag=0;
            cout<<t_current<<" ";
            if(n_s>0)
            {
                if(n_s<(working[0].req_time+t_current))
                {
                    wait += (n_s-t_current)*(working.size()-1);//////////////////////////////////////////////////////////////////////////////////////////
                    total -= (n_s-t_current);
                    working[0].req_time -=(n_s-t_current);
                    points_list.push_back(point(t_current,working[0].name));
                    t_current = n_s;
                    points_list.push_back(point(t_current,working[0].name));
                }
                else
                {
                    wait += working[0].req_time*(working.size()-1);///////////////////////////////////////////////////////////////////////////////////////
                    total -=working[0].req_time;
                    points_list.push_back(point(t_current,working[0].name));
                    t_current += working[0].req_time;
                    points_list.push_back(point(t_current,working[0].name));
                    working[0].req_time=0;

                }
            }
            if(n_s==-1)
            {
                wait += working[0].req_time*(working.size()-1);////////////////////////////////////////////////////////////////////////////////////////////
                total -= working[0].req_time;
                points_list.push_back(point(t_current,working[0].name));
                t_current += working[0].req_time;
                points_list.push_back(point(t_current,working[0].name));
                working[0].req_time = 0;
            }
            cout<<working[0].name<<" "<<t_current<<"\n";
            if(working[0].req_time<=0)
                working.erase(working.begin());
        }
        if(flag==1)
        {
            points_list.push_back(point(t_current,0));
            t_current=next_arrivetime(p_list);
            points_list.push_back(point(t_current,0));
        }
    }
    wait=wait/size;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
    void priority_preemptive(vector<point> &points_list)
    {

        wait=0;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int size=p_list.size();////////////////////////////////////////////////////////////////////////////////////////////////////////
        points_list.clear();
        float t_current=0,t_wait=0,total=total_time(p_list),n_s;
        bool flag;
        vector<proces> working;
        sort(p_list.begin(),p_list.end(),arrive_first);
        cout<<total_time(p_list)<<"\n";
        for(;total>0.00001;)
        {
            flag = 1;
            for(int i=0;i<p_list.size();i++)
            {
            if(p_list[i].arriv_time<=t_current)
            {
                    working.push_back(p_list[i]);
                    sort(working.begin(),working.end(),more_important);
                    p_list.erase(p_list.begin()+i);
                    if(p_list.size()!=0)
                    i=-1;
            }
            }
            if(working.size()!=0)
            {
                n_s = next_important(working[0].priority,p_list);
                flag=0;
                cout<<t_current<<" ";
                if(n_s>0)
                {
                    if(n_s<(working[0].req_time+t_current))
                    {
                        total -= (n_s-t_current);
                        working[0].req_time -=(n_s-t_current);
                        points_list.push_back(point(t_current,working[0].name));
                        t_current = n_s;
                        points_list.push_back(point(t_current,working[0].name));
                    }
                    else
                    {
                        total -=working[0].req_time;
                        points_list.push_back(point(t_current,working[0].name));
                        t_current += working[0].req_time;
                        points_list.push_back(point(t_current,working[0].name));
                        working[0].req_time=0;

                    }
                }
                if(n_s==-1)
                {
                    total -= working[0].req_time;
                    points_list.push_back(point(t_current,working[0].name));
                    t_current += working[0].req_time;
                    points_list.push_back(point(t_current,working[0].name));
                    working[0].req_time = 0;
                }
                cout<<working[0].name<<" "<<t_current<<"\n";
                if(working[0].req_time<=0)
                {
                    wait += t_current-working[0].arriv_time-working[0].real_req_time;///////////////////////////////////////////////////////////
                    working.erase(working.begin());
                }
            }
            if(flag==1)
            {
                points_list.push_back(point(t_current,0));
                if(p_list.size()!=0)
                t_current=next_arrivetime(p_list);
                points_list.push_back(point(t_current,0));
            }
        }
            wait=wait/size;////////////////////////////////////////////////////////////////////////////////////////////////////////

    }
};







my_list p_list;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);





}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_comboBox_activated(const QString &arg1)
{
    if(ui->comboBox->currentText()=="Round Robin"){
        ui->spinBox_priority->setEnabled(false);
        ui->doubleSpinBox_time_slice->setEnabled(true);

    }

    else if (ui->comboBox->currentText()=="Priority Non Pre-emptive"
             ||ui->comboBox->currentText()=="Priority Pre-emptive")
    {

        ui->spinBox_priority->setEnabled(true);
        ui->doubleSpinBox_time_slice->setEnabled(false);
    }

    else{
        ui->spinBox_priority->setEnabled(false);
        ui->doubleSpinBox_time_slice->setEnabled(false);
    }

}



void MainWindow::on_pushButton_add_process_clicked()
{



    if(ui->lineEdit_Process_Name->text()!="" && !p_list.isElementExist(ui->lineEdit_Process_Name->text().toUInt())){
     // add to list
        p_list.add_process(ui->lineEdit_Process_Name->text().toInt(),
                           ui->doubleSpinBox_Arrival_Time_Arrival->text().toFloat(),
                             ui->doubleSpinBox_Burst_Time_burst->text().toFloat()
                           ,ui->spinBox_priority->text().toInt());


     //QStringList text_editor;
        ui->textEdit_process->insertPlainText("Name: "+ui->lineEdit_Process_Name->text()+
                                              " Proirity: "+ui->spinBox_priority->text()+
                                              " BURST: "+ui->doubleSpinBox_Burst_Time_burst->text()+
                                              " Arrival Time: "+ui->doubleSpinBox_Arrival_Time_Arrival->text());
        ui->textEdit_process->append("");

    }
    else if(ui->lineEdit_Process_Name->text()==""){QMessageBox::warning(this,"Erorr Message","Please Enter Process Number");}
    else if(p_list.isElementExist(ui->lineEdit_Process_Name->text().toUInt())){ QMessageBox::warning(this,"Erorr Message","The Process Number Is Already Exist");}

}

//void MainWindow::on_pushButton_clicked()
//{

//    vector<point> points;
//    //FCFS work well
//    if(ui->comboBox->currentText()=="FCFS"){

//        p_list.fcfs(points);

//    }
//    //one extra step
//    if(ui->comboBox->currentText()=="Round Robin"){

//        p_list.round_robin(ui->doubleSpinBox_time_slice->text().toFloat(),points);

//    }
//    //bl77777
//    if(ui->comboBox->currentText()=="Priority Pre-emptive"){

//        p_list.priority_preemptive(points);

//    }

//    //always takes the first
//    if(ui->comboBox->currentText()=="Priority Non Pre-emptive"){

//        p_list.priority(points);

//    }

//    if(ui->comboBox->currentText()=="Shortest Job First Pre-emptive"){

//        p_list.sjf_preemptive(points);

//    }

//    //sjf non works well
//    if(ui->comboBox->currentText()=="Shortest Job First non Pre-emptive"){

//        p_list.sjf(points);

//    }



//    SHOW_Dialog s;

//     QLineSeries *series = new QLineSeries();
////         series->append(0, 6);
////         series->append(2, 4);
////         series->append(3, 8);
////         series->append(7, 4);
////         series->append(10, 5);
////         *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6)
////                 << QPointF(18, 3) << QPointF(20, 2);
//     for(int i=0;i<points.size();i++)
//     {
//         series->append(points[i].x,points[i].y);
//         series->append(points[i].x,points[i].y);
//         //*series << QPointF(11, 1)<< QPointF(13, 3);

//     }


//     QChart *chart = new QChart();
//        chart->legend()->hide();
//        chart->addSeries(series);
//        chart->createDefaultAxes();
//        chart->setTitle("Simple line chart example");


//    QChartView *chartView = new QChartView(chart);
//      chartView->setRenderHint(QPainter::Antialiasing);

//      QMainWindow window;
//         window.setCentralWidget(chartView);
//         window.resize(400, 300);
//         window.show();

//        s.setModal(true);
//        s.exec();
//         // system("pause");

//}

//Run Button
void MainWindow::on_actionRun_triggered()
{
    if(p_list.p_list.empty()){

            QMessageBox::warning(this,"Erorr Message","Please Enter Process First" );
    }
    else{

            vector<point> points;
            //FCFS work well
            if(ui->comboBox->currentText()=="FCFS"){

                p_list.fcfs(points);

            }
            //one extra step
            if(ui->comboBox->currentText()=="Round Robin"){
                if(ui->doubleSpinBox_time_slice->text().toFloat()<.000000001){
                    QMessageBox::warning(this,"Erorr Message","Please Enter Slice Time bigger than 0");
                    return;
                }
                else{
                     p_list.round_robin(ui->doubleSpinBox_time_slice->text().toFloat(),points);
                }
            }
            //bl77777
            if(ui->comboBox->currentText()=="Priority Pre-emptive"){

                p_list.priority_preemptive(points);

            }

            //always takes the first
            if(ui->comboBox->currentText()=="Priority Non Pre-emptive"){

                p_list.priority(points);

            }

            if(ui->comboBox->currentText()=="Shortest Job First Pre-emptive"){

                p_list.sjf_preemptive(points);

            }

            //sjf non works well
            if(ui->comboBox->currentText()=="Shortest Job First non Pre-emptive"){

                p_list.sjf(points);

            }



            SHOW_Dialog s(wait);

             QLineSeries *series = new QLineSeries();
        //         series->append(0, 6);
        //         series->append(2, 4);
        //         series->append(3, 8);
        //         series->append(7, 4);
        //         series->append(10, 5);
        //         *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6)
        //                 << QPointF(18, 3) << QPointF(20, 2);
             for(int i=0;i<points.size();i++)
             {
                 series->append(points[i].x,points[i].y);
                 series->append(points[i].x,points[i].y);
                 //*series << QPointF(11, 1)<< QPointF(13, 3);

             }


             QChart *chart = new QChart();
                chart->legend()->hide();
                chart->addSeries(series);
                chart->createDefaultAxes();
                chart->setTitle("Simple line chart example");


            QChartView *chartView = new QChartView(chart);
              chartView->setRenderHint(QPainter::Antialiasing);

              QMainWindow window;
                 window.setCentralWidget(chartView);
                 window.resize(400, 300);
                 window.show();
                 ui->doubleSpinBox_time_slice->text()="0.00";

                s.setModal(true);

                s.exec();
                 // system("pause");
    }

}


//Reset Button
void MainWindow::on_actionreset_triggered()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}
