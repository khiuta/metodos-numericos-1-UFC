#include <vector>
#include <functional>
#include <iostream>

struct interval {
    double lower;
    double upper;
};

double absol(double x){
    if(x < 0) return -x;
    else return x;
}

double root(double x, double num, double den);
double my_ln(double x);
double expo(double x);

double factIter(double x, double acc){
    if(x<0){
        std::cout << "Erro: Fatorial de número negativo." << std::endl;
        return -1;
    }
    if(x==0) return acc;
    if(x==1) return acc;
    return factIter(x-1, acc*x);
}

double fact(double x){
    return factIter(x, 1.0);
}

double powerIter(double b, double e, double acc){
    if(e == 0) return acc;
    if(e < 0) {
        return powerIter(1/b, absol(e), acc);
    }
    if(int(e) != e){
        // usa a definição a^b = e^{b*ln(a)}
        // primeiro calcula o ln
        double ln = my_ln(b);
        double exp = e*ln;
        // agora fazemos e^exp
        return expo(exp);
    }
    return powerIter(b, e-1, acc*b);
}

double power(double b, double e){
    return powerIter(b, e, 1.0);
}

double my_ln(double x){
    double y = (x-1)/(x+1);
    // ln(x) = 2*(y/1 + y³/3 + y⁵/5 + y⁷/7 ...)
    // aproximando com 30 iterações
    double result = 0;
    double y_pow = power(y, 1);
    for(int i = 0; i < 30; i++){
        result += y_pow/(2*i+1);
        y_pow *= y*y;
    }
    result *= 2;

    return result;
}

double expo(double x){
    // e^x = 1 + x + x²/2! + x³/3! + x⁴/4! ...
    // aproximando com 30 iterações
    double result = 0;
    double x_pow = power(x, 0);
    double i_fact = fact(0);
    for(int i = 0; i < 30; i++){
        result += x_pow/i_fact;
        x_pow *= x;
        i_fact *= i+1;
    }

    return result;
}

double rootIter(double x, double guess, double den){
    std::cout << guess << std::endl;
    if(absol(x-power(guess, den))/x < 0.000001) return guess;
    return rootIter(x, (guess + x/guess)/2.0, den);
}

double root(double x, double num, double den){
    std::cout << num << std::endl;
    return rootIter(power(x, num), 1, den);
}

std::vector<interval> find_root_intervals(std::function<double(double)> equation){
    std::vector<interval> intervals;
    // procurando raízes entre -100 e 100
    for(int i = -100; i < 100; i++){
        double val1 = equation(i);
        double val2 = equation(i+1);

        if(val1*val2 < 0){
            interval t;
            t.lower = i;
            t.upper = i+1;
            intervals.push_back(t);
        }
    }
    return intervals;
}


// ROBUSTAS

std::vector<double> false_position(std::vector<interval> intervals, float threshold, std::function<double(double)> equation){
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
            if(absol(curr_lower - curr_upper) < threshold){
                double middle = (curr_upper+curr_lower)/2;
                roots.push_back(middle);
                break;
            }
        }
    }
    return roots;
}

std::vector<double> bissection(std::vector<interval> intervals, float threshold, std::function<double(double)> equation){
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
            if(absol(curr_lower-curr_upper) < threshold){
                double novo_meio = (curr_upper+curr_lower)/2;;
                roots.push_back(novo_meio);
                break;
            }
        }
    }
    return roots;
}

// RÁPIDAS
std::vector<double> new_raph(float threshold, double x0, std::function<double(double)> equation, std::function<double(double)> derivative){
    std::vector <double> roots;
    if(absol(equation(x0)) < threshold) {
        roots.push_back(x0);
        return roots;
    }
    for(int i = 0; i < 100; i++){
        int x1 = x0 - equation(x0)/derivative(x0);
        if(absol(equation(x1)) < threshold || absol(x1-x0) < threshold){
            roots.push_back(x1);
            break;
        }
        x0 = x1;
    }
    return roots;
}