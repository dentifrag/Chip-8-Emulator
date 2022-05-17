#include <iostream>
#include "fstream"
#include "chip.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include<windows.h>
#include <cstdlib>
using namespace std;
using namespace sf;

class Chip {
public:
    unsigned char fonts[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void initializeMemory();

    static void loadRom();

    static void emulateCycle();

    static void getNextInstruction();
};

void Chip::getNextInstruction(){
    programCounter += 2;
}
void Chip::initializeMemory(){
    // ROMS start loading at 0x200
    programCounter = 0x200;
    opcode = 0;
    indexRegister = 0;
    stackPointer = 0;

    // loading fonts into the first 80 memory addresses
    for (int i = 0; i < 80; i ++){
        memory[i] = fonts[i];
    }
}

void Chip::loadRom() {
    ifstream ROM("../ROM/test_opcode.ch8", ios::in | ios::binary);
    char byte;
    if (ROM.is_open()) {
        for (int i = 0x200; ROM.get(byte) ; i++){
            memory[i] = (uint8_t) byte;
        }
    }
    ROM.close();

}

void Chip::emulateCycle(){
    // Fetch opcode
    opcode = memory[programCounter];
    opcode <<=8;
    opcode |=  memory[programCounter + 1];
    cout << hex << opcode<< endl;

    // using bitwise &s to mask off bits to extract parts of opcodes
    switch (opcode & 0xF000u) {
        case 0x0000:{
            switch (opcode) {
                default:{
                    cout << "Unknown opcode " << opcode;
                    break;
                }
                case 0x0000: {
                    break;
                }
                case 0x00E0:{
                    cout<<"clearing"<< endl;
                    memset(display, false, sizeof(display));
                    break;
                }
                case 0x00EE:{
                    programCounter = chipStack->top();
                    stackPointer --;
                    break;
                }
            }
            break;
        }
        case 0x1000: {
            programCounter = opcode & 0x0FFF;
            break;

        }
        case 0x2000: {
            stackPointer += 1;
            chipStack->push(programCounter);
            programCounter = opcode & 0x0FFF;
            break;
        }
        case 0x3000: {
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                programCounter += 2;
        }
            break;
            }
        case 0x4000: {
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                programCounter += 2;
        }break;
            }
        case 0x5000: {
            if (V[(opcode & 0x0F00) >> 8] == V[opcode & 0x00F0]){
                programCounter += 2;
            }
            break;
        }
        case 0x6000: {
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            break;
        }
        case 0x7000: {
            int registerAddress = (opcode & 0x0F00) >> 8;
            V[registerAddress] += opcode & 0x00FF;
            cout << "Adds the value " << hex << +V[registerAddress] << " to the value of register Vx, then stores the result in Vx"<< endl;
            break;
        }
        case 0x8000: {
            switch (opcode & 0x000F) {
                case 0x0000:{
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    break;
                }
                case 0x0001: {
                    (V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4]);
                } break;

                case 0x0002: {
                    (V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4]);
                } break;

