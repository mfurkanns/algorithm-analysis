#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S 300000    // graph size


typedef struct Node{
	char name[150];
	int type;  // 0 film , 1 oyuncudur, -1 ise bostur
	struct Node *next;	
	int ancestor;
	int place;
	
}Node;

typedef struct Graph{		// graph struct
	struct Node **list;
	int count;
	
}Graph;

typedef struct Que{		// BFS te kuyruk yapýsý kullanacaðýz.
	
	int dizi[25000000];
	int rear;
	
}Que;

int key(char *name){	// stringlere key ureten fonksiyon
	
	int i,R=1;
	int key = 0;
	
	for(i=0;i<strlen(name);i++){
		key = key + R*name[i];
		R=R*1;
	}
	return key;	
}

void insertNode(Graph *graph,Node *node,int key){	// grapha node ekleyen fonksiyon
	
	int i = 0;
	int adr = key % S;
	int ad = key % S;
	long long int j=0;
	Node *tmp = graph->list[adr];
	while((strlen(tmp->name)!=0) && i<S){			
		adr = (ad + (j*j)%S) % S;
		tmp = graph->list[adr];
		j++;
		i++;
	}
	if(i<S){
		node->place = adr;
		graph->list[adr] = node;
		graph->count++;
	}	
}

int isEmpty(Que *q){	// kuyruðun boþ olup olmadýðý döndürülür.
	if(q->rear==0)
		return 1;
	else
		return 0;
}       

int isFull(Que *q){		// kuyruðun dolu olup olmadýðý döndürülür.
	if(q->rear==S)
		return 1;
	else
		return 0;
}      

int enQueue(Que *q,int x){		// kuyruða yeni eleman ekler.	
	if(isFull(q)==1){
		return 0;
	}
	else{
		q->dizi[q->rear]=x;
		q->rear++;
		return 1;
	}
}

int deQueue(Que *q){		// kuyruktan eleman çýkar ve çýkan eleman döndürülür.
	int x,i,item;
	if(isEmpty(q))
		return 0;
	else{
		item=q->dizi[0];
		for(i=1;i<=q->rear;i++){
			q->dizi[i-1]=q->dizi[i];
		}
		q->rear--;
	}
	return item;
}

int bul(Graph *graph,char *name){	// ismi verilen node'un graphýn kaacýncý gozunde oldugunu bulan fonksiyon
	
	int anahtar = key(name);
	long long int i=0, j = 0;
	int adr;
	int ad = anahtar % S;
	adr = anahtar % S;
	Node *tmp = graph->list[adr];
	while( (strcmp(tmp->name,"")) && (strcmp(tmp->name,name)) && (i<S) ) {
		adr = (ad + (j*j)%S) % S;
		tmp = graph->list[adr];
		j++;
		i++;
	}
	if(!strcmp(tmp->name,name)){		
		return adr; // eger ismi verilen node graptaysa graphtaký yerý dondurulur
	}
	else{
		return -1; // eger ismi verilen node graph'ta yoksa -1 dondurulur
	}
}

