#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
typedef struct {
    char x, y;
    char health;
    char symbol;
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
int main(void) {
    GameState game;
    game.monster_count = 0;
    game.map = NULL;
    game.maprow = 5;
    game.mapcol = 9;
    game.enemies = (Enemy*)malloc(sizeof(Enemy));
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
}

    do {
        GenerateRoom(&game);
        printf("ekane save to room!");
        if (game.level > 1){
        makeWalls(&game);
        }
         SpawnFurniture(&game);
         SpawnEnemies(&game);
        DisplayMap(&game);
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