                case 0x0003: {
                    (V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4]);
                break;
                }

                case 0x0004: {
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    if (V[opcode & 0x00F0 >> 4] > (0xFF - V[opcode & 0x0F00 >> 8])){
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }

                break;
                }

                case 0x0005: {
                    V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
                    if ((V[(opcode & 0x0F00) >> 8]) > V[(opcode & 0x00F0) >> 4]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                break;
                }

                case 0x0006: {
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    break;
                }

                case 0x0007: {
                    if (V[opcode & 0x00F0 >> 4] > V[opcode & 0x0F00 >> 8]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[opcode & 0x0F00 >> 8] = V[opcode & 0x00F0 >> 4] - V[opcode & 0x0F00 >> 8];
                break;
                }
                case 0x000E: {
                    V[0xF] = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[opcode & 0x0F00 >> 8] <<= 1;
                    break;
                }

            } break;
        }
        case 0x9000: {
            if(V[opcode & 0x0F00 >> 8] != V[opcode & 0x00F0 >> 4]){
                programCounter += 2;
            }
            break;
        }
        case 0xA000: {
            indexRegister = (opcode & 0x0FFF);
            break;
        }
        case 0xB000: {
            programCounter += (opcode & 0x0FFF) + V[0];
            break;
        }
        case 0xC000: {
            V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
            break;
        }
        case 0xD000: {
            // DXYN
            uint8_t pixel;
            uint8_t height = opcode & 0x000F;
            uint8_t vX = V[(opcode & 0x0F00) >> 8];
            uint8_t vY = V[(opcode & 0x00F0) >> 4];

            // setting collision to false
            V[0xF] = 0;

            for (int y = 0; y < height; y++){
                pixel = memory[(indexRegister) + y];
                for (int x = 0; x < 8; x++ ){
                    // checking if the bit (pixel) from the memory is on, and if the pixel at coordinates Vx, Vy is on
                    if (((pixel & (0x80 >> x)) != 0)) {
                        if (display[(vX + x + ((vY +y) * 64))] == 1)
                        // setting collision to true
                            V[0xF] = 1;
                        display[vX + x + ((vY + y) * 64)] ^= 1;
                    }

                    }
                }
                drawFlag = true;
            break;
            }
            case 0xF000: {
                case 0x0007: {
                    V[(opcode & 0x0F00) >> 8] = delayTimer;
//                    cout << hex << vRegisters[(opcode & 0x0F00) >> 8] + 0 << "is equal to " << delayTimer;
                    break;
                }
                case 0x0015: {
                    delayTimer = V[(opcode & 0x0F00) >> 8];
                    break;
                }
                case 0x0018: {
                    soundTimer = V[(opcode & 0x0F00) >> 8];
                    break;
                }
                case 0x001E: {
                    indexRegister += V[(opcode * 0x0F00) >> 8];
                    break;
                }
                case 0x0029: {
                    indexRegister = V[(opcode & 0x0F00) >> 8] * 0x5;
                    break;
                }
                case 0x0033: {
                    memory[indexRegister] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[indexRegister + 1] = (V[(opcode & 0x0F00) >> 8] /10) % 10;
                    memory[indexRegister + 2] = (V[(opcode & 0x0F00) >> 8] %100) % 10;
                    break;
                }
                case 0x0055: {
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        memory[indexRegister + i] = V[i];
                    }
                    indexRegister += ((opcode & 0x0F00) >> 8) + 1;
                    break;
                }
                case 0x0065: {
                    for (int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
                        V[i] = memory[indexRegister + i];
                    }
                    indexRegister += ((opcode & 0x0F00) >> 8) + 1;
                    break;
                }
            }
            // TODO
//            case 0xE000: {
//                switch(opcode & 0x000F) {
//                    case 0x009E: {
//                        break;
//                    }
//                    case 0x00A1:{
//                        break;
//                    }
//                }
        }
    getNextInstruction();
    }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    // Decode
    // Execute
int main() {
    RenderWindow window(VideoMode(640, 320), "Chip 8", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    Event ev{};
    RectangleShape pixel(Vector2f(10, 10));

    Chip c;
    c.initializeMemory();
    c.loadRom();

    while(true){

        while(window.isOpen()){
            c.emulateCycle();
            while (window.pollEvent(ev)) {
                switch (ev.type) {
                    case Event::Closed: {
                        window.close();
                        break;
                        }
                }
            }

            if(ev.type == Event::KeyPressed){
                switch(ev.key.code) {

                }
            }
            if (drawFlag) {
                window.clear();
                    for (int y = 0; y < 32; y++){
                        for (int x = 0; x < 64; x++){
                            int index = x+(y*64);
                            if (display[index]) {
                                pixel.setPosition(Vector2f(x*10, (y * 10)));
                                pixel.setFillColor(Color::White);
                                window.draw(pixel);
                            }
                        }
                    }
                }drawFlag = false;
            window.display();
        }
        Sleep(2);
    }
    return 0;
}
