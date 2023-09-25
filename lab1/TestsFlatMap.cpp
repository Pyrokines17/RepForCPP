#include "FlatMap.h"
#include "gtest/gtest.h"

namespace {
	
	TEST(FlatMapTest, size) {
		FlatMap homework;

		EXPECT_EQ(homework.size(), 0);

		homework["����"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.size(), 1);

		homework["���"] = "�. 16, 17";
		homework["���"] = "���. ������ 1-2";

		EXPECT_EQ(homework.size(), 3);

		homework["���������"] = "1.66";
		homework["������"] = "�������� ������";

		EXPECT_EQ(homework.size(), 5);

		homework.erase("������");

		EXPECT_EQ(homework.size(), 4);
	}

	TEST(FlatMapTest, contains) {
		FlatMap homework;

		EXPECT_EQ(homework.contains("����"), false);

		homework["����"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.contains("����"), true);

		homework["���"] = "�. 16, 17";
		homework["���"] = "���. ������ 1-2";

		EXPECT_EQ(homework.contains("������"), false);
		EXPECT_EQ(homework.contains("���"), true);
		EXPECT_EQ(homework.contains("���"), true);

		homework.erase("���");

		EXPECT_EQ(homework.contains("���"), false);
	}

	TEST(FlatMapTest, erase) {
		FlatMap homework;

		EXPECT_EQ(homework.erase("����"), 0);

		homework["����"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.erase("����"), 1);

		homework["���"] = "�. 16, 17";
		homework["���"] = "���. ������ 1-2";

		EXPECT_EQ(homework.erase("������"), 0);
		EXPECT_EQ(homework.erase("���"), 1);
		EXPECT_EQ(homework.erase("���"), 0);
	}

	TEST(FlatMapTest, clear) {
		FlatMap homework;

		homework.clear();

		EXPECT_EQ(homework.size(), 0);
		EXPECT_EQ(homework.contains("����"), false);

		homework["����"] = "3985, 3987, 3989, 3998";

		EXPECT_EQ(homework.size(), 1);
		EXPECT_EQ(homework.contains("����"), true);

		homework["���"] = "�. 16, 17";
		homework["���"] = "���. ������ 1-2";

		EXPECT_EQ(homework.size(), 3);
		EXPECT_EQ(homework.contains("������"), 0);
		EXPECT_EQ(homework.contains("���"), true);
		EXPECT_EQ(homework.contains("���"), true);

		homework.clear();

		EXPECT_EQ(homework.size(), 0);
		EXPECT_EQ(homework.contains("����"), false);
		EXPECT_EQ(homework.contains("���"), false);
		EXPECT_EQ(homework.contains("���"), false);
	}

	//�������� ������ �� ���.������
}