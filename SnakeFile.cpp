#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<windows.h>
#include<fstream>
#define Key_Up 72
#define Key_Down 80
#define Key_Left 75
#define Key_Right 77
#define key_ESCAPE 27 
#define reverse_R 114

using namespace std;


/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct snake{
	char ch;
	int x;
	int y;
	struct snake *next;
};

struct food{
	char food_f1;
	int x;
	int y;
};
class SnakeGame{
	public:
		int grab_food;
		char *saved;
	SnakeGame(){
		grab_food=0;
	}
struct snake *generate_snake(struct snake*);
struct food *generate(struct snake*,char **);
void displaySnake(struct snake *);
void displayFood(struct food *);
void print(char**);
char **print_all(char**,struct snake*);
struct snake* Eatenfood(struct snake*,struct snake*,char **);
struct snake* Not_Eatenfood(struct snake*,struct snake *,char**);
struct snake* moveMent(struct snake *,struct food*,int dir,int flag,char** boundary);
struct snake* ReverseSnake(struct snake*,char**);
char* LinkedList_to_String(struct snake *head,int ,struct food *f1);
char* Store_String_In_File(char *string);
void Saving_String(char *str);   

};

struct snake* SnakeGame:: ReverseSnake(struct snake *head,char **boundary){
struct snake *prev=NULL;
struct snake *current=head;
struct snake *Next=NULL;
while(current!=NULL){
	Next=current->next;
	Next->ch=current->next->ch;
	boundary[Next->x][Next->y]=boundary[current->next->x][current->next->y];
    current->next=prev;
	current->next->ch=prev->ch;
	boundary[current->next->x][	current->next->y]=boundary[prev->x][prev->y];
	prev=current;
	prev->ch=current->ch;
	boundary[prev->x][prev->y]=boundary[current->x][current->y];
	current=Next;
	current->ch=Next->ch;
   boundary	[current->x][current->y]=boundary[Next->x][Next->y];
}
head=prev;
head->ch=prev->ch;
boundary[head->x][head->y]=boundary[prev->x][prev->y];
return head;	
}
char* SnakeGame::Store_String_In_File(char* string){
	ifstream StoringFile;
	StoringFile.open("String.txt");
	if(StoringFile.is_open()){
		while(!StoringFile.eof()){
			StoringFile>>string;
		}
		
	}else
	cout<<"File is Closed And String Is Stored";
	StoringFile.close();
	cout <<string<<endl;
	return string;
}
void SnakeGame::Saving_String(char *string){
	ofstream file("String.txt");
	file<<string;
	file.close();
}
char* SnakeGame::LinkedList_to_String(struct snake *head,int Size,struct food *food1){
	char* created_snake=new char[Size];
	struct snake *temp=head;
	 int i=0;
	while(temp!=NULL){
		created_snake[i]=temp->ch;
		++i;
		created_snake[i]=',';
		++i;
		if(head->x<10){
			created_snake[i]=temp->x+48;
			++i;
		}else{
			int first=temp->x/10;
			int second=temp->x%10;
			created_snake[i]=first+48;
			++i;
			created_snake[i]=second+48;
			++i;
		}
		created_snake[i]=',';
		i++;
			if(head->y<10){
			created_snake[i]=temp->y+48;
			++i;
		}else{
			int first=temp->y/10;
			int second=temp->y%10;
			created_snake[i]=first+48;
			++i;
			created_snake[i]=second+48;
			++i;
		}
		created_snake[i]='&';
		i++;
		temp=temp->next;
	}
	created_snake[i]='@';
	i++;
	created_snake[i]=food1->food_f1;
	++i;
	created_snake[i]=',';
	++i;
	if(food1->x<10){
		created_snake[i]=food1->x+48;
		++i;
	}else{
		int first=food1->x/10;
		int second=food1->y%10;
		created_snake[i]=first+48;
		++i;
		created_snake[i]=second+48;
		++i;
	}
	created_snake[i]='\0';
//for(int i=0;i<Size;i++)
//cout<<created_snake[i];
//Sleep(200);
    saved=created_snake;
	Store_String_In_File(saved);
	return saved;
}

