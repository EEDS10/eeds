#include "utils.h"
#include "minunit.h"


MU_TEST(stringbeginswith){

    /* act and assert */
    mu_check(strbeginswith("hello world", "hello"));
    mu_check(strbeginswith("hello world", "hel"));
    mu_check(strbeginswith("hello world", "hello world per"));

    mu_check(!strbeginswith("hello world", "llo"));
    mu_check(!strbeginswith("hello world", "lo"));
}

MU_TEST_SUITE(utils_suite){
    MU_RUN_TEST(stringbeginswith);
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(utils_suite);
    MU_REPORT();
    return minunit_fail;
}
