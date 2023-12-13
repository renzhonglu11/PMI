#include <my_utils.h>


uint32_t output_data(void)
{
    float x =0.0;
    float y =0.0;
    float z =0.0;
    char output_str[50];



    int32_t success_get_data =  adxl345_acc_data(&x,&y,&z);

   


    float2str(output_str,elements_of(output_str),x,3);

    uart_tx_str(output_str);
    uart_tx_char('\n');
}