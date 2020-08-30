#include <stdio.h>

int main(int argc, char const *argv[])
{
	char caracter = 0;
	int entero = 0;
	float flotante = 0;
	long largo = 0;
	double doble = 0;
	
	char *pcaracter = 0;
	int *pentero = 0;
	float *pflotante = 0;
	long *plargo = 0;
	double *pdoble = 0;

	int i = 0;
	char cadena[] = "ESCOM - IPN";

	int numero = 1234567890;
	int n1 = -1;
	int n2 = 2;

	int max_integer = 2147483648;
	
	printf("Caracter: %p\n", &pcaracter);
	printf("Entero: %p\n", &pentero);
	printf("Flotante: %p\n", &pflotante);
	printf("Largo: %p\n", &plargo);
	printf("Doble: %p\n", &pdoble);
	printf("%ld\n", sizeof(pdoble));

	// ======================== 4 ================================
	printf("\n");
	printf("Direccion de variable caracter: %p\n", &caracter);
	pcaracter = &caracter;
	printf("Direccion contenida en apuntador pcaracter: %p\n", pcaracter);

	// ======================== 5 ================================
	printf("\n");
	i = 0;
	while(cadena[i] != '\0')
	{
		printf("%p\n", &cadena[i]);
		i++;
	}

	printf("%c\n", *(cadena + 4));

	if(cadena[11] == '\0')
		printf("Fin de cadena\n");

	// ======================== 6 ================================
	printf("\n");
	printf("%d\n", numero);
	printf("%x\n", numero);

	printf("%d\n", n1);
	printf("%x\n", n1);
	printf("%d\n", n2);
	printf("%x\n", n2);

	printf("\n%d\n", max_integer);

	return 0;
}