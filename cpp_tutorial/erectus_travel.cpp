#include <iostream>
using namespace std;

class erectus_travel
{
private:
    int speed;
    int distance;

public:
    void set_value(int a, int b)
    {
        speed = a;
        distance = b;
    }

    friend double erectus_travel_time(erectus_travel T);
};

double erectus_travel_time(erectus_travel T)
{
    double Time = double(T.distance) / double(T.speed);
    return Time;
}

int main()
{

    // Instantiate the object
    erectus_travel T;

    // initialize with values
    T.set_value(10, 20);

    // print the result
    cout << erectus_travel_time(T) << endl;
}