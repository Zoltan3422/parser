#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "toojpeg.h"
using namespace std;

std::ofstream myFile;
void myOutput(unsigned char byte)
{
    myFile << byte;
}

int main(int argc, char* argv[]) {


    if((string)argv[1]=="-caff"){
        cout << argv[argc-1]<<endl;
        char caff[4]{'C', 'A', 'F', 'F'};
        char ciffog[4]{'C', 'I', 'F', 'F'};
        string newfilename=((string)argv[2]).substr(0,((string)argv[2]).find_last_of('.'))+".jpg";
        myFile.open(newfilename, std::ios_base::out | std::ios_base::binary);
        ifstream MyFile(argv[2], ios::binary);
        char first_len[8];
        uint64_t pos = 1;
        vector<char> bytes(
                (std::istreambuf_iterator<char>(MyFile)),
                (std::istreambuf_iterator<char>()));
        MyFile.close();

        //First ID=1
        int id = (unsigned char)bytes.at(0);
        if(id!=1){
            return -1;
        }
        for (int i = 0; i < 8; i++) {
            first_len[i] = bytes.at(i + pos);
            //cout << text[i];
        }
        pos += 8;
        //uint64_t test = *(uint64_t *)text;
        uint64_t firstl = *(uint64_t *) first_len;
        cout << firstl << endl;

        char text[firstl];
        char magic[4];
        for (int i = 0; i < firstl; i++) {
            text[i] = bytes.at(i + pos);
        }
        for (int i = 0; i < 4; i++) {
            magic[i] = text[i];
            cout << magic[i];
            if(magic[i]!=caff[i]){
                return -1;
            }
        }
        cout<<endl;


        char sec_len[8];
        for (int i = 0; i < 8; i++) {
            sec_len[i] = text[i + 4];
        }
        pos += firstl;
        uint64_t secl = *(uint64_t *) sec_len;
        cout << secl << endl;
        if(firstl!=secl){
            return -1;
        }
        if(secl!=20){
            return -1;
        }

        char num_anim[8];
        for (int i = 0; i < 8; i++) {
            num_anim[i] = text[i + 12];
        }
        uint64_t anim_number = *(uint64_t *) num_anim;
        cout << anim_number << endl;

        int id2 = (unsigned char)bytes.at(pos);
        cout << id2 << endl;
        if(id2!=2){
            return -1;
        }
        pos += 1;

        char third_len[8];
        for (int i = 0; i < 8; i++) {
            third_len[i] = bytes.at(i + pos);
        }
        uint64_t tmp = *(uint64_t *) third_len;
        cout << tmp << endl;

        pos += 8;

        char yearc[2];
        for (int i = 0; i < 2; i++) {
            yearc[i] = bytes.at(i + pos);
        }
        int year, month, day, hour, minute;
        year = *(uint16_t *) yearc;
        pos += 2;

        month = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        day = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        hour = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        minute = (int) (unsigned char)bytes.at(pos);
        cout << year << '/' << month << '/' << day << ' ' << hour << ':' << minute << endl;
        pos += 1;

        char fourth_len[8];
        for (int i = 0; i < 8; i++) {
            fourth_len[i] = bytes.at(i + pos);
        }
        tmp = *(uint64_t *) fourth_len;
        cout << tmp << endl;
        pos += 8;

        char creator[tmp];
        for (int i = 0; i < tmp; i++) {
            creator[i] = bytes.at(i + pos);
        }
        for (int i = 0; i < tmp; i++) {
            cout << creator[i];
        }
        cout << endl;
        pos += tmp;


        for (int i = 0; i < anim_number; i++) {

            //imageID=3
            cout << i + 1 << ".CIFF IMAGE" << endl;
            int id3 = (unsigned char)bytes.at(pos);
            cout << id3 << endl;
            if(id3!=3){
                return -1;
            }
            pos += 1;

            //data length
            char leng[8];
            for (int j = 0; j < 8; j++) {
                leng[j] = bytes.at(j + pos);
            }
            tmp = *(uint64_t *) leng;
            cout << tmp << endl;

            pos += 8;
            //duration
            char dur[8];
            for (int j = 0; j < 8; j++) {
                dur[j] = bytes.at(j + pos);
            }

            uint64_t duration;

            duration = *(uint64_t *) dur;
            cout << duration << endl;

            pos += 8;

            //CIFF Image
            uint64_t carry;
            uint64_t imagepos = 0;

            //CIFF Header
            char ciff[4];
            for (int j = 0; j < 4; j++) {
                ciff[j] = bytes.at(j + pos + imagepos);
            }
            for (int j = 0; j<4; j++) {
                cout << ciff[j];
                if(ciff[j]!=ciffog[j]){
                    return -1;
                }
            }
            cout << endl;
            imagepos += 4;

            //CIFF header_size
            char header_size[8];
            for (int j = 0; j < 8; j++) {
                header_size[j] = bytes.at(j + pos + imagepos);
            }
            carry = *(uint64_t *) header_size;
            cout << carry << endl;

            imagepos += 8;

            //Content size
            char contentc[8];
            uint64_t content;

            for (int j = 0; j < 8; j++) {
                contentc[j] = bytes.at(j + pos + imagepos);
            }
            content = *(uint64_t *) contentc;
            cout << content << endl;

            imagepos += 8;

            //Image width
            char widthc[8];
            uint64_t width;

            for (int j = 0; j < 8; j++) {
                widthc[j] = bytes.at(j + pos + imagepos);
            }
            width = *(uint64_t *) widthc;
            cout << width << endl;

            imagepos += 8;

            //Image Height
            char heightc[8];
            uint64_t height;

            for (int j = 0; j < 8; j++) {
                heightc[j] = bytes.at(j + pos + imagepos);
            }
            height = *(uint64_t *) heightc;
            cout << height << endl;

            imagepos += 8;

            //Caption and Tags
            char captionc[carry - 36];
            int captionsize = 0;


            for (int j = 0; j < carry - 36; j++) {
                if ((char) bytes.at(j + pos + imagepos) == '\n') {
                    captionc[j] = bytes.at(j + pos + imagepos);
                    captionsize = j + 1;
                    break;
                } else {
                    captionc[j] = bytes.at(j + pos + imagepos);
                }
            }

            for (int j = 0; j < captionsize; j++) {
                cout << captionc[j];
            }
            cout << endl;
            imagepos += captionsize;

            char tagc[carry - 36 - captionsize];

            for (int j = 0; j < carry - 36 - captionsize; j++) {
                tagc[j] = bytes.at(j + pos + imagepos);
            }

            for (int j = 0; j < carry - 36 - captionsize; j++) {
                cout << tagc[j];
            }
            cout << endl;
            cout << carry - 36 - captionsize;
            imagepos += (carry - 36 - captionsize);

            unsigned char image[content];
            for (int j = 0; j < content; j++) {
                image[j] = bytes.at(j + pos + imagepos);
            }

            //TOJPEG------------------------------------------------------------------------------------

            if (i == 0) {
                TooJpeg::writeJpeg(myOutput, image, width, height, true, 90, false, captionc);
            }
            //---------------------------------------------------------------------------------------------



            pos += tmp - 8;
        }


        return 0;
    }if((string)argv[1]=="-ciff"){
        char ciffog[4]{'C', 'I', 'F', 'F'};
        ifstream MyFile(argv[2], ios::binary);
        vector<char> bytes(
                (std::istreambuf_iterator<char>(MyFile)),
                (std::istreambuf_iterator<char>()));
        MyFile.close();

        uint64_t carry;
        uint64_t imagepos = 0;

        //CIFF Header
        char ciff[4];
        for (int j = 0; j < 4; j++) {
            ciff[j] = bytes.at(j + imagepos);
        }
        for (int j = 0; j<4; j++) {
            cout << ciff[j];
            if(ciff[j]!=ciffog[j]){
                return -1;
            }
        }
        cout << endl;
        imagepos += 4;

        //CIFF header_size
        char header_size[8];
        for (int j = 0; j < 8; j++) {
            header_size[j] = bytes.at(j + imagepos);
        }
        carry = *(uint64_t *) header_size;
        cout << carry << endl;

        imagepos += 8;

        //Content size
        char contentc[8];
        uint64_t content;

        for (int j = 0; j < 8; j++) {
            contentc[j] = bytes.at(j + imagepos);
        }
        content = *(uint64_t *) contentc;
        cout << content << endl;

        imagepos += 8;

        //Image width
        char widthc[8];
        uint64_t width;

        for (int j = 0; j < 8; j++) {
            widthc[j] = bytes.at(j + imagepos);
        }
        width = *(uint64_t *) widthc;
        cout << width << endl;

        imagepos += 8;

        //Image Height
        char heightc[8];
        uint64_t height;

        for (int j = 0; j < 8; j++) {
            heightc[j] = bytes.at(j + imagepos);
        }
        height = *(uint64_t *) heightc;
        cout << height << endl;

        imagepos += 8;

        //Caption and Tags
        char captionc[carry - 36];
        int captionsize = 0;


        for (int j = 0; j < carry - 36; j++) {
            if ((char) bytes.at(j + imagepos) == '\n') {
                captionc[j] = bytes.at(j + imagepos);
                captionsize = j + 1;
                break;
            } else {
                captionc[j] = bytes.at(j + imagepos);
            }
        }

        for (int j = 0; j < captionsize; j++) {
            cout << captionc[j];
        }
        cout << endl;
        imagepos += captionsize;

        char tagc[carry - 36 - captionsize];

        for (int j = 0; j < carry - 36 - captionsize; j++) {
            tagc[j] = bytes.at(j + imagepos);
        }

        for (int j = 0; j < carry - 36 - captionsize; j++) {
            cout << tagc[j];
        }
        cout << endl;
        cout << carry - 36 - captionsize;
        imagepos += (carry - 36 - captionsize);

        unsigned char image[content];
        for (int j = 0; j < content; j++) {
            image[j] = bytes.at(j + imagepos);
        }

        //TOJPEG------------------------------------------------------------------------------------

        TooJpeg::writeJpeg(myOutput, image, width, height, true, 90, false, captionc);

        return 0;
    }
}
