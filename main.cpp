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
    cout << argv[argc-1]<<endl;
    char ciffog[4]{'C', 'I', 'F', 'F'};
    char caff[4]{'C', 'A', 'F', 'F'};
    string testfilename=((string)argv[2]).substr(((string)argv[2]).find_last_of("/\\") + 1);
    string::size_type const p(testfilename.find_last_of('.'));

    string filenamefortest = testfilename.substr(0, p)+".jpg";




    ifstream MyFile(argv[2], ios::binary);
    if(MyFile.bad()){
        return -1;
    }
    char first_len[8];
    int64_t pos = 1;
    vector<char> bytes(
            (std::istreambuf_iterator<char>(MyFile)),
            (std::istreambuf_iterator<char>()));



    if((string)argv[1]=="-caff"){

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
        //int64_t test = *(int64_t *)text;
        int64_t firstl = *(int64_t *) first_len;
        cout << firstl << endl;
        if(firstl<1){
            return -1;
        }

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
        int64_t secl = *(int64_t *) sec_len;
        cout << secl << endl;

        if(secl<1){
            return -1;
        }

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
        int64_t anim_number = *(int64_t *) num_anim;
        cout << anim_number << endl;
        if(anim_number<1){
            return -1;
        }

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
        int64_t tmp = *(int64_t *) third_len;
        cout << tmp << endl;
        if(tmp<1){
            return -1;
        }

        pos += 8;

        char yearc[2];
        for (int i = 0; i < 2; i++) {
            yearc[i] = bytes.at(i + pos);
        }
        int year, month, day, hour, minute;
        year = *(int16_t *) yearc;
        pos += 2;

        //GIF első megjelenése
        if(year<1987){
            return -1;
        }

        month = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        if(month<1 || month>12){
            return -1;
        }

        day = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        if(day<1 || (year % 4 ==0 && month == 2 && day>29) || (year % 4 >0 && month == 2 && day>28) || ((month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) && day > 31) || ((month==4 || month==6 || month==9 || month==11) && day > 30)){
            return -1;
        }

        hour = (int) (unsigned char)bytes.at(pos);
        pos += 1;

        if(hour<0 || hour>24){
            return -1;
        }

        minute = (int) (unsigned char)bytes.at(pos);

        if(minute<0 || minute>60){
            return -1;
        }
        cout << year << '/' << month << '/' << day << ' ' << hour << ':' << minute << endl;
        pos += 1;

        char fourth_len[8];
        for (int i = 0; i < 8; i++) {
            fourth_len[i] = bytes.at(i + pos);
        }
        tmp = *(int64_t *) fourth_len;
        cout << tmp << endl;

        if(tmp<0){
            return -1;
        }

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
            tmp = *(int64_t *) leng;
            cout << tmp << endl;

            int64_t blocklength=tmp;

            if(tmp<1){
                return -1;
            }

            pos += 8;
            //duration
            char dur[8];
            for (int j = 0; j < 8; j++) {
                dur[j] = bytes.at(j + pos);
            }

            int64_t duration;

            duration = *(int64_t *) dur;
            cout << duration << endl;

            if(duration<1){
                return -1;
            }

            pos += 8;

            //CIFF Image
            int64_t carry;
            int64_t imagepos = 0;

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
            carry = *(int64_t *) header_size;
            cout << carry << endl;

            if(carry<36){
                return -1;
            }


            imagepos += 8;

            //Content size
            char contentc[8];
            int64_t content;

            for (int j = 0; j < 8; j++) {
                contentc[j] = bytes.at(j + pos + imagepos);
            }
            content = *(int64_t *) contentc;
            cout << content << endl;

            if(content<0){
                return -1;
            }

            if(carry+content+8!=blocklength){
                return -1;
            }

            imagepos += 8;

            //Image width
            char widthc[8];
            int64_t width;

            for (int j = 0; j < 8; j++) {
                widthc[j] = bytes.at(j + pos + imagepos);
            }
            width = *(int64_t *) widthc;
            cout << width << endl;

            if(width<0){
                return -1;
            }

            imagepos += 8;

            //Image Height
            char heightc[8];
            int64_t height;

            for (int j = 0; j < 8; j++) {
                heightc[j] = bytes.at(j + pos + imagepos);
            }
            height = *(int64_t *) heightc;
            cout << height << endl;

            if(height<0){
                return -1;
            }

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

            if(captionsize<0){
                return -1;
            }

            for (int j = 0; j < captionsize; j++) {
                cout << captionc[j];
            }
            cout << endl;
            imagepos += captionsize;

            char tagc[carry - 36 - captionsize];

            for (int j = 0; j < carry - 36 - captionsize; j++) {
                tagc[j] = bytes.at(j + pos + imagepos);
                if(tagc[j]=='\n'){
                    return -1;
                }
                if(j==carry-37-captionsize && tagc[j] != '\0'){
                    return -1;
                }
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
            myFile.open(filenamefortest, std::ios_base::out | std::ios_base::binary);
            if (i == 0) {
                TooJpeg::writeJpeg(myOutput, image, width, height, true, 90, false, captionc);
            }
            //---------------------------------------------------------------------------------------------



            pos += tmp - 8;
        }


    }if((string)argv[1]=="-ciff"){

        int64_t carry;
        int64_t imagepos = 0;

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
        carry = *(int64_t *) header_size;
        cout << carry << endl;

        if(carry<36){
            return -1;
        }


        imagepos += 8;

        //Content size
        char contentc[8];
        int64_t content;

        for (int j = 0; j < 8; j++) {
            contentc[j] = bytes.at(j + imagepos);
        }
        content = *(int64_t *) contentc;
        cout << content << endl;

        if(content<0){
            return -1;
        }


        imagepos += 8;

        //Image width
        char widthc[8];
        int64_t width;

        for (int j = 0; j < 8; j++) {
            widthc[j] = bytes.at(j + imagepos);
        }
        width = *(int64_t *) widthc;
        cout << width << endl;

        if(width<0){
            return -1;
        }

        imagepos += 8;

        //Image Height
        char heightc[8];
        int64_t height;

        for (int j = 0; j < 8; j++) {
            heightc[j] = bytes.at(j + imagepos);
        }
        height = *(int64_t *) heightc;
        cout << height << endl;

        if(height<0){
            return -1;
        }

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

        if(captionsize<0){
            return -1;
        }

        for (int j = 0; j < captionsize; j++) {
            cout << captionc[j];
        }
        cout << endl;
        imagepos += captionsize;

        char tagc[carry - 36 - captionsize];

        for (int j = 0; j < carry - 36 - captionsize; j++) {
            tagc[j] = bytes.at(j + imagepos);
            if(tagc[j]=='\n'){
                return -1;
            }
            if(j==carry-37-captionsize && tagc[j] != '\0'){
                return -1;
            }
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
        myFile.open(filenamefortest, std::ios_base::out | std::ios_base::binary);
        TooJpeg::writeJpeg(myOutput, image, width, height, true, 90, false, captionc);
        //---------------------------------------------------------------------------------------------

    }
    MyFile.close();
    return 0;
}
