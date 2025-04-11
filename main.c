#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
typedef struct {
    char x, y;
    char health;
    char symbol;
    int allowedMovementUnits;
    bool has_moved, has_attacked;
} Player;

typedef struct {
    int x, y;
    char healthSymbol;
    int health;
} Enemy;

typedef struct {
    int difficulty;
    int player_count;
    int monster_count;
    int level;
    int arxhdwmatiou;
    int maprow;
    int mapcol;
    char** map;
    Enemy* enemies;
    Player* players;
} GameState;
void DisplayMap(GameState* game);
void SpawnEnemies(GameState *game);
void GenerateRoom(GameState *game);
void SpawnFurniture(GameState *game);
void makeDoor(GameState *game);
void makeWalls(GameState *game);
void spawnPlayers(GameState *game);
void turnBasedSystem(GameState *game);
bool handleUserMovement(GameState *game,int i);
bool moveHero(GameState *game,int i, char c,int mDigits);
void handleUserAttack(GameState *game,int i);
bool isAcceptedMovementCharacter(char c);
bool isAcceptedAttackCharacter(char c);
int main(void) {
    GameState game;
    game.monster_count = 0;
    game.map = NULL;
    game.maprow = 5;
    game.mapcol = 9;
    game.enemies = (Enemy*)malloc(sizeof(Enemy));
    game.players = (Player*)malloc(sizeof(Player));
    char bobmarley;
    srand(time(NULL));
    game.map = (char**)malloc(game.maprow * sizeof(char*));
    for (int i = 0; i < game.maprow; i++) {
        game.map[i] =(char*)malloc(game.mapcol * sizeof(char));
    }
    game.level = 1;
    game.difficulty = 0;
    game.player_count = 0;
    while (game.difficulty < 1 || game.difficulty > 3) {
        printf("Enter Game difficulty, Please enter a number between 1 to 3.\n");
        scanf("%d", &game.difficulty);
    }
    while (game.player_count < 1 || game.player_count > 4) {
        printf("Enter number of heroes in the game. (1-4)\n");
        scanf("%d", &game.player_count);
    }
    for (int i = 0; i < game.player_count; i++) {
        game.players = realloc(game.players, (i + 1) * sizeof(Player));
        printf("Enter class for player %d (B for barbarian, W for wizard, E for elf and D for dwarf):\n", i + 1);
        do{
        scanf(" %c", &game.players[i].symbol);
        } while (game.players[i].symbol != 'B' && game.players[i].symbol != 'W' && game.players[i].symbol != 'E' && game.players[i].symbol != 'D');
        if (game.players[i].symbol == game.players[i - 1].symbol || game.players[i].symbol == game.players[i - 2].symbol || game.players[i].symbol == game.players[i - 3].symbol) {
            printf("You can only choose 1 of each class!\n");
            i--;
            continue;
        }
        switch (game.players[i].symbol) {
            {
                case 'B':
                    game.players[i].health = 8;
                    game.players[i].allowedMovementUnits = 8;
                    
                    break;
                case 'D':
                    game.players[i].health = 7;
                    game.players[i].allowedMovementUnits = 6;
                    break;
                case 'E':
                    game.players[i].health = 6;
                    game.players[i].allowedMovementUnits = 12;
                    break;
                case 'W':
                    game.players[i].health = 4;
                    game.players[i].allowedMovementUnits = 10;
                    break;
            }
            }
        }
        

    do {
        GenerateRoom(&game);
        if (game.level > 1){
        makeWalls(&game);
        }
        SpawnFurniture(&game);
        SpawnEnemies(&game);
        spawnPlayers(&game);
        DisplayMap(&game);
        while (game.player_count > 0 || game.monster_count > 0)
        {
          turnBasedSystem(&game);
        }
        
        printf("dwse 'K' gia epomeno level!\n");
        scanf(" %c", &bobmarley);
        if(bobmarley == 'w'){
            break;
        }
        if (((game.monster_count <= 0) && (game.player_count > 0)) || bobmarley == 'k') {
            game.level++;
        }
    } while (game.player_count > 0);
    for (int i = 0; i < game.maprow; i++) {
        free(game.map[i]);
    }
    free(game.map);
    game.map = NULL;
    
    return 0;
}
void spawnPlayers(GameState *game){
    int failSafeAttempts;
    int i;
    int randx, randy;
    for (i = 0; i < game->player_count; i++) {
        failSafeAttempts = 25;
        do {
            randy = rand() % 9; 
            randx = (rand() % game->maprow);
            if (randx >= 0 && randx < game->maprow && randy >= 0 && randy < game->mapcol) {
                if (game->map[randx][randy] == '.') {
                    game->map[randx][randy] = game->players[i].symbol;
                    game->players[i].x = randx;
                    game->players[i].y = randy;
                    break;
                }
            }
            failSafeAttempts--;
        } while (game->map[randx][randy] != '.' && failSafeAttempts > 0);
    }
}
void SpawnEnemies(GameState* game){
         int failSafeAttempts;
         int mCount;
         int roomRows;
         int wallRows;
         int startRow;
         roomRows = 5;
         wallRows = 2;
         int endRow;
         int i;
         int room;
         int randx, randy;
        for (room = 0; room < game->level; room++){
            if(game->level == 1){
                wallRows = 0;
            }
            startRow = room*(roomRows + wallRows);
            endRow = (startRow + roomRows) - 1;
            mCount = (rand() % (game->difficulty* 2)) + 1;
         for (i=0; i < mCount; i++){
            game->enemies = realloc(game->enemies,(game->monster_count+ 1) * sizeof(Enemy));
            game->monster_count++;
            Enemy newEnemy;
            switch (game->difficulty){
                case 1:             
                newEnemy.health = (rand() % 3) + 1;      
                break;
                case 2:
                newEnemy.health= (rand() % 3) + 4;
                break;
                case 3:
                newEnemy.health= (rand() % 3) + 7;
                break;
            }
            failSafeAttempts = 25;
            do {
                randy = rand() % 8; 
                randx = (rand() % (endRow - startRow + 1)) + startRow; 
                if (randx >= 0 && randx < game->maprow && randy >= 0 && randy < game->mapcol) {
                    if (game->map[randx][randy] == '.') {
                        game->map[randx][randy] = newEnemy.health + '0';
                        newEnemy.x = randx;
                        newEnemy.y = randy;
                        break;
                    }
                }
                failSafeAttempts--;
            } while (game->map[randx][randy] != '.' && failSafeAttempts > 0);
        }
        }
    }

