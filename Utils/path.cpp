#include "path.h"

std::string path()
{
    return std::string(SDL_GetBasePath());
}

std::string path(std::string file_path)
{
    return std::format("{}{}", path(), file_path);
}
