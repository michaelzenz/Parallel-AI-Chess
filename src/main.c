#include"AI.h"
#include"GUI.h"
#include"struct.h"
#include"constant.h"
#include"timer.h"

#include<sys/stat.h>
#include<sys/types.h>

#define MODEL 1

int play(GameState *gameState,Player *player,int model)
{
    int quit;
    
    if(player->identity==HUMAN) quit=gui_play(gameState,player);
    else quit=ai_play(gameState,player,model);
    return quit;
}

void Game(int argc, char *argv[],int model)
{
    gui_init_window(argc,argv);

    Player player_arr[2];
    GameState gameState=env_init();
    Player player1,player2;
    gui_init(&gameState,player_arr);
    player1=player_arr[0];
    player2=player_arr[1];
    player1.id=PLAYER1;
    player2.id=PLAYER2;

    int quit=0;
    while(quit==0)
    {
        int quit;
        if(gameState.playerTurn==player1.color)
            quit=play(&gameState,&player1,4);
        else 
            quit=play(&gameState,&player2,4);
        // stack_print_log(&(gameState.moves_stack));
        if(quit==CUR_PLAYER_LOSE||quit==CUR_PLAYER_WIN)
        {
            gui_refresh(&gameState,player_arr);
            gui_checkmate_window(&gameState,quit);
            return;
        }
        else if(quit==QUIT)
        {
            return;
        }
        else gui_refresh(&gameState,player_arr);
    }
}

void print_board(GameState *gameState)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        printf("%d\t",gameState->board[i]);
    }
    printf("\n*****************\n");
}

void fprint_board(GameState *gameState,FILE *fp)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        fprintf(fp,"%d\t",gameState->board[i]);
    }
    fprintf(fp,"\n*****************\n");
}

void AI_Contest(int argc, char *argv[])
{
    assert(argc>3);
    int model1=atoi(argv[1]),model2=atoi(argv[2]);
    int maxStep=atoi(argv[3]);

    GameState gameState=env_init();
    Player player1,player2;

    player1.id=0;
    player2.id=1;
    player1.identity=player2.identity=COMPUTER;

    int ContestTimes=50;
    char str_color[2]={'W','B'};
    
    mkdir("data",S_IRWXU);
    FILE *fp = NULL;
    char fileName[30];
    sprintf(fileName,"data/model%d_vs_model%d maxStep:%d\n",model1,model2,maxStep);
    printf("model%d_vs_model%d maxStep:%d\n",model1,model2,maxStep);
    fp = fopen(fileName, "w");
    
    int model1_wins=0,model2_wins=0,draw=0;
    double model1_sum_time=0,model2_sum_time=0;
    for(int i=0;i<ContestTimes;i++){

        int rand_color=(rand()%2)*2-1;
        player1.color=rand_color;
        player2.color=rand_color*-1;

        printf("\nmodel%d:%c vs model%d:%c\n",
            model1,str_color[MAX(player1.color*-1,0)],
            model2,str_color[MAX(player2.color*-1,0)]);
        fprintf(fp,"\nmodel%d:%c vs model%d:%c\n",
            model1,str_color[MAX(player1.color*-1,0)],
            model2,str_color[MAX(player2.color*-1,0)]);

        int quit=0;
        int round1=0, round2=0;
        double model1_time=0,model2_time=0;
        double model1_round_time=0,model2_round_time=0;
        while(quit==0)
        {
            if(gameState.playerTurn==player1.color){
                quit=ai_experiment(&gameState,&player1,
                    model1,&model1_round_time,maxStep);
                round1++;
            }
            else{
                quit=ai_experiment(&gameState,&player2,
                    model2,&model2_round_time,maxStep);
                round2++;
            }
            model1_time+=model1_round_time;
            model2_time+=model2_round_time;
            
            if(round1+round2>200)break;
        }
        model1_time/=round1;
        model2_time/=round2;
        model1_sum_time+=model1_time;
        model2_sum_time+=model2_time;
        fprintf(fp,"model%d takes %f ms, model%d takes %f ms to make a move\n",
            model1, model1_time, model2, model2_time);
        if(quit==0) draw++;
        else if(quit==1){
            if(gameState.playerTurn==player1.color)model1_wins++;
            else model2_wins++;
        }
        else if(quit==2){
            if(gameState.playerTurn==player1.color)model2_wins++;
            else model1_wins++;
        }
        env_reset(&gameState);

        printf("game %d, model%d takes %f ms, model%d takes %f ms to make a move\n",
            i+1,model1,model1_time,model2,model2_time);
    }
    printf("\nSummary:\n\n");
    fprintf(fp,"\nSummary:\n\n");
    fprintf(fp,"model%d wins %d times, model%d wins %d times, draw %d times\n",
    model1,model1_wins,model2,model2_wins,draw);
    printf("model%d wins %d times, model%d wins %d times, draw %d times\n",
    model1,model1_wins,model2,model2_wins,draw);
    fprintf(fp,"model%d takes %f ms, model%d takes %f ms in avg to make a move\n",
        model1,model1_sum_time/ContestTimes, model2, model2_sum_time/ContestTimes);
    printf("model%d takes %f ms, model%d takes %f ms in avg to make a move\n",
        model1,model1_sum_time/ContestTimes, model2, model2_sum_time/ContestTimes);
    fclose(fp);
}

