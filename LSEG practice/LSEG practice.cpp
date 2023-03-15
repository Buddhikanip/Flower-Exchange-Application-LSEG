#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

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

    Order(int,string,string,int,int,int,double,string);
    Order(string, string, string, int, int, int, double, string);
    void display();
    void write_csv();
    int validation();
};

Order::Order(int i, string client_order_id, string instrument, int side, int exec_status, int quantity, double price, string reason)
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

void Order::display()
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

void Order::write_csv()
{
    display();
    ofstream file("execution_rep.csv", ios::app);
    if (file.is_open())
    {
        file << Order_Id << "," << Client_Order_Id << "," << Instrument << "," << Side << "," << Exec_Status << "," << Quantity << "," << fixed << setprecision(2) << Price << "," << Reason << "," << Transaction_Time << endl;
    }
    file.close();
}

int Order::validation()
{
    // instrument validation
    if (Instrument != "Rose")
        if (Instrument != "Lavender")
            if (Instrument != "Lotus")
                if (Instrument != "Tulip")
                    if (Instrument != "Orchid")
                    {
                        Reason = "Invalid Instrument";
                        return 0;
                    }

    // side validation
    if (!(Side == 1 || Side == 2))
    {
        Reason = "Invalid Side";
        return 0;
    }

    // quantity validation
    if (Quantity < 1000 && Quantity >= 10)
    {
        if (!(Quantity % 10 == 0))
        {
            Reason = "Invalid Size";
            return 0;
        }
    }
    else
    {
        Reason = "Invalid Size";
        return 0;
    }

    // price validation
    if (Price <= 0.00)
    {
        Reason = "Invalid Price";
        return 0;
    }

    return 1;
}

bool buy_compare(Order a, Order b)
{
    return a.Price > b.Price;
}
bool sell_compare(Order a, Order b)
{
    return a.Price < b.Price;
}

void calculation(Order &order, vector<Order> &buy, vector<Order> &sell)
{
    if (order.Side == 1) // buy
    {
        if (sell.empty())
        {
            buy.push_back(order);
            sort(buy.begin(), buy.end(), buy_compare);
            if (order.Exec_Status != 3)
                order.write_csv();
            return;
        }
        for (int j = 0; j <= sell.size(); j++)
        {
            Order sells = sell[j];
            if (sells.Instrument != order.Instrument)
                continue;
            if (sells.Price > order.Price)
            {
                buy.push_back(order);
                sort(buy.begin(), buy.end(), buy_compare);
                if (order.Exec_Status != 3)
                    order.write_csv();
                return;
            }
            else if (sells.Price == order.Price)
            {
                if (sells.Quantity == order.Quantity)
                {
                    order.Exec_Status = 2;
                    order.write_csv();

                    sells.Exec_Status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);
                    sort(sell.begin(), sell.end(), sell_compare);

                    return;
                }
                if (sells.Quantity < order.Quantity)
                {
                    order.Exec_Status = 3;
                    order.Quantity = order.Quantity - sells.Quantity;

                    Order order_temp = order;
                    order_temp.Quantity = sells.Quantity;
                    order_temp.write_csv();

                    sells.Exec_Status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);
                    sort(sell.begin(), sell.end(), sell_compare);

                    if (!sell.empty() || order.Quantity != 0)
                        calculation(order, buy, sell);

                    if (order.Quantity > 0)
                    {
                        buy.push_back(order);
                        sort(buy.begin(), buy.end(), buy_compare);
                    }

                    return;
                }
                else //sells.Quantity > quantity
                {
                    order.Exec_Status = 2;
                    order.write_csv();

                    sells.Quantity = sells.Quantity - order.Quantity;
                    sells.Exec_Status = 3;
                    sells.write_csv();

                    return;
                }
            }
            else //sells.Price < price
            {
                if (sells.Quantity == order.Quantity)
                {
                    order.Exec_Status = 2;

                    Order order_temp = order;
                    order_temp.Price = sells.Price;
                    order_temp.write_csv();

                    sells.Exec_Status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);
                    sort(sell.begin(), sell.end(), sell_compare);

                    return;
                }
                else if (sells.Quantity > order.Quantity)
                {
                    // write
                }
                else
                {
                    // write
                }
            }
            if (sells.Price < order.Price)
            {
                if (sells.Quantity == order.Quantity)
                {
                    order.Exec_Status = 2;

                    Order order_temp = order;
                    order_temp.Price = sells.Price;
                    order_temp.write_csv();

                    sells.Exec_Status = 2;
                    sells.write_csv();
                    sell.erase(sell.begin() + j);

                    return;
                }
            }
        }
        buy.push_back(order);
        sort(buy.begin(), buy.end(), buy_compare);
        if (order.Exec_Status != 3)
            order.write_csv();
    }
    else // sell
    {
        if (buy.empty())
        {
            sell.push_back(order);
            sort(sell.begin(), sell.end(), sell_compare);
            if (order.Exec_Status != 3)
                order.write_csv();
            return;
        }
        // for (auto buys : buy)
        for (int j = 0; j <= sell.size(); j++)
        {
            Order buys = buy[j];
            if (buys.Instrument != order.Instrument)
                continue;
            if (buys.Price > order.Price)
            {
                if (buys.Quantity == order.Quantity)
                {
                    order.Exec_Status = 2;
                    order.write_csv();

                    buys.Exec_Status = 2;
                    buys.write_csv();
                    buy.erase(buy.begin() + j);
                    sort(buy.begin(), buy.end(), buy_compare);

                    return;
                }
                if (buys.Quantity < order.Quantity)
                {
                    order.Exec_Status = 3;
                    order.Quantity = order.Quantity - buys.Quantity;

                    Order order_temp = order;
                    order_temp.Quantity = buys.Quantity;
                    order_temp.Price = buys.Price;
                    order_temp.write_csv();

                    buys.Exec_Status = 2;
                    buys.write_csv();
                    buy.erase(buy.begin() + j);
                    sort(buy.begin(), buy.end(), buy_compare);

                    if (!buy.empty() || order.Quantity != 0)
                        calculation(order,buy,sell);

                    if (order.Quantity > 0)
                    {
                        sell.push_back(order);
                        sort(sell.begin(), sell.end(), sell_compare);
                    }

                    return;
                }
                else
                {
                    // write
                }
            }
            else if (buys.Price = order.Price)
            {
                if (buys.Quantity > order.Quantity)
                {
                    // write
                }
                else if (buys.Quantity == order.Quantity)
                {
                    // write
                }
                else
                {
                    // write
                }
            }
            else
            {
                if (buys.Quantity > order.Quantity)
                {
                    // write
                }
                else if (buys.Quantity == order.Quantity)
                {
                    // write
                }
                else
                {
                    // write
                }
            }
        }
        sell.push_back(order);
        sort(sell.begin(), sell.end(), sell_compare);
        if (order.Exec_Status != 3)
            order.write_csv();
    }
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
        Price = stold(tempString);

        // getline(inputString, Trader_Id, ',');

        Order order(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price, Reason);

        int reason_int = order.validation();
        string temp_oid;

        if (reason_int)
        {
            calculation(order, buy, sell);
        }
        else
        {
            Exec_status = 1;
            
            //set(order);
            order.write_csv();
        }

        line = "";
    }
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

    return 0;
}