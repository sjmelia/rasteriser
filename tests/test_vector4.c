#include <stdio.h>
#include <check.h>

#include "vector4.h"

START_TEST(test_vector4_dot)
{
    vector4* a = vector4_create(1, 2, 3, 0);
    vector4* b = vector4_create(4, -5, 6, 0);

    double result = vector4_dot(a, b);
    
    ck_assert(result == 12.0);
    vector4_free(a);
    vector4_free(b);
}
END_TEST

START_TEST(test_vector4_cross)
{
    vector4* a = vector4_create(3, -3, 1, 0);
    vector4* b = vector4_create(4, 9, 2, 0);
    vector4* result = vector4_create(0, 0, 0, 0);

    vector4_cross(result, a, b);
    ck_assert(result->x == -15.0);
    ck_assert(result->y == -2.0);
    ck_assert(result->z == 39.0);

    vector4_free(a);
    vector4_free(b);
    vector4_free(result);
}
END_TEST

START_TEST(test_vector4_cross2)
{
    vector4* a = vector4_create(3, 2, -2, 0);
    vector4* b = vector4_create(1, 0, -5, 0);
    vector4* result = vector4_create(0, 0, 0, 0);
    
    vector4_cross(result, a, b);
    ck_assert(result->x == -10.0);
    ck_assert(result->y == 13.0);
    ck_assert(result->z == -2.0);

    vector4_free(a);
    vector4_free(b);
    vector4_free(result);
}
END_TEST

START_TEST(test_vector4_subtract)
{
    vector4* a = vector4_create(1, 2, 3, 0);
    vector4* b = vector4_create(4, 5, 6, 0);
    vector4* result = vector4_create(0, 0, 0, 0);
    
    vector4_minus(result, a, b);
    ck_assert(result->x == -3.0);
    ck_assert(result->y == -3.0);
    ck_assert(result->z == -3.0);

    vector4_free(a);
    vector4_free(b);
    vector4_free(result);
}
END_TEST


Suite* vector4_suite(void) {
        Suite *suite = suite_create("vector4");
        TCase *tcase = tcase_create("case");
        tcase_add_test(tcase, test_vector4_dot);
        tcase_add_test(tcase, test_vector4_cross);
        tcase_add_test(tcase, test_vector4_cross2);
        tcase_add_test(tcase, test_vector4_subtract);
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
