#include "FlatMap.h"
#include "gtest/gtest.h"

namespace {
	
	TEST(FlatMapTest, size) {
		FlatMap homework;

		EXPECT_EQ(homework.size(), 0);

		homework["ДГМА"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.size(), 1);

		homework["ЛОП"] = "П. 16, 17";
		homework["ЭВМ"] = "Лаб. работа 1-2";

		EXPECT_EQ(homework.size(), 3);

		homework["Дискретка"] = "1.66";
		homework["Физика"] = "Изученик теории";

		EXPECT_EQ(homework.size(), 5);

		homework.erase("Физика");

		EXPECT_EQ(homework.size(), 4);
	}

	TEST(FlatMapTest, contains) {
		FlatMap homework;

		EXPECT_EQ(homework.contains("ДГМА"), false);

		homework["ДГМА"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.contains("ДГМА"), true);

		homework["ЛОП"] = "П. 16, 17";
		homework["ЭВМ"] = "Лаб. работа 1-2";

		EXPECT_EQ(homework.contains("ЛОПЭВМ"), false);
		EXPECT_EQ(homework.contains("ЛОП"), true);
		EXPECT_EQ(homework.contains("ЭВМ"), true);

		homework.erase("ЭВМ");

		EXPECT_EQ(homework.contains("ЭВМ"), false);
	}

	TEST(FlatMapTest, erase) {
		FlatMap homework;

		EXPECT_EQ(homework.erase("ДГМА"), 0);

		homework["ДГМА"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.erase("ДГМА"), 1);

		homework["ЛОП"] = "П. 16, 17";
		homework["ЭВМ"] = "Лаб. работа 1-2";

		EXPECT_EQ(homework.erase("ЛОПЭВМ"), 0);
		EXPECT_EQ(homework.erase("ЭВМ"), 1);
		EXPECT_EQ(homework.erase("ЭВМ"), 0);
	}

	TEST(FlatMapTest, clear) {
		FlatMap homework;

		homework.clear();

		EXPECT_EQ(homework.size(), 0);
		EXPECT_EQ(homework.contains("ДГМА"), false);

		homework["ДГМА"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.size(), 1);
		EXPECT_EQ(homework.contains("ДГМА"), true);

		homework["ЛОП"] = "П. 16, 17";
		homework["ЭВМ"] = "Лаб. работа 1-2";

		EXPECT_EQ(homework.size(), 3);
		EXPECT_EQ(homework.contains("ЛОПЭВМ"), 0);
		EXPECT_EQ(homework.contains("ЛОП"), true);
		EXPECT_EQ(homework.contains("ЭВМ"), true);

		homework.clear();

		EXPECT_EQ(homework.size(), 0);
		EXPECT_EQ(homework.contains("ДГМА"), false);
		EXPECT_EQ(homework.contains("ЛОП"), false);
		EXPECT_EQ(homework.contains("ЭВМ"), false);
	}

	//добавить тестов на доп.задачи
}