#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

#define NUMHIJOS 3
int cantLetras(char cade[]);
int cantNumeros(char cade[]);
int cantPuntosYComas(char cade[]);

int main(){	
	int n=0,shm_id,i;
	pid_t hijos[NUMHIJOS], padre;
	padre=getpid();
	FILE *txt=fopen("archivo.txt","r");
	if(txt!=NULL){
		while(!feof(txt)){
		fgetc(txt);
		n++;
		}
	}
	fclose(txt);
	char *cade;
	shm_id=shmget(IPC_PRIVATE, (sizeof(char)*n), IPC_CREAT | S_IRUSR | S_IWUSR);
	cade=shmat(shm_id, 0, 0);


	for(i=0;i<NUMHIJOS;i++){
		if(!(hijos[i]=fork())){break;}
	}
	if(padre==getpid()){
		int k=0;
		FILE *txt=fopen("archivo.txt","r");
		if(txt!=NULL){
			while(!feof(txt)){
				cade[k]=fgetc(txt);
				k++;
			}
		}
		fclose(txt);
		cade[k]='\0';
		printf("%s\n", cade);
		for(int j=0;j<NUMHIJOS;j++){
			wait(NULL);
		}
		shmdt(cade);
		shmctl(shm_id, IPC_RMID, 0);
	}else{
		if(hijos[0]==0){
			int n=cantLetras(cade);
    		printf("El archivo tiene: %d letras \n", n);
    		shmdt(cade);
		}else{
			if(hijos[1]==0){
				int n=cantNumeros(cade);
    			printf("El archivo tiene: %d numeros \n", n);
    			shmdt(cade);
			}else{
				if(hijos[2]==0){
					int n=cantPuntosYComas(cade);
    				printf("El archivo tiene: %d puntos o comas \n", n);
    				shmdt(cade);
				}
			}
		}
		
	}
	return 0;
}


int cantLetras(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]>=65 && cadena[i]<=90 || cadena[i]>=97 && cadena[i]<=122){
			cont++;	
		}
		i++;
	}
	return cont;

}

int cantNumeros(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]>=48 && cadena[i]<=57){
			cont++;	
		}
		i++;
	}
	return cont;

}

int cantPuntosYComas(char cadena[]){
	int i=0, cont=0;
	while(cadena[i]!='\0'){
		if(cadena[i]==46 || cadena[i]==44){
			cont++;	
		}
		i++;
	}
	return cont;
}