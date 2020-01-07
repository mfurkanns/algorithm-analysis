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

typedef struct Que{		// BFS te kuyruk yap�s� kullanaca��z.
	
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

int isEmpty(Que *q){	// kuyru�un bo� olup olmad��� d�nd�r�l�r.
	if(q->rear==0)
		return 1;
	else
		return 0;
}       

int isFull(Que *q){		// kuyru�un dolu olup olmad��� d�nd�r�l�r.
	if(q->rear==S)
		return 1;
	else
		return 0;
}      

int enQueue(Que *q,int x){		// kuyru�a yeni eleman ekler.	
	if(isFull(q)==1){
		return 0;
	}
	else{
		q->dizi[q->rear]=x;
		q->rear++;
		return 1;
	}
}

int deQueue(Que *q){		// kuyruktan eleman ��kar ve ��kan eleman d�nd�r�l�r.
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

int bul(Graph *graph,char *name){	// ismi verilen node'un graph�n kaac�nc� gozunde oldugunu bulan fonksiyon
	
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
		return adr; // eger ismi verilen node graptaysa graphtak� yer� dondurulur
	}
	else{
		return -1; // eger ismi verilen node graph'ta yoksa -1 dondurulur
	}
}

void breadthFirstSearch(Graph *graph,int source,int destination,char *sourceName,char *destinationName){	// bfs
	// parametre olarak source, destination indeksleri ve source, destination oyuncunlar�n�n isimleri al�n�r
	Que *q=(Que*)malloc(sizeof(Que));	// kuyruk olu�turulur.
	q->rear=0;
	int girdiMi[S];		// node'un kuyru�a girip girmedi�ini kontrol etti�imiz bir dizi mevcut.
	int v,i;
	Node *tmp ;
	for(i=0;i<S;i++){			// ilk ba�ta dizi s�f�rlan�r.��nk� hen�z kuyru�a giren yok .
		girdiMi[i]=0;
	}
	
	enQueue(q,source);			// ilk kelimenin graphtaki yeri kuyru�a eklenir.
	girdiMi[source]=1;			// kuyru�a girdi�i i�in kontrol dizimizin ilgili indisi 1 yap�l�r.Yani kuyru�a girdi�i i�aretlenir.
	
	while(!isEmpty(q)){	// kuyruk tamamen bo�alana kadar veya ��k�� d���m�ne ula�ana kadar d�ng�de kal�n�r.
	
		v=deQueue(q);			// kuyruktan eleman cekilir
		tmp = graph->list[v];  // kuyruktan c�kar�lan node'un komsular� gezilir
		
		if(!strcmp(tmp->name,destinationName)){		// kuyruktan ��kan eleman�n aradg�m�z eleman olup olmad�g� kontrol edilir.
			Node *yaz = tmp;	// uzakl�g� bulmak icin gecici bir node olusturulur.
			int sayi = 0;		// uzakl�k bulmak �c�n kullan�l�r.
			while(yaz->ancestor!=-1){	// gecici node uzakl�g� bulmak �c�n kullan�l�r.
				if(yaz->type == 0) // eger ilgili node film ise sayi artt�r�l�r.
					sayi++;
				yaz = graph->list[yaz->ancestor];
			}			
			if(sayi>6){ // eger uzakl�k 6'dan fazla ise mesaj yazd�r�l�r.
				printf("Uzaklik = %d , 6'dan buyuk\n",sayi);
				return;
			}
			else{	// eger uzakl�k 6'dan kucuk ise.
				printf("Uzaklik = %d\n",sayi);
				while(tmp->ancestor!=-1){	// node'un nereden geldigi bulunur.
					if(tmp->type == 1)	// eger oyuncu ise.
						printf("Oyuncu = ");
					else
						printf("Film = ");	// eger film ise.
					printf(" %s ---> ",tmp->name);	// isim yazd�r�l�r.
					tmp = graph->list[tmp->ancestor]; // bir ust atas�na gidilir.
				}
				return ;
			}				
		}
		else{										// kuyruktan c�kan eleman aradg�m�z eleman deg�lse. 
			while(tmp!=NULL){					// o kelimenin kom�ular�n�n bulundu�u dizi gezilir.
				long long int anhtr = bul(graph,tmp->name);
				if(girdiMi[anhtr]==0){				// ve kuyru�a girmediyse
					enQueue(q,anhtr);				// kuyru�a eklenir.
					girdiMi[anhtr]=1;				// kuyru�a girdi�i i�aretlenir.
					Node *yaz = graph->list[anhtr]; // gecici node olusturulur.
					yaz->ancestor = v;				// aatas� isaretlenir.
				}
				tmp = tmp->next;					// komsular� gezilir.
			}
		}
	}
	
	if(isEmpty(q))			// kuyruk bossa baglant� yoktur.
		printf("Baglanti yok !\n");	
}

int main(){
	
	FILE *fp;	// dosya okunacak.
	char satir[3000];	// sat�r sat�r okunacak.
	int i;			// dongu indisi.
	char *parca; // strtok ile parcalanacaak olan parca
	char *tmpP; // gecici degisken
	Graph *graph;
		
	graph = (Graph*)malloc(sizeof(Graph));	// graph tan�mlan�r
	graph->list = (Node**)malloc(sizeof(Node*)*S); // graph allocate edilir
	graph->count= 0;
	
	for(i=0;i<S;i++){ // graph allocate edilir
		graph->list[i] = (Node*)malloc(sizeof(Node));
		Node *tmp = graph->list[i];
		tmp->type = -1;	// tipi belirsizdir, yani bos oldugu anlam�na gelir
		tmp->ancestor = -1;	// atas� yoktur
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
		
		fgets(satir,sizeof(satir),fp);			// sat�r sat�r okuma yap�l�r
		parca = strtok(satir,"/\n");		// sat�r parcalanmaya baslar
		
		strcpy(film->name,parca);	// filmin ad� al�n�r
		film->type = 0;	// node'un tipi film olarak isaretlenir
		
		anahtarF = key(parca); // filme key uretilir
		
		tmpFilm = film;
		while(parca!=NULL){	// sat�r parcalanmaya devam eder						
			Node *oyuncu; // oyuncu node olusturulur
			oyuncu = (Node*)malloc(sizeof(Node));
			int anahtarO ; // oyuncu �c�n b�r key uret�l�r
			parca = strtok(NULL,"/\n"); // '/' ve alt sat�r �saretlerine gore parcalan�r
			
			if(parca!=NULL){ // sona gelene kadar
				strcpy(oyuncu->name,parca);
				
				oyuncu->type = 1; // node'un oyuncu oludugu �saretlen�r
				film->next = oyuncu; // oyuncu node'u film node'u komsu olarak eklenir
				film=film->next;
				anahtarO = key(oyuncu->name); // oyuncuya key uret�l�r
				if(bul(graph,oyuncu->name)==-1){ // eger graphta yoksa grapha eklenir
					Node *tmpOYUN = (Node*)malloc(sizeof(Node)); // gecici oyun node'un olusturulur
					strcpy(tmpOYUN->name,parca); // ismi kopyalan�r
					tmpOYUN->type = 1; // node'un tipi oyuncu olarak isaretlenir
					Node *newNode1 = (Node*)malloc(sizeof(Node)); // yeni bir tmp film node'u olusturulur
					strcpy(newNode1->name,tmpFilm->name); // isim kopyalan�r
					newNode1->type = 0;		// node'un tipi film olarak isaretlenir
					newNode1->next = NULL;
					tmpOYUN->next = newNode1;	// oyun node'una komsu olarak film eklenir
					insertNode(graph,tmpOYUN,anahtarO);	// olusturulan node grapha eklenir
				}
				else{ // eger node daha onceden graha eklenmisse
					int aa = bul(graph,oyuncu->name); // node'un graphtak� yer� al�n�r
					Node *oyTmp = graph->list[aa];   // tmp node'a atan�r
					Node *newNode = (Node*)malloc(sizeof(Node)); // yeni film node'u olusturulur
					strcpy(newNode->name,tmpFilm->name); // filmin ad� kopyalan�r
					newNode->type = 0;	// node'un tipi film olarak isaretlenir
								
					newNode->next = oyTmp->next;
					oyTmp->next = newNode; // oyuncu node'una komsu olarak film node'u eklen�r
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