struct snake* SnakeGame::Eatenfood(struct snake*head,struct snake *latest,char **boundary){
grab_food=1;
latest->next=head;
head=latest;
boundary[head->x][head->y]='A';
boundary[head->next->x][head->next->y]='#';
return head;	
}
struct snake* SnakeGame::Not_Eatenfood(struct snake *head,struct snake *latest,char **boundary){
	grab_food=0;
	struct snake* GameOver=head;
	while(GameOver!=NULL){
		if(GameOver->x==latest->x&&GameOver->y==latest->y){
			cout<<"SnakeGame Over"<<endl;
			exit(0);
		}
		GameOver=GameOver->next;
	}
	latest->next=head;
	head=latest;
	struct snake* temp=head;
	while(temp->next->next!=NULL)
	temp=temp->next;
	boundary[temp->next->x][temp->next->y]=' ';
	boundary[head->x][head->y]=head->ch;
	boundary[head->next->x][head->next->y]=head->next->ch;
	struct snake *to_delete=new snake;
	to_delete=temp->next;
	temp->next=NULL;
	delete to_delete;
	return head;	
}
struct snake * SnakeGame::moveMent(struct snake *head,struct food *food_f1,int direction,int flag,char **boundary){
	if((flag==1&& head->next->x==direction)||(flag==0&&head->next->y==direction))
	return head;
	struct snake *latest=new snake;
	latest->next=NULL;
	latest->ch=head->ch;
	head->ch=head->next->ch;
	if(flag==1){
		latest->x=direction;
		latest->y=head->y;
	}else{
		latest->y=direction;
		latest->x=head->x;
	}
	if(latest->x==food_f1->x&&latest->y==food_f1->y){
		head=Eatenfood(head,latest,boundary);
		return head;
	}else{
		head=Not_Eatenfood(head,latest,boundary);
		return head;
	}
}


struct snake * SnakeGame ::generate_snake(struct snake* head){
	int size=3;
	srand(time(NULL));
	int x=rand()%30+4;
	int y=rand()%30+4;
	while(size--){
		struct snake *latest=new snake;
		latest->x=x;
		latest->y=y;
		latest->next=NULL;
		if(head==NULL){
			latest->ch='A';
			head=latest;
		}else {
			latest->ch='#';
			struct snake *temp=head;
			while(temp->next!=NULL)
			temp=temp->next;
			temp->next=latest;
			
		}
		++y;
	}
return head;
}
struct food * SnakeGame :: generate(struct snake *head,char **boundary){
	int same_coor=1;
	int x;
	int y;
	while(same_coor){
		same_coor=0;
		srand(time(NULL));
		x=rand()%40;
		y=rand()%40;
		struct snake *temp=head;
		while(temp!=NULL){
			if(temp->x==x&&temp->y==y){
				same_coor=1;
				break;
			}
			temp=temp->next;
		}
		
	}
	struct food *latest=new food;
	latest->x=x;
	latest->y=y;
	latest->food_f1='*';
	boundary[x][y]='*';
	return latest;
}
void SnakeGame::displaySnake(struct snake *head){
struct snake *temp=head;
while(temp!=NULL){
	cout<<temp->ch<<" x: "<<temp->x<<" y: "<<temp->y<<endl;
	temp=temp->next;
}
cout<<endl;
}
void SnakeGame::displayFood(struct food *f1){
	cout<<f1->food_f1<<" x: "<<f1->x<<" y: "<<f1->y<<endl;
}
void SnakeGame::print(char **boundary){
	for(int i=0;i<40;i++){
		for(int j=0;j<40;j++){
			cout<<boundary[i][j];
		}
		cout<<endl;
	}
}
char ** SnakeGame::print_all(char **boundary,struct snake *head)
{	struct snake *temp=head;
	while(temp!=NULL){
		boundary[temp->x][temp->y]=temp->ch;
		temp=temp->next;	}
	return boundary;
}

