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
    window.setFramerateLimit(60);
    Event ev{};
    RectangleShape pixel(Vector2f(10, 10));

    Chip c;
    c.initializeMemory();
    c.loadRom();
    cout << "This is 3DC " << hex << c.memory[0x3DC] + 0<< "\n";
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
            if (c.drawFlag) {
                window.clear();
                    for (int y = 0; y < 32; y++){
                        for (int x = 0; x < 64; x++){
                            int index = x+(y*64);
                            if (c.display[index]) {
                                pixel.setPosition((float)(x*10), (float)(y * 10));
                                pixel.setFillColor(Color::White);
                                window.draw(pixel);
                            }
                        }
                    }
                }c.drawFlag = false;
            window.display();
        Sleep(2);
        }
    }
    return 0;
}
