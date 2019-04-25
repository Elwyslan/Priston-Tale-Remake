#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>
namespace fs = std::experimental::filesystem;

void decodeBMP(fs::path bmpPath){
    //Check if file is '.bmp' _AND_ not 'decoded_'
    std::string bmpExt(".bmp");
    std::string dec("decoded_");
    if(bmpExt.compare(bmpPath.extension())==0 && bmpPath.filename().string().find(dec)!=0){
        std::ifstream encodedBMP(bmpPath, std::ios::binary);
        fs::path outFile = bmpPath.parent_path() /= "decoded_"+bmpPath.filename().string();
        std::ofstream decodedBMP(outFile, std::ios::binary);
        if(encodedBMP.is_open()){
            //get file size
            encodedBMP.seekg(0, encodedBMP.end);
            long size = encodedBMP.tellg();
            encodedBMP.seekg(0);

            //create memory buff and read .bmp image
            char* buffer = new char[size];
            encodedBMP.read(buffer,size);

            //.BMP DECODIFICATION ALGORITHM
            //First byte equal 0x42, Second byte equal 0x4D
            buffer[0] = 0x42;
            buffer[1] = 0x4D;
            //byte index 2 TO byte index 13...
            for(unsigned int idx=2; idx<14; idx++){
                buffer[idx] = (unsigned int)buffer[idx] - (idx * idx);
            }

            //write decoded .bmp
            decodedBMP.write(buffer,size);
        }
        encodedBMP.close();
        decodedBMP.close();
    }
}

void encodeBMP(fs::path bmpPath){
    //Check if file is '.bmp' _AND_ 'decoded_'
    std::string bmpExt(".bmp");
    std::string dec("decoded_");
    if(bmpExt.compare(bmpPath.extension())==0 && bmpPath.filename().string().find(dec)==0){
        std::ifstream decodedBMP(bmpPath, std::ios::binary);
        fs::path outFile = bmpPath.parent_path() /= bmpPath.filename().string().erase(0,8);
        std::ofstream encodedBMP(outFile, std::ios::binary);
        if(decodedBMP.is_open()){
            //get file size
            decodedBMP.seekg(0, decodedBMP.end);
            long size = decodedBMP.tellg();
            decodedBMP.seekg(0);

            //create a buffer and read .bmp image
            char* buffer = new char[size];
            decodedBMP.read(buffer,size);

            //.BMP CODIFICATION ALGORITHM
            //First byte equal 0x41, Second byte equal 0x38
            buffer[0] = 0x41;
            buffer[1] = 0x38;
            //byte index 2 TO byte index 13...
            for(unsigned int idx=2; idx<14; idx++){
                buffer[idx] = (unsigned int)buffer[idx] + (idx * idx);
            }
            
            //write encoded .bmp
            encodedBMP.write(buffer,size);
        }
        encodedBMP.close();
        decodedBMP.close();
    }
}

int main(){
    fs::path p1("../testSamples/chaoscara/");
    fs::path p2("../testSamples/spider/");
    
    for (auto& dirEntry : fs::recursive_directory_iterator(p1)){
        decodeBMP(dirEntry.path());
    }
    for (auto& dirEntry : fs::recursive_directory_iterator(p1)){
        encodeBMP(dirEntry.path());
    }
    
    for (auto& dirEntry : fs::recursive_directory_iterator(p2)){
        decodeBMP(dirEntry.path());
    }
    for (auto& dirEntry : fs::recursive_directory_iterator(p2)){
        encodeBMP(dirEntry.path());
    }
    
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    return 0;
}
