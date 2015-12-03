
#include <string>

#include <fun.hpp>

#include "gtest/gtest.h"

using namespace std;
using namespace app;

/*
void test1()
{
	int i = 100;
	string txt = run_fun(i);
	// do assert here
}

int main()
{
	test1();
}
*/

TEST(FunTest, FunBasic) {
  EXPECT_EQ("5", run_fun(5));
  EXPECT_EQ("-5", run_fun(-5));
}

TEST(FunTest, FunIntentionalError) {
  EXPECT_EQ("5", run_fun(3));
  EXPECT_EQ("-5", run_fun(-5));
}
