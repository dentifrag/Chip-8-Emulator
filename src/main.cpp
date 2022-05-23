#include <iostream>
#include "fstream"
#include "chip.h"
#include<windows.h>

using namespace std;
using namespace sf;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main() {

    RenderWindow window(VideoMode(640, 320), "Chip 8", Style::Titlebar | Style::Close);
    window.setFramerateLimit(144);
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
                if (ev.type == sf::Event::KeyPressed) {
                    switch (ev.key.code) {

                        case sf::Keyboard::Q: {
                            c.keypad[0x4] = true;

                            break;
                        }
                        case  sf::Keyboard::W: {
                            c.keypad[0x05] = true;
                            break;
                        }
                        case  sf::Keyboard::E: {
                            c.keypad[0x06] = true;
                            break;
                        }
                        case  sf::Keyboard::R: {
                            c.keypad[0x0D] = true;
                            break;
                        }
                        case  sf::Keyboard::A: {
                            c.keypad[0x07] = true;
                            break;
                        }
                        case  sf::Keyboard::S: {
                            c.keypad[0x08] = true;
                            break;
                        }
                        case  sf::Keyboard::D: {
                            c.keypad[0x09] = true;
                            break;
                        }
                        case  sf::Keyboard::F: {
                            c.keypad[0x0E] = true;
                            break;
                        }
                        case  sf::Keyboard::Z:{
                            c.keypad[0x0A] = true;
                            break;
                        }
                        case  sf::Keyboard::X:{
                            c.keypad[0x0] = true;

                            break;
                        }
                        case  sf::Keyboard::C:{
                            c.keypad[0xB] = true;

                            break;
                        }
                        case  sf::Keyboard::V:{
                            c.keypad[0xF] = true;

                            break;
                        }
                    }
                    break;
                }

                if(ev.type == sf::Event::KeyReleased){
                    switch(ev.key.code) {
                        case sf::Keyboard::Q: {
                            c.keypad[0x4] = false;;
                            break;
                        }
                        case  sf::Keyboard::W: {
                            c.keypad[0x05] = false;
                            break;
                        }
                        case  sf::Keyboard::E: {
                            c.keypad[0x06] = false;
                            break;
                        }
                        case  sf::Keyboard::R: {
                            c.keypad[0x0D] = false;
                            break;
                        }
                        case  sf::Keyboard::A: {
                            c.keypad[0x07] = false;
                            break;
                        }
                        case  sf::Keyboard::S: {
                            c.keypad[0x0D8] = false;
                            break;
                        }
                        case  sf::Keyboard::D: {
                            c.keypad[0x09] = false;
                            break;
                        }
                        case  sf::Keyboard::F: {
                            c.keypad[0x0E] = false;
                            break;
                        }
                        case  sf::Keyboard::Z:{
                            c.keypad[0x0A] = false;
                            break;
                        }
                        case  sf::Keyboard::X:{
                            c.keypad[0x0] = false;
                            cout << "X is pushed";
                            break;
                        }
                        case  sf::Keyboard::C:{
                            c.keypad[0xB] = false;
                            cout << "X is pushed";
                            break;
                        }
                        case  sf::Keyboard::V:{
                            c.keypad[0xF] = false;
                            cout << "X is pushed";
                            break;
                        }
                    }
                }

                break;
            }
            if (c.drawFlag) {
                window.clear();
                    for (int y = 0; y < 32; y++){
                        for (int x = 0; x < 64; x++){
                            int index = x+(y*64);
                            if (c.display[index]) {
                                pixel.setPosition((float)(x*10), (float)(y * 10));
                                pixel.setFillColor(Color::Green);
                                window.draw(pixel);
                            }
                        }
                    }
                }c.drawFlag = false;
            window.display();
        }
    }
    return 0;

}
