#include"functions.hpp"
#include<fstream>
#include<vector>
#include<cmath>

void read_img(const std::string& file_name, std::vector<uint8_t>& buffer, std::vector<std::vector<uint8_t>>& matrix_img, BITMAPFILEHEADER& bmp_header, BITMAPINFOHEADER& bmp_info){
	std::ifstream file;
	file.open(file_name, std::ifstream::binary);

	if(!file.is_open()){
		throw std::runtime_error("File cannot open!");
	}

	file.read((char*)& bmp_header, sizeof(bmp_header));
	file.read((char*)& bmp_info, sizeof(bmp_info));
	char ch;
	matrix_img.resize(bmp_info.biHeight * 3);

	for(int i = 0; i < bmp_info.biHeight * 3; i++){
		matrix_img[i].resize(bmp_info.biWidth * 3);
		for(int j = 0; j < bmp_info.biWidth * 3; j++){
			file.get(ch);
			buffer.push_back(ch);
			matrix_img[i][j] = ch;
		}
	}

	while(file.get(ch)){
		buffer.push_back(ch);
	}

	file.close();
}

void write_img(const std::string& file_name, const std::vector<uint8_t>& buffer, const BITMAPFILEHEADER& bmp_header, const BITMAPINFOHEADER& bmp_info){
	std::ofstream file;
	file.open(file_name, std::ofstream::binary);

	if(!file.is_open()){
		throw std::runtime_error("File cannot open!");
	}

	file.write((char*)& bmp_header, sizeof(bmp_header));
	file.write((char*)& bmp_info, sizeof(bmp_info));
	for(int i = 0; i < buffer.size(); i++){
		file.put(buffer[i]);
	}

}

std::vector<uint8_t> split_RGB(const std::vector<uint8_t>& pixels, const BITMAPFILEHEADER& bmp_header, const BITMAPINFOHEADER& bmp_info, char color_to_split){
	std::vector<uint8_t> new_pixels(pixels.size());

	size_t i = 0; // 1-ый байт хранит синий, 2-ой хранит зеленый и 3-ий красный
	if(color_to_split == 'B') i = 0;
	if(color_to_split == 'G') i = 1;
	if(color_to_split == 'R') i = 2;
	for(i; i < new_pixels.size(); i += 3){
		new_pixels[i] = pixels[i];
	}

	return new_pixels;
}

std::vector<std::vector<uint8_t>> transform_RGB_to_YCC(const std::vector<uint8_t>& red_component, const std::vector<uint8_t>& green_component, const std::vector<uint8_t>& blue_component){
	size_t size = red_component.size();
	std::vector<uint8_t> Y_component(size); 
	std::vector<uint8_t> Cb_component(size); 
	std::vector<uint8_t> Cr_component(size);

	for(size_t i = 0; i < size; i += 3){
		Y_component[i] = 0.299 * red_component[i + 2] + 0.587 * green_component[i + 1] + 0.114 * blue_component[i];
		Y_component[i + 1] = Y_component[i];
		Y_component[i + 2] = Y_component[i];

		Cb_component[i] = 0.5643 * (blue_component[i] - Y_component[i]) + 128;
		Cb_component[i + 1] = Cb_component[i];
		Cb_component[i + 2] = Cb_component[i];

		Cr_component[i] = 0.7132 * (red_component[i + 2] - Y_component[i]) + 128;
		Cr_component[i + 1] = Cr_component[i];
		Cr_component[i + 2] = Cr_component[i];
	}

	std::vector<std::vector<uint8_t>> result = {Y_component, Cb_component, Cr_component};
	return result;
}

