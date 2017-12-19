## Discrete Event Simulation
There is a businessman who wants to build new Starbucks in Etiler. There are two types of employees in Starbucks, one of them is cashier who takes the order of the customer and the other one is barista who prepares the coffee for the customer.
There are two different models in Starbucks. Each model has N cashiers and N/3 baristas(N is divisible by 3 in examples). First model has one queue for cashiers and one queue for baristas(Fig. 1). The other model has one queue for cashiers and N/3 queues for baristas. First 3 cashiers send the orders to the first barista and next 3 cashiers send the orders to the second barista and goes on like that(Fig. 2). In both systems, cashier queue works by first come first serve design and barista queue works by most expensive coffee served first.
This program simulates these two different models and collect the following statistics for each model:
1. Total running time of the system
2. Utilization of each cashier and barista 3. Turnaround time of each order
4. Maximum length of each queue

## Input Format
1. First line is number of cashiers (N) which is always divisible by 3.
2. Second line is the number of orders (M).
3. Following M lines contain information about orders. There are 4 vari- ables separated with space. They are:
(a) Arrival Time: The time when customer enters to the coffee shop. (in seconds)
(b) Order Time: The time required to give coffee order. (in seconds)
(c) Brew Time: The time required to make coffee. (in seconds)
(d) Price of Order: Price of the coffee.
The output file has the following format:
1. First line is the total running time of the coffee shop. (in seconds)
2. Next line is the maximum length of the cashier queue.
3. (a) If the coffee shop has one barista queue, next line is maximum length of this queue.
(b) If the coffee shop has N/3 barista queues, next N/3 lines are max- imum lengths of each barista queue.
4. Next N lines are the utilization of the cashiers. 5. Next N/3 lines are the utilization of the baristas. 6. Next M lines are turnaround times of orders.

## How to compile

In a terminal, call commands:
```
>cmake CMakeLists.txt

>make

OR

>cmake CMakeLists.txt && make

```
Make sure the executable is produced.

Then you can test the project with the command:
```
>./project2 inputFile outputFile
```
