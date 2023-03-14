// ParsingCsvDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
// #include <string.h>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Order
{
private:
    string Order_Id;
    string Client_Order_Id;
    string Instrument;
    int Side;
    string Exec_Status;
    int Quantity;
    double Price;
    string Trader_Id;
    string Reason;

public:
    Order(
        int i,
        string client_order_id,
        string instrument,
        int side,
        string exec_status,
        int quantity,
        double price,
        string trader_id,
        string reason)
    {
        Order_Id = "odd" + to_string(i);
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = exec_status;
        Quantity = quantity;
        Price = price;
        Trader_Id = trader_id;
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
        cout << "         Trader_Id : " << Trader_Id << endl;
        cout << "            Reason : " << Reason << endl;
        cout << endl;
    }

    int validation(string client_order_id, string instrument, int side, int quantity, double price, string trader_id, vector<Order> &orders);
    void get_data(vector<Order> &orders);
    void displayOrders(vector<Order> &orders);
    void set_data(vector<Order> &orders);
};

int Order::validation(string client_order_id, string instrument, int side, int quantity, double price, string trader_id, vector<Order> &orders)
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
            return -1;

        for (auto order : orders)
        {
            if (order.Client_Order_Id == client_order_id)
                return -1;
        }
    }
    else
        return -1;

    // instrument validation
    if (instrument != "Rose")
        if (instrument != "Lavender")
            if (instrument != "Lotus")
                if (instrument != "Tulip")
                    if (instrument != "Orchid")
                        return -2;

    // side validation
    if (!(side == 1 || side == 2))
        return -3;

    // price validation
    if (price <= 0.00)
        return -4;

    // quantity validation
    if (quantity <= 1000 && quantity >= 10)
    {
        if (!(quantity % 10 == 0))
            return -5;
    }
    else
        return -5;

    return 1;
}

void Order::get_data(vector<Order> &orders)
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
        string Trader_Id;
        string tempString;
        string exec_status = "New";
        string reason = "";

        getline(inputString, Client_Order_Id, ',');
        getline(inputString, Instrument, ',');

        getline(inputString, tempString, ',');
        Side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Quantity = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        //Price = atof(tempString.c_str());
        Price = stold(tempString);

        getline(inputString, Trader_Id, ',');

        int reason_int = validation(Client_Order_Id, Instrument, Side, Quantity, Price, Trader_Id, orders);

        if (reason_int == 1)
        {
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }
        else if (reason_int == -1)
        {
            exec_status = "Reject";
            reason = "Invalid Client ID";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }
        else if (reason_int == -2)
        {
            exec_status = "Reject";
            reason = "Invalid Instrument";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }
        else if (reason_int == -3)
        {
            exec_status = "Reject";
            reason = "Invalid Side";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }
        else if (reason_int == -4)
        {
            exec_status = "Reject";
            reason = "Invalid Price";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }
        else if (reason_int == -5)
        {
            exec_status = "Reject";
            reason = "Invalid Size";
            Order order(i, Client_Order_Id, Instrument, Side, exec_status, Quantity, Price, Trader_Id, reason);
            orders.push_back(order);
        }

        line = "";
    }
}

void Order::displayOrders(vector<Order> &orders)
{
    for (auto order : orders)
    {
        order.display();
    }
}

void Order::set_data(vector<Order> &orders)
{
    // ofstream file("execution_rep.csv", ios::app);
    ofstream file("execution_rep.csv", ofstream::trunc);
    if (file.is_open())
    {
        file << "Execution_Rep.csv" << endl;
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Trader ID,Reason" << endl;
        for (auto order : orders)
        {
            file << order.Order_Id << "," << order.Client_Order_Id << "," << order.Instrument << "," << order.Side << "," << order.Exec_Status << "," << order.Quantity << "," << fixed << setprecision(2) << order.Price << "," << order.Trader_Id << "," << order.Reason << endl;
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