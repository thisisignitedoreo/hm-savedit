
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define DA_IMPL
#include "da.h"

#include "savefile.c"

#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "guidialogue.h"

#include "style_dark.h"

float sfxvolume = 1.0f;
float musicvolume = 1.0f;

bool masks[26] = {0};
bool levels[20] = {0};
bool weaponary[16] = {0};
bool puzzle[16] = {0};

unsigned long overall_score = 0;

unsigned int level_ranks[22] = {0};
unsigned long level_scores[22] = {0};

bool puzzle_solved = false;

bool level_rank_sel[41] = {0};

File* savefile = 0;

bool file_dialogue = false;
GuiWindowFileDialogState fds;

void save_savefile() {
    char* savesdat = da_new(char);
    for (int i = 0; i < 26; i++) da_push(savesdat, masks[i] ? '1' : '0');
    da_push(savesdat, '\n');
    for (int i = 0; i < 20; i++) da_push(savesdat, levels[i] ? '1' : '0');
    da_push(savesdat, '\n');
    for (int i = 0; i < 16; i++) da_push(savesdat, weaponary[i] ? '1' : '0');
    da_push(savesdat, '\n');
    for (int i = 0; i < 16; i++) da_push(savesdat, puzzle[i] ? '1' : '0');
    da_push(savesdat, '\n');
    char* str = TextFormat("%f\n", (float) overall_score);
    for (int i = 0; i < strlen(str); i++) da_push(savesdat, str[i]);
    for (int i = 0; i < 22; i++) {
        char* str = TextFormat("%u\n", level_ranks[i]);
        for (int i = 0; i < strlen(str); i++) da_push(savesdat, str[i]);
    }
    for (int i = 0; i < 22; i++) {
        char* str = TextFormat("%u\n", level_scores[i]);
        for (int i = 0; i < strlen(str); i++) da_push(savesdat, str[i]);
    }
    da_push(savesdat, puzzle_solved ? '1' : '0');
    for (int i = 0; i < da_length(savefile); i++) {
        if (strcmp(savefile[i].filename, "saves.dat") == 0) {
            da_free(savefile[i].content);
            savefile[i].content = savesdat;
        }
    }
    for (int i = 0; i < da_length(savesdat); i++) putchar(savesdat[i]);
    save_save(savefile, TextFormat("%s\\Documents\\My Games\\HotlineMiami\\SaveData.sav", getenv("USERPROFILE")));
}

void draw_ui() {
    if (fds.windowActive == true) GuiDisable();
    GuiPanel((Rectangle) {5, 5, GetScreenWidth()-10, 30}, NULL);
    if (GuiButton((Rectangle) {10, 10, 110, 20}, savefile == 0 ? "Load Savefile" : "Reload Savefile")) {
        if (FileExists(TextFormat("%s\\Documents\\My Games\\HotlineMiami\\SaveData.sav", getenv("USERPROFILE")))) {
            savefile = load_save(TextFormat("%s\\Documents\\My Games\\HotlineMiami\\SaveData.sav", getenv("USERPROFILE")));
            load_globals(savefile);
        }
        else fds.windowActive = true;
    }
    if (savefile != 0) if (GuiButton((Rectangle) {130, 10, 100, 20}, "Save Savefile")) save_savefile();
    GuiLabel((Rectangle) {240, 10, 120, 20}, "Made by aciddev_");
    if (GuiButton((Rectangle) {360, 10, 120, 20}, "Open GitHub Page")) OpenURL("https://github.com/thisisignitedoreo/hm-savedit/");
    if (savefile == 0) GuiDisable();
    GuiPanel((Rectangle) {5, 45, 100, GetScreenHeight()-10-40}, NULL);
    GuiLabel((Rectangle) {10, 45, 110, 30}, "Masks");
    char* mask_names[] = { "Richard", "Rasmus", "Tony", "Aubrey", "Don Juan", "Graham", "Dennis", "George", "Ted", "Rufus", "Rami", "Willem", "Peter", "Zack", "Oscar", "Rick", "Brandon", "Charlie", "Louie", "Phil", "Nigel", "Earl", "Jones", "Carl", "Jake", "Richter" };
    for (int i = 0; i < sizeof(mask_names)/sizeof(mask_names[0]); i++) {
        GuiCheckBox((Rectangle) {10, 75 + i*20, 10, 10}, mask_names[i], masks + i);
    }
    GuiPanel((Rectangle) {110, 45, 110, GetScreenHeight()-10-40}, NULL);
    GuiLabel((Rectangle) {115, 45, 110, 30}, "Levels");
    char* level_names[] = { "The Metro", "No Talk", "Overdose", "Decadence", "Tension", "Full House", "Clean Hit", "Neighbors", "Push It", "Crackdown", "Hot & Heavy", "Deadline", "Trauma", "Assault", "Vengeance", "Showdown", "Safehouse", "Fun & Games", "Prank Call", "Resolution" };
    for (int i = 0; i < sizeof(level_names)/sizeof(level_names[0]); i++) {
        GuiCheckBox((Rectangle) {115, 75 + i*20, 10, 10}, level_names[i], levels + i);
    }
    GuiPanel((Rectangle) {225, 45, 120, GetScreenHeight()-10-40}, NULL);
    GuiLabel((Rectangle) {230, 45, 120, 30}, "Weapons");
    char* weapon_names[] = { "Katana", "Glass Bottle", "Magnum", "Machete", "Beer Can", "Skorpion", "Hammer", "Brick", "Uzi", "Sledgehammer", "Ninja Star", "Dart", "MP5", "Fireaxe", "Scissors", "Silenced Pistol" };
    for (int i = 0; i < sizeof(weapon_names)/sizeof(weapon_names[0]); i++) {
        GuiCheckBox((Rectangle) {230, 75 + i*20, 10, 10}, weapon_names[i], weaponary + i);
    }
    GuiPanel((Rectangle) {350, 45, GetScreenWidth() - 350 - 5, 70}, NULL);
    GuiLabel((Rectangle) {355, 45, GetScreenWidth() - 350 - 5, 30}, "Puzzle pieces");
    GuiCheckBox((Rectangle) {355, 90, 10, 10}, "Puzzle completed?", &puzzle_solved);
    char* puzzle_letters = "IWASBORNINTHEUSA";
    int strl = strlen(puzzle_letters);
    for (int i = 0; i < strl; i++) {
        char strn[2] = {0}; strn[0] = puzzle_letters[i];
        GuiCheckBox((Rectangle) {355 + i*26, 70, 10, 10}, strn, puzzle + i);
    }
    GuiPanel((Rectangle) {350, 120, GetScreenWidth() - 350 - 5, GetScreenHeight() - 120 - 5}, NULL);
    if (GuiSpinner((Rectangle) {455, 125, GetScreenWidth() - 455 - 10, 20}, "Overall Score ", &overall_score, 0, 999999999, level_rank_sel[40])) level_rank_sel[40] = !level_rank_sel[21];
    int level_count = sizeof(level_names)/sizeof(level_names[0]);
    bool any_active = false;
    for (int i = 0; i < level_count; i++) any_active = any_active || level_rank_sel[i];
    for (int i = level_count; i > 0; i--) {
        int indice = i-1;
        GuiLabel((Rectangle) {355, 150 + indice*20, 105, 30}, level_names[indice]);
        if (GuiSpinner((Rectangle) {500, 155 + indice*20, 80, 20}, "Rank ", level_ranks + indice, 0, 15, level_rank_sel[indice])) level_rank_sel[indice] = !level_rank_sel[indice];
        if (GuiSpinner((Rectangle) {670, 155 + indice*20, 100, 20}, "High-score ", level_scores + indice, 0, 999999999, level_rank_sel[indice+20])) level_rank_sel[indice+20] = !level_rank_sel[indice+20];
    }
    GuiLabel((Rectangle) {355, 552, 800, 30}, TextFormat("0 - N/A; 1 - F-; 2 - F; 3 - F+; 4 - D-; 5 - D; 6 - D+; 7 - C-; 8 - C; 9 - C+;"));
    GuiLabel((Rectangle) {355, 567, 800, 30}, TextFormat("10 - B-; 11 - B; 12 - B+; 13 - A-; 14 - A; 15 - A+"));
    GuiEnable();
}

