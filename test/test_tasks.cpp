#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>


extern "C" {

}

TEST_GROUP(ClassName)
{
  void setup()
  {
  }
  void teardown()
  {
    ;
  }
};


TEST(ClassName, TestName)
{
    FAIL("first test");
}

int
main (int argc, char *argv[])
{
  return CommandLineTestRunner::RunAllTests(argc, argv);
}
