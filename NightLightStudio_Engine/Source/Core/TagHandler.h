#pragma once
#include "Singleton.h"

#include <vector>
class TagHandler: public Singleton<TagHandler> {
private:
	std::vector<int> tag_used;
public:
	void InsertTagToUsed(int tag)
	{
		for (int& i : tag_used)
			if (tag == i)
				return;
		tag_used.push_back(tag);
	}

	const std::vector<int>& GetTagUsed()
	{
		return tag_used;
	}
};

static TagHandler* TAG_HANDLER = TagHandler::GetInstance();
