// ParsingCsvDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<string.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Order
{
    string Order_Id;
    string Client_Order_Id;
    string Instrument;
    int Side;
    string Exec_Status;
    int Quantity;
    float Price;
    string Trader_Id;

public:
    Order(
        int i,
        string client_order_id,
        string instrument,
        int side,
        string exec_status,
        int quantity,
        double price,
        string trader_id)
    {
        Order_Id = "odd" + to_string(i);
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = exec_status;
        Quantity = quantity;
        Price = price;
        Trader_Id = trader_id;
    }

    void display()
    {
        cout << "          Order_Id : " << Order_Id << endl;
        cout << "   Client_Order_Id : " << Client_Order_Id << endl;
        cout << "        Instrument : " << Instrument << endl;
        cout << "              Side : " << Side << endl;
        cout << "       Exec_Status : " << Exec_Status << endl;
        cout << "          Quantity : " << Quantity << endl;
        cout << "             Price : " << Price << endl;
        cout << "         Trader_Id : " << Trader_Id << endl;
        cout << endl;
    }
};

int validation(string client_order_id, string instrument, int side, int quantity, double price, string trader_id, vector<Order>& orders)
{
    // client_order_id validation
    if ((client_order_id.length() > 1) && (client_order_id.length() < 8))
    {
        int fint = 0, fchar = 0;
        for (int i = 0; i < client_order_id.length(); i++) {
            if (isdigit(client_order_id[i]))
                fint++;
            else
                if (isalpha(client_order_id[i]))
                    fchar++;
        }
        if (!(fint > 0 && fchar > 0))
            return 0;

        for (auto order : orders)
        {
            if (order.Client_Order_Id == client_order_id)
                return 0;
        }
    }
    else
        return 0;

    // instrument validation
    if (instrument != "Rose")
        if (instrument != "Lavender")
            if (instrument != "Lotus")
                if (instrument != "Tulip")
                    if (instrument != "Orchid")
                        return 0;

    //side validation
    if (!(side == 1 || side == 2))
        return 0;

    //price validation
    if (price <= 0.0)
        return 0;

    // quantity validation
    if (quantity <= 1000 && quantity >= 10)
    {
        if (!(quantity % 10 == 0))
            return 0;
    }        
    else
        return 0;

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
        float Price;
        string Trader_Id;
        string tempString;
        string exec_status = "New";

        getline(inputString, Client_Order_Id, ',');
        getline(inputString, Instrument, ',');

        getline(inputString, tempString, ',');
        Side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Quantity = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Price = atof(tempString.c_str());

        getline(inputString, Trader_Id, ',');

        if (validation(Client_Order_Id, Instrument, Side, Quantity, Price, Trader_Id,orders))
        {
            Order order(i, Client_Order_Id, Instrument, Side,exec_status, Quantity, Price, Trader_Id);
            orders.push_back(order);
        }
        else
        {
            exec_status = "Reject";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id);
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
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Trader ID" << endl;
        for (auto order : orders)
        {
            file << order.Order_Id << "," << order.Client_Order_Id << "," << order.Instrument << "," << order.Side << "," << order.Exec_Status << "," << order.Quantity << "," << order.Price << "," << order.Trader_Id << endl;
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