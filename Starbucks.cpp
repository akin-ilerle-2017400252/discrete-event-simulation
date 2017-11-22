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

protected:
    vector<Employee> cashiers;
    vector<Employee> baristas;
    bool cashiersFull;
    bool baristasFull;
    priority_queue<Order,vector<Order>> eventQueue; //used to decide which event occurs first
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

        for(int i=0;i<m;i++){//reading input the input file and pushing every order into a priority queue
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


    virtual void simulate(){

        int n=cashiers.size();
        int m=turnarnoundTime.size();

        while(!eventQueue.empty()){//pops and executes every event untill there are no events to execute
            Order event = eventQueue.top();
            eventQueue.pop();
            execute(event);
        }

       write(n,m);

    }

protected:

    virtual /**
     * Writes the simulation results to output file
     * @param n: number of cashiers
     * @param m: number of orders
     */
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

    virtual /**
     * Here the input order can be at three stages: not arrived, arrived but not ordered and both arrived and ordered
     * this function identifies the stage and calls the necessary function
     * @param order
     */
    void execute(Order& order) {
        simulationTime=order.nextEventTime; //updating simulation time

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
            if(cashierQueueMaxLength<cashierQueue.size())//keep the record of the max queue length
                cashierQueueMaxLength=cashierQueue.size();
        }
        else{
            auto it = cashiers.begin();
            while(!it->free){ //find the free cashiers
                it++;
                if(it==cashiers.end()){//check if the cashiersFull flag is correct
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
            order.cashierNo = it-cashiers.begin();
            eventQueue.push(order);

            if(it==cashiers.end()-1)
                cashiersFull=true;
        }
    }

    virtual void order(Order& order){

        if(cashierQueue.empty()){//this order has just passed a cashier. If the queue is empty, it is set to be idle.
            cashiersFull=false;
            cashiers[order.cashierNo].free=true;
        }
        else{//if the queue is not free, the first in the line is sent to the cashier
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
            while(!it->free){   //find the free barista
                it++;
                if(it==baristas.end()){ //check the integrity of the baristasFull flag
                    baristasFull=true;
                    baristaQueue.push(order);
                    if(baristaQueueMaxLength<baristaQueue.size())   //keep the max of the line
                        baristaQueueMaxLength=baristaQueue.size();
                    return;
                }
            }
            order.nextEventTime=simulationTime+order.brewTime;
            it->free=false;
            it->totalWorkingTime+=order.brewTime;
            order.baristaNo = it-baristas.begin();
            eventQueue.push(order);
            if(it==baristas.end()-1)
                baristasFull=true;
        }

    }

    virtual void exit(Order& order){

        if(baristaQueue.empty()){   //if there is no order in the queue, set the barista free
            baristasFull=false;
            baristas[order.baristaNo].free=true;
        }
        else{   //if not, take the first in line and assign it to the barista
            Order firstInLine = baristaQueue.top();
            baristaQueue.pop();
            baristas[order.baristaNo].totalWorkingTime+=firstInLine.brewTime;
            firstInLine.baristaNo=order.baristaNo;
            firstInLine.baristaNo=order.baristaNo;
            firstInLine.nextEventTime=simulationTime+firstInLine.brewTime;
            //firstInLine.ordered=true;
            eventQueue.push(firstInLine);
        }

        //update the turnarount time of order:
        turnarnoundTime[order.orderNo]=simulationTime-turnarnoundTime[order.orderNo];

    }




};

#endif