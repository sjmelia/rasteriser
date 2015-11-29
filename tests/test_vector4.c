#include <stdio.h>
#include <check.h>

#include "vector4.h"

START_TEST(test_vector4_dot)
{
    vector4* a = vector4_create(1, 2, 3, 0);
    vector4* b = vector4_create(4, -5, 6, 0);

    double result = vector4_dot(a, b);

    ck_assert(result == 12.0);
}
END_TEST

Suite* vector4_suite(void) {
        Suite *suite = suite_create("vector4");
        TCase *tcase = tcase_create("case");
        tcase_add_test(tcase, test_vector4_dot);
        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
    int number_failed;
    Suite *suite = vector4_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}
