#ifndef ORDER
#define ORDER

struct Order{

    double orderTime;
    double brewTime;
    double price;
    double nextEventTime;
    bool arrived;
    bool ordered;
    long cashierNo;
    long baristaNo;
    int orderNo;

    Order(double timeOfArrival, double orderTime, double brewTime, double price, int orderNo){
        this->nextEventTime=timeOfArrival;
        this->orderTime=orderTime;
        this->brewTime=brewTime;
        this->price=price;
        this->orderNo=orderNo;
        arrived=false;
        ordered=false;
        cashierNo=0;
        baristaNo=0;
    };

    bool const operator<(const Order& order)const{
        return nextEventTime>order.nextEventTime; //ordered in reverse so that priority queue returns the min time event
    }

    bool compareByPrice(const Order& order)const {
        if(price==order.price)
            return nextEventTime>order.nextEventTime;

        return price<order.price;
    }


};

#endif