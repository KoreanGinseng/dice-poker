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
	//�����_���������p�f�o�C�X
	std::random_device randomDevice;
	//�����Z���k�c�C�X�^�[���
	std::mt19937 random{ randomDevice() };
	//��l���z���
	std::uniform_int_distribution<int> dice{ 1, 6 };

	//�_�C�X���[���֐�
	std::function<int()> dice_roll = [&]() { return dice(random); };

	//�v���C���[�𐔂����p�ӂ��ă_�C�X��U��
	Player p[PlayerCount];
	for (int i = 0; i < PlayerCount; i++)
	{
		for (int j = 0; j < DiceCount; j++)
		{
			p[i].val[j] = dice_roll();
		}
	}

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

	return 0;
}