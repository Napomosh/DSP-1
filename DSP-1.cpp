#include"functions.hpp"
#include<iostream>
#include<Windows.h>

// Всего сделано 1, 2, 3, 5-?, 6, 7-?

int main()
{
    std::vector<uint8_t> buffer;
    std::vector<std::vector<uint8_t>> matrix_img;
    BITMAPFILEHEADER bmp_header;
    BITMAPINFOHEADER bmp_info;


    try{
        read_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\img.bmp", buffer, matrix_img, bmp_header, bmp_info);
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return -1;
    }
    


    // ЗАДАНИЕ 3

    std::vector<uint8_t> red_component = split_RGB(buffer, bmp_header, bmp_info, 'R');
    std::vector<uint8_t> green_component = split_RGB(buffer, bmp_header, bmp_info, 'G');
    std::vector<uint8_t> blue_component = split_RGB(buffer, bmp_header, bmp_info, 'B');

    try{
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\r.bmp", red_component, bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\g.bmp", green_component, bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\b.bmp", blue_component, bmp_header, bmp_info);
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return -1;
    }

    // КОНЕЦ ЗАДАНИЯ 3

    // ЗАДАНИЕ 4 без графиков

    double cor_red_green = calculate_cor(red_component, green_component);
    double cor_red_blue = calculate_cor(red_component, blue_component);
    double cor_blue_green = calculate_cor(blue_component, green_component);
    std::cout << "Cor red and green: " << cor_red_green << std::endl;
    std::cout << "Cor red and blue: " << cor_red_blue << std::endl;
    std::cout << "Cor blue and green: " << cor_blue_green << std::endl;

    // КОНЕЦ ЗАДАНИЯ 4

    // ЗАДАНИЕ 5 без графиков + ЗАДАНИЕ 6

    std::vector<std::vector<uint8_t>> vectors_of_YCbCr = transform_RGB_to_YCC(red_component, green_component, blue_component);
    double cor_Y_Cr = calculate_cor(vectors_of_YCbCr[0], vectors_of_YCbCr[2]);
    double cor_Y_Cb = calculate_cor(vectors_of_YCbCr[0], vectors_of_YCbCr[1]);
    double cor_Cb_Cr = calculate_cor(vectors_of_YCbCr[1], vectors_of_YCbCr[2]);
    std::cout << "Cor Y and Cr: " << cor_Y_Cr << std::endl;
    std::cout << "Cor Y and Cb: " << cor_Y_Cb << std::endl;
    std::cout << "Cor Cb and Cr: " << cor_Cb_Cr << std::endl;

    try{
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\Y.bmp", vectors_of_YCbCr[0], bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\Cb.bmp", vectors_of_YCbCr[1], bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\Cr.bmp", vectors_of_YCbCr[2], bmp_header, bmp_info);
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return -1;
    }

    // КОНЕЦ ЗАДАНИЙ 5 И 6


    //ЗАДАНИЕ 7

    std::vector<std::vector<uint8_t>> vectors_of_new_RGB = reverse_transform_YCC_to_RGB(vectors_of_YCbCr[0], vectors_of_YCbCr[1], vectors_of_YCbCr[2]);

    try{
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\r1.bmp", vectors_of_new_RGB[0], bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\g1.bmp", vectors_of_new_RGB[1], bmp_header, bmp_info);
        write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\b1.bmp", vectors_of_new_RGB[2], bmp_header, bmp_info);
    }
    catch(std::runtime_error & e){
        std::cout << e.what();
        return -1;
    }

    std::cout << "PSNR blue: " << calculate_PSNR(blue_component, vectors_of_new_RGB[2]) << std::endl;
    std::cout << "PSNR green: " << calculate_PSNR(green_component, vectors_of_new_RGB[1]) << std::endl;
    std::cout << "PSNR red: " << calculate_PSNR(red_component, vectors_of_new_RGB[0]) << std::endl;

    //КОНЕЦ ЗАДАНИЯ 7 

    //Задание 8

    /*std::vector<std::vector<uint8_t>> decimal_Cb_Cr = do_decimation(vectors_of_YCbCr[1], vectors_of_YCbCr[2], bmp_info.biWidth);

    write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\Cb_dec.bmp", decimal_Cb_Cr[0], bmp_header, bmp_info);
    write_img("C:\\Users\\sashu\\Pictures\\Saved Pictures\\Cr_dec.bmp", decimal_Cb_Cr[1], bmp_header, bmp_info);*/
    return 0;
}
