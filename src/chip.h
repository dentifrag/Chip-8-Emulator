//
// Created by Wagner on 8/29/2021.
//

#ifndef CHIP_8_EMULATOR_CHIP_H
#define CHIP_8_EMULATOR_CHIP_H
#include "stack"
#include <SFML/Graphics.hpp>
using namespace std;

class Chip {

    typedef unsigned char byte;
    typedef unsigned short twobytes;

    static const int MEM_SIZE = 4096;
    static const int REG_SIZE = 16;
    static const int SCR_SIZE = 64*32;
    static const int STK_SIZE = 16;
    static const int HEX_SIZE = 16;
    static const int FNT_SIZE = 80;

    const sf::Keyboard::Key key0x1 = sf::Keyboard::Key::Num1;
    const sf::Keyboard::Key key0x2 = sf::Keyboard::Key::Num2;
    const sf::Keyboard::Key key0x3 = sf::Keyboard::Key::Num3;
    const sf::Keyboard::Key key0xC = sf::Keyboard::Key::Num4;

    const sf::Keyboard::Key key0x4 = sf::Keyboard::Key::Q;
    const sf::Keyboard::Key key0x5 = sf::Keyboard::Key::W;
    const sf::Keyboard::Key key0x6 = sf::Keyboard::Key::E;
    const sf::Keyboard::Key key0xD = sf::Keyboard::Key::R;

    const sf::Keyboard::Key key0x7 = sf::Keyboard::Key::A;
    const sf::Keyboard::Key key0x8 = sf::Keyboard::Key::S;
    const sf::Keyboard::Key key0x9 = sf::Keyboard::Key::D;
    const sf::Keyboard::Key key0xE = sf::Keyboard::Key::F;

    const sf::Keyboard::Key key0xA = sf::Keyboard::Key::Z;
    const sf::Keyboard::Key key0x0 = sf::Keyboard::Key::X;
    const sf::Keyboard::Key key0xB = sf::Keyboard::Key::C;
    const sf::Keyboard::Key key0xF = sf::Keyboard::Key::V;



public:

    byte V[REG_SIZE]{};
// Roms to be loaded after address 0x200
    byte memory[MEM_SIZE]{};
    byte display[64 * 32]{};
    bool drawFlag = false;
    twobytes chipStack[16]{};
    byte stackPointer{};
    byte keypad[HEX_SIZE]{};
    twobytes programCounter{};
    twobytes indexRegister{};
    twobytes opcode{};
    byte delayTimer = 0x00;
    byte soundTimer = 0x00;

    unsigned char fonts[FNT_SIZE] = {
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


    void getNextInstruction();

    void initializeMemory();

    void loadRom();

    void emulateCycle();

    void dumpMemory();

    static int getNibble(twobytes opcode, int i);
};

#endif //CHIP_8_EMULATOR_CHIP_H
