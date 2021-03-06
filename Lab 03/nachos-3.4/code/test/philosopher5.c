#include "syscall.h"

//---------------------------------------------------------Khai bao hang--------------------------------------------------------------
#define N		5			// So triet gia
#define LEFT		(i+N-1)%N		// Triet gia ben trai
#define RIGHT		(i+1)%N			// Triet gia ben phai
#define THINKING	0			// Trang thai suy nghi
#define STARVING	1			// Trang thai doi, muon lay nia de an
#define EATING		2			// Trang thi an
#define MULTEX		"multex"		// Ten cua semaphore dong quyen truy xuat

//	Ten cua cac triet gia
#define PHILO_0		"philo0"
#define PHILO_1		"philo1"
#define PHILO_2		"philo2"
#define PHILO_3		"philo3"
#define PHILO_4		"philo4"
//---------------------------------------------------------Khai bao -------------------------------------------------------------------
char *s[N];					// Tao mang luu ten cac semaphore tuong ung cho moi triet gia
int state[N];					// Luu tru trang thai cua cac triet gia

//---------------------------------------------------------Cai dat---------------------------------------------------------------------
void readState(){
	int fID; // fileID
	char c; // Ki tu doc tu file
	int i;
	
	// Cap nhat state cua cac triet gia
	// Mo file state.txt
	fID = Open("state.txt", 0);
	if(fID == -1){
		Close(fID);
		return;
	}
	
	i = 0;
	do{
		Read(&c, 1, fID);
		state[i] = (int)(c - 48);
		i++;	
	}while(i < N);	

	Close(fID);
}

void writeState(){
	int fID; // fileID
	char c; // Ki tu ghi file
	int i;
	
	// Cap nhat state cua cac triet gia
	// Mo file state.txt
	fID = Open("state.txt", 0);
	if(fID == -1){
		Close(fID);
		return;
	}
	for(i = 0; i < N; i++){
		c = (char)(state[i] + 48);
		Write(&c, 1, fID);
		if(i != N - 1)
			Write(" ", 1, fID);
	}
	Close(fID);
}

void test(i){
	readState();	
	// Neu triet gia i dang muon lay nia va kiem tra xem triet gia ben trai va ben phai khong o trang thai EATING
	if(state[i] == STARVING && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		writeState(); // Cap nhat thay doi vao state.txt
		// In dong trang thai take_forks		
		PrintString(s[i]);
		PrintString(" takes forks ");
		PrintChar(LEFT+48);
		PrintString(" and ");
		PrintChar(RIGHT+48);
		PrintString(".\n");
		Up(s[i]);
	}
}

void take_forks(int i){
	Down(MULTEX);		// vao mien gang
	state[i] = STARVING;	// luu trang thai chuan bi lay nia
	writeState(); // Cap nhat thay doi vao state.txt
	// In trang thai STARVING	
	PrintString(s[i]);
	PrintString(" is starving!\n");

	test(i);		// Thuc hien kiem tra va lay 2 nia
	Up(MULTEX);		// Thoat khoi mien gang
	Down(s[i]);		// Khoa neu khong lay duoc nia	
}

void eat(int i){
	//Down(MULTEX);
	PrintString(s[i]);
	PrintString(" is eating!\n");
	//Up(MULTEX);
}

void think(int i){
	//Down(MULTEX);
	PrintString(s[i]);
	PrintString(" is thinking!\n");
	//Up(MULTEX);
}

void put_forks(int i){
	Down(MULTEX);		// vao mien gang
	state[i] = THINKING;	// an xong
	writeState(); // Cap nhat thay doi vao state.txt
	// In trang thai put_forks	
	PrintString(s[i]);
	PrintString(" puts forks ");
	PrintChar(LEFT+48);
	PrintString(" and ");
	PrintChar(RIGHT+48);
	PrintString(" down.\n");

	test(LEFT);		// Kiem trai triet gia ben trai co the an
	test(RIGHT);		// Kiem tra triet gia ben phai co the an
	Up(MULTEX);		// Thoat mien gang
}

void philosopher(int i){
		think(i);	// Suy nghi
		take_forks(i);	// Lay nia
		eat(i);		// An
		put_forks(i);	// Tra nia
}

int main(){
	int i;
	int fID; // fileID
	char c; // Ki tu doc tu file
	
	// Khoi tao ten cac semaphore cho cac triet gia	
	for(i = 0; i < N; i++){
		switch(i){
			case 0:
				s[i] = PHILO_0;	break;
			case 1:
				s[i] = PHILO_1;	break;
			case 2:
				s[i] = PHILO_2;	break;
			case 3:
				s[i] = PHILO_3;	break;
			case 4:
				s[i] = PHILO_4;	break;
		}
	} 

	// Cap nhat state cua cac triet gia
	// Mo file state.txt
	fID = Open("state.txt", 0);
	if(fID == -1){
		Close(fID);
		return 1;
	}
	i = 0;
	do{
		Read(&c, 1, fID);
		state[i] = (int)(c - 48);
		i++;	
	}while(i < N);	

	Close(fID);	

	// Philosopher thu 4 goi ham thuc thi
	philosopher(4);
	
	return 0;
}
