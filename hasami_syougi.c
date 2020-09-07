#include <drawlib.h>
#include <stdlib.h>
#include <stdio.h>
// グローバル関数
int position[9][9], player = 1, key, xy_num[2], xy_num2[2], token_count[2] = {0, 0};
// スタート画面を描く
void start(void);
// 将棋盤を描く
void disp_bord(void);
// コマを初期状態にもどす（状態は２次元配列に代入）
void bord_riset(void);
// コマを表示する
void disp_piece(void);
// マスを強調する
void strong(int x_num,int y_num);
// 手番を表示する
void offence_and_defense(void);
// 勝敗が決したときの画面（０がプレイヤー１の勝利、１がプレイヤー２の勝利）
void disp_result(int player_num);
// スタート画面のボタン（押されたときに１を返す）
int start_button(void);
// コマの状態を判定
int judge(int side, int vertical);
//クリックされた座標をマスの番号に変え,引数が1のときxy_numに、引数が2のときxy_num2に、配列として代入する
void conversion(int ver);
// その手が打てるか判断する（可なら1,不可なら0を返す）
int judge_action(void);
//マスの状態を変更する（int "player1:1、player2:2、存在しない:0", int "コマのx座標”, int "コマのy座標”）
void change(int status, int x_num, int y_num);
// エラーを表示
void error(void);
// 自身のコマを動かす処理
void action(void);
// 相手プレイヤーの番号を表示
int rival_player(void);
//コマとコマの間のコマを消去（削除できたら1、失敗したら0を返す）
int delete(int x_1, int y_1, int x_2, int y_2);
// 置いたコマの影響を判定する
void effect(void);
// コマを数える
void player_token_count(void);
// 勝敗を決める(試合終了で１，続行で０を返す)
int result(void);
// 選択したコマと選択した場所の間にコマが存在していないか判定（存在している：０,存在していない：１）
int judge_is(void);
// とったコマを表示する
void disp_cemetery(void);

// メイン関数
int main(void){
  // drawlib を初期化
  dl_initialize(1.0);
  // スタート画面を描写
  start();

  if (start_button() == 1){
    // 将棋盤を初期状態にリセット
    bord_riset();

    while (1){
      // 将棋盤を表示
      disp_bord();
      // コマを表示する
      disp_piece();

      // プレイヤー切り替え
      if (player == 1){
        player = 0;
      }else if (player == 0){
        player = 1;
      }

      // 手番を表示する
      offence_and_defense();
      // コマを操作する
      action();
      // コマの効果を表示する
      effect();
      // 残りのコマを数える
      player_token_count();

      // とったコマを表示する
      disp_cemetery();

      // 勝利条件を確認する
      if (result() == 1){
        break;
      }

      token_count[0] = 0;
      token_count[1] = 0;
    }
  }
  return 0;
}

// スタート画面を描く
void start(void){
  dl_clear(dl_color_from_name("black"));
  dl_rectangle(5, 5, 635, 475, dl_color_from_name("white"), 2, 0);
  dl_rectangle(12, 12, 628, 468, dl_color_from_name("white"), 2, 0);
  dl_text("Hasami", 70, 150, 3, dl_color_from_name("white"), 6);
  dl_text("Shogi", 300, 250, 3, dl_color_from_name("white"), 6);
  dl_rectangle(195, 310, 445, 400, dl_color_from_name("white"), 2, 0);
  dl_rectangle(200, 315, 440, 395, dl_color_from_name("white"), 1, 0);
  dl_text("Start!", 235, 377, 2, dl_color_from_name("white"), 2);
}

// 将棋盤を描く
void disp_bord(void){
  dl_rectangle(90, 10, 550, 470, dl_color_from_name("black"), 1, 1);
  for (int i = 0; i < 10; i++)
  {
    dl_line(95 + (i * 50), 15, 95 + (i * 50), 465, dl_color_from_name("white"), 2);
    dl_line(95, 15 + (i * 50), 545, 15 + (i * 50), dl_color_from_name("white"), 2);
  }
}

// コマを初期状態にもどす（状態は２次元配列に代入）
void bord_riset(void){
  dl_clear(dl_color_from_name("white"));
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      position[i][j] = 0;
    }
  }
  for (int g = 0; g < 9; g++){
    position[g][0] = 2;
    position[g][8] = 1;
  }
}

