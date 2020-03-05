#pragma once
#include<string>
#include<vector>
#include<cstdint>
#include<Windows.h>

void read_img(const std::string& file_name, std::vector<uint8_t>& buffer, std::vector<std::vector<uint8_t>>& matrix_img, BITMAPFILEHEADER& bmp_header, BITMAPINFOHEADER& bmp_info);
void write_img(const std::string& file_name, const std::vector<uint8_t>& buffer, const BITMAPFILEHEADER& bmp_header, const BITMAPINFOHEADER& bmp_info);
std::vector<uint8_t> split_RGB(const std::vector<uint8_t>& pixels, const BITMAPFILEHEADER& bmp_header, const BITMAPINFOHEADER& bmp_info, char color_to_split);
std::vector<std::vector<uint8_t>> transform_RGB_to_YCC(const std::vector<uint8_t>& red_component, const std::vector<uint8_t>& green_component, const std::vector<uint8_t>& blue_component);
std::vector<std::vector<uint8_t>> reverse_transform_YCC_to_RGB(const std::vector<uint8_t>& Y_component, const std::vector<uint8_t>& Cb_component, const std::vector<uint8_t>& Cr_component);
double calculate_cor(const std::vector<uint8_t>& first_component, const std::vector<uint8_t>& second_component);
double calculate_M(const std::vector<uint8_t>& component);
void write_file_for_autocor_grafic(const std::vector<uint8_t>& component, const BITMAPINFOHEADER& bmp_info);
double calculate_PSNR(const std::vector<uint8_t>& component, const std::vector<uint8_t>& recover_component);
std::vector<std::vector<uint8_t>> do_decimation(std::vector<uint8_t> Cb_component, std::vector<uint8_t> Cr_component, int width_in_pixels);
void prepare_for_autocor(std::vector<uint8_t>& component_for_autocor, const std::vector<std::vector<uint8_t>>& source_component,
						uint64_t begin_i, uint64_t begin_j, uint64_t end_i, uint64_t end_j)