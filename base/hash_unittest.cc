// Author:Zhouxiaoliang

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "base/hash.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "base/logging.h"

using namespace std;

TEST(Hash, Fingerprint) {
  LOG(INFO) << base::Fingerprint("http://www.pku.edu.cn/");
  LOG(INFO) << base::Fingerprint("http://www.sina.com.cn/");
  LOG(INFO) << base::Fingerprint("http://www.gov.cn/");
  LOG(INFO) << base::Fingerprint("http://www.chinarank.org.cn/");

  EXPECT_EQ(base::Fingerprint("pku.edu.cn"),
            base::Fingerprint(std::string("pku.edu.cn")));
}

TEST(Hash, Fingerprint32) {
  LOG(INFO) << base::Fingerprint32("pku.edu.cn");
  LOG(INFO) << base::Fingerprint32("sina.com.cn");
  LOG(INFO) << base::Fingerprint32("gov.cn");
  LOG(INFO) << base::Fingerprint32("gow.cn");
  LOG(INFO) << base::Fingerprint32("chinarank.org.cn");


  EXPECT_EQ(base::Fingerprint32("pku.edu.cn"),
            base::Fingerprint32("pku.edu.cn", strlen("pku.edu.cn")));

  EXPECT_EQ(base::Fingerprint32("pku.edu.cn"),
            base::Fingerprint32(std::string("pku.edu.cn")));
}

TEST(Hash, Fingerprint_String) {
  uint64 fp = 17504996238468593216UL;
  string fp_str;
  base::FingerprintToString(fp, &fp_str);
  LOG(INFO) << fp_str;
  uint64 fp1 = base::StringToFingerprint(fp_str);
  EXPECT_EQ(fp1, fp);
}

TEST(Hash, ContentHash_Empty1) {
  string html = "<html></html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);

  EXPECT_EQ(base::kEmptyContentHashLow, digest[0]);
  EXPECT_EQ(base::kEmptyContentHashHigh, digest[1]);
}

TEST(Hash, ContentHash_Empty2) {
  string html = "<html><title></title><div><style ></style></html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);

  EXPECT_EQ(base::kEmptyContentHashLow, digest[0]);
  EXPECT_EQ(base::kEmptyContentHashHigh, digest[1]);
}

TEST(Hash, ContentHash_NotEmpty) {
  string html = "<html><title>111</title></html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);

  EXPECT_NE(base::kEmptyContentHashLow, digest[0]);
  EXPECT_NE(base::kEmptyContentHashHigh, digest[1]);
}

TEST(Hash, ContentHash_Simple) {
  string html = "<html><title>\tx111\n</title>aaa<div href=\"bbb1\">"
      "c  c  c</div><script>SSS</script      >111ddd22"
      "<style > eee</style></html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2] = {0};
  base::FNV128("x111aaabbb1cccSSSddd",
               strlen("x111aaabbb1cccSSSddd"),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_Meta) {
  string html = "<html><meta content=\"xxx\"/>yyy</html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2] = {0};
  base::FNV128("content=xxxyyy",
               strlen("content=xxxyyy"),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_IllegalHTML1) {
  string html = "<html><title></title>aaa<div href=\"bbb\"></div>"
      "c  \nc  c</div><script src=\"sss\"></script      >"
      "</script><a>111ddd22<div xx<style><div>dddd</div></style></html>";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2];
  base::FNV128("aaabbbcccddd",
               strlen("aaabbbcccddd"),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_IllegalHTML2) {
  string html = "<html><title xxxxxxxxxx";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2];
  base::FNV128("",
               strlen(""),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_IllegalHTML3) {
  string html = "<html><div xxxxxxxxxx><script";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2];
  base::FNV128("",
               strlen(""),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_IllegalHTML4) {
  string html = "<html><div >aa</scrip";
  uint64 digest[2];
  base::ContentHash(html.c_str(), html.length(), &digest);
  uint64 expected_digest[2];
  base::FNV128("aa",
               strlen("aa"),
               &expected_digest);
  EXPECT_EQ(expected_digest[0], digest[0]);
  EXPECT_EQ(expected_digest[1], digest[1]);
}

TEST(Hash, ContentHash_HTMLFile) {
  const char* testFiles[] = {
    "base/testdata/chashtest1.html",
    "base/testdata/chashtest2.html",
    "base/testdata/chashtest3.html",
    "base/testdata/chashtest4.html",
    "base/testdata/chashtest5.html",
    "base/testdata/chashtest6.html",
    "base/testdata/chashtest7.html",
    NULL,
  };

  uint64 digest[2];
  const int buffer_size = 409600;
  char* buffer = new char[buffer_size];
  for (int i = 0; testFiles[i] != NULL; ++i) {
    ifstream is;
    is.open(testFiles[i], ios::binary);

    // get length of file:
    is.seekg(0, ios::end);
    int length = min<int>(is.tellg(), buffer_size);
    is.seekg(0, ios::beg);

    // read data as a block:
    is.read(buffer, length);
    is.close();

    base::ContentHash(buffer, length, &digest);
  }
  delete buffer;
}
