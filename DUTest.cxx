////////////////////////////////////////////////////////////
TEST_GROUP(ParsedStrClass)
{
	void setup() {

	}
	void teardown() {

	}
};
//method IsCorrectString
TEST(ParsedStrClass, CorrectSourceString)
{
	const char *s = "|Correct|String|Status|Date\n";
	ParsedStr ps;
	CHECK(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, SourceStringWithoutEOL)
{
	const char *s = "|Correct|String|Status|Date";
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, SourceStringWithoutStartDelimiter)
{
	const char *s = "In|Correct|String|Status|Date";
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooLongSourceString)
{
	char arr[ParsedStr::srclen+1];
	arr[ParsedStr::srclen] = '\0';
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(arr));
}

TEST(ParsedStrClass, TooMuchDelimitersInSourceString)
{
	const char *s = "||||||";
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooFewDelimitersInSourceString)
{
	const char *s = "|Correct|String|Status|Date|\n";
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, CorrectOriginal)
{
	const char *s = "|Correct|||\n";
	ParsedStr ps;
	CHECK(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooLongOriginal)
{
	char arr[ParsedStr::orglen+1];
	arr[ParsedStr::orglen] = '\0';
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(arr));
}

TEST(ParsedStrClass, CorrectTranslation)
{
	const char *s = "||Correct||\n";
	ParsedStr ps;
	CHECK(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooLongTranslation)
{
	char arr[ParsedStr::trllen+1];
	arr[ParsedStr::trllen] = '\0';
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(arr));
}

TEST(ParsedStrClass, CorrectStatus)
{
	const char *s = "|||new|\n";
	ParsedStr ps;
	CHECK(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooLongStatus)
{
	char arr[ParsedStr::stlen+1];
	arr[ParsedStr::stlen] = '\0';
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(arr));
}

TEST(ParsedStrClass, CorrectDate)
{
	const char *s = "||||01-01-2001\n";
	ParsedStr ps;
	CHECK(ps.IsCorrectString(s));
}

TEST(ParsedStrClass, TooLongDate)
{
	char arr[ParsedStr::dtlen+1];
	arr[ParsedStr::dtlen] = '\0';
	ParsedStr ps;
	CHECK_FALSE(ps.IsCorrectString(arr));
}
//method CompareDates
TEST(ParsedStrClass, CompareDifferentDates)
{
	CHECK_FALSE(ParsedStr::CompareDates("10-01-2022", "11-02-2022") == 0);
}

TEST(ParsedStrClass, CompareSameDates)
{
	CHECK(ParsedStr::CompareDates("10-01-2022", "10-01-2022") == 0);
}
/*
TEST(ParsedStrClass, ChangeFormatOfDate)
{
	char *date = "10-01-2022";
	ParsedStr::DateToYMD(date);
	CHECK_EQUAL(date, "2022-01-22");
}
*/
////////////////////////////////////////////////////////////
TEST_GROUP(DFileClass)
{

};

TEST(DFileClass, DFileConstructor)
{
	DFile f;
	CHECK(f.Fl() == 0);
}
////////////////////////////////////////////////////////////
TEST_GROUP(WordListClass)
{

};

TEST(WordListClass, WordListConstructor)
{

}
