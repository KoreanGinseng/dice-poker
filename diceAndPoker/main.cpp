#include <iostream>
#include <random>
#include <functional>
#include <algorithm>

#define PII std::pair<int, int>

constexpr int PlayerCount = 4;
constexpr int DiceCount   = 3;

// ゲーム数
constexpr int GameCount   = 3;

//ダイス振るだけしか能のないやつ
std::function<int()> dice_roll;

//勝者の理由
std::string message = "";

//勝者のプレイヤー番号
std::vector<int> wonPlayer;

//プレイヤー構造体様だぞ
struct Player
{
	int val[DiceCount]{ 0 };

	//ダイス目の合計値取得
	int GetSum(void)
	{
		return (val[0] + val[1] + val[2]);
	}

	//ぞろ目ならtrueを返す
	bool IsYahtzee(void)
	{
		return (val[0] == val[1] && val[0] == val[2]);
	}

	//ストレートなら最大値を返す
	int IsStraight(void)
	{
		std::vector<int> tmp = { val[0], val[1], val[2] };
		//昇順ソート
		std::sort(tmp.begin(), tmp.end(), [](const auto& v1, const auto& v2) { return v1 < v2; });
		if (tmp[0] + 1 == tmp[1] && tmp[1] + 1 == tmp[2])
		{
			return tmp[2];
		}
		return 0;
	}

	//ペアならペアになった数字を返す
	int IsPair(void)
	{
		if (val[0] == val[1] || val[0] == val[2])
		{
			return val[0];
		}
		else if (val[1] == val[2])
		{
			return val[1];
		}
		return 0;
	}
};

//ゲームの初期化
void Initialize(void)
{
	message = "";
	wonPlayer.clear();
}

//ゲーム最初のダイス振るやつ
void DiceRoll(Player* p)
{
	//プレイヤーの数だけダイスマン
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			//p[i].val[j] = dice_roll();
			p[i].val[j] = 4 - i;
		}
	}

	//デバッグ用決め打ちダイス
	/**
	p[0].val[0] = 4; p[0].val[1] = 4; p[0].val[2] = 4;
	p[1].val[0] = 4; p[1].val[1] = 4; p[1].val[2] = 4;
	p[2].val[0] = 4; p[2].val[1] = 4; p[2].val[2] = 4;
	p[3].val[0] = 1; p[3].val[1] = 1; p[3].val[2] = 1;
	/**/

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
}

//ヤッツィー強者での勝者決定
void YahtzeeWiner(Player* p, std::vector<int>& list)
{
	//std::cout << "ヤッツィー強者あらわる" << std::endl;

	//そもそもヤッツィー強者が１人ならそれで終わり。
	if (list.size() == 1)
	{
		wonPlayer.push_back(list[0]);
		message = "・同じ目が出たPlayer";
		return;
	}
	else
	{
		//なぜか出力例が漢字で意地悪だったので。
		const std::string KANJI[] = { "一", "二", "三", "四" };
		message = "・同じ目が出たPlayerが" + KANJI[(int)list.size() - 1] + "人いるので、\n　目の大きいPlayer";
		//ヤッツィー！！
		std::vector<int> YahtzeeList = list;
		//ループフラグ
		bool bEx = false;
		do
		{
			//フラグ折の天才
			bEx = false;
			//とりまデータもっとく
			int yahtzeeMax = p[YahtzeeList[0]].val[0];
			//ヤッツィー強者のデタ目が重なってる場合
			std::vector<int> tmp{ YahtzeeList[0] };
			//他のヤッツィー強者と数字バトルを繰り広げる
			for (int i = 1; i < YahtzeeList.size(); i++)
			{
				//同点だとぉ！？
				if (yahtzeeMax == p[YahtzeeList[i]].val[0])
				{
					tmp.push_back(YahtzeeList[i]);
					continue;
				}
				else if (yahtzeeMax < p[YahtzeeList[i]].val[0])
				{
					yahtzeeMax = p[YahtzeeList[i]].val[0];
					tmp.clear();
					tmp.push_back(YahtzeeList[i]);
					bEx = true;
				}
			}
			YahtzeeList = tmp;
		} 
		while (bEx);
		
		if (YahtzeeList.size() == 1)
		{
			wonPlayer.push_back(YahtzeeList[0]);
		}
		else
		{
			for (int i = 0; i < YahtzeeList.size(); i++)
			{
				wonPlayer.push_back(YahtzeeList[i]);
			}
		}
	}
}