void play_game(){
	cout<<"Welcome to Snake Game"<<endl;
	cout<<"u for upper"<<endl<<"r for right"<<endl<<"l for left"<<endl<<"d for down"<<endl<<"q for quit"<<endl;
	int rows=40,cols=40;
	int grab_food;
	SnakeGame s;
	char **boundary=new char*[rows];
        for(int i=0;i<40;i++)
		boundary[i]=new char[cols];
		for(int i=0;i<39;i++)
		boundary[0][i]='%';
		for(int j=0;j<40;j++)
		boundary[j][39]='%';
		for(int i=39;i>=0;i--)
		boundary[39][i]='%';
		for(int j=39;j>=0;j--)
		boundary[j][0]='%';	
    for(int i=1;i<rows-1;i++){
    	for(int j=1;j<cols-1;j++){
    		boundary[i][j]=' ';
		}
	}
struct snake *head=NULL;
struct food *food_f1=NULL;
head=s.generate_snake(head);
s.displaySnake(head);
food_f1=s.generate(head,boundary);
s.displayFood(food_f1);
boundary=s.print_all(boundary,head);
s.print(boundary);
int kb_code=0;
	/*
char choice;
	cin>>choice;
	switch(choice){
		case 'u':
			if(head->x>0)
			head=moveMent(head,food_f1,(head->x-1)%40,1,boundary);
			else
			head=moveMent(head,food_f1,39,1,boundary);
			break;
			case 'r':
			if(head->y>0)
			head=moveMent(head,food_f1,(head->y-1)%40,0,boundary);
			else
			head=moveMent(head,food_f1,39,0,boundary);
			break;
			case 'd':
				head=moveMent(head,food_f1,(head->y+1)%40,1,boundary);
				break;
				case 'l':
					head=moveMent(head,food_f1,(head->x+1)%40,0,boundary);
	}
	if(choice=='z'){
		break;
	}
	boundary=print_all(boundary,head);
print(boundary);
}
*/
while(kb_code!=key_ESCAPE){
	if(kbhit()){
		kb_code=getch();
		switch(kb_code){
			case Key_Up:
                if(head->x>0&&head->x<39)
                head=s.moveMent(head,food_f1,(head->x-1)%40,1,boundary);
                else
                head=s.moveMent(head,food_f1,39,1,boundary);
                break;
            case Key_Left:
                if(head->y>0&&head->y<39)
                head=s.moveMent(head,food_f1,(head->y-1)%40,0,boundary);
                else
                head=s.moveMent(head,food_f1,39,0,boundary);
                break;
            case Key_Right:
                head=s.moveMent(head,food_f1,(head->y+1)%40,0,boundary);
                break;
            case Key_Down:
                head=s.moveMent(head,food_f1,(head->x+1)%40,1,boundary);
                break;
                case reverse_R:
                head=s.ReverseSnake(head,boundary);	
		}
	
		}
	
	
 if(s.grab_food==1){
            food_f1=s.generate(head,boundary);
          s.grab_food=0;
        }
        Sleep(500);
        system("cls");
      s.print(boundary);
   }
   if(kb_code==key_ESCAPE){
   	char choice;
   	cout<<"Enter the choice yes/no";
   cin>>choice;
	if(choice=='y'){
   	s.saved=s.LinkedList_to_String(head,30,food_f1);
   s.saved= s.Store_String_In_File(s.saved);
   	s.Saving_String(s.saved);
	   }
   
   }
        if(s.grab_food==1){
            food_f1=s.generate(head,boundary);
        }
        system("cls");
       s.print(boundary);
       
    }  
int main(int argc, char** argv) {
	play_game();
	return 0;
}