// コマを表示する
void disp_piece(void){
  for (int i = 0; i < 9; i++){
    for (int j = 0; j < 9; j++){
      if (position[i][j] == 1){//プレイヤー１のコマを置く
        dl_text("A", ((i) * 50) + 102, ((j + 1) * 50) + 10, 2, dl_color_from_name("white"), 2);
      }else if (position[i][j] == 2){//プレイヤー２のコマを置く
        dl_text("V", ((i) * 50) + 102, ((j + 1) * 50) + 10, 2, dl_color_from_name("white"), 2);
      }
    }
  }
}

// マスを強調する
void strong(int x_num,int y_num){
  dl_circle(((x_num - 1) * 50) + 120, ((y_num - 1) * 50) + 39, 25, dl_color_from_name("white"), 1, 1);

  if (player + 1 == 1){
    dl_text("A", ((x_num - 1) * 50) + 102, (y_num * 50) + 10, 2, dl_color_from_name("black"), 2);
  }else{
    dl_text("V", ((x_num - 1) * 50) + 102, (y_num * 50) + 10, 2, dl_color_from_name("black"), 2);
  }
}

// 手番を表示する
void offence_and_defense(void){
  if (player + 1 == 1){
    dl_rectangle(15, 40, 75, 100, dl_color_from_name("white"), 2, 1);
    dl_rectangle(15, 40, 75, 100, dl_color_from_name("black"), 2, 0);
    dl_line(15, 40, 75, 100, dl_color_from_name("black"), 2);
    dl_rectangle(565, 380, 625, 440, dl_color_from_name("black"), 2, 1);
    dl_text("A", 576, 430, 2, dl_color_from_name("white"), 2);
  }else{
    dl_rectangle(565, 380, 625, 440, dl_color_from_name("white"), 2, 1);
    dl_rectangle(565, 380, 625, 440, dl_color_from_name("black"), 2, 0);
    dl_line(565, 380, 625, 440, dl_color_from_name("black"), 2);
    dl_rectangle(15, 40, 75, 100, dl_color_from_name("black"), 2, 1);
    dl_text("V", 27, 91, 2, dl_color_from_name("white"), 2);
  }
}

// とったコマを表示する
void disp_cemetery(void){
  for (int i = 0; i < 9 - token_count[0]; i++){
    dl_text("A", 27, 160 + 50 * i, 2, dl_color_from_name("black"), 2);
  }
  for (int g = 0; g < 9 - token_count[1]; g++){
    dl_text("V", 576, 362 - 50 * g, 2, dl_color_from_name("black"), 2);
  }
}

// 勝敗が決したときの画面（０がプレイヤー１の勝利、１がプレイヤー２の勝利）
void disp_result(int player_num){
  // 将棋盤を表示
  disp_bord();
  // コマを表示する
  disp_piece();
  if (player_num == 1){
    for (int i = 100; i >= 0; i--){
      dl_rectangle(0, 0, 640, 240 - 2 * i, dl_color_from_name("black"), 1, 1);
      dl_rectangle(5, 5, 635, 235 - 2 * i, dl_color_from_name("white"), 2, 0);
      dl_text("lose..", 125, 170 - i, 5, dl_color_from_name("white"), 7);
      dl_rectangle(0, 240 + 2 * i, 640, 480, dl_color_from_name("white"), 1, 1);
      dl_rectangle(5, 245 + 2 * i, 635, 475, dl_color_from_name("black"), 2, 0);
      dl_text("win!!", 150, 420 + i, 5, dl_color_from_name("black"), 7);
      dl_wait(0.02);
    }
  }else if (player_num == 2){
    for (int g = 100; g >= 0; g--){
      dl_rectangle(0, 0, 640, 240 - 2 * g, dl_color_from_name("white"), 1, 1);
      dl_rectangle(5, 5, 635, 235 - 2 * g, dl_color_from_name("black"), 2, 0);
      dl_text("win!!", 150, 170 - g, 5, dl_color_from_name("black"), 7);
      dl_rectangle(0, 240 + 2 * g, 640, 480, dl_color_from_name("black"), 1, 1);
      dl_rectangle(5, 245 + 2 * g, 635, 475, dl_color_from_name("white"), 2, 0);
      dl_text("lose..", 125, 420 + g, 5, dl_color_from_name("white"), 7);
      dl_wait(0.02);
    }
  }
  dl_wait(7);
}

