set xrange[0:1]
y(x) = exp(2)*(1/(exp(4)-1))*(exp(2*x)-exp(-2*x))+x
plot y(x), "solucao.txt" 