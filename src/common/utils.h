/*
 * ChessAtNite
 *
 * Written by:
 *   Franziskus Domig
 *   Panayiotis Lipiridis
 *   Radoslav Petrik
 *   Thai Gia Tuong
 *
 * Chess implementation for AI course during 5th semester at IHK
 *
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "define.h"
#include "../model/Board.h"
using namespace std;

extern byte lookup_piece(char piece);
extern void split(string& str, vector<string>& tokens, const char delimiter);
extern string piece_symbol(int piece);
extern char piece_char(int piece);
extern const int max(const int a, const int b);
extern int get_square(const string& sq);
extern string get_square_by_index(int index);
extern string get_piece_unicode(int piece);
extern void print_possible_deltas(int* table);
extern void print_legal_endings(bool* table);

extern string string_move(move m, unsigned int length=0);
extern string string_square(int square);
extern string string_move_simple(move m);
extern string string_move_basic(move m);
extern string string_move_very_basic(move m);
extern string string_to_lower(string str);
extern string string_move_algebraic(move m);
extern int get_piece(const char piece);
extern bool same_move(move m1, move m2);

extern void print_moves(vector<move>& moves);
extern void print_history(vector<history_item>& history);
extern string to_string(int);
extern bool is_legal_move(const vector<move>& moves, move& m);

extern int get_ms();

extern int hash_rand();
extern move string_to_move(const string& text);
extern void print_help();

extern char* display_nodes_count(unsigned int nodes);
extern char* display_score(int score);
extern char* display_time(int start, int stop);

#endif
