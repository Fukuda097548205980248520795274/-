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

	/*   プレイヤー   */

	// 構造体
	struct Player player;

	// 復活
	player.respawn.isRespawn = false;
	player.respawn.timer = 120;

	// 位置
	player.pos.world = { static_cast<int>(kWidth / 2) , 0.0f };
	player.pos.screen = { 0.0f , 0.0f };

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