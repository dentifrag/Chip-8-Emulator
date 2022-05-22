//
// Created by Wagner on 5/21/2022.
//

#include "chip.h"
#include <iostream>
#include "fstream"
#include<windows.h>

void Chip::getNextInstruction(){
    pc += 2;
}
void Chip::initializeMemory(){
    memset(memory, false, sizeof(memory));
    memset(display, false, sizeof(display));
    memset(chipStack, false, sizeof(chipStack));
    // ROMS start loading at 0x200
    pc = 0x200u;
    opcode = 0;
    indexRegister = 0;
    stackPointer = 0;
    // loading fonts into the first 80 memory addresses
    for (int i = 0; i < 80; i ++){
        memory[i] = fonts[i];
    }
}
   int Chip::getNibble(uint16_t code, int bitShift) {
    int value {};
    if (bitShift == 8) {
        value = ((code & 0xF00) >> 8);
    }
    if (bitShift == 4) {
        value = ((code & 0x0F0) >> 4);
    }
    return value;
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

void Chip::dumpMemory() {
    cout << "start dump \n";
    for (unsigned char i : memory){
        cout << hex << i + 0 <<"\n";
    }
    cout << "dumped \n";
}

void Chip::emulateCycle(){
    // Fetch opcode
    opcode = memory[pc];
    opcode <<=8;
    opcode |=  memory[pc + 1];
    cout << hex << uppercase << opcode<< endl;

    // using bitwise &s to mask off bits to extract parts of opcodes
    switch (opcode & 0xF000u) {
        case 0x0000:{
            switch (opcode) {
//                default:{
//                    cout << "Unknown opcode " << opcode;
//                    break;
//                }
//                case 0x0000: {
//                    break;
//                }
                case 0x00E0:{
                    cout<<"clearing"<< endl;
                    memset(display, false, sizeof(display));
                    break;
                }
                case 0x00EE:{
                    stackPointer --;
                    pc = chipStack[stackPointer];
                    break;
                }
            }
            break;
        }
        case 0x1000: {
            pc = opcode & 0x0FFFu;
            break;

        }
        case 0x2000: {
            chipStack[stackPointer] = pc;
            stackPointer ++;
            pc = opcode & 0x0FFFu;
            break;
        }
        case 0x3000: {
            if (V[getNibble(opcode,8)] == (opcode & 0x00FFu)){
                pc += 2;
            }
            break;
        }
        case 0x4000: {
            if (V[getNibble(opcode,8)] != (opcode & 0x00FFu)){
                pc += 2;
            }break;
        }
        case 0x5000: {
            if (V[getNibble(opcode,8)] == V[getNibble(opcode,4)]){
                pc += 2;
            }
            break;
        }
        case 0x6000: {
            V[getNibble(opcode,8)] = opcode & 0x00FFu;
            break;
        }
        case 0x7000: {
            V[getNibble(opcode,8)] += opcode & 0x00FFu;
            break;
        }
        case 0x8000: {
            switch (opcode & 0x000F) {
                case 0x0000:{
                    V[getNibble(opcode,8)] = V[getNibble(opcode,4)];
                    break;
                }
                case 0x0001: {
                    (V[getNibble(opcode,8)] |= V[getNibble(opcode,4)]);
                } break;

                case 0x0002: {
                    (V[getNibble(opcode,8)] &= V[getNibble(opcode,4)]);
                } break;

                case 0x0003: {
                    (V[getNibble(opcode,8)] ^= V[getNibble(opcode,4)]);
                    break;
                }

                case 0x0004: {
                    if ((V[getNibble(opcode,4)] + V[getNibble(opcode,8)]) > 0xFFu){
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[getNibble(opcode,8)] += V[getNibble(opcode,4)];

                    break;
                }

                case 0x0005: {
                    if ((V[getNibble(opcode,8)]) > V[getNibble(opcode,4)]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[getNibble(opcode,8)] -= V[getNibble(opcode,4)];
                    break;
                }

                case 0x0006: {
                    V[0xF] = V[getNibble(opcode,8)] & 0x1;
                    V[getNibble(opcode,8)] >>= 1;
                    break;
                }

                case 0x0007: {
                    if (V[getNibble(opcode,4)] > V[getNibble(opcode,8)]) {
                        V[0xF] = 1;
                    } else {
                        V[0xF] = 0;
                    }
                    V[getNibble(opcode,8)] = V[getNibble(opcode,4)] - V[getNibble(opcode,8)];
                    break;
                }
                case 0x000E: {
                    V[0xF] = V[getNibble(opcode,8)] >> 7;
                    cout << hex << V[0xF]<< " The bit \n";
                    V[getNibble(opcode,8)] <<= 1;
                    break;
                }

            } break;
        }
        case 0x9000: {
            if(V[getNibble(opcode,8)] != V[getNibble(opcode,4)]){
                pc += 2;
            }
            break;
        }
        case 0xA000: {
            indexRegister = (opcode & 0x0FFFu);
            break;
        }
        case 0xB000: {
            pc += (opcode & 0x0FFFu) + V[0];
            // this opcode isn't used very often
            break;
        }
        case 0xC000: {
            V[getNibble(opcode,8)] = (rand() % 0xFF) & (opcode & 0x00FF);
            break;
        }
        case 0xD000: {
            // DXYN
            uint8_t pixel;
            uint8_t height = opcode & 0x000Fu;
            uint8_t vX = V[getNibble(opcode,8)];
            uint8_t vY = V[getNibble(opcode,4)];

            // setting collision to false
            V[0xF] = 0;

            for (int y = 0; y < height; y++){
                pixel = memory[(indexRegister) + y];
                for (int x = 0; x < 8; x++ ){
                    // checking if the bit (pixel) from the memory is on, and if the pixel at coordinates Vx, Vy is on
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (display[(vX + x + ((vY + y) * 64))] == 1) {
                            V[0xF] = 1;
                        }
                        // setting collision to true
                        display[vX + x + ((vY + y) * 64)] ^= 1;
                    }

                }
            }
            drawFlag = true;
            break;
        }
        case 0xF000: {
            switch (opcode & 0x00FFu) {
                case 0x0007: {
                    V[getNibble(opcode, 8)] = delayTimer;
                    break;
                }
                case 0x0015: {
                    delayTimer = V[getNibble(opcode, 8)];
                    break;
                }
                case 0x0018: {
                    soundTimer = V[getNibble(opcode, 8)];
                    break;
                }
                case 0x001E: {
                    indexRegister += V[getNibble(opcode, 8)];
                    break;
                }
                case 0x0029: {
                    indexRegister = V[getNibble(opcode, 8)] * 0x5;
                    break;
                }
                case 0x0033: {
                    memory[indexRegister] = V[getNibble(opcode, 8)] / 100;
                    memory[indexRegister + 1] = (V[getNibble(opcode, 8)] / 10) % 10;
                    memory[indexRegister + 2] = (V[getNibble(opcode, 8)] % 10) % 10;
                    break;
                }
                case 0x0055: {
                    for (int i = 0; i <= V[getNibble(opcode, 8)]; i++) {
                        memory[indexRegister + i] = V[i];
                    }
                    break;
                }
                case 0x0065: {
                    for (int i = 0; i <= getNibble(opcode, 8); i++) {
                        V[i] = memory[indexRegister + i];
                    }
                    break;
                }
            }
                    break;
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


