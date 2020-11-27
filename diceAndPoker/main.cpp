#include <iostream>
#include <random>
#include <functional>
#include <algorithm>
#include <string>

#define PII std::pair<int, int>

constexpr int PlayerCount = 4;
constexpr int DiceCount   = 3;

// �Q�[����
constexpr int GameCount   = 3;

//�_�C�X�U�邾�������\�̂Ȃ����
std::function<int()> dice_roll;

//���҂̗��R
std::string message = "";

//���҂̃v���C���[�ԍ�
std::vector<int> wonPlayer;

//�X�g���[�g���o�����̃t���O(���b�Z�[�W�p)
bool bStraight = false;
bool bStraightMulti = false;

//�y�A���o�����̃t���O(���b�Z�[�W�p)
bool bPair = false;

//�v���C���[�\���̗l����
struct Player
{
	int val[DiceCount]{ 0 };

	//�_�C�X�ڂ̍��v�l�擾
	int GetSum(void)
	{
		return (val[0] + val[1] + val[2]);
	}

	//����ڂȂ�true��Ԃ�
	bool IsYahtzee(void)
	{
		return (val[0] == val[1] && val[0] == val[2]);
	}

	//�X�g���[�g�Ȃ�ő�l��Ԃ�
	int IsStraight(void)
	{
		std::vector<int> tmp = { val[0], val[1], val[2] };
		//�����\�[�g
		std::sort(tmp.begin(), tmp.end(), [](const auto& v1, const auto& v2) { return v1 < v2; });
		if (tmp[0] + 1 == tmp[1] && tmp[1] + 1 == tmp[2])
		{
			return tmp[2];
		}
		return 0;
	}

	//�y�A�Ȃ�y�A�ɂȂ���������Ԃ�
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

//�Q�[���̏�����
void Initialize(void)
{
	message = "";
	wonPlayer.clear();
	bStraight = bPair = bStraightMulti = false;
}

//�Q�[���ŏ��̃_�C�X�U����
void DiceRoll(Player* p)
{
	//�v���C���[�̐������_�C�X�}��
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			p[i].val[j] = dice_roll();
		}
	}

	//�f�o�b�O�p���ߑł��_�C�X
	/**
	p[0].val[0] = 2; p[0].val[1] = 2; p[0].val[2] = 3;
	p[1].val[0] = 4; p[1].val[1] = 5; p[1].val[2] = 6;
	p[2].val[0] = 2; p[2].val[1] = 5; p[2].val[2] = 6;
	p[3].val[0] = 3; p[3].val[1] = 2; p[3].val[2] = 1;
	/**/

	//�U�������ʂ̕\��
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