void breadthFirstSearch(Graph *graph,int source,int destination,char *sourceName,char *destinationName){	// bfs
	// parametre olarak source, destination indeksleri ve source, destination oyuncunlarýnýn isimleri alýnýr
	Que *q=(Que*)malloc(sizeof(Que));	// kuyruk oluþturulur.
	q->rear=0;
	int girdiMi[S];		// node'un kuyruða girip girmediðini kontrol ettiðimiz bir dizi mevcut.
	int v,i;
	Node *tmp ;
	for(i=0;i<S;i++){			// ilk baþta dizi sýfýrlanýr.Çünkü henüz kuyruða giren yok .
		girdiMi[i]=0;
	}
	
	enQueue(q,source);			// ilk kelimenin graphtaki yeri kuyruða eklenir.
	girdiMi[source]=1;			// kuyruða girdiði için kontrol dizimizin ilgili indisi 1 yapýlýr.Yani kuyruða girdiði iþaretlenir.
	
	while(!isEmpty(q)){	// kuyruk tamamen boþalana kadar veya çýkýþ düðümüne ulaþana kadar döngüde kalýnýr.
	
		v=deQueue(q);			// kuyruktan eleman cekilir
		tmp = graph->list[v];  // kuyruktan cýkarýlan node'un komsularý gezilir
		
		if(!strcmp(tmp->name,destinationName)){		// kuyruktan çýkan elemanýn aradgýmýz eleman olup olmadýgý kontrol edilir.
			Node *yaz = tmp;	// uzaklýgý bulmak icin gecici bir node olusturulur.
			int sayi = 0;		// uzaklýk bulmak ýcýn kullanýlýr.
			while(yaz->ancestor!=-1){	// gecici node uzaklýgý bulmak ýcýn kullanýlýr.
				if(yaz->type == 0) // eger ilgili node film ise sayi arttýrýlýr.
					sayi++;
				yaz = graph->list[yaz->ancestor];
			}			
			if(sayi>6){ // eger uzaklýk 6'dan fazla ise mesaj yazdýrýlýr.
				printf("Uzaklik = %d , 6'dan buyuk\n",sayi);
				return;
			}
			else{	// eger uzaklýk 6'dan kucuk ise.
				printf("Uzaklik = %d\n",sayi);
				while(tmp->ancestor!=-1){	// node'un nereden geldigi bulunur.
					if(tmp->type == 1)	// eger oyuncu ise.
						printf("Oyuncu = ");
					else
						printf("Film = ");	// eger film ise.
					printf(" %s ---> ",tmp->name);	// isim yazdýrýlýr.
					tmp = graph->list[tmp->ancestor]; // bir ust atasýna gidilir.
				}
				return ;
			}				
		}
		else{										// kuyruktan cýkan eleman aradgýmýz eleman degýlse. 
			while(tmp!=NULL){					// o kelimenin komþularýnýn bulunduðu dizi gezilir.
				long long int anhtr = bul(graph,tmp->name);
				if(girdiMi[anhtr]==0){				// ve kuyruða girmediyse
					enQueue(q,anhtr);				// kuyruða eklenir.
					girdiMi[anhtr]=1;				// kuyruða girdiði iþaretlenir.
					Node *yaz = graph->list[anhtr]; // gecici node olusturulur.
					yaz->ancestor = v;				// aatasý isaretlenir.
				}
				tmp = tmp->next;					// komsularý gezilir.
			}
		}
	}
	
	if(isEmpty(q))			// kuyruk bossa baglantý yoktur.
		printf("Baglanti yok !\n");	
}

