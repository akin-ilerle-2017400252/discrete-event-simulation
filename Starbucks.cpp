#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <iomanip>
#include "Employee.cpp"
#include "Order.cpp"
using namespace std;

#ifndef ORDER_BY_PRICE
#define ORDER_BY_PRICE

struct OrderByPrice
{
    bool operator() (Order const &a, Order const &b) {
        return a.compareByPrice(b);
    }
};

#endif

#ifndef STARBUCKS
#define STARBUCKS

class Starbucks{

public:
    vector<Employee> cashiers;
    vector<Employee> baristas;
    bool cashiersFull;
    bool baristasFull;
    priority_queue<Order,vector<Order>> eventQueue;
    priority_queue<Order,vector<Order>,OrderByPrice> baristaQueue;
    queue<Order> cashierQueue;
    double simulationTime;
    long cashierQueueMaxLength;
    long baristaQueueMaxLength;
    vector<double> turnarnoundTime;
    string outputFile;

    void initialize(int n){

        for(int i=0;i<n;i++){
            cashiers.emplace_back( Employee() );
            if(i%3==0)
                baristas.emplace_back( Employee() );
        }
        cashiersFull=false;
        baristasFull=false;
        cashiersFull=false;
        simulationTime=0;
        cashierQueueMaxLength=0;
        baristaQueueMaxLength=0;
    }
public:

    Starbucks(const string &inputFile, const string &outputFile){

        ifstream reader;
        reader.open(inputFile);
        int n,m;
        reader>>n;
        reader>>m;
        this->outputFile=outputFile;
        initialize(n);

        for(int i=0;i<m;i++){
            double timeOfArrival,orderTime,brewTime,price;
            reader>>timeOfArrival;
            reader>>orderTime;
            reader>>brewTime;
            reader>>price;
            Order p =  Order(timeOfArrival, orderTime,brewTime,price,i);
            turnarnoundTime.emplace_back(timeOfArrival);
            eventQueue.push( p );
        }
        reader.close();

        //TODO: print turnaround times of all orders.

    }


    void simulate(){

        int n=cashiers.size();
        int m=turnarnoundTime.size();

        while(!eventQueue.empty()){
            Order event = eventQueue.top();
            eventQueue.pop();
            execute(event);
        }

       write(n,m);

    }

    void write(int n,int m){
        ofstream writer(outputFile);

        writer<<fixed<<setprecision(2);
        writer<<simulationTime<<endl;
        writer<<cashierQueueMaxLength<<endl;
        writer<<baristaQueueMaxLength<<endl;

        for(int i=0;i<n;i++){
            writer<<cashiers[i].totalWorkingTime/simulationTime<<endl;
        }

        for(int i=0;i<n/3;i++){
            writer<<baristas[i].totalWorkingTime/simulationTime<<endl;
        }

        for(int i=0;i<m;i++){
            writer<<turnarnoundTime[i]<<endl;
        }
        writer<<"\n";
        writer.close();
    }

    void execute(Order& order) { //TODO: create data types?? or use Order type all along
        simulationTime=order.nextEventTime;
        if(!order.arrived){
            arrive(order);
            return;
        }

        if(!order.ordered){
            this->order(order);
            return;
        }

        exit(order);

    }

    void arrive(Order& order){
        order.arrived=true;
        if(cashiersFull){
            cashierQueue.push(order);
            if(cashierQueueMaxLength<cashierQueue.size())
                cashierQueueMaxLength=cashierQueue.size();
        }
        else{
            auto it = cashiers.begin();
            while(!it->free){ //TODO: keep free and not free employees seperately
                it++;
                if(it==cashiers.end()){//if cashiers are full
                    cashiersFull=true;
                    cashierQueue.push(order);
                    if(cashierQueueMaxLength<cashierQueue.size())
                        cashierQueueMaxLength=cashierQueue.size();
                    return;
                }
            }
            order.nextEventTime=simulationTime+order.orderTime;
            it->free=false;
            it->totalWorkingTime+=order.orderTime;
            order.cashierNo = it-cashiers.begin(); //TODO: check
            eventQueue.push(order);
            if(it==cashiers.end()-1)
                cashiersFull=true;
        }
    }

    void order(Order& order){
        if(cashierQueue.empty()){
            cashiersFull=false;
            cashiers[order.cashierNo].free=true;
        }
        else{
            Order firstInLine = cashierQueue.front();
            cashierQueue.pop();
            cashiers[order.cashierNo].totalWorkingTime+=firstInLine.orderTime;
            firstInLine.cashierNo=order.cashierNo;
            firstInLine.nextEventTime=simulationTime+firstInLine.orderTime;
            eventQueue.push(firstInLine);
        }

        order.ordered=true;

        if(baristasFull){
            baristaQueue.push(order);
            if(baristaQueueMaxLength<baristaQueue.size())
                baristaQueueMaxLength=baristaQueue.size();
        }
        else{
            auto it = baristas.begin();
            while(!it->free){
                it++;
                if(it==baristas.end()){
                    baristasFull=true;
                    baristaQueue.push(order);
                    if(baristaQueueMaxLength<baristaQueue.size())
                        baristaQueueMaxLength=baristaQueue.size();
                    return;
                }
            }
            order.nextEventTime=simulationTime+order.brewTime;
            //order.ordered=true;
            it->free=false;
            it->totalWorkingTime+=order.brewTime;
            order.baristaNo = it-baristas.begin();//TODO: check
            eventQueue.push(order);
            if(it==baristas.end()-1)
                baristasFull=true;
        }

    }

    void exit(Order& order){

        if(baristaQueue.empty()){
            baristasFull=false;
            baristas[order.baristaNo].free=true;
        }
        else{//TODO: check
            Order firstInLine = baristaQueue.top();
            baristaQueue.pop();
            baristas[order.baristaNo].totalWorkingTime+=firstInLine.brewTime;
            firstInLine.baristaNo=order.baristaNo;
            firstInLine.baristaNo=order.baristaNo;
            firstInLine.nextEventTime=simulationTime+firstInLine.brewTime;
            //firstInLine.ordered=true;
            eventQueue.push(firstInLine);
        }

        turnarnoundTime[order.orderNo]=simulationTime-turnarnoundTime[order.orderNo];

    }




};

#endif