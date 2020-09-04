#include <string.h>
#include <stdio.h>


#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>


/*Antonio Junco de Haas A01339695*/
/*Christian Alberto Dalma*/

/*----------------------------------------------Metodos----------------------------------------------*/
/*Abre los directorios de manera recursiva*/
long long int* listFilesRecursively(char *path, long long int * vector);

/*Regresa el tipo de archivo, ya sea dir o file*/
unsigned char statFileType(char *fname);
/*stat para crear stats*/
int stat(const char*path, struct stat *buf);

/*Revisar si hay que crear urna, y agregar valor*/
long long int *revisarUrnas( long long int * vector, int valor);

/*Todo el directorio hasta el archivo*/
char *getFullName(char *ruta, struct dirent *ent);

/*Imprime el vector de valores*/
void printVector(long long int *vector);
/*Regresa la urna*/
long long int getUrna(long long int *vector);
/*Regresa la cantidad de urnas*/
long long int getNumero(long long int *vector);

/*Nuevo numero de urnas colocado*/
long long int *setNumero(long long int *vector, int nuevonum);

/*Previene fallas en memoria creada*/
long long int *clearNew(long long int *vector, int lastvalue);

/*Acumula un valor*/
long long int *accumulate(long long int *vector, int position);

/*Cual es la urna con mas archivos*/
int maximoArchivos (long long int *vector);

/*Imprime histograma*/
void imprimirHistograma(long long int *vector);
/*----------------------------------------------Metodos----------------------------------------------*/







int main(int argc, char *argv[]){

  int urna=1024;
  int numerournas=1;
  int limiteurna=urna*numerournas;


  /*Se ponen 3, ya que el primer valor son cantidad de urnas, el segundo el valor de urnas, y el tercero es la primera urna*/
  long long int * vector = (long long int *) malloc(3 * sizeof(long long int));
	long long int * final = vector + 3;


  /*Preparing the vector*/
  int coun=0;
  for (long long int * aux = vector; aux < final; ++aux) {
		if(coun==0){
      *aux=numerournas;
		}
			else if (coun==1){
			*aux=urna;
		}
    coun++;
	}

  getUrna(vector);
  getNumero(vector);
  //printVector(vector);
  //free(vector);
  //return(0);







  struct dirent *pDirent;
  DIR *pDir;

  // Ensure correct argument count.
  if (argc != 2) {
      printf ("Usage: testprog <dirname>\n");
      return 1;
  }

  // Ensure we can open directory.
  pDir = opendir (argv[1]);
  if (pDir == NULL) {
      printf ("Cannot open directory '%s'\n", argv[1]);
      return 1;
  }

  vector=listFilesRecursively(argv[1],vector);
  //printVector(vector);

  imprimirHistograma(vector);
  free(vector);
  closedir(pDir);
}

long long int * listFilesRecursively(char*basePath, long long int *vector){
  char path[1000];
  struct dirent *dp;
  DIR *dir=opendir(basePath);
  if (!dir){
    return vector;
  }
  while ((dp=readdir(dir))!=NULL){
    if (strcmp(dp->d_name, ".")!=0&&strcmp(dp->d_name,"..")){
      //long int a =getFileSize(dp->d_name);

      struct stat st;
      stat(dp->d_name,&st);
      long long int size=st.st_size;
      //int f=st.st_mode;

      char *nombrecompleto=getFullName(basePath, dp);
      //printf("%s: \n", nombrecompleto);
      unsigned char a=statFileType(nombrecompleto);
      if(a==DT_DIR){
        //printf("DIRECTORY \n");
      }
      else{
        struct stat data;
        stat(nombrecompleto,&data);
        //printf("FILE ");
        //printf("%lld: %s",(long long)data.st_size, nombrecompleto);
        //printf("nextFile");

        /*Agregar a urna*/
        vector=revisarUrnas(vector, (long long)data.st_size);

        //printf("\n");
      }
      //printf("%s: \n", dp->d_name);

      strcpy(path, basePath);
      strcat(path,"/");
      strcat(path,dp->d_name);
      vector=listFilesRecursively(path,vector);
      //closedir(dp);
    }
  }
  return vector;
  closedir(dir);
}




