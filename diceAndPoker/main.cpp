#include <iostream>
#include <random>
#include <functional>
#include <algorithm>

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
}

//�Q�[���ŏ��̃_�C�X�U����
void DiceRoll(Player* p)
{
	//�v���C���[�̐������_�C�X�}��
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			//p[i].val[j] = dice_roll();
			p[i].val[j] = 4 - i;
		}
	}

	//�f�o�b�O�p���ߑł��_�C�X
	/**
	p[0].val[0] = 4; p[0].val[1] = 4; p[0].val[2] = 4;
	p[1].val[0] = 4; p[1].val[1] = 4; p[1].val[2] = 4;
	p[2].val[0] = 4; p[2].val[1] = 4; p[2].val[2] = 4;
	p[3].val[0] = 1; p[3].val[1] = 1; p[3].val[2] = 1;
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

//�y�A���l�ł̏��Ҍ���
void PairWiner(Player* p, std::vector<PII>& list)
{
	std::cout << "�y�A���l������" << std::endl;
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
		return;
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
		PairWiner(p, pairPlayer);
		return;
	}

	//�c�鏈���͎G���ǂ���B
	SumWiner(p);
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
	std::cout << message.c_str();
	for (int i = 0; i < wonPlayer.size(); i++)
	{
		if (i != 0)
		{
			std::cout << "��";
		}
		std::cout << wonPlayer[i] + 1;
	}

	if (wonPlayer.size() == 1)
	{
		std::cout << "�̏���" << std::endl;
	}
	else
	{
		std::cout << "�ň�������" << std::endl;
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