#include "Player.h"

std::vector<std::pair<int, int>> CPlayer::GetCards(void) const
{
	return m_Cards;
}

std::vector<std::pair<int, int>>& CPlayer::GetCards(void)
{
	return m_Cards;
}

void CPlayer::SetCard(const int & no, const std::pair<int, int>& card)
{
	if (no >= m_Cards.size())
	{
		m_Cards.resize(no + 1);
	}
	m_Cards[no] = card;
}

void CPlayer::ResetCards(void)
{
	m_Cards.clear();
}