void DisplayMap(GameState *game) {
    int r,c;
    printf("  ");
    for (c = 0; c < game->mapcol; c++) {
        if(c == 0){
            printf("   ");
        }
        printf("%c ", 65+c);
    }
    printf("\n");
    for (c = 0; c < game->mapcol; c++) {
        if(c == 0){
            printf("     ");
        }
        printf("_ ");
    }
    printf("\n");
    
    for (r = 0; r < game->maprow; r++) {
        printf("%3d| ",r + 1);
        for (c = 0; c < game->mapcol; c++) {
            printf("%c ", game->map[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}
void SpawnFurniture(GameState *game) {
    const int minFurniture = 1;
    const int maxFurniture = 3;
    const char furnitureSymbol = '@';
    int failSafeAttempts;
    int furnCount = 0;
    int roomRows = 5;
    int wallRows = 2;
    int startRow;
    int endRow;
    int i;
    int room;
    int randx, randy;
    for (room = 0; room < game->level; room++){
            if (game->level == 1){
                wallRows = 0;
            }
            failSafeAttempts = 25;
            startRow = room*(roomRows + wallRows);
            endRow = (startRow + roomRows) -1;
            for (i = 0; i< rand() % maxFurniture + minFurniture; i++){
            do {
                randy = rand() % 9; 
                randx = (rand() % (endRow - startRow + 1)) + startRow;
                bool flag = false;
                for(int j = randx; j < endRow; j++){
                    if(game->map[j][randy] == '#'){
                        flag = true;
                        break;
                        
                } 
            }
                if (((randx >= 0 && randx < game->maprow) && (randy >= 0 ))&& ((randy < game->mapcol) && (flag = true))) {
                    if (game->map[randx][randy] == '.') {
                        game->map[randx][randy] = furnitureSymbol;
                        furnCount++;
                        break;
                    }
                }
                failSafeAttempts--;
                printf("failsafe attempts %d", failSafeAttempts);
            } while (game->map[randx][randy] != '.' && furnCount <= maxFurniture && failSafeAttempts > 0);
    }
}
    }

void GenerateRoom(GameState *game) {
    if (game->level <= 1){
    for (int i = 0; i < game->maprow; i++) {
        for (int j = 0; j < game->mapcol; j++) {
            game->map[i][j] = '.';
        }
    }
}
    if (game->level > 1){
        int newrows = 7; // 7 empty and 2 for walls
        game->maprow += newrows; // new size of cols
        game->map = realloc(game->map, game->maprow * sizeof(char*)); // increase size of rows
        for(int i = game->maprow-newrows; i < game->maprow; i++){ 
            game->map[i] = malloc(game->mapcol * sizeof(char)); // allocate memory for new rows ( logic is that the new rows are not yet allocated)
        }
        for (int i = 0; i < game->maprow; i++) { // clear objects for enw level
            for (int j = 0; j < game->mapcol; j++) {
                game->map[i][j] = '.';
            }
        }
        if (game->map == NULL) { //failsafe
            printf("tavrikeskoura h raloc\n");
            exit(1);
        }
    }
}

void makeWalls(GameState *game){
    int startingRow = 5;
    int alagh;
    int door;
        for(int i = 1; i < game->level; i++){
            for (int j = 0; j < game->mapcol; j++) {
                alagh = rand() % 2;
                game->map[startingRow + alagh][j] = '#';
            }
      door = rand() % 9;
    game->map[startingRow][door] = '.';
    game->map[startingRow+1][door] = '.';
    game->map[startingRow-1][door] = '.';
    startingRow += 7;
}
}
bool isAcceptedAttackCharacter(char c) {
    const char acceptedChars[] = "Aa";
    return strchr(acceptedChars, c) != NULL;
}
bool isAcceptedMovementCharacter(char c){
    const char acceptedChars[] = "UuLlRrDd";
    return strchr(acceptedChars, c) != NULL;
}
void turnBasedSystem(GameState *game) {
    int i;
    int selection;
    bool flag = false;
    for (i = 0; i < game->player_count; i++) {
        game->players[i].has_moved = false;
        game->players[i].has_attacked = false;
        printf("%c has %d health, what's his next action?\n", game->players[i].symbol, game->players[i].health);
        printf("1. Move\n");
        printf("2. Attack\n");
        printf("3. Skip\n");
        do {
            scanf("%d", &selection);
        } while (selection < 1 || selection > 3);
        if (selection == 1) {
            if (game->players[i].has_moved == false) {
            do{
                handleUserMovement(game,i);
            }while (handleUserMovement(game,i) == false);
        }
        else{
            printf(" %c already moved this turn!\n",game->players[i].symbol);
            i--;
            continue;
        }
        }
        else if (selection == 2){
            handleUserAttack(game,i);
        }
        else if (selection == 3)
        {
            continue;
        }
        DisplayMap(game);
    }
}
bool handleUserMovement(GameState *game, int i){
    int movCount = 0;
        char actionBuffer[15];
        printf("Enter movement command:\n");
        scanf("%s", actionBuffer);
        if (actionBuffer[0] != game->players[i].symbol){ // an evale diaforetiko symvolo to ksanakanei
            return false;
        }
        if (actionBuffer[1] != '>'){ // an evale diaforetiko apto > to ksanakanei
            return false;
        }
        if (!isAcceptedMovementCharacter(actionBuffer[2])){
            printf("Invalid movement character! Use (Uu/Dd/Ll/Rr)\n");
            return false;
        }
        for(int j = 2; j < strlen(actionBuffer); j += 2){
            if (!isAcceptedMovementCharacter(actionBuffer[j])){
                printf("Invalid movement character! Use (Uu/Dd/Ll/Rr)\n");
                return false;
            }
            char direction = actionBuffer[j];
            j++;
            if (!isdigit(actionBuffer[j])){
                printf("Invalid movement number! Use a number!\n");
                return false;
            }
            int moveUnits = actionBuffer[j] - '0';
            if (isdigit(actionBuffer[j+1])){
                moveUnits = moveUnits * 10 + (actionBuffer[j] + '0');
                j++;
            }
            movCount += moveUnits;
            if (movCount > game->players[i].allowedMovementUnits){
                return false;
              }
            if(!moveHero(game,i,direction,moveUnits)){
                printf("Invalid movement path!\n");
                return false;
            }
            }
            game->players[i].has_moved = true;
            return true;
          
    }
void handleUserAttack(GameState *game,int i){}
bool moveHero(GameState *game,int i, char c,int mDigits){
    char** map = game->map;
    int x = game->players[i].x;
    int y = game->players[i].y;
    switch (c)
    {
    case 'U':
    case 'u':
        for(int i = 0; i < mDigits; i++){
            if(map[x-i][y] != '.'){
                return false;
            }
        }
        map[x][y] = '.';
        x -= mDigits;
        map[x][y] = game->players[i].symbol;
        return true;
        break;
    case 'D':
    case 'd':
        if (x + mDigits >= game->maprow) return false;
        for(int i = 1; i <= mDigits; i++){
            if(map[x+i][y] != '.'){
                return false;
            }
        }
        map[x][y] = '.';
        x += mDigits;
        game->players[i].x = x;
        map[x][y] = game->players[i].symbol;
        return true;
        break;
    case 'L':
    case 'l':
        if (y - mDigits < 0) return false;
        for(int i = 1; i <= mDigits; i++){
            if(map[x][y-i] != '.'){
                return false;
            }
        }
        map[x][y] = '.';
        y -= mDigits;
        game->players[i].y = y;
        map[x][y] = game->players[i].symbol;
        return true;
        break;
    case 'R':
    case 'r':
        if (y + mDigits >= game->mapcol) return false;
        for(int i = 1; i <= mDigits; i++){
            if(map[x][y+i] != '.'){
                return false;
            }
        }
        map[x][y] = '.';
        y += mDigits;
        game->players[i].y = y;
        map[x][y] = game->players[i].symbol;
        return true;
        break;
    default:
        return false;
        break;
    }
}