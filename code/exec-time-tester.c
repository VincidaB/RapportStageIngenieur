#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arm_neon.h>


int simd_test(int array_size){
    float32x4_t vec = vdupq_n_f32(0.1f);

    float array[array_size];
    float result[array_size];
    float sum = 0.0f;

    for (int i = 0; i < array_size; i++) {
        array[i] = (float)i;
    }

    for (int i = 0; i < array_size; i += 4) {
        if (i + 3 < array_size) {  // Check bounds to avoid going over the end eof the array
            float32x4_t data = vld1q_f32(&array[i]);
            float32x4_t result_vec = vmulq_f32(data, vec);
            vst1q_f32(&result[i], result_vec);
        }
    }

    for (int i = 0; i < array_size; i++) {
        sum += result[i];
    }

    printf("Sum: %f\n", sum);

    return 0;
}

int for_loop_sum(int range)
{
    uint64_t k;
    /*
    using a uint64_t to avoid overflow (max value of uint32_t is only 4294967295,
     while uint64_t is 18446744073709551615 or 2^64 - 1)
    we can thus sum up to range = sqrt(2^64 - 1)*2  = 8589934592
    
    */
    volatile uint64_t c = 0;
    for(k = 0; k < range; k++){
        c += k;
    }
    return 0;
}




int main(int argc, char *argv[]) {

    int argument;

    if (argc < 2){
        printf("Usage : %s [options] \n", argv[0]);
        return 1;
    }


    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "---help") == 0){
            //Display of help message
            printf("Usage : %s [options]\n", argv[0]);
            printf("Options : \n");
            printf("-h or --help  Display this message\n");
            printf("--simd [arraySize] runs multiple simd operations\n");
            printf("--sum [range] runs a sum up to `range`\n");
        } else if (strcmp(argv[i], "--simd") == 0){
            if (i+1<argc){
                argument = atoi(argv[i+1]);
                printf("running simd_test with arraysize = %i\n", argument);
                return simd_test(argument);
            } else {
                printf("--simd requires an array size on which the operations will be performed\n");
                return 1; //early return, incorect arguments
            }
        }
        else if (strcmp(argv[i], "--sum") == 0){
            if (i+1<argc){
                argument = atoi(argv[i+1]);
                printf("running for_loop_sum with range = %i\n", argument);
                return for_loop_sum(argument);
            } else {
                printf("--sum requires an range up to which the operations will be performed\n");
                return 1; //early return, incorect arguments
            }
        }
    }
}