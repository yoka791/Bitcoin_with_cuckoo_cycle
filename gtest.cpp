#include <gtest/gtest.h>
#include "sha256.h"

TEST(MathTest, TwoPlusTwoEqualsFour) {
	EXPECT_EQ(2 + 2, 4);
}

TEST(SHA_256_TEST, do_nothing) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(SHA_256_TEST, regular_test_vectors) {
	std::string sha_output_1 = sha256("grape");
	std::string sha_output_2 = sha256("abc");
	std::string sha_output_3 = sha256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	std::string sha_output_4 = sha256("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
	EXPECT_EQ(sha_output_1, "0f78fcc486f5315418fbf095e71c0675ee07d318e5ac4d150050cd8e57966496");
	EXPECT_EQ(sha_output_2, "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
	EXPECT_EQ(sha_output_3, "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
	EXPECT_EQ(sha_output_4, "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");
}

TEST(SHA_256_TEST, empty_string) {
	std::string sha_output_1 = sha256("");
	EXPECT_EQ(sha_output_1, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

TEST(SHA_256_TEST, long_input) {
	std::string a_milion_times(1000000, 'a'); // "a"*1000000
	std::string sha_output_1 = sha256(a_milion_times);
	EXPECT_EQ(sha_output_1, "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
