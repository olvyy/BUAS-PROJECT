#pragma once
#include "item.hpp"
#include <iostream>
#include <memory>
#include <vector>

class itemPool
{
public:
	itemPool(int size)
		: maxSize(size)
	{
		for (int i = 0; i < maxSize; i++)
		{
			items.push_back(std::make_shared<Item>());
		}
	}

	std::shared_ptr<Item> getItem()
	{
		if (items.empty())
		{
			return std::make_shared<Item>();
		}
		else
		{
			auto item = items.back();
			items.pop_back();
			return item;
		}
	}

	void returnItem(std::shared_ptr<Item> item)
	{
		if (items.size() < maxSize)
		{
			items.push_back(item);
		}
		else
		{
			std::cout << "Pool full" << std::endl;
		}
	}

	const std::vector<std::shared_ptr<Item>>& getPool() const
	{
		return items;
	}

private:
	std::vector<std::shared_ptr<Item>> items;
	int maxSize;
};