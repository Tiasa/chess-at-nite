/*
 * This file is part of the chess-at-nite project.
 *
 * Copyright (c) 2009-2010 by
 *   Franziskus Domig
 *   Panayiotis Lipiridis
 *   Radoslav Petrik
 *   Thai Gia Tuong
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CLI.h"

CLI::CLI() {
    fen = DEFAULT_FEN;
    rotated_board = false;
    max_thinking_time = DEFAULT_THINKING_TIME;
    show_best_score = false;
    show_thinking = false;
    show_about();
    start();
}

CLI::CLI(int option) {
    fen = DEFAULT_FEN;
    rotated_board = false;
    max_thinking_time = DEFAULT_THINKING_TIME;
    show_best_score = true;
    show_thinking = true;
    show_about();
    apply_option(option);
}

CLI::~CLI() {

}

void CLI::start() {
    int user_option = -1;
    while (user_option != QUIT) {
        show_options();
        user_option = get_user_option();
        apply_option(user_option);
    }
}

void CLI::settings() {
    int user_option = -1;
    while (user_option != QUIT) {
        show_settings();
        user_option = get_user_option();
        apply_settings(user_option);
    }
}

void CLI::apply_settings(int option) {
    switch (option) {
        case SET_MAX_TIME:
            set_max_time_from_user();
            break;
        case SET_SHOW_BEST_SCORE:
            show_best_score = !show_best_score;
            break;
        case SET_SHOW_THINKING:
            show_thinking = !show_thinking;
            break;
    }

}

void CLI::init_game(int game_type) {
    rotated_board = false;
    both_human = false;
    switch (game_type) {
        case HUMAN_VS_CPU:
            white_player = new HumanPlayer();
            black_player = new ComputerPlayer();
            break;
        case CPU_VS_HUMAN:
            white_player = new ComputerPlayer();
            black_player = new HumanPlayer();
            rotated_board = true;
            break;
        case HUMAN_VS_HUMAN:
            white_player = new HumanPlayer();
            black_player = new HumanPlayer();
            both_human = true;
            break;
        case CPU_VS_CPU:
            white_player = new ComputerPlayer();
            black_player = new ComputerPlayer();
            break;
    }

    //if you reached at this point and players are not initialized.. then
    //something went totally wrong...
    white_player->set_max_thinking_time(max_thinking_time);
    white_player->set_show_best_score(show_best_score);
    white_player->set_show_thinking(show_thinking);

    black_player->set_max_thinking_time(max_thinking_time);
    black_player->set_show_best_score(show_best_score);
    black_player->set_show_thinking(show_thinking);
}

void CLI::apply_option(int option) {
    switch (option) {
        case HUMAN_VS_CPU:
        case CPU_VS_HUMAN:
        case HUMAN_VS_HUMAN:
        case CPU_VS_CPU:
            init_game(option);
            start_game();
            end_game();
            break;
        case SELECT_FEN:
            select_fen();
            break;
        case SHOW_HELP:
            print_help();
            break;
        case BENCHMARK:
            run_benchmark();
            break;
        case WAC:
            run_wac_test();
            break;
        case SETTINGS:
            settings();
            break;
        case QUIT:
            cout << "Thanks for playing...!! Have fun..\n";
            break;
    }
}

void CLI::show_options() {
    cout << "-----------------------------------\n";
    cout << "   1. Human vs Computer\n";
    cout << "   2. Computer vs Human\n";
    cout << "   3. Human vs Human\n";
    cout << "   4. Computer vs Computer\n";
    cout << "   5. Load fen\n";
    cout << "   6. Help (Moves/Commands)\n";
    cout << "   7. Run Benchmark\n";
    cout << "   8. Win At Chess Test\n";
    cout << "   9. Settings\n";
    cout << "-----------------------------------\n";
    cout << "   0. Quit\n";
    cout << "-----------------------------------\n";
}

void CLI::show_settings() {
    cout << "-----------------------------------\n";
    cout << "   1. Set max thinking time (" << max_thinking_time << "s)\n";

    if (show_best_score) {
        cout << "   2. Don't show best score\n";
    } else {
        cout << "   2. Show best score\n";
    }

    if (show_thinking) {
        cout << "   3. Don't show what I'm thinking\n";
    } else {
        cout << "   3. Show what I'm thinking\n";
    }
    cout << "-----------------------------------\n";
    cout << "   0. Back\n";
    cout << "-----------------------------------\n";
}

string CLI::get_line() {
    string text;
    cin >> text;
    return text;
}

int CLI::get_user_option() {
    int result = -1;
    string temp;
    cout << "Select something: ";
    cin >> temp;

    if (temp.size() > 0 && temp[0] >= '0' && temp[0] <= '9') {
        result = temp[0] - '0';
    }
    return result;
}

void CLI::set_max_time_from_user() {
    string temp;

    int seconds = 0;
    while (seconds == 0) {
        cout << "Current time: " << max_thinking_time << "s\n";
        cout << "Enter thinking time (sec): ";
        cin >> temp;
        seconds = atoi(temp.c_str());
        if (seconds == 0) {
            cerr << "The time should be greater than 0!\n";
        }
    }
    max_thinking_time = seconds;
}

void CLI::start_game() {
    board = new Board(fen, rotated_board);
    game = new Game(board, white_player, black_player, both_human);
    game->start_game();
}

void CLI::end_game() {
    delete board;
    delete game;
    delete white_player;
    delete black_player;
    fen = DEFAULT_FEN;
}

void CLI::show_about() {
    cout << PROJECT_NAME << " " << VERSION << " (c) 2009-2010\n";
    cout << "http://chess-at-nite.googlecode.com\n";
}

void CLI::select_fen() {
    //lots of knights to test the algebraic notation
    fen = "k7/8/8/2N1N3/1N3N2/8/1N3N2/2N1N2K w - - 0 18";
    fen = BENCHMARK_FEN;
    Board b = Board(fen);
    cout << b << endl;
}

/**
 * Benchmark test that calculates how many nodes/second chess-at-nite searches.
 *
 * It sets the position to move 18 of "The Game of the Century"
 * Donald Byrne vs Robert James Fischer (Rosenwald Memorial Tour, Oct 17 1956)
 * More info:
 *   - http://en.wikipedia.org/wiki/The_Game_of_the_Century_%28chess%29
 *   - http://www.chessgames.com/perl/chessgame?gid=1008361
 *   - http://goo.gl/Yp0o (shortet the above game)
 */
