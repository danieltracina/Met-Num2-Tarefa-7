#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define H 0.1

typedef double (*p_func)(double,double* );

double f(double t, double *y) {
	return(y[1]);
}

double g(double t, double *y) {
	return(100*y[0]);
}

double *k_1(double t, double *y, p_func *sistema) {
	double *k1;
	
	k1 = malloc(2*sizeof(double));
	
	for(int i=0; i<2; i++)
		k1[i] = sistema[i](t,y);
	
	return(k1);
}

double *sum_vector(double *a, double *b) {
	double *c = malloc(2*sizeof(double));
	
	for(int i=0; i < 2; i++)
		c[i] = a[i] + b[i];
	
	return(c);
}

double *mult_vector(double *a, double k) {
	double *c = malloc(2*sizeof(double));
	
	for(int i=0; i < 2; i++)
		c[i] = a[i] * k;
	
	return(c);
}

double *k_2(double t, double *y, double *k1, p_func *sistema) {
	double *k2;
	
	k2 = malloc(2*sizeof(double));
	
	
	for(int i=0; i<2; i++)
		k2[i] = sistema[i](t+H/2.0,sum_vector(y,mult_vector(k1,H/2.0)));
	
	return(k2);
}

double *k_3(double t, double *y, double *k2, p_func *sistema) {
	double *k3;
	
	k3 = malloc(2*sizeof(double));
	
	
	for(int i=0; i<2; i++)
		k3[i] = sistema[i](t+H/2.0,sum_vector(y,mult_vector(k3,H/2.0)));
	
	return(k3);
}

double *k_4(double t, double *y, double *k3, p_func *sistema) {
	double *k4;
	
	k4 = malloc(2*sizeof(double));
	
	
	for(int i=0; i<2; i++)
		k4[i] = sistema[i](t+H,sum_vector(y,mult_vector(k3,H)));
	
	return(k4);
}

int runge_kuta(double x, double b, double *y, p_func *sistema, char *nome) {
	FILE *fp = fopen(nome, "w");
	int count = 0;
	double *k1, *k2, *k3, *k4, *aux;
	
	
	while(x <= b) {
		fprintf(fp, "%lf\t\t%lf\n", x, y[0]);
		
		k1 = k_1(x, y, sistema);
		k2 = k_2(x, y, k1, sistema);
		k3 = k_3(x, y, k2, sistema);
		k4 = k_4(x, y, k3, sistema);
		
		aux = mult_vector(sum_vector(k2,k3),2);
		aux = sum_vector(sum_vector(k1,k4),aux);
		aux = mult_vector(aux,H/6.0);
		
		y = sum_vector(y,aux);
		x += H;
		
		count++;
	}
	
	fclose(fp);
	return(count);
}

void geraSolucao(char *arq1, char *arq2, double f_b, int tamanho) {
	FILE *arq_1 = fopen(arq1,"r"), *arq_2 = fopen(arq2,"r"), *arq_3 = fopen("solucao.txt", "w");
	double aux, x[tamanho], y_1[tamanho], y_2[tamanho], y_3[tamanho];
	
	for(int i=0; i<tamanho; i++) {
		fscanf(arq_1,"%lf\t\t%lf", &x[i], &y_1[i]);
		fscanf(arq_2,"%lf\t\t%lf", &aux, &y_2[i]);
	}
	
	aux = (f_b-y_1[tamanho-1])/y_2[tamanho-1]; 
	
	for(int i=0; i<tamanho; i++) { 
		y_3[i] = y_1[i] + aux*y_2[i];
		fprintf(arq_3, "%lf\t\t%lf\n", x[i], y_3[i]);
	}
	
	fclose(arq_1);
	fclose(arq_2);
	fclose(arq_3);
}
	
	
	
int main() {
	int tamanho;
	p_func sistema[2] = {f,g};
	double a = 0, b = 1, y[2] = {1.0,0.0}, y_1[2] = {0.0,1.0}, f_b = exp(-10);
	
	tamanho = runge_kuta(a,b,y,sistema,"solucao1.txt");
	tamanho = runge_kuta(a,b,y_1,sistema,"solucao2.txt");
	
	//printf("\nTam.: %d\n", tamanho);
	
	geraSolucao("solucao1.txt","solucao2.txt", f_b, tamanho);
	
	return(0);
}