#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include <SDL3_image/SDL_image.h>
#include <cstdlib> // for rand()

SDL_Window *gWindow;                    // Declare a pointer
SDL_Renderer* gRenderer;
SDL_Texture* snakeTexture;
SDL_Texture* foodTexture;
SDL_Texture* gameOverTexture;
SDL_FRect gameOverRect;
bool done = false;
bool gameOver = false;
constexpr int SNAKE_SIZE{20};
constexpr int SCREEN_WIDTH{640};
constexpr int SCREEN_HEIGHT{480};

int directionX{1};
int directionY{0};

struct SnakeSegment {
    float x, y;
};

std::vector<SnakeSegment> snake;

 struct BallSegement {
    float x,y;
};

BallSegement food;
bool Init(){
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL3
    // Create an application window with the following settings:
   gWindow = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (gWindow == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, NULL);
        // Check that the window was successfully created
    if (gRenderer == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s\n", SDL_GetError());
        return false;
    }
return true;
}

SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_DestroySurface(loadedSurface);
    }
    return newTexture;
}

void loadMedia(){

snakeTexture = loadTexture("images/snake1.bmp");
foodTexture = loadTexture("images/ball.bmp");
gameOverTexture = loadTexture("images/gameover.png");
}
void renderSnake(){
    //snakeTexture = loadTexture("E:/Programming 2/CPractice/INTERSHIP PROGRAMS/SDL3 snake game/hello.png");

    //SDL_FRect snakeRect  = {snakeRect.x = 10,snakeRect.y=50,snakeRect.w=25,snakeRect.h=25};
    for (size_t i = 0; i < snake.size(); ++i) {

        SDL_FRect snakeRect = {snake[i].x, snake[i].y, SNAKE_SIZE, SNAKE_SIZE};
         SDL_RenderTexture(gRenderer, snakeTexture, nullptr, &snakeRect);
    }
}
void renderFood(){
    SDL_FRect ballRect = {food.x,food.y , SNAKE_SIZE, SNAKE_SIZE};
    SDL_RenderTexture(gRenderer,foodTexture,nullptr,&ballRect);
}
void renderGameOver(){
    SDL_RenderTexture(gRenderer,gameOverTexture,nullptr,&gameOverRect);
}

void moveSnake(){
    float newX = snake[0].x + directionX*SNAKE_SIZE;
    float newY = snake[0].y+ directionY*SNAKE_SIZE;

     SnakeSegment newHead = {newX, newY};
    snake.insert(snake.begin(), newHead);
    snake.pop_back();
}

void spawnFood(){

    food.x = rand() % (SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
    food.y = rand() % (SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;
}
void checkCollisionWithFood(){
       SnakeSegment& head = snake[0]; // Get a reference to the head of the snake

    // Check if the head collides with the food
    if (head.x == food.x && head.y == food.y) {
        // Increase snake size by adding a new segment at the tail
        SnakeSegment tail = snake.back();
        snake.push_back({tail.x, tail.y});

        // Spawn new food at a random location
        spawnFood();
    }
}
bool checkCollisionWithWalls(){

    if(snake[0].x<0 || snake[0].x>=SCREEN_WIDTH || snake[0].y<0 || snake[0].y>=SCREEN_HEIGHT){
        return true;
    }
    else 
    return false;
}

bool selfCollision() {
    SnakeSegment &head = snake[0];
    for (size_t i = 1; i < snake.size(); ++i) { // Start from 1, not 0
        if (head.x == snake[i].x && head.y == snake[i].y) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {

if(!Init()){
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize window: %s\n", SDL_GetError());
    return 1;
}
loadMedia();
  

snake.push_back({SCREEN_WIDTH/2,SCREEN_HEIGHT/2});

spawnFood();


    gameOverRect.x = (SCREEN_WIDTH - 350) / 2; // Adjust these values as needed
    gameOverRect.y = (SCREEN_HEIGHT - 250) / 2;
    gameOverRect.w = 350;
    gameOverRect.h = 250;

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
         // Get the current keyboard state
    const bool* currentKeyState = SDL_GetKeyboardState(NULL);
    //moving snake
        if(currentKeyState[SDL_SCANCODE_UP] && directionY==0){
            directionY = -1;
            directionX = 0;
        }
         if(currentKeyState[SDL_SCANCODE_DOWN] && directionY==0){
            directionY = 1;
            directionX = 0;
        }
         if(currentKeyState[SDL_SCANCODE_RIGHT] && directionX==0){
            directionY = 0;
            directionX = 1;
        }
         if(currentKeyState[SDL_SCANCODE_LEFT] && directionX==0){
            directionY = 0;
            directionX = -1;
        }
if(checkCollisionWithWalls()){
    gameOver = true;
}
if(selfCollision()){
    gameOver = true;
}
checkCollisionWithFood();
moveSnake();

        SDL_SetRenderDrawColor(gRenderer,0,255,0,255);
        SDL_RenderClear(gRenderer);
        //SDL_RenderTexture(gRenderer,snakeTexture,nullptr,&snakeRect);
        if(gameOver==true){
            renderGameOver();
        }
        else{
        renderSnake();
         renderFood();
     }
        SDL_RenderPresent(gRenderer);
        SDL_Delay(100);
        // Do game logic, present a frame, etc.
    }

    // Close and destroy the window
    SDL_DestroyWindow(gWindow);

    // Clean up
    SDL_Quit();
    return 0;
}