//ストレートお化けでの勝者決定
void StraightWiner(Player* p, std::vector<PII>& list)
{
	//std::cout << "ストレートお化けあらわる" << std::endl;
	if (list.size() == 1)
	{
		wonPlayer.push_back(list[0].first);
	}
	else
	{
		int straight = list[0].second;
		for (int i = 1; i < list.size(); i++)
		{
			if (straight < list[i].second)
			{
				straight = list[i].second;
			}
		}
	}
}

//ペア魔人での勝者決定
void PairWiner(Player* p, std::vector<PII>& list)
{
	std::cout << "ペア魔人あらわる" << std::endl;
}

//合計での勝者決定
void SumWiner(Player* p)
{
	//合計の配列を作っちゃう(first : プレイヤー番号, second : 配列の合計)
	std::vector< PII > sum(PlayerCount);
	for (int i = 0; i < PlayerCount; i++)
	{
		sum[i] = PII(i, p[i].GetSum());
	}

	//降順でソートしちゃえ
	std::sort(sum.begin(), sum.end(), [](const auto& v1, const auto& v2) { return v1.second > v2.second; });

	//合計による勝利者を記録してあげる
	int win = 0;
	do
	{
		wonPlayer.push_back(sum[win++].first);
	} while (win < PlayerCount && sum[win - 1].second == sum[win].second);
}

//ゲーム本体
void Game(void)
{
	//プレイヤーを数だけ用意してダイスを振る
	Player p[PlayerCount];
	DiceRoll(p);

	//ヤッツィー強者を探す
	std::vector<int> yahtzeePlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsYahtzee())
		{
			yahtzeePlayer.push_back(i);
		}
	}

	//ヤッツィー強者が存在するか否かで処理を変えよう
	if (yahtzeePlayer.size())
	{
		YahtzeeWiner(p, yahtzeePlayer);
		return;
	}

	//ストレートお化けの探索
	std::vector<PII> straightPlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsStraight())
		{
			straightPlayer.push_back(PII(i, p[i].IsStraight()));
		}
	}

	//ストレートお化けが存在するか否かで処理を変えよう
	if (straightPlayer.size())
	{
		StraightWiner(p, straightPlayer);
		return;
	}

	//ペア魔人の捜索
	std::vector<PII> pairPlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsPair())
		{
			pairPlayer.push_back(PII(i, p[i].IsPair()));
		}
	}
	
	//ペア魔人が存在するか否かで処理を変えよう
	if (pairPlayer.size())
	{
		PairWiner(p, pairPlayer);
		return;
	}

	//残る処理は雑魚どもよ。
	SumWiner(p);
}

//勝者の表示をしてあげよう
void PrintWonMessage(void)
{
	std::cout << "...";
	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::cout << "と";
		}
		std::cout << "Player" << wonPlayer[i] + 1;
	}
	if (wonPlayer.size() == 1)
	{
		std::cout << "の勝ち" << std::endl;
	}
	else
	{
		std::cout << "の引き分け" << std::endl;
	}

	//理由の表示
	std::cout << message.c_str();
	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::cout << "と";
		}
		std::cout << wonPlayer[i] + 1;
	}

	if (wonPlayer.size() == 1)
	{
		std::cout << "の勝ち" << std::endl;
	}
	else
	{
		std::cout << "で引き分け" << std::endl;
	}
}

signed main(int argc, char** argv)
{
	//ランダム初期化用デバイス
	std::random_device randomDevice;
	//メルセンヌツイスターやつ
	std::mt19937 random{ randomDevice() };
	//一様分布やつ
	std::uniform_int_distribution<int> dice{ 1, 6 };

	//ダイスロール関数
	dice_roll = [&]() { return dice(random); };

	//ここからゲーム
	for (int gameRound = 1; gameRound <= GameCount; gameRound++)
	{
		Initialize();
		std::cout << "\nRound" << gameRound << ":" << std::endl;
		Game();
		PrintWonMessage();
	}

	return 0;
}