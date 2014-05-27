#include <gmock/gmock.h>

int main(int argc, char **argv)
{
	testing::InitGoogleMock(&argc, argv);
	int i = RUN_ALL_TESTS();
	getchar();
	return i;
}