//���b�c�B�[���҂ł̏��Ҍ���
void YahtzeeWiner(Player* p, std::vector<int>& list)
{
	//std::cout << "���b�c�B�[���҂�����" << std::endl;

	//�����������b�c�B�[���҂��P�l�Ȃ炻��ŏI���B
	if (list.size() == 1)
	{
		wonPlayer.push_back(list[0]);
		message = "�E�����ڂ��o��Player";
		return;
	}
	else
	{
		//�Ȃ����o�͗Ⴊ�����ňӒn���������̂ŁB
		const std::string KANJI[] = { "��", "��", "�O", "�l" };
		message = "�E�����ڂ��o��Player��" + KANJI[(int)list.size() - 1] + "�l����̂ŁA\n�@�ڂ̑傫��Player";
		//���b�c�B�[�I�I
		std::vector<int> YahtzeeList = list;
		//���[�v�t���O
		bool bEx = false;
		do
		{
			//�t���O�܂̓V��
			bEx = false;
			//�Ƃ�܃f�[�^�����Ƃ�
			int yahtzeeMax = p[YahtzeeList[0]].val[0];
			//���b�c�B�[���҂̃f�^�ڂ��d�Ȃ��Ă�ꍇ
			std::vector<int> tmp{ YahtzeeList[0] };
			//���̃��b�c�B�[���҂Ɛ����o�g�����J��L����
			for (int i = 1; i < YahtzeeList.size(); i++)
			{
				//���_���Ƃ��I�H
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

//�X�g���[�g�������ł̏��Ҍ���
void StraightWiner(Player* p, std::vector<PII>& list)
{
	//std::cout << "�X�g���[�g������������" << std::endl;
	if (list.size() == 1)
	{
		message = "�EPlayer" + std::to_string(list[0].first + 1) + "�̂ݖ�(�A��)������";
		wonPlayer.push_back(list[0].first);
	}
	else
	{
		bStraightMulti = true;
		message = "�EPlayer";
		for (int i = 0; i < list.size(); i++)
		{
			message += std::to_string(list[i].first + 1);
			if (i + 1 != list.size())
			{
				message += "�A";
			}
		}
		//�X�g���[�g�I�I
		std::vector<PII> StraightList = list;
		//���[�v�t���O
		bool bEx = false;
		do
		{
			//�t���O�܂̓V��
			bEx = false;
			//�Ƃ�܃f�[�^�����Ƃ�
			int straight = StraightList[0].second;
			//�X�g���[�g�������̃f�^�ڂ��d�Ȃ��Ă�ꍇ
			std::vector<PII> tmp{ StraightList[0] };
			//���̃X�g���[�g�������Ɛ����o�g�����J��L����
			for (int i = 1; i < StraightList.size(); i++)
			{
				//���_���Ƃ��I�H
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

		message += "����(�A��)���������Ă��邪�APlayer";
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

//�y�A���l�ł̏��Ҍ���
void PairWiner(Player* p, std::vector<PII>& list)
{
	//std::cout << "�y�A���l������" << std::endl;
	if (list.size() == 1)
	{
		if (!bStraight)
		{
			message = "�EPlayer" + std::to_string(list[0].first + 1) + "�̂ݖ�(�y�A)������";
			wonPlayer.push_back(list[0].first);
		}
		else
		{
			message = "�EPlayer" + std::to_string(list[0].first + 1) + "����(�y�A)���������Ă��邪�APlayer";
		}
	}
	else
	{
		message = "�EPlayer";
		for (int i = 0; i < list.size(); i++)
		{
			message += std::to_string(list[i].first + 1);
			if (i + 1 != list.size())
			{
				message += "�A";
			}
		}
		//�y�A�I�I
		std::vector<PII> PairList = list;
		//���[�v�t���O
		bool bEx = false;
		do
		{
			//�t���O�܂̓V��
			bEx = false;
			//�Ƃ�܃f�[�^�����Ƃ�
			int pair = PairList[0].second;
			//�y�A���l�̃f�^�ڂ��d�Ȃ��Ă�ꍇ
			std::vector<PII> tmp{ PairList[0] };
			//���̃y�A���l�Ɛ����o�g�����J��L����
			for (int i = 1; i < PairList.size(); i++)
			{
				//���_���Ƃ��I�H
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

		message += "����(�y�A)���������Ă��邪�APlayer";

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

//���v�ł̏��Ҍ���
void SumWiner(Player* p)
{
	//���v�̔z���������Ⴄ(first : �v���C���[�ԍ�, second : �z��̍��v)
	std::vector< PII > sum(PlayerCount);
	for (int i = 0; i < PlayerCount; i++)
	{
		sum[i] = PII(i, p[i].GetSum());
	}

	//�~���Ń\�[�g�����Ⴆ
	std::sort(sum.begin(), sum.end(), [](const auto& v1, const auto& v2) { return v1.second > v2.second; });

	//���v�ɂ�鏟���҂��L�^���Ă�����
	int win = 0;
	do
	{
		wonPlayer.push_back(sum[win++].first);
	} while (win < PlayerCount && sum[win - 1].second == sum[win].second);

	if (wonPlayer.size() == 1)
	{
		message = "�E�����̍��v����ԑ傫��Player";
	}
	else
	{
		message = "�E�����̍��v������Player";
	}
}

//�Q�[���{��
void Game(void)
{
	//�v���C���[�𐔂����p�ӂ��ă_�C�X��U��
	Player p[PlayerCount];
	DiceRoll(p);

	//���b�c�B�[���҂�T��
	std::vector<int> yahtzeePlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsYahtzee())
		{
			yahtzeePlayer.push_back(i);
		}
	}

	//���b�c�B�[���҂����݂��邩�ۂ��ŏ�����ς��悤
	if (yahtzeePlayer.size())
	{
		YahtzeeWiner(p, yahtzeePlayer);
		return;
	}

	//�X�g���[�g�������̒T��
	std::vector<PII> straightPlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsStraight())
		{
			straightPlayer.push_back(PII(i, p[i].IsStraight()));
		}
	}

	//�X�g���[�g�����������݂��邩�ۂ��ŏ�����ς��悤
	if (straightPlayer.size())
	{
		StraightWiner(p, straightPlayer);
		bStraight = true;
	}

	//�y�A���l�̑{��
	std::vector<PII> pairPlayer;
	for (int i = 0; i < PlayerCount; i++)
	{
		if (p[i].IsPair())
		{
			pairPlayer.push_back(PII(i, p[i].IsPair()));
		}
	}
	
	//�y�A���l�����݂��邩�ۂ��ŏ�����ς��悤
	if (pairPlayer.size())
	{
		bPair = true;
		PairWiner(p, pairPlayer);
		return;
	}

	if (!bStraight)
	{
		//�c�鏈���͎G���ǂ���B
		SumWiner(p);
	}

}

//���҂̕\�������Ă����悤
void PrintWonMessage(void)
{
	std::cout << "...";
	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::cout << "��";
		}
		std::cout << "Player" << wonPlayer[i] + 1;
	}
	if (wonPlayer.size() == 1)
	{
		std::cout << "�̏���" << std::endl;
	}
	else
	{
		std::cout << "�̈�������" << std::endl;
	}

	//���R�̕\��
	std::cout << message;
	
	if (wonPlayer.size() == 1 && bStraight && !bPair)
	{
		return;
	}

	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::string tmpMessage = (bStraight || bPair) ? "�A" : "��";
			std::cout << tmpMessage;
		}
		std::cout << wonPlayer[i] + 1;
	}

	if (wonPlayer.size() == 1)
	{
		std::string wonMessage = bStraight ? ((bPair && !bStraightMulti) ? "����̖�(�A��)���������Ă���" : "�̂ق����������傫��") : (bPair ? "�̂ق����������傫��" : "�̏���");
		std::cout << wonMessage << std::endl;
	}
	else
	{
		std::string wonMessage = bStraight ? "����(�A��)�ň�������" : (bPair ? "����(�y�A)�ň�������" : "�ň�������");
		std::cout << wonMessage << std::endl;
	}
}

signed main(int argc, char** argv)
{
	//�����_���������p�f�o�C�X
	std::random_device randomDevice;
	//�����Z���k�c�C�X�^�[���
	std::mt19937 random{ randomDevice() };
	//��l���z���
	std::uniform_int_distribution<int> dice{ 1, 6 };

	//�_�C�X���[���֐�
	dice_roll = [&]() { return dice(random); };

	//��������Q�[��
	for (int gameRound = 1; gameRound <= GameCount; gameRound++)
	{
		Initialize();
		std::cout << "\nRound" << gameRound << ":" << std::endl;
		Game();
		PrintWonMessage();
	}

	return 0;
}