std::vector<std::vector<uint8_t>> reverse_transform_YCC_to_RGB(const std::vector<uint8_t>& Y_component, const std::vector<uint8_t>& Cb_component, const std::vector<uint8_t>& Cr_component){
	size_t size = Y_component.size();
	std::vector<uint8_t> new_red_component(size);
	std::vector<uint8_t> new_green_component(size);
	std::vector<uint8_t> new_blue_component(size);

	for(size_t i = 0; i < size; i += 3){
		new_red_component[i + 2] = Y_component[i] + 1.402 * (Cr_component[i] - 128);
		/*new_red_component[i + 1] = new_red_component[i];
		new_red_component[i + 2] = new_red_component[i];*/

		new_green_component[i + 1] = Y_component[i] - 0.714 * (Cr_component[i] - 128) - 0.334 * (Cb_component[i] - 128);
		/*new_green_component[i + 1] = new_green_component[i];
		new_green_component[i + 2] = new_green_component[i];*/

		new_blue_component[i] = Y_component[i] + 1.772 * (Cb_component[i] - 128);
		/*new_blue_component[i + 1] = new_blue_component[i];
		new_blue_component[i + 2] = new_blue_component[i];*/
	}

	std::vector<std::vector<uint8_t>> result = {new_red_component, new_green_component, new_blue_component};
	return result;
}

double calculate_cor(const std::vector<uint8_t>& first_component, const std::vector<uint8_t>& second_component){
	double result = 0.0;
	double cov = 0.0;
	double disp_a = 0.0;
	double disp_b = 0.0;
	double M_first_component = calculate_M(first_component);
	double M_second_component = calculate_M(second_component);

	for(size_t i = 0; i < first_component.size(); i++){
		cov += (first_component[i] - M_first_component) * (second_component[i] - M_second_component);
		disp_a += (first_component[i] - M_first_component) * (first_component[i] - M_first_component);
		disp_b += (second_component[i] - M_second_component) * (second_component[i] - M_second_component);
	}

	result = cov / sqrt(disp_a * disp_b);
	return result;
}

double calculate_M(const std::vector<uint8_t>& component){
	double result = 0.0;
	for(int i = 0; i < component.size(); i++){
		result += component[i];
	}

	return result / component.size();
}

void write_file_for_autocor_grafic(const std::vector<std::vector<uint8_t>>& component, const BITMAPINFOHEADER& bmp_info){
	std::ofstream file;
	file.open("C:\\Users\\sashu\\Documents\\MATLAB\\DSP1.txt", std::ofstream::binary);

	if(!file.is_open()){
		throw std::runtime_error("File cannot open!");
	}

	double result = 0.0;
	std::vector<uint8_t> component_for_cor_1;
	std::vector<uint8_t> component_for_cor_2;

	uint64_t height = bmp_info.biHeight / 2;
	uint64_t width = bmp_info.biWidth / 5;
	for(int y = -10; y <= 10; y += 5){
		for(int x = 0; x < width; x++){
			prepare_for_autocor();
		}
	}

	file.close();
}

void prepare_for_autocor(std::vector<uint8_t>& component_for_autocor, const std::vector<std::vector<uint8_t>>& source_component,
						uint64_t begin_i, uint64_t begin_j, uint64_t end_i, uint64_t end_j){
	for(size_t i = 1; i < height; i++){

	}
}

double calculate_PSNR(const std::vector<uint8_t>& component, const std::vector<uint8_t>& recover_component){
	double result = 0.0;
	double sum_of_component = 0.0;
	double coef = component.size() * (pow(2, 8) - 1) * (pow(2, 8) - 1);

	for(int i = 0; i < component.size(); i++){
		sum_of_component += (component[i] - recover_component[i]) * (component[i] - recover_component[i]);
	}

	result = 10 * log10(coef / sum_of_component);
	return result;  
}

std::vector<std::vector<uint8_t>> do_decimation(std::vector<uint8_t> Cb_component, std::vector<uint8_t> Cr_component, int width_in_pixels){
	width_in_pixels *= 3;
	std::vector<std::vector<uint8_t>> res(2);
	size_t size = Cb_component.size();
	for(int i = 0; i < size; i++){
		//Удаление четных столбцов
		if(i % 2 == 0){ 
			Cb_component.erase(Cb_component.begin() + i);
			Cr_component.erase(Cr_component.begin() + i);
			size--;
		}
		//Удаление четных строк
		while((i % width_in_pixels) % 2 == 1){
			i++;
			Cb_component.erase(Cb_component.begin() + i);
			Cr_component.erase(Cr_component.begin() + i);
			size--;
		}
	}

	res = {Cb_component, Cr_component};
	return res;
}