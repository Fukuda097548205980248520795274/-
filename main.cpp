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
struct Pos2
{
	// ワールド座標
	struct Coordinate2 world;

	// スクリーン座標
	struct Coordinate2 screen;
};


/*----------------
    定数を作る
----------------*/

// 縦幅
const int KHeight = 800;

// 横幅
const int kWidth = 700;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWidth, KHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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