// スタート画面のボタン（押されたときに１を返す）
int start_button(void){
  int t, k, x, y;
  while (1){
    if(dl_get_event(&t, &k, &x, &y)){
      if(t==DL_EVENT_L_DOWN){
        if (195 <= x && x <= 445 && 310 <= y && y <= 400){
          return 1;
        }
      }
    }
    dl_wait(0.2);
  }
}

// コマの状態を判定
int judge(int side, int vertical){
  if (position[side - 1][vertical - 1] == 1){
    return 1;
  }else if (position[side - 1][vertical - 1] == 2){
    return 2;
  }else{
    return 0;
  }
}

//クリックされた座標をマスの番号に変え,引数が1のときxy_numに、引数が2のときxy_num2に、配列として代入する
void conversion(int ver){
  int is_event = 0 , t, k, x, y;
  while (1){
    if(dl_get_event(&t, &k, &x, &y)){
      if(t==DL_EVENT_L_DOWN){
        int x_position = x - 95, y_position = y - 15;
        if (x_position < 0 || x_position > 450 || y_position < 0 || y_position > 450){
          continue;
        }
        for (int i = 0; i < 9; i++){
          if (i * 50 <= x_position && x_position < (i + 1) * 50){
            if (ver == 1){
              xy_num[0] = i + 1;
              is_event = 1;
            }else if (ver == 2){
              xy_num2[0] = i + 1;
              is_event = 1;
            }
          }
          if (i * 50 <= y_position && y_position < (i + 1) * 50){
            if (ver == 1){
              xy_num[1] = i + 1;
              is_event = 1;
            }else if (ver == 2){
              xy_num2[1] = i + 1;
              is_event = 1;
            }
          }
        }
      }
    }
    if (is_event == 1){
      break;
    }
    dl_wait(0.1);
  }
}

// 選択したコマと選択した場所の間にコマが存在していないか判定（存在している：０,存在していない：１）
int judge_is(void){
  if (xy_num[0] == xy_num2[0]){
    if (xy_num[1] > xy_num2[1]){
      for (int i = xy_num2[1] + 1; i < xy_num[1]; i++){
        if (judge(xy_num[0],i) == 1 || judge(xy_num[0],i) == 2){
          return 0;
        }
      }
      return 1;
    }else if (xy_num[1] < xy_num2[1]){
      for (int g = xy_num[1] + 1; g < xy_num2[1]; g++){
        if (judge(xy_num[0],g) == 1 || judge(xy_num[0],g) == 2){
          return 0;
        }
      }
      return 1;
    }
    return 0;
  }else if (xy_num[1] == xy_num2[1]){
    if (xy_num[0] > xy_num2[0]){
      for (int i = xy_num2[0] + 1; i < xy_num[0]; i++){
        if (judge(i,xy_num[1]) == 1 || judge(i,xy_num[1]) == 2){
          return 0;
        }
      }
      return 1;
    }else if (xy_num[0] < xy_num2[0]){
      for (int i = xy_num[0] + 1; i < xy_num2[0]; i++){
        if (judge(i,xy_num[1]) == 1 || judge(i,xy_num[1]) == 2){
          return 0;
        }
      }
      return 1;
    }
    return 0;
  }
}

// その手が打てるか判断する（可なら1,不可なら0を返す）
int judge_action(void){
  if (judge(xy_num[0],xy_num[1]) == player + 1//選択したコマが自身のコマか判定
      && judge(xy_num2[0],xy_num2[1]) == 0//選択した場所にコマが存在していないか判定
      && judge_is() == 1//選択したコマと選択した場所の間にコマが存在していないか判定 
      && (xy_num[0] == xy_num2[0] || xy_num[1] == xy_num2[1])){//ルールに準拠した移動であるか判定
    return 1;
  }
  return 0;
}

//マスの状態を変更する（int "player1:1、player2:2、存在しない:0", int "コマのx座標”, int "コマのy座標”）
void change(int status, int x_num, int y_num){
  if (status == 0){
    position[x_num - 1][y_num - 1] = 0;
    return;
  }else if (status == 1){
    position[x_num - 1][y_num - 1] = 1;
    return;
  }else if (status == 2){
    position[x_num - 1][y_num - 1] = 2;
    return;
  }
  printf("error:can not change");
}