unsigned char statFileType(char *fname)
{
  struct stat sdata;
  /* Intentamos el stat() si no funciona, devolvemos tipo desconocido */
  if (stat(fname, &sdata)==-1)
    {
      return DT_UNKNOWN;
    }

  switch (sdata.st_mode & S_IFMT)
    {
    case S_IFBLK:  return DT_BLK;
    case S_IFCHR:  return DT_CHR;
    case S_IFDIR:  return DT_DIR;
    case S_IFIFO:  return DT_FIFO;
    case S_IFLNK:  return DT_LNK;
    case S_IFREG:  return DT_REG;
    case S_IFSOCK: return DT_SOCK;
    default:       return DT_UNKNOWN;
    }
}

char *getFullName(char *ruta, struct dirent *ent)
{
  char *nombrecompleto;
  int tmp;

  tmp=strlen(ruta);
  nombrecompleto=malloc(tmp+strlen(ent->d_name)+2); /* Sumamos 2, por el \0 y la barra de directorios (/) no sabemos si falta */
  if (ruta[tmp-1]=='/')
    sprintf(nombrecompleto,"%s%s", ruta, ent->d_name);
  else
    sprintf(nombrecompleto,"%s/%s", ruta, ent->d_name);

  return nombrecompleto;
}


long long int * revisarUrnas(long long int * vector, int valor){

  //Agregar las urnas para satisfacer el limite apropiado
  int urna= getUrna(vector);
  int cantidad=getNumero(vector);
  int limiteurna=(urna*cantidad);
  int lastvalue=getNumero(vector);


  while (valor>=limiteurna){
    cantidad++;
    limiteurna=urna*cantidad;

    vector = (long long int *) realloc(vector, sizeof(long long int) * (cantidad+2));

    vector=setNumero(vector, cantidad);
    vector =clearNew(vector, cantidad);
  }

  /*Insertar el valor a urna*/
  int pos=valor/urna;
  long long int * final = vector + cantidad+2;
  /*-2 para no contar el size y urna*/
  int counter=0;
  long long int * naux;
  int flag=0;
  for (naux = vector; naux < final; ++naux) {

    if ((counter>(pos+1))&&(flag==0)){
      *naux=(*naux)+1;
      vector=accumulate(vector,counter);
      flag=1;
    }

    counter++;
  }
  return(vector);

}
void printVector(long long int *vector){
  printf("\n\nprinting vector: \n");
  long long int * aux = vector;
  long long int *final=vector + *aux+2;
  int counter=0;

  for (aux; aux < final; ++aux) {

		printf("counter: %d value: %lld\n",counter,*aux);
    counter++;
	}
}

/*Regresa el segundo valor de el vector, que es el numero de urnas*/
long long int getUrna(long long int *vector){
  long long int * aux = vector;
  long long int *final=++aux;
  //printf("Urna: %lld\n",*final);
  return *final;

}

/*Regresa el primer valor de el vector, que es el numero de urnas*/
long long int getNumero(long long int *vector){

  long long int * aux = vector;

  return *aux;

}

long long int *setNumero(long long int *vector, int nuevonum){

  *vector=nuevonum;

  return vector;
}

long long int *clearNew(long long int *vector, int lastvalue){
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>lastvalue){
      //printf("CLEARING: %lld",*aux);
		  *aux=0;
    }
    count++;
	}
  return vector;
}


long long int *accumulate(long long int *vector, int position){
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count==(position+2)) {
      //printf("Added new: %lld",*aux++);
		  *aux++;;
    }
    count++;
	}
  return vector;

}

int maximoArchivos(long long int *vector){
  int max=0;
  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>=2) {
      if(max<*aux)
        max=*aux;
    }
    count++;
	}
  return max;
}


void imprimirHistograma(long long int *vector){
  int max=maximoArchivos(vector);
  double asteriscos=40.0;
  int total=getNumero(vector);
  int urna=getUrna(vector);
  double array [total];
  int array2[total];
  //printf("Total de urnas: %d",total);

  int count=0;
  long long int *aux=vector;
  long long int *final=vector + *aux+2;
  for (aux=vector; aux < final; ++aux) {
    if (count>=2) {
      array[count-2]=(*aux*asteriscos)/max;
      array2[count-2]=*aux;
      //printf("AUX, %lld, MAX, %d, Division, %f\n",*aux,max,array[count-2]);
    }
    count++;
	}
  printf("%19s %11s %10s\n","Urnas"," #de Archivos","Histograma");
  for(int i=0;i<total;i++){
    printf("%8d - %8d",i*urna,(i+1)*urna-1);
    printf("%15d",array2[i]);
    while(array[i]>0){
      printf("*");
      array[i]--;
    }
    printf("\n");
  }

}
