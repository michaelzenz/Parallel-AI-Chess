#include"AI.h"
#include"time.h"
#include"timer.h"
//scores for different pieces at different position

#define MAXSTEP_PLAY 3

int pos_scores_pawn[2][64]=
{
    {0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0},

    {0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10,-20,-20, 10, 10,  5,
    5, -5,-10,  0,  0,-10, -5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5,  5, 10, 25, 25, 10,  5,  5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0,  0,  0,  0,  0,  0,  0,  0}
};

int pos_scores_knights[2][64]=
{
    {-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50},

    {-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50}
};

int pos_scores_bishops[2][64]=
{
    {-20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20},

    {-20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20}
};

int pos_scores_rooks[2][64]=
{
    {0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    1,  0,  0,  5,  5,  0,  0,  1},

    { 1,  0,  0,  5,  5,  0,  0,  1,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    5, 10, 10, 10, 10, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0}
};

int pos_scores_queens[2][64]=
{
    {-20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20},

    {-20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  5,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, 0,
    -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20}
};

int pos_scores_kings_mid[2][64]=
{
    {-30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20},

    {20, 30, 10,  0,  0, 10, 30, 20,
    20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30}
};

int pos_scores_kings_end[2][64]=
{
    {-50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50},

    {-50,-30,-30,-30,-30,-30,-30,-50,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -50,-40,-30,-20,-20,-30,-40,-50}
};

//scores of different pieces
int piece_scores[7]={0,100,300,500,300,900,9000};

//the print board function for ai, for logging
void ai_print_board(GameState *gameState)
{
    for(int i=0;i<64;i++)
    {
        if(i%8==0)printf("\n");
        printf("%d\t",gameState->board[i]);
    }
    printf("\n*****************\n*****************\n");
}

//sum up the scores including pieces and location scores
int ai_sum_scores(GameState *gameState, Player *player)
{
    int sum=0;
    for(int i=0;i<64;i++)
    {
        int color=gameState->board[i]/(MAX(abs(gameState->board[i]),1));
        int color_id=MAX(color*-1,0);
        switch(abs(gameState->board[i]))
        {
            case BLANK:
                break;
            case PAWN:
                sum+=color*(piece_scores[PAWN]+pos_scores_pawn[color_id][i]);
                break;
            case KNIGHT:
                sum+=color*(piece_scores[KNIGHT]+pos_scores_knights[color_id][i]);
                break;
            case CASTLE:
                sum+=color*(piece_scores[CASTLE]+pos_scores_rooks[color_id][i]);
                break;
            case BISHOP:
                sum+=color*(piece_scores[BISHOP]+pos_scores_bishops[color_id][i]);
                break;
            case QUEEN:
                sum+=color*(piece_scores[QUEEN]+pos_scores_queens[color_id][i]);
                break;
            case KING:
                sum+=color*(piece_scores[KING]+pos_scores_kings_mid[color_id][i]);
                break;
            default:
                break;
        }
    }
    #ifdef CHECK_SCORES
    ai_print_board(gameState);
    printf("%d is playing, Scores: %d", player->color, sum);
    #endif
    return sum;
}

//the global function for ai to select model and play
int ai_play(GameState *gameState,Player *player, int model)
{
    int quit=0;
    switch(model)
    {
        case AI_SERIAL:
            quit=ai_serial_play(gameState,player,MAXSTEP_PLAY);
            break;
        case EASY:
            quit=ai_model1_play(gameState,player,MAXSTEP_PLAY);
            break;
        case MEDIUM:
            quit=ai_model2_play(gameState,player,MAXSTEP_PLAY);
            break;
        case ADVANCED:
            quit=ai_model3_play(gameState,player,MAXSTEP_PLAY);
            break;
        case 4:
            quit=ai_model4_play(gameState,player,MAXSTEP_PLAY);
            break;
    }
    return quit;
}

int ai_experiment(GameState *gameState,Player *player, 
    int model, double *time, int maxStep)
{
    int quit=0;
    struct stopwatch_t* timer = stopwatch_create ();
    stopwatch_start (timer);
    switch(model)
    {
        case AI_SERIAL:
            quit=ai_serial_play(gameState,player,maxStep);
            break;
        case EASY:
            quit=ai_model1_play(gameState,player,maxStep);
            break;
        case MEDIUM:
            quit=ai_model2_play(gameState,player,maxStep);
            break;
        case ADVANCED:
            quit=ai_model3_play(gameState,player,maxStep);
            break;
        case 4:
            quit=ai_model4_play(gameState,player,maxStep);
            break;
        case 5:
            quit=ai_model2_AB_play(gameState,player,maxStep);
            break;
        case 6:
            quit=ai_model3_AB_play(gameState,player,maxStep);
            break;
    }
    *time=stopwatch_stop (timer);
    stopwatch_destroy(timer);
    return quit;
}