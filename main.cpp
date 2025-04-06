#include <FileSystem.h>

int main()
{
    file f("File", "C:/danil/");
    directory d("Direct", "C:/danil/");
    fileSystem S;
    S.addComponent(&f);
    S.addComponent(&d);
    return 0;
}