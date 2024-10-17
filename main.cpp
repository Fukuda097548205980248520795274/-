#include <Novice.h>

const char kWindowTitle[] = "LC1C_20_フクダソウワ_ヒカリウム";


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
const int KHeight = 800;

// 横幅
const int kWidth = 700;


/*---------------
    関数を作る
---------------*/

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>座標変換を行った構造体を返却する</returns>
struct Coordinate2 CoordinateTransformation(struct Coordinate2 world);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWidth, KHeight);

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
		SCREEN_TYPE_GAME
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
		STAGE_TYPE_1,
		STAGE_TYPE_2,
		STAGE_TYPE_3
	};

	// 現在のステージ
	int stageNo = -1;


	// ゲームフレーム
	int gameFrame = 0;

	// ゲームが動いているかどうか（ゲームフラグ）
	int isGameOperation = false;




	/*   プレイヤー   */

	// 構造体
	struct Player player;

	// 復活
	player.respawn.isRespawn = false;
	player.respawn.timer = 120;

	// 位置
	player.pos.world = { static_cast<int>(kWidth / 2) , 0.0f };
	player.pos.screen = CoordinateTransformation(player.pos.world);

	// 移動速度
	player.vel = { 0.0f , 0.0f };

	// 加速度
	player.acceleration = { 0.0f , 0.0f };

	// 図形の半径
	player.radius = { 0.0f , 0.0f };


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

			// ゲームが動いている（ゲームフラグがtrueである）ときに、ゲームフレームを動かす
			if (isGameOperation)
			{
				gameFrame++;
			}

			// スタート画面切り替え
			switch (startNo)
			{
			case START_TYPE_ORPNING:

				/*   オープニング画面   */

				if (menuNo == MENU_TYPE_START)
				{
					// スペースキーを押すと、ステージセレクトに切り替わる

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
						if (isGameOperation >= 30)
						{
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
						if (isGameOperation >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_1;


							// オープニング画面に切り替わる
							startNo = START_TYPE_ORPNING;

							// スタートボタンを選ぶ
							menuNo = MENU_TYPE_START;


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
						if (isGameOperation >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_2;


							// オープニング画面に切り替わる
							startNo = START_TYPE_ORPNING;

							// スタートボタンを選ぶ
							menuNo = MENU_TYPE_START;


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
						if (isGameOperation >= 70)
						{
							// ゲーム画面に切り替わる
							screenNo = SCREEN_TYPE_GAME;

							// ステージ1を選ぶ
							stageNo = STAGE_TYPE_3;


							// オープニング画面に切り替わる
							startNo = START_TYPE_ORPNING;

							// スタートボタンを選ぶ
							menuNo = MENU_TYPE_START;


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
			    ステージを設定する1
			-----------------------*/

			switch (stageNo)
			{
			case STAGE_TYPE_1:

				break;

			case STAGE_TYPE_2:

				break;

			case STAGE_TYPE_3:

				break;
			}


			///
			/// ↑ ゲーム画面ここまで
			/// 

			break;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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
	screen.y = -world.y + static_cast<float>(KHeight) - 100.0f;

	return screen;
}