// エラーを表示
void error(void){
  dl_rectangle(95, 190, 545, 290, dl_color_from_name("red"), 1, 1);
  if (player == 0){
    dl_text("Can not put A", 95, 255, 2, dl_color_from_name("white"), 2);
  }else {
    dl_text("Can not put Y", 95, 255, 2, dl_color_from_name("white"), 2);
  }
  dl_wait(1);
  // 将棋盤を表示
  disp_bord();
  // コマを表示する
  disp_piece();
}

// 自身のコマを動かす処理
void action(void){
  while (1){
    conversion(1);
    // 選んだコマを強調する
    if (judge(xy_num[0],xy_num[1]) == player + 1){
      strong(xy_num[0], xy_num[1]);
    }else {
      continue;
    }
    conversion(2);
    if (judge_action() == 1){
      change(0, xy_num[0], xy_num[1]);
      change(player + 1, xy_num2[0], xy_num2[1]);
      break;
    }else{
      error();
    }
  }
}

// 相手プレイヤーの番号を表示
int rival_player(void){
  if (player == 0){
    return 1;
  }else if (player == 1){
    return 0;
  }
}

//コマとコマの間のコマを消去（削除できたら1、失敗したら0を返す）
int delete(int x_1, int y_1, int x_2, int y_2){
  if (x_1 == x_2){
    if (y_1 < y_2){
      for (int i = y_1; i < y_2 - 1; i++){
        position[x_1 - 1][i] = 0;
      }
    }else if (y_1 > y_2){
      for (int g = y_2; g < y_1 - 1; g++){
        position[x_1 - 1][g] = 0;
      }
    }else{
      return 0;
    }
    return 1;
  }else if (y_1 == y_2){
    if (x_1 < x_2){
      for (int i = x_1; i < x_2 - 1; i++){
        position[i][y_1 - 1] = 0;
      }
    }else if (x_1 > x_2){
      for (int g = x_2; g < x_1 - 1; g++){
        position[g][y_1 - 1] = 0;
      }
    }else{
      return 0;
    }
    return 1;
  }else{
    return 0;
  }
}

// 置いたコマの影響を判定する
void effect(void){
  int count;
  //上下方向の影響を判定
  for (int h = 0; h < 2; h++){
    count = xy_num2[1];
    while (1){
      if (h == 0){
        count--;
      }else if (h == 1){
        count++;
      }
      if (count == 0 || count == 10){
        break;
      }else if (judge(xy_num2[0], count) == rival_player() + 1){
        continue;
      }else if (judge(xy_num2[0], count) == player + 1){
        delete(xy_num2[0],xy_num2[1],xy_num2[0],count);
        break;
      }else {
        break;
      }
    }
  }
  // 左右方向の影響を判定
  for (int j = 0; j < 2; j++){
    count = xy_num2[0];
    while (1){
      if (j == 0){
        count++;
      }else if (j == 1){
        count--;
      }
      if (count == 0 || count == 10){
        break;
      }else if (judge(count, xy_num2[1]) == rival_player() + 1){
        continue;
      }else if (judge(count, xy_num2[1]) == player + 1){
        delete(xy_num2[0],xy_num2[1], count, xy_num2[1]);
        break;
      }else {
        break;
      }
    }
  }
}

// コマを数える
void player_token_count(void){
  for (int i = 1; i <= 9; i++){
    for (int g = 1; g <= 9; g++){
      if (judge(i, g) == 1){
        token_count[0]++;
      }else if (judge(i, g) == 2){
        token_count[1]++;
      }
    }
  }
}

// 勝敗を決める(試合終了で１，続行で０を返す)
int result(void){
  if (token_count[0] - token_count[1] >= 3 && player == 1){
    disp_result(1);
    return 1;
  }else if (token_count[1] - token_count[0] >= 3 && player == 0){
    disp_result(2);
    return 1;
  }else if (token_count[1] <= 4){
    disp_result(1);
    return 1;
  }else if (token_count[0] <= 4){
    disp_result(2);
    return 1;
  }else{
    return 0;
  }
}