#include <gtest/gtest.h>


#include "USIOption.h"

TEST(USIOptionTest, test)
{
	USIOptionCheck opt_check("name", "true");

	USIOption* opt = &opt_check;

	ASSERT_EQ(opt->Name, "name");
	ASSERT_EQ(opt->Type, USIOptionType::CHECK);
	ASSERT_EQ(opt->ValueToString(), "true");
	opt->SetValue(false);
	ASSERT_EQ(opt->ValueToString(), "false");
	

	USIOptionSpin opt_spin("namex", 10, 1, 15);
	opt = &opt_spin;
	ASSERT_EQ(opt_spin.DefaultValue, 10);
	ASSERT_EQ(opt_spin.Min, 1);
	ASSERT_EQ(opt_spin.Max, 15);
	
	ASSERT_EQ(opt->Name, "namex");
	ASSERT_EQ(opt->Type, USIOptionType::SPIN);
	ASSERT_EQ(opt->ValueToString(), "10");

	opt->SetValue(15);
	ASSERT_EQ(opt->ValueToString(), "15");

	std::vector<std::string> strlist;

	strlist.push_back("str1");
	strlist.push_back("str2");
	strlist.push_back("str3");
	USIOptionCombo opt_combo("name combo", "combo value", strlist);

	opt = &opt_combo;
	ASSERT_EQ(opt_combo.DefaultValue, "combo value");
	ASSERT_EQ(opt_combo.ComboValues.size(), 3);

	ASSERT_EQ(opt->Name, "name combo");
	ASSERT_EQ(opt->Type, USIOptionType::COMBO);
	ASSERT_EQ(opt->ValueToString(), "combo value");
	opt_combo.SetValue("moji2");
	opt->SetValue("moji");
	ASSERT_EQ(opt->ValueToString(), "moji");
	opt->SetValue(1);
	ASSERT_EQ(opt->ValueToString(), "str2");

	USIOptionString opt_string("name string", "default value");
	opt = &opt_string;

	ASSERT_EQ(opt->Name, "name string");
	ASSERT_EQ(opt->Type, USIOptionType::STRING);
	ASSERT_EQ(opt->ValueToString(), "default value");
	opt->SetValue("moji moji");
	ASSERT_EQ(opt->ValueToString(), "moji moji");

	USIOptionFilename opt_file("name file", "filename");
	opt = &opt_file;

	ASSERT_EQ(opt->Name, "name file");
	ASSERT_EQ(opt->Type, USIOptionType::FILENAME);
	ASSERT_EQ(opt->ValueToString(), "filename");
	opt->SetValue("moji moji");
	ASSERT_EQ(opt->ValueToString(), "moji moji");
}


TEST(USIOptionTest, testOptions)
{
	USIOptions options;

	ASSERT_EQ(options.AddOption("option name name1 type check default true"), true);
	ASSERT_EQ(options.Contents("name1"), true);
	ASSERT_EQ(options.Contents("name"), false);

	USIOption* opt = options.GetOption("name1");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name1");
	ASSERT_EQ(opt->ValueToString(), "true");

	ASSERT_EQ(options.AddOption("option name name2 type check default false"), true);
	ASSERT_EQ(options.AddOption("option name name3 type spin default 10 min 2 max 5"), true);
	ASSERT_EQ(options.AddOption("option name name 4 type combo default combo str1 var combo str0 var combo str1 var combo str2"), true);
	ASSERT_EQ(options.AddOption("option name name 5 type string default string 3"), true);
	ASSERT_EQ(options.AddOption("option name name 7 type filename default string 3"), true);

	ASSERT_EQ(options.size(), 6);

	ASSERT_EQ(options.Contents("name2"), true);
	opt = options.GetOption("name2");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name2");
	ASSERT_EQ(opt->ValueToString(), "false");


	opt = options.GetOption("name3");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name3");
	ASSERT_EQ(opt->Type, USIOptionType::SPIN);
	ASSERT_EQ(opt->ValueToString(), "10");
	const USIOptionSpin* spin = static_cast<const USIOptionSpin *>(opt);
	ASSERT_EQ(spin->Min, 2);
	ASSERT_EQ(spin->Max, 5);

	opt = options.GetOption("name 4");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name 4");
	ASSERT_EQ(opt->Type, USIOptionType::COMBO);
	ASSERT_EQ(opt->ValueToString(), "combo str1");
	opt->SetValue(0);
	ASSERT_EQ(opt->ValueToString(), "combo str0");
	opt->SetValue(1);
	ASSERT_EQ(opt->ValueToString(), "combo str1");
	opt->SetValue(2);
	ASSERT_EQ(opt->ValueToString(), "combo str2");


	opt = options.GetOption("name 5");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name 5");
	ASSERT_EQ(opt->Type, USIOptionType::STRING);
	ASSERT_EQ(opt->ValueToString(), "string 3");

	opt = options.GetOption("name 7");
	ASSERT_TRUE(opt != nullptr);
	ASSERT_EQ(opt->Name, "name 7");
	ASSERT_EQ(opt->Type, USIOptionType::FILENAME);
	ASSERT_EQ(opt->ValueToString(), "string 3");


	options.Clear();
	ASSERT_EQ(options.size(), 0);
}
