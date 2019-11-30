#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


#define SAMPLE "Index.txt" 										// dosyalarýn adýnýn tutuldugu txt dosyasý

#define KOM1 "move C:\\Users\\FATSAN\\Desktop\\"				// masaüstündeki dosya adý gelecek komut
#define KOM2 " C:\\Users\\FATSAN\\Desktop\\AA_Hw2"				// destination pathi

#define PATH "C:\\Users\\FATSAN\\Desktop\\AA_Hw2\\"				// veritabaný klasorunun pathi



typedef struct {	// Element struct
	
	long long int key;
	char dosyaAdi[15];
	
	
}Element;


int asalMi(int k){	// bir sayýnýn asal olup olmadýgý dondurulur
	
	int div=2;
	int sqr=sqrt(k);
	int cik = 1;
	
	if(k==2||k==3)
		return 1;
	else if (k==1)
		return 0;
	while(cik&&div<=sqr){
		
		if(k%div==0){
			cik=0;
		}
		else
			div++;
		
	}
	
	if(cik==0)
		return 0;
	else
		return 1;	
	
	
	
	
	
	
}


int tabloUzunluguBul(double z){ // tablo uzunlugunu bulur
	
	int cik = 1;
	int tmp=z;
	
		
	while(cik){
		
		if(asalMi(tmp))
			return tmp;
		else
			tmp++;
		
	}
}



void createTable(Element *ht,long long int m){  // veritabanýndan dosyalar okunur ve hash tablosuna ekler hash table olusturulmus olur
	srand(time(NULL));	
	

	int cik = 1,j;
	char p[15],tempP[15];
	FILE *fp,*fp1;
	char path[50],x[50];
	int asal = 7;
	
	
	fp = fopen(SAMPLE,"r");
	
	while(!feof(fp)){
		int cik2=1;
		strcpy(path,PATH);
		fscanf(fp,"%s",&p);
		strcpy(tempP,p);
		strcat(path,p);
		strcat(path,".txt");		// dosya adýnýn sonuna .txt eklenir
		fp1= fopen(path,"r");
		long long int key=0;
		long long int temp;
		int nmb=0;
		
		
		
		while(!feof(fp1)){			// R^n her kelimenin key degeri olusturulur.
			fscanf(fp1,"%s",x);
			int tmpK=0;
			temp=pow(asal,strlen(x));
			for(j=0;j<strlen(x);j++){
				tmpK = tmpK + x[j]*temp;
				temp=temp/asal;
				
			}
			key = key+tmpK;
			key = key % 52351;
//			printf("girdi ");
		}
		printf("key = %lld\n",key);
		int i=0;
		int hash;
		hash = ((key%m) + i*(1+ key%(m-1)))%m;
		while(strcmp(ht[hash].dosyaAdi,"")&&i<m){  		// hash table'a ekleme yapýlýr 
			i++;			
			hash = ((key%m) + i*(1+ key%(m-1)))%m;;
		}
		
		if(i<m){
			ht[hash].key = key;
			strcpy(ht[hash].dosyaAdi,tempP);
		}
		
		fclose(fp1);
		
		
		
	}	
	
	
	
	
	
}




