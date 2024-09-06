
typedef struct {
    char* filename;
    char* content;
} File;

File* load_save(char* path) {
    File* save = da_new(File);
    FILE* f = fopen(path, "rb");
    if (f == NULL) return NULL;
    fseek(f, 0, SEEK_END);
    unsigned long file_size = ftell(f);
    fseek(f, 1, SEEK_SET);

    while (ftell(f) < file_size) {
        unsigned int name_length;
        fread(&name_length, 1, sizeof(int), f);
        if (name_length == 0xDEADBEEF) break;

        char* name = da_new(char);
        for (int i = 0; i < name_length; i++) da_push(name, 0);
        fread(name, 1, name_length, f);

        unsigned int content_length;
        fread(&content_length, 1, sizeof(int), f);

        char* content = da_new(char);
        for (int i = 0; i < content_length; i++) da_push(content, 0);
        fread(content, 1, content_length, f);

        File file = {name, content};
        da_push(save, file);
    }

    fclose(f);
    return save;
}

void save_save(File* savefile, char* path) {
    FILE* f = fopen(path, "wb");
    char a = 1;
    fwrite(&a, 1, 1, f);

    for (int i = 0; i < da_length(savefile); i++) {
        File file_i = savefile[i];
        unsigned int namesize = da_length(file_i.filename); 
        unsigned int filesize = da_length(file_i.content); 
        fwrite(&namesize, sizeof(int), 1, f);
        fwrite(file_i.filename, 1, namesize, f);
        fwrite(&filesize, sizeof(int), 1, f);
        fwrite(file_i.content, 1, filesize, f);
    }

    int deadbeef = 0xDEADBEEF;
    fwrite(&deadbeef, sizeof(int), 1, f);

    if (ftell(f) < (256*1024)) {
        int zeros = (256*1024) - ftell(f);
        char zero = 0;
        for (int i = 0; i < zeros; i++) fwrite(&zero, 1, 1, f);
    }

    fclose(f);
}
