#include <gtest/gtest.h>

#include <iostream>
#include "KeyValue.h"

TEST(KeyValueTest, test)
{
	KeyValue kv;

	kv["name1"] = "value1";
	kv["name2"] = "value2";
	kv["name3"] = "value3";
	kv["name4"] = "value4";

	kv.Save("kv.ini");

	kv.clear();

	KeyValue kv2;

	kv2.Load("kv.ini");

	ASSERT_EQ(kv2["name1"], "value1");
	ASSERT_EQ(kv2["name2"], "value2");
	ASSERT_EQ(kv2["name3"], "value3");
	ASSERT_EQ(kv2["name4"], "value4");


}