int main(){
	
	FILE *fp;	// dosya okunacak.
	char satir[3000];	// satýr satýr okunacak.
	int i;			// dongu indisi.
	char *parca; // strtok ile parcalanacaak olan parca
	char *tmpP; // gecici degisken
	Graph *graph;
		
	graph = (Graph*)malloc(sizeof(Graph));	// graph tanýmlanýr
	graph->list = (Node**)malloc(sizeof(Node*)*S); // graph allocate edilir
	graph->count= 0;
	
	for(i=0;i<S;i++){ // graph allocate edilir
		graph->list[i] = (Node*)malloc(sizeof(Node));
		Node *tmp = graph->list[i];
		tmp->type = -1;	// tipi belirsizdir, yani bos oldugu anlamýna gelir
		tmp->ancestor = -1;	// atasý yoktur
	}
	
	fp = fopen("input-mpaa.txt","r");
	if(fp==NULL){
		printf("Dosya acilamadi ! \n");
		return 0;
	}
	printf("Graph olusturuluyor ...\n");
	while(!feof(fp)){
		Node *film,*tmpFilm;
		int anahtarF;
		film = (Node*)malloc(sizeof(Node));	// dosyadaki ilk isim film ismidir
		tmpFilm = (Node*)malloc(sizeof(Node));
		
		fgets(satir,sizeof(satir),fp);			// satýr satýr okuma yapýlýr
		parca = strtok(satir,"/\n");		// satýr parcalanmaya baslar
		
		strcpy(film->name,parca);	// filmin adý alýnýr
		film->type = 0;	// node'un tipi film olarak isaretlenir
		
		anahtarF = key(parca); // filme key uretilir
		
		tmpFilm = film;
		while(parca!=NULL){	// satýr parcalanmaya devam eder						
			Node *oyuncu; // oyuncu node olusturulur
			oyuncu = (Node*)malloc(sizeof(Node));
			int anahtarO ; // oyuncu ýcýn býr key uretýlýr
			parca = strtok(NULL,"/\n"); // '/' ve alt satýr ýsaretlerine gore parcalanýr
			
			if(parca!=NULL){ // sona gelene kadar
				strcpy(oyuncu->name,parca);
				
				oyuncu->type = 1; // node'un oyuncu oludugu ýsaretlenýr
				film->next = oyuncu; // oyuncu node'u film node'u komsu olarak eklenir
				film=film->next;
				anahtarO = key(oyuncu->name); // oyuncuya key uretýlýr
				if(bul(graph,oyuncu->name)==-1){ // eger graphta yoksa grapha eklenir
					Node *tmpOYUN = (Node*)malloc(sizeof(Node)); // gecici oyun node'un olusturulur
					strcpy(tmpOYUN->name,parca); // ismi kopyalanýr
					tmpOYUN->type = 1; // node'un tipi oyuncu olarak isaretlenir
					Node *newNode1 = (Node*)malloc(sizeof(Node)); // yeni bir tmp film node'u olusturulur
					strcpy(newNode1->name,tmpFilm->name); // isim kopyalanýr
					newNode1->type = 0;		// node'un tipi film olarak isaretlenir
					newNode1->next = NULL;
					tmpOYUN->next = newNode1;	// oyun node'una komsu olarak film eklenir
					insertNode(graph,tmpOYUN,anahtarO);	// olusturulan node grapha eklenir
				}
				else{ // eger node daha onceden graha eklenmisse
					int aa = bul(graph,oyuncu->name); // node'un graphtaký yerý alýnýr
					Node *oyTmp = graph->list[aa];   // tmp node'a atanýr
					Node *newNode = (Node*)malloc(sizeof(Node)); // yeni film node'u olusturulur
					strcpy(newNode->name,tmpFilm->name); // filmin adý kopyalanýr
					newNode->type = 0;	// node'un tipi film olarak isaretlenir
								
					newNode->next = oyTmp->next;
					oyTmp->next = newNode; // oyuncu node'una komsu olarak film node'u eklenýr
				}
			}
		}
		insertNode(graph,tmpFilm,anahtarF); // film node'u grapha eklenir
	}
	fclose(fp);
	char sourceName[50]; // 1. oyuncu ismini tutar
	char destinationName[50]; // 2. oyuncu ismini tutar
	int source; // oyuncunun graphtaki yerini tutar
	int destination; // aranmak istenen oyuncunun graphtaki yerini tutar
	int secenek;
	
	
	printf("Graph olusturuldu ! \n");

		
	printf("(1) Bir oyuncunun Kevin Bacon sayisini bul\n");
	printf("(2) Iki oyuncunun mesafesini bul\n");
	printf("Secenek = ");
	
	scanf("%d",&secenek);
	
	if(secenek == 1){
		printf("Oyuncunun Adi = ");
		fflush(stdin);
		gets(sourceName);
		strcpy(destinationName,"Bacon, Kevin");
		source = bul(graph,sourceName);
		destination = bul(graph,destinationName);
		if(source == -1){
			printf("Yanlis oyuncu ismi girdiniz\n");		
		}
		else{
		printf("Araniyor ...\n");
			breadthFirstSearch(graph,destination,source,destinationName,sourceName);
		}
		printf("\n");
	}
	else if(secenek == 2){
		printf("1.oyuncunun Adi = ");
		fflush(stdin);
		gets(sourceName);
		printf("2.oyuncunun Adi = ");
		gets(destinationName);
		source = bul(graph,sourceName);
		destination = bul(graph,destinationName);
		if(source == -1 || destination == -1){
			printf("Yanlis oyuncu ismi girdiniz\n");
	
		}
		else{
			
			printf("Araniyor ...\n");
			breadthFirstSearch(graph,destination,source,destinationName,sourceName);
		}
		printf("\n");		
	}
	else{
		printf("Yanlis tuslama yaptiniz\n");
		printf("\n");
	}
	
	
	return 0 ;
}
