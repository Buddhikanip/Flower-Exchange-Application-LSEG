// ParsingCsvDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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
    //string Trader_Id;
    string Reason;
    string Transaction_Time;


    Order(int i,string client_order_id,string instrument,int side,int exec_status,int quantity,double price,string reason)
    {
        Order_Id = "odd" + to_string(i);
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = exec_status;
        Quantity = quantity;
        Price = price;
        //Trader_Id = trader_id;
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
        //Trader_Id = trader_id;
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
        //cout << "         Trader_Id : " << Trader_Id << endl;
        cout << "            Reason : " << Reason << endl;
        cout << "  Transaction_Time : " << Transaction_Time << endl;
        cout << endl;
    }

};

void calculation(int i, string client_order_id, string &instrument, int side, int &exec_status, int quantity, double price, string reason, vector<Order>& orders)
{
    if (side == 1)
        buy_count++;
    else
        sell_count++;

    if (buy_count == 0 && sell_count == 0)
    {
        Order order_new1(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
        orders.push_back(order_new1);
    }

    for (auto order : orders)
    {
        if (order.Instrument == instrument)
        {
            if (side == 1)
            {
                if (order.Side == 2)
                {
                    if (order.Price == price)
                    {
                        if (order.Quantity == quantity)
                        {
                            exec_status = 2;
                            Order order_new2(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
                            orders.push_back(order_new2);

                            Order order_new3(order.Order_Id, order.Client_Order_Id, order.Instrument, order.Side, 2, order.Quantity, order.Price, order.Reason);
                            orders.push_back(order_new3);

                            return;
                        }
                    }
                }
            }
        }
    }
    Order order_new4(i, client_order_id, instrument, side, exec_status, quantity, price, reason);
    orders.push_back(order_new4);
}

int validation(string client_order_id, string instrument, int side, int quantity, double price,string &reason, vector<Order> &orders)
{
    // client_order_id validation
    if ((client_order_id.length() > 1) && (client_order_id.length() < 8))
    {
        int fint = 0, fchar = 0, fother = 0;
        for (int i = 0; i < client_order_id.length(); i++)
        {
            if (isdigit(client_order_id[i]))
                fint++;
            else if (isalpha(client_order_id[i]))
                fchar++;
            else
                fother++;
        }
        if (!(fint > 0 && fchar > 0 && fother == 0))
        {
            reason = "Invalid Client ID";
            return 0;
        }

        for (auto order : orders)
        {
            if (order.Client_Order_Id == client_order_id)
            {
                reason = "Invalid Client ID";
                return 0;
            }
        }
    }
    else
    {
        reason = "Invalid Client ID";
        return 0;
    }

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

void get_data(vector<Order> &orders)
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
        //string Trader_Id;
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
        //Price = atof(tempString.c_str());
        Price = stold(tempString);

        //getline(inputString, Trader_Id, ',');

        int reason_int = validation(Client_Order_Id, Instrument, Side, Quantity, Price,Reason, orders);
        string temp_oid;

        if (reason_int)
        {
            calculation(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price, Reason, orders);
        }
        else
        {
            Exec_status = 1;
            Order order(i, Client_Order_Id, Instrument, Side, Exec_status, Quantity, Price, Reason);
            orders.push_back(order);
        }

        line = "";
    }
}

void displayOrders(vector<Order> &orders)
{
    for (auto order : orders)
    {
        order.display();
    }
}

void set_data(vector<Order> &orders)
{
    // ofstream file("execution_rep.csv", ios::app);
    ofstream file("execution_rep.csv", ofstream::trunc);
    if (file.is_open())
    {
        file << "Execution_Rep.csv" << endl;
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time" << endl;
        for (auto order : orders)
        {
            file << order.Order_Id << "," << order.Client_Order_Id << "," << order.Instrument << "," << order.Side << "," << order.Exec_Status << "," << order.Quantity << "," << fixed << setprecision(2) << order.Price << "," << order.Reason<<","<< order.Transaction_Time << endl;
        }
    }
    file.close();
}

int main()
{
    vector<Order> orders;

    get_data(orders);

    displayOrders(orders);

    set_data(orders);
}