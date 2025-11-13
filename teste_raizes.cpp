#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "numerical_methods.hpp"

double oscilation_equation(double d){
    return 1.0*power(2.71828, d) - 4*(d*d);
}

double oscilation_derivative(double d){
    return power(2.71828, d) - 8*d;
}

double equation(double x){
    return x*x*x - 9*x + 3.0;
}

double derivative(double x){
    return 3*(x*x) - 9;
}

int main(){
    // std::vector<interval> intervals = find_root_intervals(equation);
    
    // if (intervals.empty()) {
    //     printf("Nenhum intervalo de raiz foi encontrado.\n");
    // } else {
    //     printf("Encontrado %zu intervalos de raiz:\n", intervals.size());
    //     for (const auto& iv : intervals) {
    //         printf("Intervalo: [%f, %f]\n", iv.lower, iv.upper);
    //     }
    // }

    std::vector<double> roots = new_raph(0.0001, 0.5, equation, derivative);

    if(roots.empty()){
        printf("Nenhuma raíz encontrada.\n");
    } else {
        printf("Encontrado %zu raízes: \n", roots.size());
        for(const auto& r : roots){
            printf("Raiz: %f\n", r);
        }
    }

    //printf("%.15f\n", power(4, 0.23));
    
    return 0;
}