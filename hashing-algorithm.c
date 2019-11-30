#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

typedef struct{ // nokta structý x ve y degerlerinden oluþur.
	
	int x;
	int y;
	int label;
	
}Nokta;

typedef struct {  // ikililer structýdýr 2 tane noktanýn indisi ve birbirlerine olan uzaklýklarý tutulur.
	
	int n1Ind;
	int n2Ind;
	double uzaklik;
}Ikili;


void dosyaOku(Nokta *dizi,int n){ // noktalar.txt dosyasýndan deðerler okunur.
	int i,tmp,z=0,y=0,ite=1;
	char x;
	FILE *fp;
	
	fp = fopen("Input.txt","r");
	
	for(i=0;i<n;i++){		
		fscanf(fp,"%d",&dizi[i].x);
		fscanf(fp,"%d",&dizi[i].y);			
	}
	
	
}

double distance(Nokta n1,Nokta n2){		// 2 nokta arasý uzaklýgý hesaplayan fonksiyon
	return sqrt(pow((n1.x-n2.x),2)+pow((n1.y-n2.y),2));	
}

int partition(Nokta *arr,int p,int r){  // quicksortun partition kýsmý
	
	int i,j;
	Nokta tmp,pivot;
	
	pivot=arr[p];
	i=p;
	j=r;
	
	while(i<j)
	{
		while(arr[i].x<=pivot.x)
			i++;
			
		while(arr[j].x>pivot.x)
			j--;
			
		tmp=arr[i];
		arr[i]=arr[j];
		arr[j]=tmp;
	}
	tmp=arr[i];
	arr[i]=arr[j];
	arr[j]=tmp;

	tmp=arr[j];
	arr[j]=arr[p];
	arr[p]=tmp;
	
	return j;
}

void quickSort(Nokta *arr,int p,int r){ // quicksort sýralama algoritmasý
	int s;
	if(p<r)
	{
		s=partition(arr,p,r);
		quickSort(arr,p,s-1);
		quickSort(arr,s+1,r);		
	}
}


Nokta* sirala(Nokta *dizi,int n){ // noktalarý sýralayan fonksiyonudur.
	
	int i,j;
	Nokta tmp;
	
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			if(dizi[i].x>dizi[j].x){
				tmp=dizi[j];
				dizi[j]=dizi[i];
				dizi[i]=tmp;
			}
		}
	}
	return dizi;
}

Nokta *diziAyir(Nokta *dizi,int x,int n,int *med){ // dizileri saða ve sola gruplayan fonksiyondur.
	
	int i,medInd;
	n=n-x+1;
	if(n%2==1)
		*med=dizi[n/2].x;		
	else
		*med=(dizi[n/2].x+dizi[n/2-1].x)/2;	
	
	for(i=x;i<n;i++){
		if(dizi[i].x<=*med)
			dizi[i].label=0;
		else
			dizi[i].label=1;		
	}
	return dizi;	
}



void yazdir(Nokta *dizi,int n){ // diziyi yazdiran fonksiyondur
	
	int i;	
	for(i=0;i<n;i++){
		printf("%d.x= %d y= %d \n",i,dizi[i].x,dizi[i].y);
	}
}

void *diziDoldur(Nokta *dizi,int n){	// diziyi rastgele dolduran fonksiyondur.
	int i;
	srand(time(NULL));	
	for(i=0;i<n;i++){		
		dizi[i].x = ((rand()%255)*7)%50;
		dizi[i].y = ((rand()%255)*19)%50;		
	}	
}

Ikili minBul(Nokta *dizi,int x,int n){  // birbirine en yakýn 2 noktayý bulan brute force fonksiyonudur.
	
	int i,j;
	double minDist;
	Ikili min;
		
	minDist = distance(dizi[x],dizi[x+1]);
	min.n1Ind = x;
	min.n2Ind = x+1;
	min.uzaklik=minDist; 
	for(i=x;i<n-1;i++){		
		for(j=i+1;j<n;j++){			
			if(distance(dizi[i],dizi[j])<minDist){
				minDist = distance(dizi[i],dizi[j]);
				min.n1Ind = i;
				min.n2Ind = j;
				min.uzaklik=minDist; 
			}
		}		
	}	
	return min;	
}