void CLI::run_benchmark() {
    // old bench: "rq3rk1/4bppp/p1Rp1n2/8/4p3/1B2BP2/PP4PP/3Q1RK1 w - - 0 17"
    string fen = BENCHMARK_FEN;
    Board* board = new Board(fen);
    Player* player = new ComputerPlayer(false);
    player->set_board(board);
    player->set_max_thinking_time(max_thinking_time);
    //during the benchmark show the thinking it's fun...
    player->set_show_thinking(true);

    cout << "----- The Game of the Century -----\n";
    cout << "Donald Byrne vs Robert J. Fischer\n";
    cout << "Rosenwald Memorial, Oct 17 1956\n";
    cout << "Watch the game: http://goo.gl/Yp0o\n";

    cout << *board;
    int times[3];
    int nodes[3];
    for (int i = 0; i < 3; i++) {
        int start = get_ms();
        player->get_move();
        times[i] = get_ms() - start;
        nodes[i] = board->checked_nodes;
    }
    cout << "-------- Benchmark Results --------\n";
    double total = 0;
    for (int i = 0; i < 3; i++) {
        double nps = (nodes[i] / (double) times[i]) * 1000;
        total += nps;
        cout << "     Run " << i + 1 << ": " << (int) nps << " nodes/sec\n";
    }
    cout << "   Average: " << (int) (total / 3) << " nodes/sec\n";
    delete player;
    delete board;
}

bool CLI::compare_found_move(string found, string should) {
    vector<string> moves;
    split(should, moves, ' ');

    for (unsigned int i = 0; i < moves.size(); i++) {
        if (found.compare(moves[i]) == 0) {
            return true;
        }
    }
    return false;
}

/*
 * Testsuite "Win at Chess"
 *
 * 300 Board positions which should be found a path to checkmate.
 *
 */
void CLI::run_wac_test() {
    ifstream file;
    file.open(WAC_FILE);
    if (!file) {
        cerr << "Can not run Win At Chess test: Test file '" << WAC_FILE;
        cerr << "' is missing.\n";
        return;
    }

    vector<string> found;
    vector<string> should;
    vector<bool> results;
    vector<string> fens;
    string algebraic = "";

    int total_tested = 0;
    int total_solved = 0;

    string line = "";
    while (!file.eof()) {
        getline(file, line);
        if (line.size() < 4) {
            continue;
        }

        string cmd = "";
        cmd = line.substr(0, 4);

        if (cmd.compare("noop") == 0) {
            continue;
        } else if (cmd.compare("echo") == 0) {
            printf("%s\n", line.substr(5, line.length() - 5).c_str());
        } else if (cmd.compare("svfe") == 0) {
            string fen = "";
            fen = line.substr(5, line.length() - 5);
            fens.push_back(fen);
            Board board = Board(fen);
            Player* player = new ComputerPlayer(false);
            player->set_board(&board);
            player->set_max_thinking_time(max_thinking_time);
            player->set_show_best_score(show_best_score);
            player->set_show_thinking(show_thinking);
            move m = player->get_move();
            algebraic = move_to_algebraic(m, board);
            found.push_back(algebraic);
            delete player;
        } else if (cmd.compare("srch") == 0) {
            string answer = line.substr(5, line.length() - 5);
            should.push_back(answer);
            bool success = compare_found_move(algebraic, answer);
            results.push_back(success);
            total_tested++;

            if (success) {
                total_solved++;
                cout << "Test " << total_tested << " successful. ";
                cout << "Found: " << algebraic << endl;
            } else {
                cout << "Test " << total_tested << " failed. ";
                cout << "Found: " << algebraic;
                cout << ", should be: " << answer << endl;
            }
            printf("Solved %d/%d tests (%.1f%%)\n\n",
                    total_solved, total_tested,
                    (float) (total_solved) / total_tested * 100);
        }
    }

    file.close();

    cout << "---- Test Results ----\n";
    printf("Solved %d/%d tests (%.1f%%)\n\n",
            total_solved,
            total_tested,
            (float) (total_solved) / total_tested * 100);

    cout << "---- Failed results ----\n";

    if (found.size() == should.size() && fens.size() == results.size()
            && fens.size() == found.size()) {
        for (unsigned i = 0; i < results.size(); i++) {
            if (!results[i]) {
                cout << setw(3) << i + 1 << ". " << fens[i] << endl;
                cout << "     Found: " << found[i] << ", should be: " << should[i] << endl;
                cout << endl;
            }
        }
    } else {
        cerr << "Something went wrong with the results. Check the test file.\n";
    }
}

