#include <stdio.h>
#include <vector>
#include <cmath>

struct interval {
    double lower;
    double upper;
};

double equation(double x){
    return x*x*x - 9*x + 3;
}

std::vector<interval> find_root_intervals(){
    std::vector<interval> intervals;
    // procurando raízes entre -100 e 100
    for(int i = -100; i < 100; i++){
        long long val1 = equation(i);
        long long val2 = equation(i+1);

        if(val1*val2 < 0){
            interval t;
            t.lower = i;
            t.upper = i+1;
            intervals.push_back(t);
        }
    }
    return intervals;
}

std::vector<double> bissection(std::vector<interval> intervals, float threshold){
    std::vector<double> roots;
    for(const auto& t : intervals){
        double curr_lower = t.lower; // limite inferior atual
        double curr_upper = t.upper; // limite superior atual
        // tentando achar com 100 iterações
        for(int i = 0; i < 100; i++){
            double meio = (curr_upper+curr_lower)/2;
            // evaluando a função no meio e nos limites do intervalo
            double valor_meio = equation(meio);
            double valor_lower = equation(curr_lower);
            double valor_upper = equation(curr_upper);
            // troca de sinal
            if(valor_meio * valor_lower < 0){
                curr_upper = meio;
            }
            else if(valor_meio * valor_upper < 0){
                curr_lower = meio;
            }
            // intervalo menor que o limite de erro
            if(std::abs(curr_lower-curr_upper) < threshold){
                double novo_meio = (curr_upper+curr_lower)/2;;
                roots.push_back(novo_meio);
                break;
            }
        }
    }
    return roots;
}

std::vector<double> false_position(std::vector<interval> intervals, float threshold){
    std::vector<double> roots;
    for(const auto& t : intervals){
        double curr_lower = t.lower; // current lower bound
        double curr_upper = t.upper; // current upper bound
        // trying to find with a 100 iterations
        for(int i = 0; i < 100; i++){
            // finding x_i
            // (a(f(b)) - b(f(a)))/(f(b)-f(a))
            double denominator = equation(curr_upper) - equation(curr_lower);
            double xi = (curr_lower*(equation(curr_upper)) - curr_upper*(equation(curr_lower)))/denominator;
            // evaluating the function on the interval bounds and xi 
            double lower_value = equation(curr_lower);
            double upper_value = equation(curr_upper);
            double xi_value = equation(xi);
            // looking for signal change
            if(lower_value*xi_value < 0){
                curr_upper = xi;
            }
            else if(xi_value*upper_value < 0){
                curr_lower = xi;
            }
            else if(xi_value == 0) {
                roots.push_back(xi);
                break;
            }
            // interval size is lesser than the threshold
            if(std::abs(curr_lower - curr_upper) < threshold){
                double middle = (curr_upper+curr_lower)/2;
                roots.push_back(middle);
                break;
            }
        }
    }
    return roots;
}

int main(){
    std::vector<interval> intervals = find_root_intervals();
    
    if (intervals.empty()) {
        printf("Nenhum intervalo de raiz foi encontrado.\n");
    } else {
        printf("Encontrado %zu intervalos de raiz:\n", intervals.size());
        for (const auto& iv : intervals) {
            printf("Intervalo: [%f, %f]\n", iv.lower, iv.upper);
        }
    }

    std::vector<double> roots = false_position(intervals, 0.01);

    if(roots.empty()){
        printf("Nenhuma raíz encontrada.\n");
    } else {
        printf("Encontrado %zu raízes: \n", roots.size());
        for(const auto& r : roots){
            printf("Raiz: %f\n", r);
        }
    }
    
    return 0;
}