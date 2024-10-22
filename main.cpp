#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "LC1C_20_フクダソウワ_ピカピカ風船";


/*-----------------
    構造体を作る
-----------------*/

// 座標
struct Coordinate2
{
	float x;
	float y;
};

// 位置
struct Pos
{
	// ワールド座標
	struct Coordinate2 world;

	// スクリーン座標
	struct Coordinate2 screen;
};

// 移動速度
struct Vel2
{
	float x;
	float y;
};

// 加速度
struct Acceleration2
{
	float x;
	float y;
};

// 図形の半径
struct Radius2
{
	float x;
	float y;
};

// フラグ
struct Flug
{
	// 飛んでいるかどうか（飛行フラグ）
	int isFlying;

	// 逃げているかどうか（逃げるフラグ）
	int isRunAway;
};

// チュートリアル
struct Tutorial
{
	// 手順
	int step;

	// 飛行時間
	int flyingTimer;

	// 滞空時間
	int hoveringTimer;

	// 照射時間
	int illuminatingTimer;

	// スイッチを起動したかどうか
	int isSwitchOn;
};

// 復活
struct Respawn
{
	// 復活しているかどうか（復活フラグ）
	int isRespawn;

	// 復活処理
	int timer;
};

// プレイヤー
struct Player
{
	// 復活
	struct Respawn respawn;

	// フラグ
	struct Flug flug;

	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 加速度
	struct Acceleration2 acceleration;

	// 図形の半径
	struct Radius2 radius;
};

// 光
struct Light
{
	// 位置
	struct Pos pos;

	// 図形の半径
	struct Radius2 radius;
};

// 敵
struct Enemy
{
	// 復活
	struct Respawn respawn;

	// 出現しているかどうか（出現フラグ）
	int isArrival;

	// 出現している時間
	int arrivalTimer;

	// 種類
	int type;


	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 加速度
	struct Acceleration2 acceleration;

	// 図形の半径
	struct Radius2 radius;

	// 透明度
	int transparency;

	// エネルギー
	int energy;
};


// アイテム
struct Item
{
	// 復活
	struct Respawn respawn;

	// 出現しているかどうか（出現フラグ）
	int isArrival;

	// 出現している時間
	int arrivalTimer;


	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 加速度
	struct Acceleration2 acceleration;

	// 図形の半径
	struct Radius2 radius;
};


/*----------------
    定数を作る
----------------*/

// 縦幅
const int kHeight = 800;

// 横幅
const int kWidth = 700;

// 敵の数
const int kEnemyNum = 256;


/*---------------
    関数を作る
---------------*/

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>座標変換を行った構造体を返却する</returns>
struct Coordinate2 CoordinateTransformation(struct Coordinate2 world);

/// <summary>
/// 復活処理を行う
/// </summary>
/// <param name="respawn">復活</param>
void RespawnProcess(struct Respawn* respawn);

/// <summary>
/// プレイヤーの初期位置を決める
/// </summary>
/// <param name="player">プレイヤー</param>
void PlayerInitialState(struct Player* player);

/// <summary>
/// プレイヤーの操作を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="keys">キー</param>
void PlayerMove(struct Player* player , char* keys);

/// <summary>
/// 敵を作る
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeEnemy(struct Enemy* enemy, int type , float posX, float posY, float velX, float velY, float radius);

/// <summary>
/// アイテムを作る
/// </summary>
/// <param name="item">アイテム</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeItem(struct Item* item, float posX, float posY, float velX, float velY, float radius);

