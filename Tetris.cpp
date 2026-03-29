int rows[8] = {13,12,11,10,9,8,7,6};
int cols[8] = {5,4,3,2,A1,A2,A3,A4};

int leftBtn = A5;
int rightBtn = A0;

int board[8][8] = {0};

int shapes[2][4][4] = {
  {
    {1,1,0,0},
    {1,1,0,0},
    {0,0,0,0},
    {0,0,0,0}
  },
  {
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0},
    {0,0,0,0}
  }
};

int currentShape[4][4];
int shapeType = 0;

int x = 2;
int y = 0;

unsigned long lastFall = 0;
int fallDelay = 500;

void setup() {
  for(int i=0;i<8;i++){
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }

  pinMode(leftBtn, INPUT_PULLUP);
  pinMode(rightBtn, INPUT_PULLUP);

  spawnShape();
}

void spawnShape() {
  shapeType = random(0,2);

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      currentShape[i][j] = shapes[shapeType][i][j];
    }
  }

  x = 2;
  y = 0;
}

void draw() {
  for(int r=0;r<8;r++){
    digitalWrite(rows[r], LOW);

    for(int c=0;c<8;c++){
      bool pixel = board[r][c];

      for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
          if(currentShape[i][j]){
            if(r == y+i && c == x+j){
              pixel = true;
            }
          }
        }
      }

      digitalWrite(cols[c], pixel ? HIGH : LOW);
    }

    delayMicroseconds(2000);
    digitalWrite(rows[r], HIGH);
  }
}

bool canMove(int nx, int ny) {
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      if(currentShape[i][j]){
        int newX = nx + j;
        int newY = ny + i;

        if(newX < 0 || newX >= 8 || newY >= 8)
          return false;

        if(newY >= 0 && board[newY][newX])
          return false;
      }
    }
  }
  return true;
}

void lockShape() {
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      if(currentShape[i][j]){
        board[y+i][x+j] = 1;
      }
    }
  }
}

void clearLines() {
  for(int r=0;r<8;r++){
    bool full = true;

    for(int c=0;c<8;c++){
      if(board[r][c] == 0){
        full = false;
        break;
      }
    }

    if(full){
      for(int rr=r; rr>0; rr--){
        for(int c=0;c<8;c++){
          board[rr][c] = board[rr-1][c];
        }
      }

      for(int c=0;c<8;c++){
        board[0][c] = 0;
      }
    }
  }
}

void rotate() {
  int temp[4][4];

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      temp[i][j] = currentShape[3-j][i];
    }
  }

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      if(temp[i][j]){
        int newX = x + j;
        int newY = y + i;

        if(newX < 0 || newX >= 8 || newY >= 8)
          return;

        if(newY >= 0 && board[newY][newX])
          return;
      }
    }
  }

  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      currentShape[i][j] = temp[i][j];
    }
  }
}

void loop() {

  if(digitalRead(leftBtn) == LOW){
    if(canMove(x-1,y)) x--;
    delay(120);
  }

  if(digitalRead(rightBtn) == LOW){
    if(canMove(x+1,y)) x++;
    delay(120);
  }

  if(digitalRead(leftBtn) == LOW && digitalRead(rightBtn) == LOW){
    rotate();
    delay(200);
  }

  if(millis() - lastFall > fallDelay){
    if(canMove(x, y+1)){
      y++;
    } else {
      lockShape();
      clearLines();
      spawnShape();
    }
    lastFall = millis();
  }

  draw();
}
