// ParsingCsvDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

int buy_count = 0, sell_count = 0;

class Order
{
public:
    string Order_Id;
    string Client_Order_Id;
    string Instrument;
    int Side;
    int Exec_Status;
    int Quantity;
    double Price;
    // string Trader_Id;
    string Reason;
    string Transaction_Time;

    Order(int i, string client_order_id, string instrument, int side, int exec_status, int quantity, double price, string reason)
    {
        Order_Id = "odd" + to_string(i);
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = exec_status;
        Quantity = quantity;
        Price = price;
        // Trader_Id = trader_id;
        Reason = reason;
    }
    Order(string order_id, string client_order_id, string instrument, int side, int exec_status, int quantity, double price, string reason)
    {
        Order_Id = order_id;
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = exec_status;
        Quantity = quantity;
        Price = price;
        // Trader_Id = trader_id;
        Reason = reason;
    }

    void display()
    {
        cout << "          Order_Id : " << Order_Id << endl;
        cout << "   Client_Order_Id : " << Client_Order_Id << endl;
        cout << "        Instrument : " << Instrument << endl;
        cout << "              Side : " << Side << endl;
        cout << "       Exec_Status : " << Exec_Status << endl;
        cout << "          Quantity : " << Quantity << endl;
        cout << "             Price : " << fixed << setprecision(2) << Price << endl;
        // cout << "         Trader_Id : " << Trader_Id << endl;
        cout << "            Reason : " << Reason << endl;
        cout << "  Transaction_Time : " << Transaction_Time << endl;
        cout << endl;
    }
};

void set(Order order);

bool buy_compare(Order a, Order b)
{
    return a.Price > b.Price;
}
bool sell_compare(Order a, Order b)
{
    return a.Price < b.Price;
}