void AI_ContestWithGUI(int argc, char *argv[])
{
    assert(argc>3);
    int model1=atoi(argv[1]),model2=atoi(argv[2]);
    int maxStep=atoi(argv[3]);

    gui_init_window(argc,argv);
    GameState gameState=env_init();
    Player player1,player2;

    player1.id=0;
    player2.id=1;
    player1.identity=player2.identity=COMPUTER;
    gui_gameplay_window(&gameState);

    int ContestTimes=50;
    char str_color[2]={'W','B'};
    
    mkdir("data",S_IRWXU);
    FILE *fp = NULL;
    char fileName[30];
    sprintf(fileName,"data/model%d_vs_model%d maxStep:%d\n",model1,model2,maxStep);
    printf("model%d_vs_model%d maxStep:%d\n",model1,model2,maxStep);
    fp = fopen(fileName, "w");
    
    int model1_wins=0,model2_wins=0,draw=0;
    double model1_sum_time=0,model2_sum_time=0;
    for(int i=0;i<ContestTimes;i++){

        int rand_color=(rand()%2)*2-1;
        player1.color=rand_color;
        player2.color=rand_color*-1;

        printf("\nmodel%d:%c vs model%d:%c\n",
            model1,str_color[MAX(player1.color*-1,0)],
            model2,str_color[MAX(player2.color*-1,0)]);
        fprintf(fp,"\nmodel%d:%c vs model%d:%c\n",
            model1,str_color[MAX(player1.color*-1,0)],
            model2,str_color[MAX(player2.color*-1,0)]);

        int quit=0;
        int round1=0, round2=0;
        double model1_time=0,model2_time=0;
        double model1_round_time=0,model2_round_time=0;
        while(quit==0)
        {
            if(gameState.playerTurn==player1.color){
                quit=ai_experiment(&gameState,&player1,
                    model1,&model1_round_time,maxStep);
                round1++;
            }
            else{
                quit=ai_experiment(&gameState,&player2,
                    model2,&model2_round_time,maxStep);
                round2++;
            }
            model1_time+=model1_round_time;
            model2_time+=model2_round_time;
            // sleep(1);
            //print_board(&gameState);
            gui_refresh(&gameState,&player1);
            
            if(round1+round2>200)break;
        }
        model1_time/=round1;
        model2_time/=round2;
        model1_sum_time+=model1_time;
        model2_sum_time+=model2_time;
        fprintf(fp,"model%d takes %f ms, model%d takes %f ms to make a move\n",
            model1, model1_time, model2, model2_time);
        if(quit==0) draw++;
        else if(quit==1){
            if(gameState.playerTurn==player1.color)model1_wins++;
            else model2_wins++;
        }
        else if(quit==2){
            if(gameState.playerTurn==player1.color)model2_wins++;
            else model1_wins++;
        }
        env_reset(&gameState);

        printf("game %d, model%d takes %f ms, model%d takes %f ms to make a move\n",
            i+1,model1,model1_time,model2,model2_time);
    }
    printf("\nSummary:\n\n");
    fprintf(fp,"\nSummary:\n\n");
    fprintf(fp,"model%d wins %d times, model%d wins %d times, draw %d times\n",
    model1,model1_wins,model2,model2_wins,draw);
    printf("model%d wins %d times, model%d wins %d times, draw %d times\n",
    model1,model1_wins,model2,model2_wins,draw);
    fprintf(fp,"model%d takes %f ms, model%d takes %f ms in avg to make a move\n",
        model1,model1_sum_time/ContestTimes, model2, model2_sum_time/ContestTimes);
    printf("model%d takes %f ms, model%d takes %f ms in avg to make a move\n",
        model1,model1_sum_time/ContestTimes, model2, model2_sum_time/ContestTimes);
    fclose(fp);
}


int main(int argc, char *argv[])
{   
    srand(time(0));
    stopwatch_init ();
    // while(1){
    //     Game(argc,argv,1);
    // }
    
    AI_Contest(argc,argv);
    
    return 0;
}