void load_globals(File* savefile) {
    long line = 0;
    char string[1024];
    int string_length = 0;
    long ch = 0;
    for (int i = 0; i < da_length(savefile); ++i) {
        if (strcmp(savefile[i].filename, "saves.dat") == 0) {
            File sv = savefile[i];
            for (int c = 0; c < da_length(sv.content); ++c) {
                if (sv.content[c] == '\n') {
                    string[string_length] = 0;
                    if (line == 4) overall_score = atof(string);
                    if (line >= 5 && line <= 26) level_ranks[line-5] = atoi(string);
                    if (line >= 27 && line <= 48) level_scores[line-27] = atol(string);
                    string_length = 0;
                    line++; ch = 0; continue;
                }
                if (line == 0) masks[ch] = sv.content[c] == '1';
                if (line == 1) levels[ch] = sv.content[c] == '1';
                if (line == 2) weaponary[ch] = sv.content[c] == '1';
                if (line == 3) puzzle[ch] = sv.content[c] == '1';
                if (line >= 4 && line <= 48) string[string_length++] = sv.content[c];
                if (line == 49) puzzle_solved = sv.content[c] == '1';
                ch++;
            }
            break;
        }
    }
}

void print_save(File* savefile) {
    if (savefile == 0) return;
    printf("save {\n");
    for (int i = 0; i < da_length(savefile); ++i) {
        File file = savefile[i];
        da_push(file.filename, 0);
        printf("  %s: {\n    ", file.filename);
        for (int i = 0; i < da_length(file.content); i++) {
            putchar(file.content[i]);
            if (file.content[i] == '\n') printf("    ");
        }
        printf("\n  }, %zu bytes,\n", da_length(file.content));
        da_pop(file.filename, 0);
    }
    printf("}\n");
}

int main(void) {
    InitWindow(800, 600, "Hotline Miami Save Editor");
    SetExitKey(0);

    char* str = TextFormat("%s\\Documents\\My Games\\HotlineMiami\\", getenv("USERPROFILE"));
    if (!DirectoryExists(str)) str = GetWorkingDirectory();
    fds = InitGuiWindowFileDialog(str);
    GuiLoadStyleDark();
    
    if (FileExists(TextFormat("%s\\Documents\\My Games\\HotlineMiami\\SaveData.sav", getenv("USERPROFILE")))) {
        savefile = load_save(TextFormat("%s\\Documents\\My Games\\HotlineMiami\\SaveData.sav", getenv("USERPROFILE")));
        load_globals(savefile);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        if (fds.SelectFilePressed) {
            if (strcmp(fds.fileNameText, "SaveData.sav") == 0) {
                savefile = load_save(TextFormat("%s\\%s", fds.dirPathText, fds.fileNameText));
                if (savefile == NULL) {
                    TraceLog(LOG_ERROR, strerror(errno));
                    savefile = 0;
                }
                load_globals(savefile);
                // print_save(savefile);
                fds.SelectFilePressed = false;
            }
        }

        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            draw_ui();
            GuiWindowFileDialog(&fds);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

