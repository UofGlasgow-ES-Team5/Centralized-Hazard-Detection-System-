#include<iostream>
#include<omp.h>

void printingDisaster() {
    #pragma omp prallel num_threads(4)
    {
        std::cout<<"My thread number is:";
        std::cout<<omp_get_thread_num();
        std::cout<<"\n";

    }
}

int main(){
    std::cout<<"The first parallel program\n";
    printingDisaster();
    return 0;
}