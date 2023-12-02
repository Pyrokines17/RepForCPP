#include "Game.h"

int main() { //maybe ToDo a tunnel or levelSys
    mainInit();
    std::random_device rd;
    std::mt19937 gen(rd());

    try {
        if (has_colors() == FALSE) {
            throw SupOfColor();
        }
    } catch (const SupOfColor& e) {
        endwin();
        std::cerr << e.what();
        return 4;
    }

    std::vector<int> pairs;
    initPairs(pairs);

    int height,
        weight;
    getmaxyx(stdscr, height, weight);

    sf::SoundBuffer buf;

    try {
        if (!buf.loadFromFile("data.wav")) {
            throw CanNotOpen();
        }
    } catch (const CanNotOpen& e) {
        endwin();
        std::cerr << e.what();
        return 1;
    }

    sf::Sound sound;
    sound.setBuffer(buf);
    sound.play();
    sound.setLoop(true);

    std::uniform_int_distribution<> distH(2, height - 3);
    std::uniform_int_distribution<> distW(2, weight - 3);
    std::uniform_int_distribution<> distC(10, 30);
    std::uniform_int_distribution<> distR(0, 1);

    int countOfEnemy = distC(gen);
    int countOfBlock = distC(gen);
    Map map(weight, height, countOfEnemy, countOfBlock);
    std::vector<int> parameters = {0, 0, 0};

    map.init(parameters, now(), '-', "player");

    parameters[0] = distW(gen);
    parameters[1] = distH(gen);
    map.init(parameters, now(), '-', "gun");

    for (int i = 0; i < countOfEnemy; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        int ran = distR(gen);

        if (ran == 0) {
            map.init(parameters, now(), '-', "enemy");
        } else {
            map.init(parameters, now(), '-', "altEnemy");
        }
    }

    for (int i = 0; i < countOfBlock; i++) {
        parameters[0] = distW(gen);
        parameters[1] = distH(gen);
        parameters[2] = distR(gen);
        int ran = distR(gen);

        if (ran == 0) {
            map.init(parameters, now(), '-', "block");
        } else {
            map.init(parameters, now(), '-', "altBlock");
        }
    }

    std::ofstream ofile;

    try {
        ofile.open("table.txt", std::ios::app);
        if (!ofile.is_open()) {
            throw CanNotOpen();
        }
    } catch (const CanNotOpen& e) {
        endwin();
        std::cerr << e.what();
        return 1;
    }

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

        int centre = (weight / 3 + weight / 2) / 2;
        printFirstScr(height, centre);

        std::string str = "Your name:";
        out(height / 2 + 3, centre, str);
        out(height / 2 + 4, centre, strN);
        refresh();
    }

    ofile << strN << ":";

    int c,
        res = 0,
        score = 0;
    while ('q' != (c = getch())) {
        std::ifstream ifile;

        try {
            ifile.open("table.txt", std::ios::binary);
            if (!ifile.is_open()) {
                throw CanNotOpen();
            }
        } catch (const CanNotOpen& e) {
            res = -1;
            break;
        }

        clear();
        map.drawBorders();

        if (c == '[') {
            try {
                map.save();
            } catch (const CanNotOpen& e) {
                res = -1;
                break;
            }
        } else if (c == ']') {
            try {
                map.load();
            } catch (const CanNotRead& e) {
                res = -1;
                break;
            }
        }

        map.actionOfObj(c);
        map.drawObj(pairs, c);

        map.printStat();

        try {
            Map::drawTable(ifile);
        } catch (const CanNotRead& e) {
            res = -1;
            break;
        }

        res = map.countingOfRes(score);

        map.printText();

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
        case -1: str = "Sth was bad :( Your score: %d"; break;
        case 0: str = "You leave :( Your score: %d"; break;
        case 1: str = "You win! Your score: %d"; break;
        case 2: str = "You lose... Your score: %d"; break;
        default: break;
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