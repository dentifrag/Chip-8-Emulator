//
// Created by Wagner on 8/29/2021.
//

#ifndef CHIP_8_EMULATOR_CHIP_H
#define CHIP_8_EMULATOR_CHIP_H
#include "stack"
#include <SFML/Graphics.hpp>
using namespace std;

uint8_t V[16]{};
// Roms to be loaded after address 0x200
uint8_t memory[4096]{};
bool display[64 * 32]{};
bool drawFlag = false;
stack<uint16_t> chipStack[16]{};
uint8_t stackPointer{};
uint8_t keypad[16]{};
uint16_t programCounter{};
uint16_t indexRegister{};
uint16_t opcode{};
uint8_t delayTimer = 0x00;
uint8_t soundTimer = 0x00;
#endif //CHIP_8_EMULATOR_CHIP_H

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