// ParsingCsvDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Order {
public:
    Order(
        int i,
        string client_order_id,
        string instrument,
        int side,
        //string exec_status,
        int quantity,
        double price,
        string trader_id
    ) {
        Order_Id ="odd" + to_string(i);
        Client_Order_Id = client_order_id;
        Instrument = instrument;
        Side = side;
        Exec_Status = "New";
        Quantity = quantity;
        Price = price;
        Trader_Id = trader_id;
    }

    void display() {
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

    string Order_Id;
    string Client_Order_Id;
    string Instrument;
    int Side;
    string Exec_Status;
    int Quantity;
    float Price;
    string Trader_Id;

};


void displayOrders(vector<Order>& orders) {

    for (auto order : orders) {
        order.display();
    }
}

void out(vector<Order>& orders)
{
    //ofstream file("execution_rep.csv", ios::app);
    ofstream file("execution_rep.csv", ofstream::trunc);
    if (file.is_open()) {
        file << "Execution_Rep.csv" << endl;
        file << "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Trader ID" << endl;
            for (auto order : orders)
            {
                file << order.Order_Id << "," <<order.Client_Order_Id<<","<<order.Instrument<<","<<order.Side<<","<<order.Exec_Status<<","<<order.Quantity<<","<<order.Price<<","<<order.Trader_Id << endl;
            }
    }
    file.close();
    
}

int main()
{
    int i=0;

    ifstream inputFile;
    inputFile.open("orders.csv");
    string line = "";

    vector<Order> orders;

    getline(inputFile, line);
    line = "";
    getline(inputFile, line);
    line = "";
    while (getline(inputFile, line)) {

        stringstream inputString(line);

        i++;

        string Client_Order_Id;
        string Instrument;
        int Side;
        int Quantity;
        float Price;
        string Trader_Id;
        string tempString;

        getline(inputString, Client_Order_Id, ',');
        getline(inputString, Instrument, ',');

        getline(inputString, tempString, ',');
        Side = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Quantity = atoi(tempString.c_str());

        getline(inputString, tempString, ',');
        Price = atof(tempString.c_str());

        getline(inputString, Trader_Id, ',');

        Order order(i ,Client_Order_Id, Instrument,Side, Quantity, Price, Trader_Id);
        orders.push_back(order);
        line = "";
    }

    displayOrders(orders);

    out(orders);
 
}