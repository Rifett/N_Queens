#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h> 

typedef struct queen {
    int x;
    int y;
} Queen;

typedef struct Field {
    int Exist; //0-free space, 1 and more-Queen is here (number shows the number of a queen standing on the place)
    int AttackCounter;
} Field;

void RandomCreation (Field ** field, int n, Queen queen[]) {//creates random field
    srand(time(NULL));
    int random;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            field[i][j].Exist = 0;
            field[i][j].AttackCounter = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        random = rand() % (n);
        field[random][i].Exist = i+1;
        queen[i].x = i;
        queen[i].y = random;
    }
    return ;
}

int FindChosenOne (Field ** field, int n, Queen queen[]) {//finds the most dangerous queen
    int chosen = 0;//the number of a chosen node
    int Max = 0;//something to compare (max danger)
    int Right;
    int Down;
    int Left;
    int Up;
    int count; 
    for (int i = 0; i < n; i++) { 
        count = 0;
        Right = n - queen[i].x - 1;
        Down = n - queen[i].y - 1;
        Left = n - Right - 1;
        Up = n - Down - 1;
        for (int a = 1; a <= Right; a++) {//copypaste it to all directions
            if (field[queen[i].y][queen[i].x + a].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (field[queen[i].y][queen[i].x - a].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Down; a++) {
            if (field[queen[i].y + a][queen[i].x].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Up; a++) {
            if (field[queen[i].y - a][queen[i].x].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Right; a++) {//start of a diagonal checks
            if (a > Down) break;
            if (field[queen[i].y + a][queen[i].x + a].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Right; a++) {
            if (a > Up) break;
            if (field[queen[i].y - a][queen[i].x + a].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (a > Up) break;
            if (field[queen[i].y - a][queen[i].x - a].Exist > 0) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (a > Down) break;
            if (field[queen[i].y + a][queen[i].x - a].Exist > 0) 
                count++;
        } 
        if (count > Max) {
            Max = count; //to find out the solution case we can use {if (Max = 0) then return n+1}
            chosen = i;
        }
    }
    if (Max == 0) return n+1;
    return chosen;
}

void FieldMark (Field ** field, int n, Queen queen[], int ChosenOne) {//marks the field in terms of an attack count
    int chosen;//the number of a chosen node
    int Right;
    int Down;
    int Left;
    int Up;
    int count;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
        count = 0;
        Right = n - j - 1;
        Down = n - i - 1;
        Left = n - Right - 1;
        Up = n - Down - 1;
        if (field[i][j].Exist > 0) {
            field[i][j].AttackCounter = n + 1;
            continue;
        }
        for (int a = 1; a <= Right; a++) {//copypaste it to all directions
            if (field[i][j + a].Exist > 0 && field[i][j + a].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (field[i][j - a].Exist > 0 && field[i][j - a].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Down; a++) {
            if (field[i + a][j].Exist > 0 && field[i + a][j].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Up; a++) {
            if (field[i - a][j].Exist > 0 && field[i - a][j].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Right; a++) {//start of a diagonal checks
            if (a > Down) break;
            if (field[i + a][j + a].Exist > 0 && field[i + a][j + a].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Right; a++) {
            if (a > Up) break;
            if (field[i - a][j + a].Exist > 0 && field[i - a][j + a].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (a > Up) break;
            if (field[i - a][j - a].Exist > 0 && field[i - a][j - a].Exist != ChosenOne+1) 
                count++;
        }
        for (int a = 1; a <= Left; a++) {
            if (a > Down) break;
            if (field[i + a][j - a].Exist > 0 && field[i + a][j - a].Exist != ChosenOne+1) 
                count++;
        }
        field[i][j].AttackCounter = count;
        }
    }
}

void SwapFunc (Field ** field, int n, Queen queen[], int ChosenOne) { //finds the best place to swap + does the actual swapping
int Min = n + 1;
int Ymin;
int Xmin;
int Xdestroy;
int Ydestroy;
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        if (field[i][j].AttackCounter < Min) {
            Min = field[i][j].AttackCounter;
            Ymin = i;//best coordinates to swap with
            Xmin = j;
        }
    }
}
Xdestroy = queen[ChosenOne].x;//coordinates to clear
Ydestroy = queen[ChosenOne].y;
queen[ChosenOne].x = Xmin;
queen[ChosenOne].y = Ymin;
field[Ydestroy][Xdestroy].Exist = 0;
field[Ymin][Xmin].Exist = ChosenOne+1;
}





int main () {
printf ("Please, enter the size of a field:\n");
int n;
scanf ("%d", &n);
Field ** field = (Field **)malloc(n*sizeof(Field*));
for (int r = 0; r < n; r++) {
  field[r] = (Field*)malloc (n*sizeof(Field)); 
}
Queen * queen = (Queen *)malloc(n*sizeof(Queen));
int ResCount = 0;

//main part
RandomCreation (field, n, queen); //initial creation
while (FindChosenOne(field, n, queen) != n+1) {
    if (ResCount > n) {//reset detector and activator
        RandomCreation (field, n, queen);
        ResCount = 0;
    }
FieldMark(field, n, queen, FindChosenOne(field, n, queen));
SwapFunc(field, n, queen, FindChosenOne(field, n, queen));
ResCount++;
}

for (int i = 0; i < n; i++) {
printf ("~ ");
} printf ("\n");
for (int i = 0; i < n; i++) {//printing the result chess field
    for (int j = 0; j < n; j++) {
        if (field[i][j].Exist == 0)
        printf ("0 ");
        else
        printf ("Q ");
    }
    printf ("\n");
}
for (int i = 0; i < n; i++) {
printf ("~ ");
} printf ("\n");

for (int i = 0; i < n; i++) { //make it free
    free (field[i]);
}
free (queen);
free (field);
    return 0;
}