Ikili divideConq(Nokta *dizi,int x,int n,int elemanS){ // noktalarýn tutuldugu dizi,dizinin baslangýc ve bitiþ indisi ve eleman sayýsý
	
	int i,j;
	int *med; // medyan degeri tutulur
	Ikili leftIkili,rightIkili,minIkili,tmpIkili; // ikililer tutulacaktýr
	Nokta *tmp; // medyandan d kadar ve -d kadar uzaklýkta olan noktalarýn tutulacagý dizidir.(x ekseninde)
	tmp = (Nokta*)malloc(sizeof(Nokta)*elemanS);
	
	dizi=diziAyir(dizi,x,n,med);  // dizi saða ve sola ayrýlýr.
	int mid = elemanS/2;   // dizinin ortasýndaki elemanýn indisi tutulur.
	
	if(elemanS<=3){		// recursionun durma þartý.
		minIkili = minBul(dizi,x,n);
	}
	else{				// n>3 oldugu durumda devam edecek olan kýsým
		leftIkili = divideConq(dizi,x,mid,elemanS/2+1);  // dizinin sol kýsmý fonksiyona yollanýr.
		rightIkili = divideConq(dizi,mid+1,n,elemanS-mid);	// dizinin sað kýsmý fonksiyona yollanýr.
		
		if(leftIkili.uzaklik<rightIkili.uzaklik)  // sað ve soldaki birbirine en yakýn ikililerin en küçüðü seçilir.
			minIkili=leftIkili;
		else
			minIkili=rightIkili;
		
		j=0;   // medyandan d kadar ve -d kadar uzaklýkta olan noktalarýn tutulacagý dizinin oluþturulmasý iþlemidir.
		for(i=x;i<=n;i++){
			if((dizi[i].x<=*med&&dizi[i].x>*med-minIkili.uzaklik)||dizi[i].x>=*med&&dizi[i].x<=*med+minIkili.uzaklik){
				tmp[j]=dizi[i];
				j++;
			}
		}		
		tmpIkili = minBul(tmp,0,j-1);	// bu aralýktaki birbirine en yakýn noktalar secilir.
		if(tmpIkili.uzaklik<minIkili.uzaklik){
			minIkili.n1Ind= tmpIkili.n1Ind;
			minIkili.n2Ind= tmpIkili.n2Ind;
			minIkili.uzaklik= tmpIkili.uzaklik;
		}		
	}
	yazdir(dizi,elemanS);
	printf("\n");
	
	printf("%d.nokta x = %d y = %d\n",minIkili.n1Ind,dizi[minIkili.n1Ind].x,dizi[minIkili.n1Ind].y);
	printf("%d.nokta x = %d y = %d\n",minIkili.n2Ind,dizi[minIkili.n2Ind].x,dizi[minIkili.n2Ind].y);
	printf("uzaklik = %lf\n",minIkili.uzaklik);
	printf("\n");
	
	return minIkili;
	
}

int main(){
	
	
	int n;
	Nokta *dizi;
	Ikili dene,de1;
	
	
	printf("Nokta sayisini giriniz : ");
	scanf("%d",&n);	
	dizi=(Nokta*)calloc(n,sizeof(Nokta));
	dosyaOku(dizi,n);
	printf("Noktalarin x degerlerine gore siralanmis hali :\n");
	quickSort(dizi,0,n-1);
	yazdir(dizi,n);
	printf("\n");
	printf("\n");
	printf("Baslangic:\n");
	dene=divideConq(dizi,0,n-1,n);
	printf("\n");
	printf("Sonuc:\n");
	printf("%d.nokta x = %d y = %d \n",dene.n1Ind,dizi[dene.n1Ind].x,dizi[dene.n1Ind].y);
	printf("%d.nokta x = %d y = %d \n",dene.n2Ind,dizi[dene.n2Ind].x,dizi[dene.n2Ind].y);
	printf("uzaklik = %lf \n",dene.uzaklik);
	printf("\n");
	
	
	return 0;
}
