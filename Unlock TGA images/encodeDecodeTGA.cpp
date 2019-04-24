#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <string>
namespace fs = std::experimental::filesystem;

void decodeTGA(fs::path tgaPath){
    //Check if file is '.tga' _AND_ not decoded
    std::string tgaExt(".tga");
    std::string dec("decoded_");
    if(tgaExt.compare(tgaPath.extension())==0 && tgaPath.filename().string().find(dec)!=0){
        std::ifstream encodedTGA(tgaPath, std::ios::binary);
        fs::path outFile = tgaPath.parent_path() /= "decoded_"+tgaPath.filename().string();
        std::ofstream decodedTGA(outFile, std::ios::binary);
        if(encodedTGA.is_open()){
            //get file size
            encodedTGA.seekg(0, encodedTGA.end);
            long size = encodedTGA.tellg();
            encodedTGA.seekg(0);

            //create memory buff and read .tga image
            char* buffer = new char[size];
            encodedTGA.read(buffer,size);

            //.TGA DECODIFICATION ALGORITHM
            //First two bytes equal 0x00
            buffer[0] = 0x00;
            buffer[1] = 0x00;
            //byte index 2 TO byte index 17...
            for(unsigned int idx=2; idx<18; idx++){
                buffer[idx] = (unsigned int)buffer[idx] - (idx * idx);
            }

            //write decoded .tga
            decodedTGA.write(buffer,size);
        }
        encodedTGA.close();
        decodedTGA.close();
    }
}

void encodeTGA(fs::path tgaPath){
    //Check if file is '.tga' _AND_ 'decoded_'
    std::string tgaExt(".tga");
    std::string dec("decoded_");
    if(tgaExt.compare(tgaPath.extension())==0 && tgaPath.filename().string().find(dec)==0){
        std::ifstream decodedTGA(tgaPath, std::ios::binary);
        fs::path outFile = tgaPath.parent_path() /= tgaPath.filename().string().erase(0,8);
        std::ofstream encodedTGA(outFile, std::ios::binary);
        if(decodedTGA.is_open()){
            //get file size
            decodedTGA.seekg(0, decodedTGA.end);
            long size = decodedTGA.tellg();
            decodedTGA.seekg(0);

            //create a buffer and read .tga image
            char* buffer = new char[size];
            decodedTGA.read(buffer,size);

            //.TGA CODIFICATION ALGORITHM
            //First two bytes equal 0x47 and 0x38
            buffer[0] = 0x47;
            buffer[1] = 0x38;
            //byte index 2 TO byte index 17...
            for(unsigned int idx=2; idx<18; idx++){
                buffer[idx] = (unsigned int)buffer[idx] + (idx * idx);
            }
            
            //write encoded .tga
            encodedTGA.write(buffer,size);
        }
        encodedTGA.close();
        decodedTGA.close();
    }
}


int main(){
    fs::path p1("../testSamples/chaoscara/");
    fs::path p2("../testSamples/spider/");

    for (auto& dirEntry : fs::recursive_directory_iterator(p1)){
        decodeTGA(dirEntry.path());
    }
    for (auto& dirEntry : fs::recursive_directory_iterator(p1)){
        encodeTGA(dirEntry.path());
    }

    for (auto& dirEntry : fs::recursive_directory_iterator(p2)){
        decodeTGA(dirEntry.path());
    }
    for (auto& dirEntry : fs::recursive_directory_iterator(p2)){
        encodeTGA(dirEntry.path());
    }

    if (__cplusplus == 201703L) std::cout << "C++17\n";
    return 0;
}
