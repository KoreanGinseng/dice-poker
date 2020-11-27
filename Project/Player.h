#pragma once
#include "GameDefine.h"

class CPlayer
{
protected:

	std::vector<std::pair<int, int>> m_Cards;

public:
	explicit CPlayer(void) = default;
	virtual ~CPlayer(void) = default;

	virtual std::vector<std::pair<int, int>> GetCards(void) const;

	virtual std::vector<std::pair<int, int>>& GetCards(void);

	virtual void SetCard(const int& no, const std::pair<int, int>& card);

	virtual void ResetCards(void);
};

