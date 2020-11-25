#include <iostream>
#include <random>
#include <functional>

constexpr int PlayerCount = 4;
constexpr int DiceCount   = 3;

struct Player
{
	int val[DiceCount]{ 0 };
};

signed main(int argc, char** argv)
{
	//ランダム初期化用デバイス
	std::random_device randomDevice;
	//メルセンヌツイスターやつ
	std::mt19937 random{ randomDevice() };
	//一様分布やつ
	std::uniform_int_distribution<int> dice{ 1, 6 };

	//ダイスロール関数
	std::function<int()> dice_roll = [&]() { return dice(random); };

	//プレイヤーを数だけ用意してダイスを振る
	Player p[PlayerCount];
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			p[i].val[j] = dice_roll();
		}
	}

	//振った結果の表示
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "Player" << i + 1 << ": ";
		for (int j = 0; j < DiceCount; j++)
		{
			std::cout << p[i].val[j];
			if (j + 1 != DiceCount)
			{
				std::cout << ",";
			}
		}
		std::cout << std::endl;
	}

	return 0;
}