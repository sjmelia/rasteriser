#include <stdio.h>
#include <check.h>
#include <math.h>

#include "vector4.h"
#include "matrix4.h"
#include "affine.h"
#include "rasteriser.h"

#define EPSILON 0.001

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

// http://stackoverflow.com/questions/17565664/gluproject-and-2d-display 
START_TEST(test_rast_transform)
{
    rasteriser* rast = rasteriser_create(640, 480);
    float width = 640;
    float height = 480;
    float ratio = width / height;
    rasteriser_perspective(rast, 60.0, ratio, 1.0, 1024.0);

    vector4* result = vector4_create(0, 0, 0, 0);
    vector4* point = vector4_create(-1.0, -1.0, -4.0, 0);

    rasteriser_transform(rast, result, point);

    //printf("test_rast_transform: %f, %f, %f\n", result->x, result->y, result->z);

    ck_assert(fabs(result->x - 216.077) < EPSILON);
    ck_assert(fabs(result->y - 136.077) < EPSILON);
    
    // @todo this is the correct value for z, see
    // note in rasteriser.c
    //ck_assert(fabs(result->z - 0.750733) < EPSILON);

    rasteriser_free(rast);
    vector4_free(result);
    vector4_free(point);
}
END_TEST


START_TEST(test_affine_projection)
{
    rasteriser* rast = rasteriser_create(640, 480);
    float width = 640;
    float height = 480;
    float ratio = width / height;
    rasteriser_perspective(rast, 60.0, ratio, 1.0, 1024.0);
    matrix4* mtx = rast->projection_matrix;

    //printf("test_affine_projection: c4:%f d1:%f d2:%f d3:%f d4:%f\n", mtx->c4, mtx->d1, mtx->d2, mtx->d3, mtx->d4);

    // all unspecified values are zero
    ck_assert(fabs(mtx->a1 - 1.299038) < EPSILON);
    ck_assert(mtx->a2 == 0.000000);
    ck_assert(mtx->a3 == 0.000000);
    ck_assert(mtx->a4 == 0.000000);
    ck_assert(fabs(mtx->b2 - 1.732051) < EPSILON);
    ck_assert(fabs(mtx->c3 - -1.001955) < EPSILON);
    ck_assert(fabs(mtx->c4 - -2.001955) < EPSILON);
    ck_assert(fabs(mtx->d3 - -1.0) < EPSILON);

    rasteriser_free(rast);
}
END_TEST

Suite* vector4_suite(void) {
        Suite *suite = suite_create("vector4");
        TCase *tcase = tcase_create("case");
        tcase_add_test(tcase, test_vector4_dot);
        tcase_add_test(tcase, test_vector4_cross);
        tcase_add_test(tcase, test_vector4_cross2);
        tcase_add_test(tcase, test_vector4_subtract);
        tcase_add_test(tcase, test_affine_projection);
        tcase_add_test(tcase, test_rast_transform);
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
