/*
In CPP promises and futures work side by side , by moving (using std:: move(future_obj)) the future object as a parameter to thread, we ask our thread to
perform something and then our future object takes the result from the thread set by the promise object. If the thread takes time to execute then the future object 
will wait for the thread to complete the execution and return the value as promised.
*/

#include <iostream>
#include <thread>
#include <future>

using namespace std;
typedef unsigned long long int ull;

// Can use the below mentioned function definition also if not using std :: move()
// void oddSumFunc(promise<ull> &OddSumPromise, ull start, ull end)
// move function returns a reference, that's why have used double reference here
void oddSumFunc(promise<ull> &&OddSumPromise, ull start, ull end)  
{
    ull oddSum = 0;
    for(ull i = start; i<end; ++i ){
        if(i%2!=0)
            oddSum += i;
    }
    OddSumPromise.set_value(oddSum);
}

int main(){
    ull start = 0, end = 19000000000;

    // creating promise object
    promise<ull> oddSum;   

    // creating future object which will take the data from the promise object, it is derived from promise object only
    future<ull> oddFuture = oddSum.get_future();  

    cout << "Thread created!\n";

    // created a thread : thread will start running parallely to our main thread
    thread t1(oddSumFunc, move(oddSum), start, end);  

    // can pass reference of the promise object also instead of moving it to the oddSumFunc
    // thread t1(oddSumFunc, ref(oddSum), start, end);  

    cout << "Waiting for promise result and in between doing something else!\n";

    // Adding extra work in our main thread
    for (int i = 0; i < 15; ++i)
    {
        cout << "Doing some other work...\n";
        this_thread::sleep_for(chrono::seconds(1));
    }

    // program will halt here until oddFuture object doesn't get the result from the promise object as promised otherwise the program will continue as we were asynchronously running our OddSumFunc thread parallely
    cout << "OddSum : " << oddFuture.get() << '\n';  

    cout << "Completed!\n";

    // Joining the thread back to the main thread
    t1.join(); 
    return 0;
}