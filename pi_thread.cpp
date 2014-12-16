#include <iostream>
#include <thread>
#include <cmath>
#include <ctime>
#include <random>
#include <future>
#include <iomanip>
using namespace std;

double darts( unsigned long long count)
{
    mt19937 rng (rand());
    uniform_real_distribution<double> dist (0.0,1.0);
    unsigned long long circle=0;
    for( unsigned long long  i=0; i<count; i++)
    {
        double x = dist(rng);
        double y = dist(rng);
        if(sqrt(x*x+y*y)<1)
            circle+=1;
    }
    double pi=4*(double)circle/count;

    return pi;

}

int main()
{
    clock_t begin = clock();
    unsigned int threads = std::thread::hardware_concurrency();
    if(!threads)
        threads=1;
    srand(time(0));
    unsigned long long num_darts=rand()*10000+rand();
    vector<future<double> > runs;
    runs.resize(threads);
    for(int i=0; i<threads; i++)
        runs[i]= std::async(std::launch::async, darts, num_darts);
    double average=0.0;
    for(int i=0; i<threads; i++)
        average+=runs[i].get();
    average/=threads;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"running "<<threads<<" threads that each threw "<<num_darts;
    cout<<" darts,\npi is  estimated to be "<<std::setprecision(9)<<average<<"\nit took "<< elapsed_secs<<" seconds.\n\n";
    begin = clock();
    average=0;
    for(int i=0; i<threads; i++)
        average+=darts(num_darts);
    average/=threads;
    end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"running  one thread that threw "<<num_darts<<" darts, "<<threads<<" times,\n";
    cout<<"pi is  estimated to be "<<std::setprecision(9)<<average<<"\n it took "<< elapsed_secs<<" seconds.\n";
}