/// <summary>
/// 光を照らす操作を行う
/// </summary>
/// <param name="light">光</param>
/// <param name="keys">キー</param>
void LightIlluminate(struct Light* light, char* keys);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWidth + 300, kHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	/*----------------------------
	    変数を作り、初期値を入れる
	----------------------------*/

	/*   ゲーム   */

	// 画面の種類
	enum SCREEN_TYPE
	{
		SCREEN_TYPE_START,
		SCREEN_TYPE_GAME,
		SCREEN_TYPE_END
	};

	// 現在の画面
	int screenNo = SCREEN_TYPE_START;

	// スタート画面の種類
	enum START_TYPE
	{
		START_TYPE_ORPNING,
		START_TYPE_STAGE_SELECT
	};

	// 現在のスタート画面
	int startNo = START_TYPE_ORPNING;

	// メニューの種類
	enum MENU_TYPE
	{
		MENU_TYPE_START,
		MENU_TYPE_STAGE_1,
		MENU_TYPE_STAGE_2,
		MENU_TYPE_STAGE_3
	};

	// 現在のメニュー
	int menuNo = MENU_TYPE_START;

	// ステージの種類
	enum STAGE_TYPE
	{
		STAGE_TYPE_TUTORIAL,
		STAGE_TYPE_1,
		STAGE_TYPE_2,
		STAGE_TYPE_3
	};

	// 現在のステージ
	int stageNo = -1;

	// ステージフレーム
	int stageFrame = 0;


	// ゲームフレーム
	int gameFrame = 0;

	// ゲームが動いているかどうか（ゲームフラグ）
	int isGameOperation = false;

	// クリアしたかどうか（クリアフラグ）
	int isClear = false;


	/*   チュートリアル   */

	// 構造体
	struct Tutorial tutorial;

	enum STEP
	{
		STEP_1,
		STEP_2,
		STEP_3,
		STEP_4,
		STEP_5
	};

	// 手順
	tutorial.step = -1;

	// 飛行
	tutorial.flyingTimer = 0;

	// 滞空
	tutorial.hoveringTimer = 0;

	// 照射
	tutorial.illuminatingTimer = 0;

	// スイッチ起動
	tutorial.isSwitchOn = false;


	/*   プレイヤー   */

	// 構造体
	struct Player player;

	// 復活
	player.respawn.isRespawn = false;
	player.respawn.timer = 120;

	// フラグ
	player.flug.isFlying = false;
	player.flug.isRunAway = false;

	// 位置
	player.pos.world = { static_cast<int>(kWidth / 2) , 0.0f };
	player.pos.screen = CoordinateTransformation(player.pos.world);

	// 移動速度
	player.vel = { 0.0f , 1.5f };

	// 加速度
	player.acceleration = { 1.0f , 0.0f };

	// 図形の半径
	player.radius = { 0.0f , 0.0f };


	/*   光   */

	// 構造体
	struct Light light;

	// 位置
	light.pos.world = { player.pos.world.x , player.pos.world.y };
	light.pos.screen = CoordinateTransformation(player.pos.world);

	// 図形の半径
	light.radius = { 0.0f , 0.0f };


	/*   敵   */

	// 構造体
	struct Enemy enemy[kEnemyNum];

	// 敵の種類
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_STONE,
		ENEMY_TYPE_DENGER,
		ENEMY_TYPE_SOLAR_PANEL_1,
		ENEMY_TYPE_SOLAR_STONE_1,
		ENEMY_TYPE_SOLAR_PANEL_2,
		ENEMY_TYPE_SOLAR_STONE_2,
		ENEMY_TYPE_SOLAR_PANEL_TURORIAL,
	};
	
	for (int i = 0; i < kEnemyNum; i++)
	{
		// 復活
		enemy[i].respawn.isRespawn = true;
		enemy[i].respawn.timer = 120;

		// 出現しているかどうか（出現フラグ）
		enemy[i].isArrival = false;

		// 出現している時間
		enemy[i].arrivalTimer = 0;

		// 敵の種類
		enemy[i].type = -1;

		// 位置
		enemy[i].pos.world = { 0.0f , 0.0f };
		enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

		// 移動速度
		enemy[i].vel = { 0.0f , 0.0f };

		// 加速度
		enemy[i].acceleration = { 1.0f , 1.0f };

		// 図形の半径
		enemy[i].radius = { 0.0f , 0.0f };

		// 透明度
		enemy[i].transparency = 0;

		// エネルギー
		enemy[i].energy = 0;
	}


	/*   アイテム   */

	// 構造体
	struct Item item;

	// 復活
	item.respawn.isRespawn = true;
	item.respawn.timer = 120;

	// 出現しているかどうか（出現フラグ）
	item.isArrival = false;

	// 出現している時間
	item.arrivalTimer = 0;

	// 位置
	item.pos.world = { 0.0f , 0.0f };
	item.pos.screen = CoordinateTransformation(item.pos.world);

	// 移動速度
	item.vel = { 0.0f , 0.0f };

	// 加速度
	item.acceleration = { 0.0f , 0.0f };

	// 図形の半径
	item.radius = { 0.0f , 0.0f };


	/*   画像   */

	// タイトル
	int ghTitle = Novice::LoadTexture("./Resources/images/title/title.png");

	// 背景
	int ghBg = Novice::LoadTexture("./Resources/images/Bg/Bg.png");
	int ghGameOver = Novice::LoadTexture("./Resources/images/Bg/gameOver.png");
	int ghGameClear = Novice::LoadTexture("./Resources/images/Bg/gameClear.png");

	// キー
	int ghAkey = Novice::LoadTexture("./Resources/images/keys/A_Key.png");
	int ghSkey = Novice::LoadTexture("./Resources/images/keys/S_Key.png");
	int ghDkey = Novice::LoadTexture("./Resources/images/keys/D_Key.png");
	int ghSpacekey = Novice::LoadTexture("./Resources/images/keys/Space_Key.png");

	// プレイヤー
	int ghPlayerShine = Novice::LoadTexture("./Resources/images/player/player.png");
	int ghItemKey = Novice::LoadTexture("./Resources/images/player/itemKey.png");

	// 敵
	int ghEnemyStone = Novice::LoadTexture("./Resources/images/enemy/stone.png");
	int ghEnemySwitchOn = Novice::LoadTexture("./Resources/images/enemy/switchOn.png");


	/*   テキスト   */

	// ゲーム
	int ghTextGetKey = Novice::LoadTexture("./Resources/texts/game/getKey.png");
	int ghTextRunAway = Novice::LoadTexture("./Resources/texts/game/runAway.png");

	// ステージ
	int ghTextStage[3];
	ghTextStage[0] = Novice::LoadTexture("./Resources/texts/stage/stage1.png");
	ghTextStage[1] = Novice::LoadTexture("./Resources/texts/stage/stage2.png");
	ghTextStage[2] = Novice::LoadTexture("./Resources/texts/stage/stage3.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		// 画面切り替え
		switch (screenNo)
		{
		case SCREEN_TYPE_START:

			///
			/// ↓ スタート画面ここから
			/// 

			/*--------------------
			    ゲームを初期化
			--------------------*/

			/*   ゲーム   */

			// 現在のステージ
			stageNo = -1;

			// ステージフレーム
			stageFrame = 0;

			// クリアしたかどうか（クリアフラグ）
			isClear = false;


			/*   チュートリアル   */

			// 手順
			tutorial.step = -1;

			// 飛行
			tutorial.flyingTimer = 0;

			// 滞空
			tutorial.hoveringTimer = 0;

			// 照射
			tutorial.illuminatingTimer = 0;

			// スイッチ起動
			tutorial.isSwitchOn = false;


			/*   プレイヤー   */

			// 復活
			player.respawn.isRespawn = false;
			player.respawn.timer = 120;

			// フラグ
			player.flug.isFlying = false;
			player.flug.isRunAway = false;

			// 位置
			player.pos.world = { static_cast<int>(kWidth / 2) , 0.0f };
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 移動速度
			player.vel = { 0.0f , 1.5f };

			// 加速度
			player.acceleration = { 1.0f , 0.0f };

			// 図形の半径
			player.radius = { 0.0f , 0.0f };


			/*   光   */

			// 位置
			light.pos.world = { player.pos.world.x , player.pos.world.y };
			light.pos.screen = CoordinateTransformation(player.pos.world);

			// 図形の半径
			light.radius = { 0.0f , 0.0f };


			/*   敵   */

			for (int i = 0; i < kEnemyNum; i++)
			{
				// 復活
				enemy[i].respawn.isRespawn = true;
				enemy[i].respawn.timer = 120;

				// 出現しているかどうか（出現フラグ）
				enemy[i].isArrival = false;

				// 出現している時間
				enemy[i].arrivalTimer = 0;

				// 敵の種類
				enemy[i].type = -1;

				// 位置
				enemy[i].pos.world = { 0.0f , 0.0f };
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

				// 移動速度
				enemy[i].vel = { 0.0f , 0.0f };

				// 加速度
				enemy[i].acceleration = { 1.0f , 1.0f };

				// 図形の半径
				enemy[i].radius = { 0.0f , 0.0f };

				// 透明度
				enemy[i].transparency = 0;

				// エネルギー
				enemy[i].energy = 0;
			}


			/*   アイテム   */

			// 復活
			item.respawn.isRespawn = true;
			item.respawn.timer = 120;

			// 出現しているかどうか（出現フラグ）
			item.isArrival = false;

			// 出現している時間
			item.arrivalTimer = 0;

			// 位置
			item.pos.world = { 0.0f , 0.0f };
			item.pos.screen = CoordinateTransformation(item.pos.world);

			// 移動速度
			item.vel = { 0.0f , 0.0f };

			// 加速度
			item.acceleration = { 0.0f , 0.0f };

			// 図形の半径
			item.radius = { 0.0f , 0.0f };


			/*----------------------
			    スタート画面の操作
			----------------------*/

			// ゲームが動いている（ゲームフラグがtrueである）ときに、ゲームフレームを動かす
			if (isGameOperation)
			{
				gameFrame++;
			}


			// ゲームが動いている（ゲームフラグがtrueである）ときは、操作ができない

			// スタート画面切り替え
			switch (startNo)
			{
			case START_TYPE_ORPNING:

				/*   オープニング画面   */

				if (menuNo == MENU_TYPE_START)
				{
					// スペースキーを押すと、チュートリアルが開始する

					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						// ゲームが動いていない（ゲームフラグがfalseである）とき
						if (isGameOperation == false)
						{
							// ゲームが動く（ゲームフラグがtrueになる）
							isGameOperation = true;
						}
					}

					// ゲームが動いている（ゲームフラグがtrueである）とき
					if (isGameOperation)
					{
						if (gameFrame >= 30)
						{
							// ゲーム画面に移る
							screenNo = SCREEN_TYPE_GAME;

							// チュートリアルステージ
							stageNo = STAGE_TYPE_TUTORIAL;

							// ステップ1から開始する
							tutorial.step = STEP_1;

							// プレイヤーの初期状態
							PlayerInitialState(&player);


							// ステージセレクトに切り替わる
							startNo = START_TYPE_STAGE_SELECT;

							// ステージ1を選ぶ
							menuNo = MENU_TYPE_STAGE_1;


							// ゲームが止まる（ゲームフラグがfalseになる）
							isGameOperation = false;

							// ゲームフレームを初期化する
							gameFrame = 0;
						}
					}

				}


				break;

			case START_TYPE_STAGE_SELECT:

				/*   ステージセレクト   */

				// メニュー切り替え
				switch (menuNo)
				{
				case MENU_TYPE_STAGE_1:

					// スペースキーでゲームを開始する
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						// ゲームが動いていない（ゲームフラグがfalseである）とき
						if (isGameOperation == false)
						{
							// ゲームが動く（ゲームフラグがtrueになる）
							isGameOperation = true;
						}
					}

					// ゲームが動いている（ゲームフラグがtrueである）とき
					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_1;

							// プレイヤーの初期状態
							PlayerInitialState(&player);


							// ゲームが止まる（ゲームフラグがfalseになる）
							isGameOperation = false;

							// ゲームフレームを初期化する
							gameFrame = 0;
						}
					}


					// S or ↓ でステージ2を選ぶ
					if (!preKeys[DIK_S] && keys[DIK_S] || !preKeys[DIK_DOWN] && keys[DIK_DOWN])
					{
						if (isGameOperation == false)
						{
							menuNo = MENU_TYPE_STAGE_2;
						}
					}

					break;

				case MENU_TYPE_STAGE_2:

					// スペースキーでゲームを開始する
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						// ゲームが動いていない（ゲームフラグがfalseである）とき
						if (isGameOperation == false)
						{
							// ゲームが動く（ゲームフラグがtrueになる）
							isGameOperation = true;
						}
					}

					// ゲームが動いている（ゲームフラグがtrueである）とき
					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_2;

							// プレイヤーの初期状態
							PlayerInitialState(&player);


							// ゲームが止まる（ゲームフラグがfalseになる）
							isGameOperation = false;

							// ゲームフレームを初期化する
							gameFrame = 0;
						}
					}


					// W or ↑ でステージ1を選ぶ
					if (!preKeys[DIK_W] && keys[DIK_W] || !preKeys[DIK_UP] && keys[DIK_UP])
					{
						if (isGameOperation == false)
						{
							menuNo = MENU_TYPE_STAGE_1;
						}
					}

					// S or ↓ でステージ3を選ぶ
					if (!preKeys[DIK_S] && keys[DIK_S] || !preKeys[DIK_DOWN] && keys[DIK_DOWN])
					{
						if (isGameOperation == false)
						{
							menuNo = MENU_TYPE_STAGE_3;
						}
					}

					break;

				case MENU_TYPE_STAGE_3:

					// スペースキーでゲームを開始する
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						// ゲームが動いていない（ゲームフラグがfalseである）とき
						if (isGameOperation == false)
						{
							// ゲームが動く（ゲームフラグがtrueになる）
							isGameOperation = true;
						}
					}

					// ゲームが動いている（ゲームフラグがtrueである）とき
					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_3;

							// プレイヤーの初期状態
							PlayerInitialState(&player);


							// ゲームが止まる（ゲームフラグがfalseになる）
							isGameOperation = false;

							// ゲームフレームを初期化する
							gameFrame = 0;
						}
					}


					// W or ↑ でステージ2を選ぶ
					if (!preKeys[DIK_W] && keys[DIK_W] || !preKeys[DIK_UP] && keys[DIK_UP])
					{
						if (isGameOperation == false)
						{
							menuNo = MENU_TYPE_STAGE_2;
						}
					}

					break;
				}


				break;
			}

			///
			/// ↑ スタート画面ここまで
			/// 

			break;

		case SCREEN_TYPE_GAME:

			///
			/// ↓ ゲーム画面ここから
			/// 

			/*-----------------------
			    ステージを設定する
			-----------------------*/

			switch (stageNo)
			{
			case STAGE_TYPE_TUTORIAL:

				// ステップ
				switch (tutorial.step)
				{
				case STEP_1:

					// 飛行
					if (player.flug.isFlying)
					{
						tutorial.flyingTimer++;

						if (tutorial.flyingTimer >= 50)
						{
							// 次のステップに移る
							tutorial.step = STEP_2;
						}
					}

					break;

				case STEP_2:

					// 滞空
					if (keys[DIK_S])
					{
						if (player.flug.isFlying)
						{
							tutorial.hoveringTimer++;

							if (tutorial.hoveringTimer >= 50)
							{
								// 次のステップに移る
								tutorial.step = STEP_3;

								// 敵を出現させる
								MakeEnemy(enemy, ENEMY_TYPE_STONE, static_cast<float>(kWidth / 2), 500.0f, 0.0f, 0.0f, 40.0f);
							}
						}
					}

					break;

				case STEP_3:

					// 照射
					if (tutorial.illuminatingTimer >= 50)
					{
						// 次のステップに移る
						tutorial.step = STEP_4;

						for (int i = 0; i < kEnemyNum; i++)
						{
							enemy[i].isArrival = false;
						}

						// 敵を出現させる
						MakeEnemy(enemy, ENEMY_TYPE_SOLAR_PANEL_TURORIAL, static_cast<float>(kWidth / 2), 500.0f, 0.0f, 0.0f, 40.0f);
					}

					break;

				case STEP_4:

					// スイッチを起動
					if (tutorial.isSwitchOn)
					{
						// 次のステップに移る
						tutorial.step = STEP_5;

						// 鍵が出現する
						MakeItem(&item, static_cast<float>(kWidth / 2), 500.0f, 0.0f, 0.0f, 20.0f);
					}

					break;
				}

				break;

			case STAGE_TYPE_1:

				if (stageFrame == 0)
				{
					for (int i = 0; i < 26; i++)
					{
						MakeEnemy(enemy,ENEMY_TYPE_STONE , 25.0f, 25.0f + i * 25.0f, 0.0f, 0.0f, 10.0f);

						MakeEnemy(enemy, ENEMY_TYPE_STONE, static_cast<float>(kWidth) - 25.0f, 25.0f + i * 25.0f, 0.0f, 0.0f, 10.0f);
					}

					for (int i = 0; i < 13; i++)
					{
						MakeEnemy(enemy, ENEMY_TYPE_STONE, static_cast<float>(kWidth) - 50.0f - 25.0f * i, 250.0f, 0.0f, 0.0f, 10.0f);

						MakeEnemy(enemy, ENEMY_TYPE_STONE, 50.0f + i * 25.0f, 450.0f, 0.0f, 0.0f, 10.0f);
					}

					for (int i = 0; i < 4; i++)
					{
						MakeEnemy(enemy, ENEMY_TYPE_STONE, 50.0f + i * 25.0f, 250.0f, 0.0f, 0.0f, 10.0f);

						MakeEnemy(enemy, ENEMY_TYPE_STONE, static_cast<int>(kWidth) - 50.0f - i * 25.0f, 450.0f, 0.0f, 0.0f, 10.0f);
					}

					MakeItem(&item , 350.0f , 600.0f , 0.0f , 0.0f , 20.0f);
				}

				break;

			case STAGE_TYPE_2:

				if (stageFrame == 0)
				{
					for (int i = 0; i < 8; i++)
					{
						if (i == 2)
						{
							MakeEnemy(enemy, ENEMY_TYPE_SOLAR_PANEL_1, static_cast<float>(kWidth / 2) + 100.0f, 250.0f + i * 25.0f, 0.0f, 0.0f, 10.0f);
						} 
						else
						{
							MakeEnemy(enemy, ENEMY_TYPE_STONE, static_cast<float>(kWidth / 2) + 100.0f, 250.0f + i * 25.0f, 0.0f, 0.0f, 10.0f);
						}
					}

					for (int i = 0; i < 8; i++)
					{
						MakeEnemy(enemy, ENEMY_TYPE_STONE, 425.0f - i * 25.0f, 250.0f, 0.0f, 0.0f, 10.0f);

						MakeEnemy(enemy, ENEMY_TYPE_STONE, 25.0f + i * 25.0f, 350.0f, 0.0f, 0.0f, 10.0f);

						MakeEnemy(enemy, ENEMY_TYPE_STONE, 425.0f - i * 25.0f, 450.0f, 0.0f, 0.0f, 10.0f);
					}


					for (int i = 0; i < 8; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							MakeEnemy(enemy, ENEMY_TYPE_SOLAR_STONE_2, 475.0f + 25 * j, 250.0f + i * 25.0f, 0.0f, 0.0f, 10.0f);
						}
					}

					MakeEnemy(enemy, ENEMY_TYPE_SOLAR_PANEL_2, 475.0f, 600.0f, 0.0f, 0.0f, 20.0f);

					MakeItem(&item, 350.0f, 600.0f, 0.0f, 0.0f, 10.0f);
				}

				break;

			case STAGE_TYPE_3:

				if (stageFrame == 0)
				{
					MakeItem(&item, 350.0f, 600.0f, 0.0f, 0.0f, 10.0f);
				}

				break;
			}

			// ステージフレームを100フレームまで進める
			if (stageFrame < 100)
			{
				stageFrame++;
			}

			// 逃げている（逃げるフラグがtrueである）ときに、ステージフレームを動かす
			if (player.flug.isRunAway)
			{
				stageFrame++;
			}


			/*--------------------
			    ゲームの終了条件
			--------------------*/

			// プレイヤーが復活処理を進めたら、ゲームが終了する
			if (player.respawn.timer <= 5)
			{
				if (stageNo != STAGE_TYPE_TUTORIAL)
				{
					screenNo = SCREEN_TYPE_END;
				}
			}

			// プレイヤーが逃げている（逃げるフラグがtrueである）ときに、陸地に着くと、ゲームが終了（クリア）する
			if (player.flug.isRunAway)
			{
				if (player.pos.world.y - player.radius.y <= 0.0f)
				{
					if (stageNo != STAGE_TYPE_TUTORIAL)
					{
						screenNo = SCREEN_TYPE_END;

						// ゲームクリアになる（クリアフラグがtrueになる）
						isClear = true;

						// ゲームフレームを初期化する
						gameFrame = 0;
					}
					else
					{
						screenNo = SCREEN_TYPE_START;
					}
				}
			}


			/*--------------
			    復活処理
			--------------*/

			// プレイヤー
			RespawnProcess(&player.respawn);

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				RespawnProcess(&enemy[i].respawn);
			}

			// アイテム
			RespawnProcess(&item.respawn);


			/*--------------------
			    プレイヤーの操作
			--------------------*/

			// プレイヤーの操作
			PlayerMove(&player , keys);

			// 光を照らす操作
			LightIlluminate(&light, keys);

			// 光をプレイヤーの位置に置く
			light.pos.world = { player.pos.world.x , player.pos.world.y };


			/*-------------
			    敵の動き
			-------------*/

			// 出現している（出現フラグがtrueである）敵を動かす
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].isArrival)
				{
					switch (enemy[i].type)
					{
					case ENEMY_TYPE_STONE:

						break;

					case ENEMY_TYPE_DENGER:

						enemy[i].radius.x = static_cast<float>(kWidth / 2);
						enemy[i].radius.y += 0.5f;

						break;
					}

					// 敵を動かす
					enemy[i].pos.world.x += enemy[i].vel.x * enemy[i].acceleration.x;
					enemy[i].pos.world.y += enemy[i].vel.y * enemy[i].acceleration.y;
				}
			}


			/*---------------
			    当たり判定
			---------------*/

			// プレイヤー と 敵
			if (player.respawn.isRespawn)
			{
				for (int i = 0; i < kEnemyNum; i++)
				{
					if (enemy[i].isArrival)
					{
						// 危険な何か以外の敵
						if (enemy[i].type != ENEMY_TYPE_DENGER)
						{
							if (powf(player.radius.x + enemy[i].radius.x, 2) >=
								powf(player.pos.world.x - enemy[i].pos.world.x, 2) + powf(player.pos.world.y - enemy[i].pos.world.y, 2))
							{
								// プレイヤーがやられる（復活フラグがfalseになる）
								player.respawn.isRespawn = false;
							}
						}
						else
						{
							// 危険な何かの敵

							if (player.pos.world.x - player.radius.x < enemy[i].pos.world.x + enemy[i].radius.x &&
								player.pos.world.x + player.radius.x > enemy[i].pos.world.x - enemy[i].radius.x)
							{
								if (player.pos.world.y - player.radius.y < enemy[i].pos.world.y + enemy[i].radius.y &&
									player.pos.world.y + player.radius.y > enemy[i].pos.world.y - enemy[i].radius.y)
								{
									// プレイヤーがやられる（復活フラグがfalseになる）
									player.respawn.isRespawn = false;
								}
							}
						}
					}
				}
			}

			// 光 と 敵
			if (player.respawn.isRespawn)
			{
				for (int i = 0; i < kEnemyNum ; i++)
				{
					if (enemy[i].isArrival)
					{
						if (powf(light.radius.x + enemy[i].radius.x, 2) >=
							powf(light.pos.world.x - enemy[i].pos.world.x, 2) + powf(light.pos.world.y - enemy[i].pos.world.y, 2))
						{
							// 敵がみえるようになる
							if (enemy[i].transparency < 255)
							{
								enemy[i].transparency += 3;
							}
							
							if(enemy[i].transparency >= 255)
							{
								enemy[i].transparency = 255;
							}

							// チュートリアル（照射）
							if (stageNo == STAGE_TYPE_TUTORIAL)
							{
								if (tutorial.step == STEP_3)
								{
									tutorial.illuminatingTimer++;
								}
							}

						}
						else
						{
							// 敵がみえなくなっていく
							if (enemy[i].transparency > 0)
							{
								enemy[i].transparency -= 3;
							}
							
							if(enemy[i].transparency < 0)
							{
								enemy[i].transparency = 0;
							}
						}
					}
				}
			}

			// 光とソーラーパネル
			if (player.respawn.isRespawn)
			{
				for (int i = 0; i < kEnemyNum; i++)
				{
					if (enemy[i].isArrival)
					{
						// 光が届くと、エネルギーがたまる
						if (powf(light.radius.x + enemy[i].radius.x, 2) >=
							powf(enemy[i].pos.world.x - light.pos.world.x, 2) + powf(enemy[i].pos.world.y - light.pos.world.y, 2))
						{
							// ソーラーパネルの番号で一致させる
							switch (enemy[i].type)
							{
							case ENEMY_TYPE_SOLAR_PANEL_1:

								// エネルギーがたまる
								enemy[i].energy++;

								// エネルギーが最大になったら、スイッチと連動している石が消える（復活、出現フラグがfalseになる）
								if (enemy[i].energy >= 150)
								{
									enemy[i].respawn.isRespawn = false;
									enemy[i].isArrival = false;

									for (int j = 0; j < kEnemyNum; j++)
									{
										if (enemy[j].isArrival)
										{
											if (enemy[j].type == ENEMY_TYPE_SOLAR_STONE_1)
											{
												enemy[j].respawn.isRespawn = false;
												enemy[j].isArrival = false;
											}
										}
									}
								}

								break;

							case ENEMY_TYPE_SOLAR_PANEL_2:

								// エネルギーがたまる
								enemy[i].energy++;

								// エネルギーが最大になったら、スイッチと連動している石が消える（復活、出現フラグがfalseになる）
								if (enemy[i].energy >= 150)
								{
									enemy[i].respawn.isRespawn = false;
									enemy[i].isArrival = false;

									for (int j = 0; j < kEnemyNum; j++)
									{
										if (enemy[j].isArrival)
										{
											if (enemy[j].type == ENEMY_TYPE_SOLAR_STONE_2)
											{
												enemy[j].respawn.isRespawn = false;
												enemy[j].isArrival = false;
											}
										}
									}
								}

								break;

							case ENEMY_TYPE_SOLAR_PANEL_TURORIAL:

								// エネルギーがたまる
								enemy[i].energy++;

								// エネルギーが最大になったら、スイッチが起動し、敵が消える（復活、出現フラグがfalseになる）
								if (enemy[i].energy >= 150)
								{
									enemy[i].respawn.isRespawn = false;
									enemy[i].isArrival = false;

									tutorial.isSwitchOn = true;
								}


								break;
							}
						}
						else
						{
							// 光が届いてない場合は、エネルギーがゼロになる
							enemy[i].energy = 0;
						}
					}
				}
			}

			// プレイヤー と アイテム
			if (player.respawn.isRespawn)
			{
				if (item.isArrival)
				{
					if (powf(player.radius.x + item.radius.x, 2) >=
						powf(player.pos.world.x - item.pos.world.x, 2) + powf(player.pos.world.y - item.pos.world.y, 2))
					{
						// アイテムが消える（復活、出現フラグがfalseになる）
						item.respawn.isRespawn = false;
						item.isArrival = false;

						// 危険な何かが出現する
						MakeEnemy(enemy, ENEMY_TYPE_DENGER, static_cast<float>(kWidth / 2), static_cast<float>(kHeight), 0.0f, 0.0f, 0.0f);

						// プレイヤーが逃げ始める（逃げるフラグがtrueになる）
						player.flug.isRunAway = true;

						// ステージフレームを100まで進める
						stageFrame = 100;
					}
				}
			}


			/*-------------
			    座標変換
			-------------*/

			// プレイヤー
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 光
			light.pos.screen = CoordinateTransformation(light.pos.world);

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);
			}

			// アイテム
			item.pos.screen = CoordinateTransformation(item.pos.world);


			///
			/// ↑ ゲーム画面ここまで
			/// 

			break;

		case SCREEN_TYPE_END:

			///
			/// ↓ ゲーム終了画面ここから
			/// 

			/*-----------------------
			    ゲームを初期化する
			-----------------------*/

			/*   ゲーム   */

			// ステージフレーム
			stageFrame = 0;


			/*   チュートリアル   */

			// 手順
			tutorial.step = -1;

			// 飛行
			tutorial.flyingTimer = 0;

			// 滞空
			tutorial.hoveringTimer = 0;

			// 照射
			tutorial.illuminatingTimer = 0;

			// スイッチ起動
			tutorial.isSwitchOn = false;


			/*   プレイヤー   */

			// 復活
			player.respawn.isRespawn = false;
			player.respawn.timer = 120;

			// フラグ
			player.flug.isFlying = false;
			player.flug.isRunAway = false;

			// 位置
			player.pos.world = { static_cast<int>(kWidth / 2) , 0.0f };
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 移動速度
			player.vel = { 0.0f , 1.5f };

			// 加速度
			player.acceleration = { 1.0f , 0.0f };

			// 図形の半径
			player.radius = { 0.0f , 0.0f };


			/*   光   */

			// 位置
			light.pos.world = { player.pos.world.x , player.pos.world.y };
			light.pos.screen = CoordinateTransformation(player.pos.world);

			// 図形の半径
			light.radius = { 0.0f , 0.0f };


			/*   敵   */

			for (int i = 0; i < kEnemyNum; i++)
			{
				// 復活
				enemy[i].respawn.isRespawn = true;
				enemy[i].respawn.timer = 120;

				// 出現しているかどうか（出現フラグ）
				enemy[i].isArrival = false;

				// 出現している時間
				enemy[i].arrivalTimer = 0;

				// 敵の種類
				enemy[i].type = -1;

				// 位置
				enemy[i].pos.world = { 0.0f , 0.0f };
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

				// 移動速度
				enemy[i].vel = { 0.0f , 0.0f };

				// 加速度
				enemy[i].acceleration = { 1.0f , 1.0f };

				// 図形の半径
				enemy[i].radius = { 0.0f , 0.0f };

				// 透明度
				enemy[i].transparency = 0;

				// エネルギー
				enemy[i].energy = 0;
			}


			/*   アイテム   */

			// 復活
			item.respawn.isRespawn = true;
			item.respawn.timer = 120;

			// 出現しているかどうか（出現フラグ）
			item.isArrival = false;

			// 出現している時間
			item.arrivalTimer = 0;

			// 位置
			item.pos.world = { 0.0f , 0.0f };
			item.pos.screen = CoordinateTransformation(item.pos.world);

			// 移動速度
			item.vel = { 0.0f , 0.0f };

			// 加速度
			item.acceleration = { 0.0f , 0.0f };

			// 図形の半径
			item.radius = { 0.0f , 0.0f };



			/*-------------------------
			     ゲーム終了画面の操作
			-------------------------*/

			// ゲームが動いている（ゲームフラグがtrueである）ときに、ゲームフレームを動かす
			if (isGameOperation)
			{
				gameFrame++;
			}


			// ゲームが動いている（ゲームフラグがtrueである）ときは、操作ができない

			switch (stageNo)
			{
			case STAGE_TYPE_1:

				// クリアしたかどうか
				if (isClear)
				{
					// スペースキーを押すと、次のステージに進む
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						if (isGameOperation == false)
						{
							// ゲームを動かす（ゲームフラグをtrueにする）
							isGameOperation = true;
						}
					}

					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// ステージ2に進む
							stageNo = STAGE_TYPE_2;

							// ゲーム画面に戻る
							screenNo = SCREEN_TYPE_GAME;

							// プレイヤーの初期状態
							PlayerInitialState(&player);

							
							// ゲームフレームを初期化する
							gameFrame = 0;

							// ゲームが止まる（ゲームフラグをfalseに変える）
							isGameOperation = false;

							// クリアを取り消す（クリアフラグをfalseに変える）
							isClear = false;
						}
					}
				}
				else
				{
					// スペースキーを押すと、スタート画面に戻る
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						if (isGameOperation == false)
						{
							// ゲームを動かす（ゲームフラグをtrueにする）
							isGameOperation = true;
						}
					}

					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// スタート画面に戻る
							screenNo = SCREEN_TYPE_START;


							// ゲームフレームを初期化する
							gameFrame = 0;

							// ゲームが止まる（ゲームフラグをfalseに変える）
							isGameOperation = false;
						}
					}
				}

				break;

			case STAGE_TYPE_2:

				// クリアしたかどうか
				if (isClear)
				{
					// スペースキーを押すと、次のステージに進む
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						if (isGameOperation == false)
						{
							// ゲームを動かす（ゲームフラグをtrueにする）
							isGameOperation = true;
						}
					}

					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// ステージ3に進む
							stageNo = STAGE_TYPE_3;

							// ゲーム画面に戻る
							screenNo = SCREEN_TYPE_GAME;

							// プレイヤーの初期状態
							PlayerInitialState(&player);


							// ゲームフレームを初期化する
							gameFrame = 0;

							// ゲームが止まる（ゲームフラグをfalseに変える）
							isGameOperation = false;

							// クリアを取り消す（クリアフラグをfalseに変える）
							isClear = false;
						}
					}
				}
				else
				{
					// スペースキーを押すと、スタート画面に戻る
					if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
					{
						if (isGameOperation == false)
						{
							// ゲームを動かす（ゲームフラグをtrueにする）
							isGameOperation = true;
						}
					}

					if (isGameOperation)
					{
						if (gameFrame >= 70)
						{
							// スタート画面に戻る
							screenNo = SCREEN_TYPE_START;


							// ゲームフレームを初期化する
							gameFrame = 0;

							// ゲームが止まる（ゲームフラグをfalseに変える）
							isGameOperation = false;
						}
					}
				}

				break;

			case STAGE_TYPE_3:

				// スペースキーを押すと、スタート画面に戻る
				if (!preKeys[DIK_SPACE] && keys[DIK_SPACE])
				{
					if (isGameOperation == false)
					{
						// ゲームを動かす（ゲームフラグをtrueにする）
						isGameOperation = true;
					}
				}

				if (isGameOperation)
				{
					if (gameFrame >= 70)
					{
						// スタート画面に戻る
						screenNo = SCREEN_TYPE_START;


						// ゲームフレームを初期化する
						gameFrame = 0;

						// ゲームが止まる（ゲームフラグをfalseに変える）
						isGameOperation = false;
					}
				}

				break;
			}

			///
			/// ↑ ゲーム終了画面ここまで
			/// 

			break;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		switch (screenNo)
		{
		case SCREEN_TYPE_START:

			///
			/// ↓ スタート画面ここから
			/// 

			switch (startNo)
			{
			case START_TYPE_ORPNING:

				Novice::DrawSprite(0, 0, ghTitle, 1, 1, 0.0f, 0xFFFFFFFF);

				break;

			case START_TYPE_STAGE_SELECT:

				switch (menuNo)
				{
				case MENU_TYPE_STAGE_1:

					Novice::DrawBox(64, 64, 320, 64, 0.0f, 0xFFFFFF33, kFillModeSolid);

					break;

				case MENU_TYPE_STAGE_2:

					Novice::DrawBox(64, 64 + 150, 320, 64, 0.0f, 0xFFFFFF33, kFillModeSolid);

					break;

				case MENU_TYPE_STAGE_3:

					Novice::DrawBox(64, 64 + 150 * 2, 320, 64, 0.0f, 0xFFFFFF33, kFillModeSolid);

					break;
				}

				for (int i = 0; i < 3; i++)
				{
					Novice::DrawSprite(64 , 64 + i * 150 , ghTextStage[i] , 1,1,0.0f , 0xFFFFFFFF);
				}

				break;
			}

			///
			/// ↑ スタート画面ここまで
			/// 

			break;

		case SCREEN_TYPE_GAME:

			///
			/// ↓ ゲーム画面ここから
			/// 

			/*------------------------
			    図形や画像を描画する
			------------------------*/


			/*   背景   */

			Novice::DrawBox(0, 0, 1000, 800, 0.0f, 0x000000FF, kFillModeSolid);
			
			// 逃げているときとで、変化させる
			if (player.flug.isRunAway == false)
			{
				Novice::DrawSprite(0, 0, ghBg, 1, 1, 0.0f, 0xFFFFFFFF);
			}
			else
			{
				Novice::DrawSprite(0, 0, ghBg, 1, 1, 0.0f, 0xFF0000FF);
			}

			// キー
			if (player.flug.isFlying)
			{
				Novice::DrawSprite(740, 100, ghAkey, 1.5f, 1.5f, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(920, 100, ghDkey, 1.5f, 1.5f, 0.0f, 0xFFFFFFFF);
				Novice::DrawSprite(830, 100, ghSkey, 1.5f, 1.5f, 0.0f, 0xFFFFFFFF);
			}

			Novice::DrawSprite(800, 300, ghSpacekey, 1.5f, 1.5f, 0.0f, 0xFFFFFFFF);


			/*   光   */

			// 復活している（復活フラグがtrueである）とき
			if (player.respawn.isRespawn)
			{
				Novice::DrawEllipse
				(
					static_cast<int>(light.pos.screen.x), static_cast<int>(light.pos.screen.y),
					static_cast<int>(light.radius.x), static_cast<int>(light.radius.y),
					0.0f, 0xFFFF8833, kFillModeSolid
				);
			}


			/*   敵   */

			// 出現している（出現フラグがtrueである）とき
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].isArrival)
				{
					switch (enemy[i].type)
					{
					case ENEMY_TYPE_STONE:

						Novice::DrawQuad
						(
							static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x) , static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
							static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
							static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
							static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
							0,0,32,32,ghEnemyStone , 0xFFFFFF00 + enemy[i].transparency
						);

						break;

					case ENEMY_TYPE_DENGER:

						Novice::DrawBox
						(
							static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x) , static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
							static_cast<int>(enemy[i].radius.x) * 2 , static_cast<int>(enemy[i].radius.y) * 2,
							0.0f , 0xFF0000FF , kFillModeSolid
						);

						break;

					case ENEMY_TYPE_SOLAR_PANEL_1:

						Novice::DrawEllipse
						(
							static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
							static_cast<int>(enemy[i].radius.x), static_cast<int>(enemy[i].radius.y),
							0.0f, 0xFF000000 + enemy[i].transparency, kFillModeWireFrame
						);

						break;

					case ENEMY_TYPE_SOLAR_STONE_1:

						Novice::DrawEllipse
						(
							static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
							static_cast<int>(enemy[i].radius.x), static_cast<int>(enemy[i].radius.y),
							0.0f, 0xFF000000 + enemy[i].transparency, kFillModeSolid
						);

						break;

					case ENEMY_TYPE_SOLAR_PANEL_2:

						Novice::DrawEllipse
						(
							static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
							static_cast<int>(enemy[i].radius.x), static_cast<int>(enemy[i].radius.y),
							0.0f, 0x0000FF00 + enemy[i].transparency, kFillModeWireFrame
						);

						break;

					case ENEMY_TYPE_SOLAR_STONE_2:

						Novice::DrawEllipse
						(
							static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
							static_cast<int>(enemy[i].radius.x), static_cast<int>(enemy[i].radius.y),
							0.0f, 0x0000FF00 + enemy[i].transparency, kFillModeSolid
						);

						break;

					case ENEMY_TYPE_SOLAR_PANEL_TURORIAL:

						Novice::DrawEllipse
						(
							static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
							static_cast<int>(enemy[i].radius.x), static_cast<int>(enemy[i].radius.y),
							0.0f, 0xFFFFFF00 + enemy[i].transparency, kFillModeWireFrame
						);

						break;
					}
				}

				// 復活していない（復活フラグがfalseである）とき
				if (enemy[i].respawn.isRespawn == false)
				{
					if (enemy[i].respawn.timer <= 119 && enemy[i].respawn.timer > 119 - 25)
					{
						switch (enemy[i].type)
						{
						case ENEMY_TYPE_SOLAR_STONE_1:

							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
								32 * ((119 - enemy[i].respawn.timer) / 5), 0, 32, 32, ghEnemySwitchOn, 0xFFFFFFFF
							);

							break;

						case ENEMY_TYPE_SOLAR_STONE_2:

							Novice::DrawQuad
							(
								static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
								static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
								32 * ((119 - enemy[i].respawn.timer) / 5), 0, 32, 32, ghEnemySwitchOn, 0xFFFFFFFF
							);

							break;
						}
					}
				}
			}


			/*   アイテム   */

			// 出現している（出現フラグがtrueである）とき
			if (item.isArrival)
			{
				Novice::DrawQuad
				(
					static_cast<int>(item.pos.screen.x - item.radius.x), static_cast<int>(item.pos.screen.y - item.radius.y),
					static_cast<int>(item.pos.screen.x - item.radius.x), static_cast<int>(item.pos.screen.y + item.radius.y),
					static_cast<int>(item.pos.screen.x + item.radius.x), static_cast<int>(item.pos.screen.y - item.radius.y),
					static_cast<int>(item.pos.screen.x + item.radius.x), static_cast<int>(item.pos.screen.y + item.radius.y),
					0, 0, 20, 20, ghItemKey, 0xFFFFFFFF
				);
			}


			/*   プレイヤー   */

			// 復活している（復活フラグがtrueである）とき
			if (player.respawn.isRespawn)
			{
				Novice::DrawQuad
				(
					static_cast<int>(player.pos.screen.x - player.radius.x) , static_cast<int>(player.pos.screen.y - player.radius.y) ,
					static_cast<int>(player.pos.screen.x + player.radius.x), static_cast<int>(player.pos.screen.y - player.radius.y),
					static_cast<int>(player.pos.screen.x - player.radius.x), static_cast<int>(player.pos.screen.y + player.radius.y),
					static_cast<int>(player.pos.screen.x + player.radius.x), static_cast<int>(player.pos.screen.y + player.radius.y),
					0,0,40,40,ghPlayerShine,0xFFFFFFFF
				);
			}


			/*   地面   */

			Novice::DrawBox(0 , kHeight - 100 , kWidth , 100 , 0.0f , 0x654321FF , kFillModeSolid);


			/*   テキスト   */

			if (stageNo != STAGE_TYPE_TUTORIAL)
			{
				if (stageFrame < 80)
				{
					Novice::DrawSprite(kWidth / 2 - 256, kHeight / 2 - 64, ghTextGetKey, 2, 2, 0.0f, 0xFFFFFFFF);
				}
			}

			// プレイヤーが逃げて（逃げフラグになって）100フレームに表示する
			if (player.flug.isRunAway)
			{
				if (stageFrame < 200)
				{
					Novice::DrawSprite(kWidth / 2 - 256, kHeight / 2 - 64, ghTextRunAway, 2, 2, 0.0f, 0xFFFFFFFF);
				}
			}


			///
			/// ↑ ゲーム画面ここまで
			/// 

			break;

		case SCREEN_TYPE_END:

			///
			/// ↓ ゲーム終了画面ここから
			/// 

			if (isClear)
			{
				Novice::DrawSprite(0, 0, ghGameClear, 2, 2, 0.0f, 0xFFFFFFFF);
			}
			else
			{
				Novice::DrawSprite(0, 0, ghGameOver, 2, 2, 0.0f, 0xFFFFFFFF);
			}

			///
			/// ↑ ゲーム終了画面ここまで
			/// 

			break;
		}


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>座標変換を行った構造体を返却する</returns>
struct Coordinate2 CoordinateTransformation(struct Coordinate2 world)
{
	// 構造体
	struct Coordinate2 screen;