void insertFile(Element *ht,long long int m,char *dosyaAdi){ 		// dosya ekleme iþleminin yapýldýgý yer
	
	char x[50];
	int asal = 7,j;
	char tmpDosyaAdi[50];
	
	strcpy(tmpDosyaAdi,dosyaAdi);
	strcat(tmpDosyaAdi,".txt");
	
	
	FILE *fp1,*fp2;
	fp1 = fopen(tmpDosyaAdi,"r");			// ismi verilen dosya acýlýr
	
	
	if (fp1 == NULL)
 	{
	    printf("Dosya acilamadi !\n");
	    fclose ( fp1 );
	    return ;
  	}
	
	
	int nmb=0;
	long long int key=0;
	long long int temp;
	
	
	
	while(!feof(fp1)){			// dosyanýn keyi belirlenir
		fscanf(fp1,"%s",x);		// her kelime ayrý ayrý degerlendirilir.
		int tmpK=0;
		temp=pow(asal,strlen(x));		
		for(j=0;j<strlen(x);j++){
			tmpK = tmpK + x[j]*temp;		// her kelimenin harfi carpýlýr
			temp=temp/asal;
			
		}
		key = key+tmpK;
		key = key % 52351; 
				
	}
	fclose(fp1);
	printf("Yeni dosyanin adi = %s  key = %lld\n",tmpDosyaAdi,key);
	
	int i=0;
	int hash;
	hash = ((key%m) + i*(1+ key%(m-1)))%m;		// hash tablosundaki gelecegi yer belirlenir

	char path[70];
	
	int cik2 = 1;
	
	while(cik2&&i<m){	
		int cik = 1;
		if(!strcmp(ht[hash].dosyaAdi,"")){			// hash tablosundaki yer bossa oraya yerleþtirilir.
			strcpy(ht[hash].dosyaAdi,dosyaAdi);
			ht[hash].key = key;	
			char komut[100];
			strcpy(komut,"");
			strcat(komut,KOM1);
			strcat(komut,tmpDosyaAdi);
			strcat(komut,KOM2);
			char *kom;
			kom = (char*)malloc(sizeof(char)*strlen(komut));
			strcpy(kom,komut);
			
			system(kom);		// dosya tasýnýr
			FILE *tmpFp;
			
			tmpFp = fopen(SAMPLE,"a");		// dosyanýn adý sample.txt ye eklenir
			fprintf(tmpFp,"%s","\n");
			fprintf(tmpFp,"%s",dosyaAdi);
			
			fclose(tmpFp);
			
			cik2=0;
		}
		else{					// eger hash tablosundaki yer doluysa ordaki dosyanýn iceriði ile karsýlastýrýlýr.
			strcpy(path,PATH);
			strcat(path,ht[hash].dosyaAdi);
			strcat(path,".txt");
			
			fp1 = fopen(tmpDosyaAdi,"r");		// dosya acýlýr
			fp2 = fopen(path,"r");			// hash tablosundaki dosya acýlýr
			
			char str1[70];
			char str2[70];
			while(cik&&!feof(fp1)&&!feof(fp2)){		// karsýlastýrma yapýlýr
				
				fscanf(fp1,"%s",&str1);
				fscanf(fp2,"%s",&str2);
				
				if(strcmp(str1,str2))
					cik=0;				
			}
			if(cik==0){
				i++;
				hash = ((key%m) + i*(1+ key%(m-1)))%m;		// double hashingle bir sonraki yere gidilir
				
			}
			else{
				printf("Dosya zaten eklenmis ! \n");	// eger dosyalarýn icerikleri ayný ise dosya zaten vardýr
				cik2=0;
			}
			
			fclose(fp1);
			fclose(fp2);
				
		}	
		
	}
	
	if(cik2==1)			// eklenemediyse tablo doludur
		printf("Hash table dolu !\n");
	
	
}





void yazdir(Element *ht,long long int z){
	
	int i;	
	for(i=0;i<z;i++){
		printf("Dosya Adi = %s  Dosya Key = %lld\n",ht[i].dosyaAdi,ht[i].key);		
	}
	
	
	printf("\n");
	printf("\n");
	printf("\n");
}





int main(){
	
	int i,j;
	int m ;
	long long int z=0;
	char x[30];
	FILE *fp;
	
	Element *ht;
	
	
	
	fp = fopen(SAMPLE,"rw");	
	while(!feof(fp)){		// dosya sayýsý bulunur
		
		fscanf(fp,"%s",&x);
		z++;
		
	}
	z= z/0.6;			// dosya sayýsý 0.6 ile carpýlýr
	z= tabloUzunluguBul(z);
	
	printf("Tablo uzunlugu = %lld\n",z);
	ht = (Element*)malloc(sizeof(Element)*z);
	
	for(i=0;i<z;i++){
		strcpy(ht[i].dosyaAdi,"");
		ht[i].key=-1;
	}
	
/*	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (PATH)) != NULL) {
		
		while ((ent = readdir (dir)) != NULL) {	
			if(strcmp(ent->d_name,".")&&strcmp(ent->d_name,"..")){
				printf ("%s\n", ent->d_name);
				
			}	
				
		}
		closedir (dir);
	} else {
	  
	  perror ("could not open directory\n");
	
	}*/
	
	createTable(ht,z);
	
	yazdir(ht,z);
	
	insertFile(ht,z,"X");
	insertFile(ht,z,"Y");
	insertFile(ht,z,"K");
	
	
	yazdir(ht,z);
	

	
	
	return 0;
}
