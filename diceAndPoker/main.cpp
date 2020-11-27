#include <iostream>
#include <random>
#include <functional>
#include <algorithm>
#include <string>

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

//ストレートが出たかのフラグ(メッセージ用)
bool bStraight = false;
bool bStraightMulti = false;

//ペアが出たかのフラグ(メッセージ用)
bool bPair = false;

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
	bStraight = bPair = bStraightMulti = false;
}

//ゲーム最初のダイス振るやつ
void DiceRoll(Player* p)
{
	//プレイヤーの数だけダイスマン
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			p[i].val[j] = dice_roll();
		}
	}

	//デバッグ用決め打ちダイス
	/**
	p[0].val[0] = 2; p[0].val[1] = 2; p[0].val[2] = 3;
	p[1].val[0] = 4; p[1].val[1] = 5; p[1].val[2] = 6;
	p[2].val[0] = 2; p[2].val[1] = 5; p[2].val[2] = 6;
	p[3].val[0] = 3; p[3].val[1] = 2; p[3].val[2] = 1;
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
		message = "・Player" + std::to_string(list[0].first + 1) + "のみ役(連番)がある";
		wonPlayer.push_back(list[0].first);
	}
	else
	{
		bStraightMulti = true;
		message = "・Player";
		for (int i = 0; i < list.size(); i++)
		{
			message += std::to_string(list[i].first + 1);
			if (i + 1 != list.size())
			{
				message += "、";
			}
		}
		//ストレート！！
		std::vector<PII> StraightList = list;
		//ループフラグ
		bool bEx = false;
		do
		{
			//フラグ折の天才
			bEx = false;
			//とりまデータもっとく
			int straight = StraightList[0].second;
			//ストレートお化けのデタ目が重なってる場合
			std::vector<PII> tmp{ StraightList[0] };
			//他のストレートお化けと数字バトルを繰り広げる
			for (int i = 1; i < StraightList.size(); i++)
			{
				//同点だとぉ！？
				if (straight == StraightList[i].second)
				{
					tmp.push_back(StraightList[i]);
					continue;
				}
				else if (straight < StraightList[i].second)
				{
					straight = list[i].second;
					tmp.clear();
					tmp.push_back(StraightList[i]);
					bEx = true;
				}
			}
			StraightList = tmp;
		} while (bEx);

		message += "が役(連番)が成立しているが、Player";
		if (StraightList.size() == 1)
		{
			wonPlayer.push_back(StraightList[0].first);
		}
		else
		{
			for (int i = 0; i < StraightList.size(); i++)
			{
				wonPlayer.push_back(StraightList[i].first);
			}
		}
	}
}

//ペア魔人での勝者決定
void PairWiner(Player* p, std::vector<PII>& list)
{
	//std::cout << "ペア魔人あらわる" << std::endl;
	if (list.size() == 1)
	{
		if (!bStraight)
		{
			message = "・Player" + std::to_string(list[0].first + 1) + "のみ役(ペア)がある";
			wonPlayer.push_back(list[0].first);
		}
		else
		{
			message = "・Player" + std::to_string(list[0].first + 1) + "が役(ペア)が成立しているが、Player";
		}
	}
	else
	{
		message = "・Player";
		for (int i = 0; i < list.size(); i++)
		{
			message += std::to_string(list[i].first + 1);
			if (i + 1 != list.size())
			{
				message += "、";
			}
		}
		//ペア！！
		std::vector<PII> PairList = list;
		//ループフラグ
		bool bEx = false;
		do
		{
			//フラグ折の天才
			bEx = false;
			//とりまデータもっとく
			int pair = PairList[0].second;
			//ペア魔人のデタ目が重なってる場合
			std::vector<PII> tmp{ PairList[0] };
			//他のペア魔人と数字バトルを繰り広げる
			for (int i = 1; i < PairList.size(); i++)
			{
				//同点だとぉ！？
				if (pair == PairList[i].second)
				{
					tmp.push_back(PairList[i]);
					continue;
				}
				else if (pair < PairList[i].second)
				{
					pair = list[i].second;
					tmp.clear();
					tmp.push_back(PairList[i]);
					bEx = true;
				}
			}
			PairList = tmp;
		} while (bEx);

		message += "が役(ペア)が成立しているが、Player";

		if (bStraight)
		{
			return;
		}
		if (PairList.size() == 1)
		{
			wonPlayer.push_back(PairList[0].first);
		}
		else
		{
			for (int i = 0; i < PairList.size(); i++)
			{
				wonPlayer.push_back(PairList[i].first);
			}
		}
	}
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

	if (wonPlayer.size() == 1)
	{
		message = "・数字の合計が一番大きいPlayer";
	}
	else
	{
		message = "・数字の合計が同じPlayer";
	}
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
		bStraight = true;
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
		bPair = true;
		PairWiner(p, pairPlayer);
		return;
	}

	if (!bStraight)
	{
		//残る処理は雑魚どもよ。
		SumWiner(p);
	}

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
	std::cout << message;
	
	if (wonPlayer.size() == 1 && bStraight && !bPair)
	{
		return;
	}

	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::string tmpMessage = (bStraight || bPair) ? "、" : "と";
			std::cout << tmpMessage;
		}
		std::cout << wonPlayer[i] + 1;
	}

	if (wonPlayer.size() == 1)
	{
		std::string wonMessage = bStraight ? ((bPair && !bStraightMulti) ? "が上の役(連番)が成立している" : "のほうが数字が大きい") : (bPair ? "のほうが数字が大きい" : "の勝ち");
		std::cout << wonMessage << std::endl;
	}
	else
	{
		std::string wonMessage = bStraight ? "が役(連番)で引き分け" : (bPair ? "が役(ペア)で引き分け" : "で引き分け");
		std::cout << wonMessage << std::endl;
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