#include "Starbucks.cpp"

class StarbucksModel2: public Starbucks{

protected:
    vector<priority_queue<Order,vector<Order>,OrderByPrice>> baristaQueue;  //holds the seperate queues of baristas
    vector<int> baristaQueueMaxLength;

public:

    StarbucksModel2(const string &inputFile, const string &outputFile) : Starbucks(inputFile, outputFile) {

        int baristaNo=baristas.size();

        for(int i=0;i<baristaNo;i++){
            baristaQueue.emplace_back(priority_queue<Order,vector<Order>,OrderByPrice>());
            baristaQueueMaxLength.emplace_back(0);
        }

    }


    void simulate() override {

        int n=cashiers.size();
        int m=turnarnoundTime.size();

        while(!eventQueue.empty()){
            Order event = eventQueue.top();
            event.baristaNo=event.cashierNo/3;  //cashiers are fixed
            eventQueue.pop();
            execute(event);
        }

        write(n,m);

    }

protected:

    void write(int n, int m) override {
        ofstream writer(outputFile,ios::app);

        writer<<fixed<<setprecision(2);
        writer<<simulationTime<<endl;
        writer<<cashierQueueMaxLength<<endl;
        for(int i=0;i<baristaQueueMaxLength.size();i++){
            writer<<baristaQueueMaxLength[i]<<endl;
        }

        for(int i=0;i<n;i++){
            writer<<cashiers[i].totalWorkingTime/simulationTime<<endl;
        }

        for(int i=0;i<n/3;i++){
            writer<<baristas[i].totalWorkingTime/simulationTime<<endl;
        }

        for(int i=0;i<m-1;i++){
            writer<<turnarnoundTime[i]<<endl;
        }
        writer<<turnarnoundTime[m-1];
        writer.close();
    }

    void execute(Order& order) override {
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

    void order(Order& order) override {

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

        if(!baristas[order.cashierNo/3].free){
            baristaQueue[order.cashierNo/3].push(order);
            if(baristaQueueMaxLength[order.cashierNo/3]<baristaQueue[order.cashierNo/3].size())
                baristaQueueMaxLength[order.cashierNo/3]=baristaQueue[order.cashierNo/3].size();
        }
        else{
            order.nextEventTime=simulationTime+order.brewTime;
            baristas[order.cashierNo/3].free=false;
            baristas[order.cashierNo/3].totalWorkingTime+=order.brewTime;
            eventQueue.push(order);
        }


    }

    void exit(Order& order) override {

        if(baristaQueue[order.cashierNo/3].empty()){
            baristas[order.cashierNo/3].free=true;
        }
        else{
            Order firstInLine = baristaQueue[order.cashierNo/3].top();
            baristaQueue[order.cashierNo/3].pop();
            baristas[order.baristaNo].totalWorkingTime+=firstInLine.brewTime;
            firstInLine.nextEventTime=simulationTime+firstInLine.brewTime;
            eventQueue.push(firstInLine);
        }

        turnarnoundTime[order.orderNo]=simulationTime-turnarnoundTime[order.orderNo];

    }



};
