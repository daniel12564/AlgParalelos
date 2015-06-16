#include <iostream>
#include <omp.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

int thread_count;

/* Calcular trapezoide*/

double f(double x)
{
  return x * x + x - 3;
}

double calc_trapezoide(double a, double b, int n)
{
  	double h = (b-a) / n;
  	double sum = (f(a) + f(b)) / 2;
	# pragma omp parallel for num_threads(thread_count)\
    reduction(+: sum)
  	for (int i = 1; i < n; ++i)
    	sum += f(a + h * i);
  	return sum * h;
}

void trapezoide()
{
	double a, b;
  	int n;
  	cin>>a>>b>>n;

  	clock_t start, finish;
  	start = clock();

  	double result = calc_trapezoide(a, b, n);

  	finish = clock();
  	double time = (double(finish-start)/CLOCKS_PER_SEC);
  	
  	cout << "Result: " << result << endl;
  	cout << "Time: " << time << " ms" << endl;
}



/* Calcular pi */

double calc_pi(int it)
{
	double accum, factor;
	accum = 0;
	# pragma omp parallel for reduction(+: accum) private(factor)\
    num_threads(thread_count)
  	for(int i = 0; i < it; i++)
  	{
  		if(i & 1)
  			factor = -1;
  		else
  			factor = 1;
    	accum += factor / (2 * i + 1);
  	}
  	return accum * 4;
}

void pi()
{
	int n;
  	cin >> n;

  	clock_t start, finish;
  	start = clock();

  	double pi = calc_pi(n);

  	finish = clock();
  	double time = (double(finish-start)/CLOCKS_PER_SEC);

  	cout << "Pi: " << pi << endl;
  	cout << "Time: " << time << " ms" << endl;
}

/* Odd-Even Sort */

void odd_eve_sort1(vector<int> a)
{
	size_t phase, i, n = a.size();
  	for(phase = 0; phase < n; ++phase)
  	{
	    if(phase & 1)
	    {
			# pragma omp parallel for num_threads(thread_count)\
			shared(a, n) private(i, phase)
      		for(i = 1; i < n - 1; i += 2)
        		if(a[i] > a[i+1])
          			swap(a[i], a[i+1]);
    	}
    	else
    	{
			# pragma omp parallel for num_threads(thread_count)\
        	shared(a, n) private(i, phase)
      		for(i = 1; i < n; i += 2)
        		if(a[i-1] > a[i])
          			swap(a[i-1], a[i]);
    	}
  	}
}

void odd_eve_sort2(vector<int> a)
{
  	size_t phase, i, n = a.size();
	# pragma omp parallel num_threads(thread_count)\
    default(none) shared(a, n) private(i, phase)
  	for(phase = 0; phase < n; ++phase)
  	{
	    if(phase & 1)
	    {
			# pragma omp for
	      	for (i = 1; i < n - 1; i += 2)
		        if (a[i] > a[i+1])
	          		swap(a[i], a[i+1]);
    	}
    	else
    	{
			# pragma omp for
      		for (i = 1; i < n; i += 2)
        		if (a[i-1] > a[i])
          			swap(a[i-1], a[i]);
    	}
  	}
}

void odd_eve_sort()
{
	int n;
  	cin >> n;
  	vector<int> a(n);
  	for (int i = 0; i < n; ++i)
	    a[i] = rand() % 1000;

  	clock_t start, finish;
  	start = clock();

  	odd_eve_sort1(a);

  	finish = clock();
  	double time = (double(finish-start)/CLOCKS_PER_SEC);

  	cout << "Time 1: " << time << " ms" << endl;

  	start = clock();

  	odd_eve_sort2(a);

  	finish = clock();
  	time = (double(finish-start)/CLOCKS_PER_SEC);

  	cout << "Time 2: " << time << " ms" << endl;
}

int main(int argc, const char *argv[])
{
  	thread_count = strtol(argv[1], NULL, 10);

  	trapezoide();
  	pi();
  	odd_eve_sort();
  	
  	return 0;
}