void calculation(int i, string client_order_id, string &instrument, int side, int &exec_status, int quantity, double price, string reason, vector<Order> &buy, vector<Order> &sell)
{
    if (side == 1)
        buy_count++;
    else
        sell_count++;

    if (side == 1) // buy
    {
        if (sell.empty())
        {
            Order order(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
            buy.push_back(order);
            sort(buy.begin(), buy.end(), buy_compare);
            if (exec_status != 3)
                set(order);
            return;
        }
        //for (auto sells : sell)
        for (int j=0;j<=sell.size();j++)
        {
            Order sells = sell[j];
            if (sells.Instrument != instrument)
                continue;
            if (sells.Price > price)
            {
                Order order(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
                buy.push_back(order);
                sort(buy.begin(), buy.end(), buy_compare);
                if (exec_status != 3)
                    set(order);
                return;
            }
            if (sells.Price == price)
            {
                if (sells.Quantity == quantity)
                {
                    exec_status = 2;
                    Order order1(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
                    set(order1);

                    Order order2(sells.Order_Id, sells.Client_Order_Id, sells.Instrument, sells.Side, 2, sells.Quantity, sells.Price, sells.Reason);
                    set(order2);
                    sell.erase(sell.begin()+j);

                    return;
                }
                if (sells.Quantity < quantity)
                {
                    exec_status = 3;
                    quantity = quantity - sells.Quantity;
                    Order order1(i, client_order_id, instrument, side, exec_status, sells.Quantity, price, reason);
                    //buy.push_back(order1);
                    set(order1);

                    Order order2(sells.Order_Id, sells.Client_Order_Id, sells.Instrument, sells.Side, 2, sells.Quantity, sells.Price, sells.Reason);
                    set(order2);
                    sell.erase(sell.begin() + j);

                    if (!sell.empty() || quantity != 0)
                        calculation(i, client_order_id, instrument, side, exec_status, quantity, price, reason, buy, sell);

                    if (quantity > 0)
                        buy.push_back(order1);

                    return;
                }
            }
        }
    }
    else // sell
    {
        if (buy.empty())
        {
            Order order(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
            sell.push_back(order);
            sort(sell.begin(), sell.end(), sell_compare);
            if (exec_status != 3)
                set(order);
            return;
        }
        //for (auto buys : buy)
        for (int j = 0;j <= sell.size();j++)
        {
            Order buys = buy[j];
            if (buys.Instrument != instrument)
                continue;
            if (buys.Price > price)
            {
                if (buys.Quantity == quantity)
                {
                    exec_status = 2;
                    Order order1(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
                    set(order1);

                    Order order2(buys.Order_Id, buys.Client_Order_Id, buys.Instrument, buys.Side, 2, buys.Quantity, buys.Price, buys.Reason);
                    set(order2);
                    buy.erase(buy.begin() + j);

                    return;
                }
                if (buys.Quantity < quantity)
                {
                    exec_status = 3;
                    quantity = quantity - buys.Quantity;
                    Order order1(i, client_order_id, instrument, side, exec_status, buys.Quantity, buys.Price, reason);
                    set(order1);

                    Order order2(buys.Order_Id, buys.Client_Order_Id, buys.Instrument, buys.Side, 2, buys.Quantity, buys.Price, buys.Reason);
                    set(order2);
                    buy.erase(buy.begin() + j);

                    if (!buy.empty() || quantity != 0)
                        calculation(i, client_order_id, instrument, side, exec_status, quantity, price, reason, buy, sell);

                    if(quantity>0)
                        sell.push_back(order1);

                    return;
                }
            }
        }
    }

    // for (auto order : orders)
    // {
    //     if (order.Instrument == instrument)
    //     {
    //         if (side == 1)
    //         {
    //             if (order.Side == 2)
    //             {
    //                 if (order.Price == price)
    //                 {
    //                     if (order.Quantity == quantity)
    //                     {
    //                         exec_status = 2;
    //                         buy_count--;
    //                         Order order_new2(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
    //                         orders.push_back(order_new2);
    //                         sell_count--;
    //                         Order order_new3(order.Order_Id, order.Client_Order_Id, order.Instrument, order.Side, 2, order.Quantity, order.Price, order.Reason);
    //                         orders.push_back(order_new3);

    //                         return;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    // Order order_new4(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
    // orders.push_back(order_new4);
}

int validation(string instrument, int side, int quantity, double price, string &reason)
{
    // instrument validation
    if (instrument != "Rose")
        if (instrument != "Lavender")
            if (instrument != "Lotus")
                if (instrument != "Tulip")
                    if (instrument != "Orchid")
                    {
                        reason = "Invalid Instrument";
                        return 0;
                    }

    // side validation
    if (!(side == 1 || side == 2))
    {
        reason = "Invalid Side";
        return 0;
    }

    // price validation
    if (price <= 0.00)
    {
        reason = "Invalid Price";
        return 0;
    }

    // quantity validation
    if (quantity <= 1000 && quantity >= 10)
    {
        if (!(quantity % 10 == 0))
        {
            reason = "Invalid Size";
            return 0;
        }
    }
    else
    {
        reason = "Invalid Size";
        return 0;
    }

    return 1;
}

void get_data(vector<Order> &buy, vector<Order> &sell)
{
    int i = 0;
    ifstream inputFile;
    inputFile.open("orders.csv");
    string line = "";

    getline(inputFile, line); // for file name
    line = "";
    getline(inputFile, line); // for headers
    line = "";
    while (getline(inputFile, line))
    {
        i++;
        stringstream inputString(line);
        string Client_Order_Id;
        string Instrument;
        int Side;
        int Quantity;
        double Price;
        // string Trader_Id;
        string tempString;
        int Exec_status = 0;
        string Reason = "";

        getline(inputString, Client_Order_Id, ',');
        getline(inputString, Instrument, ',');

        getline(inputString, tempString, ',');
        Side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Quantity = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        // Price = atof(tempString.c_str());
        Price = stold(tempString);

        // getline(inputString, Trader_Id, ',');

        int reason_int = validation(Instrument, Side, Quantity, Price, Reason);
        string temp_oid;

        if (reason_int)
        {
            calculation(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price, Reason, buy, sell);
        }
        else
        {
            Exec_status = 1;
            Order order(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price, Reason);
            set(order);
        }

        line = "";
    }
}

// void displayOrders(vector<Order> &orders)
// {
//     for (auto order : orders)
//     {
//         order.display();
//     }
// }

// void set_data(vector<Order> &orders)
// {
//     // ofstream file("execution_rep.csv", ios::app);
//     ofstream file("execution_rep.csv", ofstream::trunc);
//     if (file.is_open())
//     {
//         file << "Execution_Rep.csv" << endl;
//         file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time" << endl;
//         for (auto order : orders)
//         {
//             file << order.Order_Id << "," << order.Client_Order_Id << "," << order.Instrument << "," << order.Side << "," << order.Exec_Status << "," << order.Quantity << "," << fixed << setprecision(2) << order.Price << "," << order.Reason<<","<< order.Transaction_Time << endl;
//         }
//     }
//     file.close();
// }

void set(Order order)
{
    order.display();
    ofstream file("execution_rep.csv", ios::app);
    if (file.is_open())
    {
        file << order.Order_Id << "," << order.Client_Order_Id << "," << order.Instrument << "," << order.Side << "," << order.Exec_Status << "," << order.Quantity << "," << fixed << setprecision(2) << order.Price << "," << order.Reason << "," << order.Transaction_Time << endl;
    }
    file.close();
}

int main()
{
    vector<Order> buy;
    vector<Order> sell;

    ofstream file("execution_rep.csv", ofstream::trunc);
    if (file.is_open())
    {
        file << "Execution_Rep.csv" << endl;
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time" << endl;
    }
    file.close();

    get_data(buy, sell);

    // displayOrders(orders);

    // set_data(orders);
}