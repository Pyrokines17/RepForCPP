#include "Game.h"

int main() { //ToDo exceptions and rooms
    mainInit();

    std::random_device rd;
    std::mt19937 gen(rd());

    if (has_colors() == FALSE) {
        endwin();
        std::cerr << "Your terminal does not support color :(" << std::endl;
        exit(1);
    }

    std::vector<int> pairs;
    initPairs(pairs);

    int height,
        weight;
    getmaxyx(stdscr, height, weight);

    sf::SoundBuffer buf;

    if (!buf.loadFromFile("data.wav")) {
        endwin();
        std::cerr << "Unable to open the file :(" << std::endl;
        exit(1);
    }

    sf::Sound sound;
    sound.setBuffer(buf);
    sound.play();
    sound.setLoop(true);

    std::uniform_int_distribution<> distH(2, height - 3);
    std::uniform_int_distribution<> distW(2, weight - 3);
    std::uniform_int_distribution<> distC(10, 20);
    std::uniform_int_distribution<> distK(0, 1);

    int countOfEnemy = distC(gen);
    int countOfBlock = distC(gen);
    Map map(weight, height, countOfEnemy, countOfBlock);
    std::vector<int> parameters = {0, 0, 0};

    map.init(parameters, now(), '-', "player");

    for (int i = 0; i < countOfEnemy; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        map.init(parameters, now(), '-', "enemy");
    }

    for (int i = 0; i < countOfEnemy / 2; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        map.init(parameters, now(), '-', "altEnemy");
    }

    for (int i = 0; i < countOfBlock / 2; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        parameters[2] = distK(gen);
        map.init(parameters, now(), '-', "block");
    }

    for (int i = 0; i < countOfBlock; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        parameters[2] = distK(gen);
        map.init(parameters, now(), '-', "altBlock");
    }

    std::ofstream ofile;
    ofile.open("table.txt", std::ios::app);
    std::string strN;

    int b;
    while ('\n' != (b = getch())) {
        clear();
        map.drawBorders();

        if (b >= 'A' and b <= 'z') {
            strN.push_back(static_cast<char>(b));
        } else if (b == KEY_BACKSPACE and !strN.empty()) {
            strN.erase(strN.end() - 1);
        }

        out(height / 3, weight / 2, strN);
        printFirstScr(height, weight);
        refresh();
    }

    ofile << strN << ":";

    int c,
        res = 0,
        score = 0;
    while ('q' != (c = getch())) {
        std::ifstream ifile;
        ifile.open("table.txt", std::ios::binary);

        if (!ifile.is_open()) {
            refresh();
            endwin();
            std::cerr << "File not open :(" << std::endl;
            exit(1);
        }

        clear();
        map.drawBorders();

        if (c == '[') {
            map.save();
        } else if (c == ']') {
            map.load();
        }

        map.actionOfObj(c);
        map.drawObj(pairs, c);

        map.printStat();
        Map::drawTable(ifile);
        res = map.countingOfRes(score);

        if (res > 0) {
            break;
        }

        map.erase();
        refresh();
        ifile.close();
    }

    ofile << score << std::endl;

    const char* str;
    switch (res) {
        case 0: str = "You leave :( Your score: %d"; break;
        case 1: str = "You win! Your score: %d"; break;
        case 2: str = "You lose... Your score: %d"; break;
        default: ;
    }

    while ('\n' != (c = getch())) {
        clear();
        map.drawBorders();
        print(str, score, height / 2, weight);
        refresh();
    }

    endwin();

    return 0;
}