	screen.x = world.x;
	screen.y = -world.y + static_cast<float>(kHeight) - 100.0f;

	return screen;
}

/// <summary>
/// 復活処理を行う
/// </summary>
/// <param name="respawn">復活</param>
void RespawnProcess(struct Respawn* respawn)
{
	// nullを探す
	if (respawn == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときに、復活処理を行う
	if(respawn->isRespawn == false)
	{
		respawn->timer--;

		if (respawn->timer <= 0)
		{
			// 復活する（復活フラグがtrueになる）
			respawn->isRespawn = true;

			// 復活処理が初期化される
			respawn->timer = 120;
		}
	}
}

/// <summary>
/// プレイヤーの初期位置を決める
/// </summary>
/// <param name="player">プレイヤー</param>
void PlayerInitialState(struct Player* player)
{
	// nullを探す
	if (player == nullptr)
	{
		return;
	}


	// 復活させる（復活フラグをtrueにする）
	player->respawn.isRespawn = true;

	// 位置
	player->pos.world = { static_cast<float>(kWidth / 2) , 0.0f };

	// 図形の半径
	player->radius = { 5.0f , 5.0f };
}

/// <summary>
/// プレイヤーの操作を行う
/// </summary>
/// <param name="player">プレイヤー</param>
/// <param name="keys">キー</param>
void PlayerMove(struct Player* player, char* keys)
{
	// nullを探す
	if (player == nullptr || keys == nullptr)
	{
		return;
	}


	/*   飛行   */

	// スペースキーを押すと、飛行できる（飛行フラグがtrueになる）
	if (keys[DIK_SPACE])
	{
		player->flug.isFlying = true;
	}
	else
	{
		// スペースキーを押さないと、飛行しない（飛行フラグがfalseになる）
		player->flug.isFlying = false;
	}

	// 飛行する（飛行フラグがtrueである）と、上昇できる
	if (player->flug.isFlying)
	{
		if (player->acceleration.y < 1.0f)
		{
			player->acceleration.y += 0.1f;
		}


		// 風船が膨らむ
		if (player->radius.x < 20.0f)
		{
			player->radius.x += 0.5f;
		}

		if (player->radius.y < 20.0f)
		{
			player->radius.y += 0.5f;
		}
	}
	else
	{
		// 飛行しない（飛行フラグがfalseである）と、降下できる
		if (player->acceleration.y > -1.0f)
		{
			player->acceleration.y += -0.1f;
		}


		// 風船が縮む
		if (player->radius.x > 5.0f)
		{
			player->radius.x += -1.0f;
		}
		else
		{
			player->radius.x = 5.0f;
		}

		if (player->radius.y > 5.0f)
		{
			player->radius.y += -1.0f;
		}
		else
		{
			player->radius.y = 5.0f;
		}
	}


	/*   横移動   */

	// 移動速度を初期化する
	player->vel.x = 0.0f;

	// 飛行中（飛行フラグがtrueであるとき）に、Aキーを押すと、左に移動する
	if (keys[DIK_A])
	{
		if (player->flug.isFlying)
		{
			if (player->pos.world.x - player->radius.x > 0.0f)
			{
				player->vel.x = -2.0f;
			}
		}
	}

	// 飛行中（飛行フラグがtrueであるとき）に、Dキーを押すと、右に移動する
	if (keys[DIK_D])
	{
		if (player->flug.isFlying)
		{
			if (player->pos.world.x + player->radius.x < static_cast<float>(kHeight))
			{
				player->vel.x = 2.0f;
			}
		}
	}

	// 飛行中（飛行フラグがtrueであるとき）に、Sキーを押すと、上昇、移動しない
	if (keys[DIK_S])
	{
		if (player->flug.isFlying)
		{
			player->acceleration.y = 0.0f;

			player->vel.x = 0.0f;
		}
	}

	// プレイヤーを動かす
	player->pos.world.x += player->vel.x * player->acceleration.x;
	player->pos.world.y += player->vel.y * player->acceleration.y;


	// プレイヤーが画面外に出ないようにする
	if (player->pos.world.x - player->radius.x < 0.0f)
	{
		player->pos.world.x = player->radius.x;
	}

	if (player->pos.world.x + player->radius.x > static_cast<float>(kWidth))
	{
		player->pos.world.x = static_cast<float>(kWidth) - player->radius.x;
	}

	if (player->pos.world.y - player->radius.y < 0.0f)
	{
		player->pos.world.y = player->radius.y;
	}

	if (player->pos.world.y + player->radius.y > static_cast<float>(kHeight) - 100.0f)
	{
		player->pos.world.y = static_cast<float>(kHeight) - 100.0f - player->radius.y;
	}
}

/// <summary>
/// 敵を作る
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="type">種類</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeEnemy(struct Enemy* enemy, int type, float posX, float posY, float velX, float velY, float radius)
{
	// nullを探す
	if (enemy == nullptr)
	{
		return;
	}

	for (int i = 0; i < kEnemyNum; i++)
	{
		if (enemy[i].isArrival == false)
		{
			// 敵を出現させる（出現フラグがtrueになる）
			enemy[i].isArrival = true;

			// 敵の種類
			enemy[i].type = type;

			// 位置
			enemy[i].pos.world = { posX , posY };

			// 移動速度
			enemy[i].vel = { velX , velY };

			// 加速度
			enemy[i].acceleration = { 1.0f , 1.0f };

			// 図形の半径
			enemy[i].radius = { radius , radius };

			break;
		}
	}
}

/// <summary>
/// アイテムを作る
/// </summary>
/// <param name="item">アイテム</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeItem(struct Item* item, float posX, float posY, float velX, float velY, float radius)
{
	// nullを探す
	if (item == nullptr)
	{
		return;
	}

	if (item->isArrival == false)
	{
		// アイテムが出現する（出現フラグがtrueになる）
		item->isArrival = true;

		// 位置
		item->pos.world = { posX , posY };

		// 移動速度
		item->vel = { velX , velY };

		// 加速度
		item->acceleration = { 1.0f , 1.0f };

		// 図形の半径
		item->radius = { radius , radius };
	}
}

/// <summary>
/// 光を照らす操作を行う
/// </summary>
/// <param name="light">光</param>
/// <param name="keys">キー</param>
void LightIlluminate(struct Light* light, char* keys)
{
	// nullを探す
	if (light == nullptr || keys == nullptr)
	{
		return;
	}

	// スペースキーで、光を広げる
	if (keys[DIK_SPACE])
	{
		if (light->radius.x < 100.0f || light->radius.y < 100.0f)
		{
			light->radius.x += 1.5f;
			light->radius.y += 1.5f;
		}
	}
	else
	{
		// スペースキーを押さないと、光が縮む
		
		if (light->radius.x > 0.0f || light->radius.y > 0.0f)
		{
			light->radius.x -= 4.0f;
			light->radius.y -= 4.0f;
		}
		else
		{
			light->radius.x = 0.0f;
			light->radius.y = 0.0f;
		}
	}
}